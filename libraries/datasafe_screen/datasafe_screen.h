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

#include <ScreenLib.h>







/**** keyboard settings ****/

// TODO: we should do it in the ScreenLib
//char last_key = 0;




/**** class ****/

class datasafe_screen {
private:

	const char* _screen_messages[5];
	uint16_t _last_key_press_ms;
	ScreenLib _screen;
	void actOnButtonPress(uint16_t x, uint16_t y);
	char actOnTouchScreenInteraction(uint16_t x, uint16_t y, boolean keyboardOn);

public:
	datasafe_screen(): 
	_screen (TFT_CS, TFT_DC)
	{
		_screen_messages[0] = "DataSafe";
		_screen_messages[1] = "v1.0";
		_screen_messages[2] = "nfc card login";
		_screen_messages[3] = "unlock the nfc card with your key!";
		_screen_messages[4] = "put your nfc card on the nfc reader!";

		_last_key_press_ms = 0;
	}

	void begin();
	void reset();
	void clearScreen();
	void showSplashScreen();
	void showNFCKeyScreen();
	void showNFCCardScreen();
	void showDataScreen();
	char touchscreenAction();

	

	
};




#endif


