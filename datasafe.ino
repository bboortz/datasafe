/***************************************************
 * 	@author:	bboortz
 * 	@license:	bsd license
 ***************************************************/




/**** library includes ****/

#include <SPI.h>      
#include <Wire.h>                      // this is needed even tho we aren't using it

#include <Adafruit_GFX.h>              // the adafruit core graphics lib
#include <Adafruit_ILI9341.h>          // the adafruit tft lib
#include <Adafruit_STMPE610.h>         // the adafruit touch lib
#include <Adafruit_NFCShield_I2C.h>    // the adafruit nfc shield lib

#include <ScreenLib.h>
#include <NFCLib.h>

//#include <SD.h>
//#include <SDLib.h>




/**** widget library settings ****/

ScreenLib screen = ScreenLib(TFT_CS, TFT_DC);




/**** ntc settings ****/

//Adafruit_NFCShield_I2C nfc(NFC_IRQ_PIN, NFC_RESET_PIN);
NFCLib nfclib;

char nfc_key[6] = { 
  0, 0, 0, 0, 0, 0 }; // the key buffer
uint8_t nfc_key_count = 0;




/**** sd settings ****/

//SDLib sd;




/**** keyboard settings ****/

// TODO: we should do it in the ScreenLib
uint16_t last_key_press_ms = 0;
//char last_key = 0;




/**** current state settings ****/

#define STATE_RESET 		0
#define STATE_NFC_KEY_LOGIN 	1
#define STATE_NFC_CARD_LOGIN 	2
#define STATE_SHOW_DATA 	3
#define STATE_END 		4

uint8_t current_state = STATE_RESET;




/**** messages ****/
static const char* screen_message[] = { 
  "DataSave",
  "v1.0",
  "nfc card login",
  "unlock the nfc card with your key!",
  "put your nfc card on the nfc reader!"
};




/**** display prepare functions ****/

void clearScreen() {
  screen.clearScreen();
}


void reset() {
  // reset all global variables
  current_state = STATE_RESET;
  //  last_key_press_ms = 0;
  //  last_key = 0;
  memset(nfc_key, 0, sizeof(nfc_key));
  nfc_key_count = 0;

  // show the splash screen
  showSplashScreen();

  // reset the sd card controller
  // sd.setup();

  // reset the touchscreen
  uint16_t x, y;
  uint8_t z;  
  while (!screen.getTouchScreen().bufferEmpty()) {
    screen.getTouchScreen().readData(&x, &y, &z);
    Serial.println(screen.getTouchScreen().bufferSize());
  }
  screen.getTouchScreen().writeRegister8(STMPE_INT_STA, 0xFF);

  // setup nfc
  nfclib.setupHost();

  // show the nfc screen
  showNFCKeyScreen();
}




/**** show screen functions ****/

void showSplashScreen() {
  screen.fillScreen(ILI9341_BLACK);  
  screen.setCursor(BOXSIZE, BOXSIZE);
  screen.setTextColor(ILI9341_RED);  
  screen.setTextSize(4);
  screen.println(screen_message[0]);

  screen.setCursor(BOXSIZE*3, BOXSIZE*3);
  screen.setTextColor(ILI9341_WHITE);  
  screen.setTextSize(2);
  screen.println(screen_message[1]);
}


void showNFCKeyScreen() {
  current_state = STATE_NFC_KEY_LOGIN;
  clearScreen();
  screen.writeTextToTop(screen_message[2]);
  screen.writeTextToBottom(screen_message[3]);
  screen.drawKeyboard();
}

void showNFCCardScreen() {
  current_state = STATE_NFC_CARD_LOGIN;
  clearScreen();
  screen.writeTextToTop(screen_message[2]);
  screen.writeTextToBottom(screen_message[4]);
}


void showDataScreen() {
  current_state = STATE_SHOW_DATA;
  screen.fillScreen(ILI9341_BLACK);  
  screen.setCursor(0, 0);
  screen.setTextColor(ILI9341_WHITE);  
  screen.setTextSize(2);
  screen.println("The data:");

  screen.setCursor(BOXSIZE*2, BOXSIZE*2);
  screen.println("MY SECRET DATA !!");

  //  sd.showFile("test.txt");

  delay(3000);
}






/**** touchscreen action functions ****/

void actOnTouchScreenInteraction(uint16_t x, uint16_t y, boolean keyboardOn) {
  uint16_t millis_now = millis();
  uint16_t millis_delta = millis_now - last_key_press_ms;  
  char c;

  // check polling
  if (millis_delta <= KEYBOARD_POLLING_MS) {
    return;
  }

  // actions
  actOnButtonPress(x, y);

  if (keyboardOn) {
    if ( nfc_key_count > 5) {
      return;
    }
    c = screen.actOnKeyboardKeyPress(x, y);
    if (c != 0) {
      nfc_key[nfc_key_count] = c;
      nfc_key_count++;

      char dest[12];
      strcpy(dest, "key: ");
      strcat(dest, nfc_key);
      dest[11] = '\0';
      screen.writeTextToBottom(dest);


      // show the nfc screen when password is fullfilled
      if (nfc_key_count > 5) {
        showNFCCardScreen();
      }

    }
  }


  // set last key press for polling
  last_key_press_ms = millis_now;
}

void actOnButtonPress(uint16_t x, uint16_t y) {
  if (SCREEN_BUTTON_MINY + SCREEN_TEXT_PADDING*2.7 >= y && y <= SCREEN_BUTTON_MINY + SCREEN_TEXT_PADDING*2.7 + SCREEN_BUTTON_SIZE * 2) {
    if (SCREEN_TEXT_PADDING*3 >= x && x <= SCREEN_TEXT_PADDING*3 + SCREEN_BUTTON_SIZE * 2) {
	  Serial.print("Button pressed: ");   
	  Serial.print(x); 
	  Serial.print(" / "); 
	  Serial.print(y);
	  Serial.println("");
      reset();  
    }
  }
}




/**** state functions ****/

void nextState() {
  current_state++;
  if (current_state == STATE_END) {
    current_state = STATE_RESET;
  }
}

void doStateReset() {
  // show the splash screen
  reset();  
}

void doStateNfcKeyLogin() {
  // wait for the touch and show it
  if (screen.getTouchScreen().bufferEmpty()) {
    return;
  }
  if (! screen.getTouchScreen().touched()) {
    return;
  }


  // Retrieve a point  
  TS_Point p = screen.touchscreenGetPoint();
  /*
    if (p.z == 0) {
   return;
   }
   */

  // act on touchscreen interaction
  actOnTouchScreenInteraction(p.x, p.y, true);
}

void doStateNfcCardLogin() {

  // read the nfc card
  //uint8_t success;
  uint8_t uid[] = { 
    0, 0, 0, 0, 0, 0, 0   };                    // Buffer to store the returned UID
  uint8_t uidLength = nfclib.initTarget(&*uid);               // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uidLength = nfclib.printTarget();   

  // show the data
  showDataScreen();

  // reset everything()
  reset();  
}

void doStateShowData() {
  // show the data
  showDataScreen();

  nextState();
}




/**** arduino default functions ****/

void setup(void) {
  // setup the screen
  screen.setup();  
}

void loop()
{
  // working with the state pattern because we have different screens (states)
  switch (current_state) {
  case STATE_RESET:              
    doStateReset();              
    break;
  case STATE_NFC_KEY_LOGIN:      
    doStateNfcKeyLogin();        
    break;
  case STATE_NFC_CARD_LOGIN:     
    doStateNfcCardLogin();       
    break;
  case STATE_SHOW_DATA:          
    doStateShowData();           
    break;
  case STATE_END:                
    nextState();                 
    break;
  default:                       
    doStateReset();              
    break;
  }

}




