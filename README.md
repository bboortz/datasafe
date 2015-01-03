datasafe
========

# About

datasafe is a software you can run on an arduino to have a safe storage for you data. 
The objective is to build a hardware device withit you can store your data safely. 
The device has only a touchscreen display and a nfc interface for login. 
Your data is stored on a sd card in an encrypted way. There are no other interfaces.



# Current Features

!! This software is currently only a click dummy. I'm working on it !!



# Planned Features

see IDEAS.md



# Hardware Requirements

* Arduino 		(tested with an UNO r3)
* NFC Reader/Writer 	(tested with adafruit PN532 RFID/NFC Shield 13.56MHz)
* Touchscreen Display 	(tested with adafruit 2.8" TFT LCD shield w/Touchscreen)



# Software Requirements 

* arduino-core 			(http://www.arduino.cc)
* bboortz/arduino-libs 		(https://github.com/bboortz/arduino-libs)
* Adafruit_GFX			(https://github.com/adafruit/Adafruit-GFX-Library)
* Adafruit_ILI9341		(https://github.com/adafruit/Adafruit_ILI9341)
* Adafruit_STMPE610		(https://github.com/adafruit/Adafruit_STMPE610)
* Adafruit_NFCShield_I2C	(https://github.com/adafruit/Adafruit_NFCShield_I2C)
* MemoryFree			(https://github.com/maniacbug/MemoryFree) [if PROG_ANALYZE_MEM is defined - disabled by default]



# Installation

* Install the development sofware
$ apt-get insall arduino

* Copy the required libraries
* Copy the datasafe software



# Compiled and tested with this development environment

* Debian Wheezy
* Arduino 1.0.5
* UECIDE 0.8.7


