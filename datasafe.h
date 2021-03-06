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
//#include <SPI.h>      
//#include <Wire.h>

// project internal libraries
#include <datasafe_screen.h>

// libraries from https://github.com/bboortz/arduino-libs
//#include <LogLib.h>
#include <NFCLib.h>
#include <SDLib.h>




/**** program settings ****/
#define PROG_NAME		        "DataSafe"
#define PROG_VERSION                    "v1.0"




/**** logging and debugging ****/
// #define PROG_VERBOSE
// #define PROG_DEBUG
// #define PROG_ANALYZE_MEM

#ifdef PROG_ANALYZE_MEM
        #include <MemoryFree.h>
#endif // #ifdef PROG_ANALYZE_MEM




/**** messages ****/

#define DATASAFE_MESSAGE_000              "\r\n---------- STARTED / RESET ----------"
#define DATASAFE_MESSAGE_001              "next state: "
#define DATASAFE_MESSAGE_002              " => "
#define DATASAFE_MESSAGE_003              "free memory = "




#endif // #ifndef datasafe_h
