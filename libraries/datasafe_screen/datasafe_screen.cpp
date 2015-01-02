/***************************************************
 * 	@author:	bboortz
 * 	@license:	bsd license
 ***************************************************/



#include <datasafe_screen.h>


void datasafe_screen::begin() {
	_screen.setup();
}


void datasafe_screen::reset() {

	// show the splash screen
	showSplashScreen();

	// reset the touchscreen
	uint16_t x, y;
	uint8_t z;  
	while (!_screen.getTouchScreen().bufferEmpty()) {
		_screen.getTouchScreen().readData(&x, &y, &z);
	}
	_screen.getTouchScreen().writeRegister8(STMPE_INT_STA, 0xFF);

	// reset some variables
	_last_key_press_ms = 0;
}


void datasafe_screen::clearScreen() {
	_screen.clearScreen();
}

void datasafe_screen::showSplashScreen() {
	_screen.fillScreen(ILI9341_BLACK);  
	_screen.setCursor(BOXSIZE, BOXSIZE);
	_screen.setTextColor(ILI9341_RED);  
	_screen.setTextSize(4);
	_screen.println(_screen_messages[0]);

	_screen.setCursor(BOXSIZE*3, BOXSIZE*3);
	_screen.setTextColor(ILI9341_WHITE);  
	_screen.setTextSize(2);
	_screen.println(_screen_messages[1]);
}

void datasafe_screen::showNFCKeyScreen() {
	clearScreen();
	_screen.writeTextToTop(_screen_messages[2]);
	_screen.writeTextToBottom(_screen_messages[3]);
	_screen.drawKeyboard();
}

void datasafe_screen::showNFCCardScreen() {
	clearScreen();
	_screen.writeTextToTop(_screen_messages[2]);
	_screen.writeTextToBottom(_screen_messages[4]);
}

void datasafe_screen::showDataScreen() {
	_screen.fillScreen(ILI9341_BLACK);  
	_screen.setCursor(0, 0);
	_screen.setTextColor(ILI9341_WHITE);  
	_screen.setTextSize(2);
	_screen.println("The data:");

	_screen.setCursor(BOXSIZE*2, BOXSIZE*2);
	_screen.println("MY SECRET DATA !!");

	//  sd.showFile("test.txt");

	delay(3000);
}



/**** actions ****/


/**** touchscreen action functions ****/


char datasafe_screen::touchscreenAction() {
	char result = 0;
	
	// wait for the touch and show it
	if (_screen.getTouchScreen().bufferEmpty()) {
		return result;
	}
	if (!_screen.getTouchScreen().touched()) {
		return result;
	}


	// Retrieve a point  
	TS_Point p = _screen.touchscreenGetPoint();
	/*
    if (p.z == 0) {
	 return;
	 }
	 */

	// act on touchscreen interaction
	result = actOnTouchScreenInteraction(p.x, p.y, true);

	return result;
}

char datasafe_screen::actOnTouchScreenInteraction(uint16_t x, uint16_t y, boolean keyboardOn) {
	uint16_t millis_now = millis();
	uint16_t millis_delta = millis_now - _last_key_press_ms;  
	char c = 0;

	// check polling
	if (millis_delta <= KEYBOARD_POLLING_MS) {
		return c;
	}

	// actions
	actOnButtonPress(x, y);

	if (keyboardOn) {
		c = _screen.actOnKeyboardKeyPress(x, y);
		if (c != 0) {
			char dest[7];
			strcpy(dest, "key: ");
			dest[5] = c;
			dest[6] = '\0';
			_screen.writeTextToBottom(dest);
		}
	}


	// set last key press for polling
	_last_key_press_ms = millis_now;

	return c;
}

void datasafe_screen::actOnButtonPress(uint16_t x, uint16_t y) {
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