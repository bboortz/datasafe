/***************************************************
        @author:        bboortz
        @license:       bsd license
***************************************************/



#ifndef datasafe_h
#define datasafe_h

/**** includes ****/

// arduino basic 
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

// arduino standard libraries
#include <SPI.h>      
#include <Wire.h>

// adafruit libraries
#include <Adafruit_GFX.h>              // the adafruit core graphics lib
#include <Adafruit_ILI9341.h>          // the adafruit tft lib
#include <Adafruit_STMPE610.h>         // the adafruit touch lib
#include <Adafruit_NFCShield_I2C.h>    // the adafruit nfc shield lib

// libraries from https://github.com/bboortz/arduino-libs
#include <datasafe_screen.h>
#include <NFCLib.h>

// experimental libraries
// #include <ScreenLib.h>
//#include <SD.h>
//#include <SDLib.h>



/**** program settings ****/
#define PROG_NAME		"DataSafe"
#define PROG_VERSION	"v1.0"




/**** logging and debugging ****/
// #define PROG_DEBUG




#endif // #ifndef datasafe_h