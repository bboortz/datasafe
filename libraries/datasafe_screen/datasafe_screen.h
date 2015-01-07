/***************************************************
 * 	@author:	bboortz
 * 	@license:	bsd license
 ***************************************************/



#ifndef _DATASAFE_SCREEN_H
#define _DATASAFE_SCREEN_H

/**** includes ****/
#if (ARDUINO >= 100)
# include <Arduino.h>
#else
# include <WProgram.h>
#endif

// libraries from https://github.com/bboortz/arduino-libs
#include <ScreenLib.h>




/**** logging and debugging ****/
// #define SCREEN_VERBOSE
// #define SCREEN_DEBUG




/**** messages ****/

#define SCREEN_MESSAGE_000              "DataSafe"
#define SCREEN_MESSAGE_001              "v1.0"
#define SCREEN_MESSAGE_002              "nfc card login"
#define SCREEN_MESSAGE_003              "unlock the nfc card with your key!"
#define SCREEN_MESSAGE_004              "put your nfc card on the nfc reader!"
#define SCREEN_MESSAGE_005              "The data:"
#define SCREEN_MESSAGE_006              "key:"
#define SCREEN_MESSAGE_007              "Button pressed: "
#define SCREEN_MESSAGE_008              "secret data"


#define SCREEN_STRING_SPLIT             " / "
#define SCREEN_STRING_TERMINATION       '\0'



/**** keyboard settings ****/

// TODO: we should do it in the ScreenLib
//char last_key = 0;




/**** class ****/

class datasafe_screen {
private:
	uint16_t _last_key_press_ms;
	ScreenLib _screen;
	void actOnButtonPress(uint16_t x, uint16_t y);
	char actOnTouchScreenInteraction(uint16_t x, uint16_t y, boolean keyboardOn);

public:
	datasafe_screen(): 
	_screen (TFT_CS, TFT_DC)
	{
		_last_key_press_ms = 0;
	}

	void begin();
	void reset();
	void clearScreen();
	void showSplashScreen();
	void showNFCKeyScreen();
	void showNFCCardScreen();
	void showDataScreen(char* str);
	char touchscreenAction();

	

	
};




#endif


