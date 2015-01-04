/***************************************************
 * 	@author:	bboortz
 * 	@license:	bsd license
 ***************************************************/



#include <./datasafe_screen.h>


void datasafe_screen::begin() {
	_screen.setup();
}


void datasafe_screen::reset() {

        // reset the display and touchscreen
        _screen.reset();

	// show the splash screen
	showSplashScreen();

	// reset some variables
	_last_key_press_ms = 0;
}

void datasafe_screen::showSplashScreen() {
	_screen.clearScreen();
        _screen.writeText(BOXSIZE, BOXSIZE, 4, SCREENLIB_RED, SCREEN_MESSAGE_000);
        _screen.writeText(BOXSIZE*3, BOXSIZE*3, 2, SCREENLIB_WHITE, SCREEN_MESSAGE_001);
}

void datasafe_screen::showNFCKeyScreen() {
	_screen.drawStandardScreen();
	_screen.writeTextToTop(SCREEN_MESSAGE_002);
	_screen.writeTextToBottom(SCREEN_MESSAGE_003);
	_screen.drawKeyboard();
}

void datasafe_screen::showNFCCardScreen() {
	_screen.drawStandardScreen();
	_screen.writeTextToTop(SCREEN_MESSAGE_002);
	_screen.writeTextToBottom(SCREEN_MESSAGE_004);
}

void datasafe_screen::showDataScreen() {
        _screen.drawStandardScreen();
        _screen.writeTextToTop(SCREEN_MESSAGE_008);
        _screen.writeText(0, BOXSIZE*2, 2, SCREENLIB_WHITE, "MY SECRET DATA !!");
//        _screen.writeTextToBottom(SCREEN_MESSAGE_003);
        
//        _screen.writeText(0, 0 , 2, SCREENLIB_WHITE, SCREEN_MESSAGE_005);
        

	//  sd.showFile("test.txt");
}



/**** actions ****/


/**** touchscreen action functions ****/


char datasafe_screen::touchscreenAction() {
	char result = 0;
	
	// Retrieve a point  
	TS_Point p = _screen.touchscreenGetPoint();
	
        if (p.z == 0) {
                return result;
	}

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
			strcpy(dest, SCREEN_MESSAGE_006);
			dest[5] = c;
			dest[6] = SCREEN_STRING_TERMINATION;
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
#ifdef SCREEN_DEBUG
			Serial.print(F(SCREEN_MESSAGE_007));   
			Serial.print(F(x)); 
			Serial.print(F(SCREEN_STRING_SPLIT)); 
			Serial.print(F(y));
			Serial.println();
#endif // #ifdef SCREEN_DEBUG
			reset();  
		}
	}
}
