/***************************************************
 * 	@author:	bboortz
 * 	@license:	bsd license
 ***************************************************/




/**** library includes ****/
#include "./datasafe.h"




/**** Logging settings ****/

//LogLib logger;




/**** widget library settings ****/
datasafe_screen ds_screen;




/**** ntc settings ****/

NFCLib nfclib;

char nfc_key[6] = { 
	0, 0, 0, 0, 0, 0 }; // the key buffer
uint8_t nfc_key_count = 0;




/**** sd settings ****/

SDLib sd;




/**** current state settings ****/

#define STATE_RESET			0
#define STATE_NFC_KEY_LOGIN_BEGIN 	1
#define STATE_NFC_KEY_LOGIN 		2
#define STATE_NFC_CARD_LOGIN_BEGIN 	3
#define STATE_NFC_CARD_LOGIN 		4
#define STATE_SHOW_DATA 		5
#define STATE_END 			6

uint8_t current_state = STATE_RESET;






/**** display prepare functions ****/


void reset() {
	// reset all global variables
	current_state = STATE_RESET;
	memset(nfc_key, 0, sizeof(nfc_key));
	nfc_key_count = 0;

	// reset the screen -> shows a slashscreen
	ds_screen.reset();

	// reset the sd card controller
	sd.setup();

        // setup the nfc controller
        nfclib.setupHost();
}




/**** state functions ****/

void nextState() {
#ifdef PROG_DEBUG
	Serial.print(F(DATASAFE_MESSAGE_001));
	Serial.print(F(current_state));
	Serial.print(F(DATASAFE_MESSAGE_002));
	Serial.print(F(current_state+1));
	Serial.println();
#endif // #ifdef PROG_DEBUG
        
	current_state++;
	if (current_state == STATE_END) {
		current_state = STATE_RESET;
	}
}

void doStateReset() {
	// show the splash screen
	reset();  

	// next state
	nextState();
}

void doStateNfcKeyLoginBegin() {
	// show the nfc key screen
	ds_screen.showNFCKeyScreen();

	// next state
	nextState();
}

void doStateNfcKeyLogin() {
	char c = 0;

	// act on touchscreen interaction
	c = ds_screen.touchscreenAction();
	if (c != 0) {
		nfc_key[nfc_key_count] = c;
		nfc_key_count++;
	}
       
	// next state
	if ( nfc_key_count > 5) {
			nextState();
	}
}


void doStateNfcCardLoginBegin() {
	// show the nfc card screen
	ds_screen.showNFCCardScreen();

	// next state
	nextState();
}

void doStateNfcCardLogin() {

	// read the nfc card
	uint8_t uid[] = { 
		0, 0, 0, 0, 0, 0, 0   	};                    // Buffer to store the returned UID
	uint8_t uidLength = nfclib.initTarget(&*uid);               // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
	uidLength = nfclib.printTarget();   
	
	// next state
	nextState();
}

void doStateShowData() {
	// show the data
        char* data = sd.readFile("test.txt");
        ds_screen.showDataScreen( data );
	delay(5000);

	// next state
	nextState();
}




/**** arduino default functions ****/

void setup(void) { 
        
	// setup the screen
	ds_screen.begin();

#ifdef PROG_VERBOSE
        Serial.println(F(DATASAFE_MESSAGE_000));
#endif // #ifdef PROG_VERBOSE
}


void loop() {
        
#ifdef PROG_ANALYZE_MEM
        Serial.print(F(DATASAFE_MESSAGE_003));
        Serial.println(freeMemory());
#endif // #ifdef PROG_ANALYZE_MEM

	// working with the state pattern because we have different screens (states)
	switch (current_state) {
	case STATE_RESET:              
		doStateReset();              
		break;
	case STATE_NFC_KEY_LOGIN_BEGIN:      
		doStateNfcKeyLoginBegin();        
		break;
	case STATE_NFC_KEY_LOGIN:      
		doStateNfcKeyLogin();        
		break;
	case STATE_NFC_CARD_LOGIN_BEGIN:     
		doStateNfcCardLoginBegin();       
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


