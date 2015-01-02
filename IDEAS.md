
Features (currently not implemented)
========

* all saved data must be encrypted
* identify the user with his nfc card
** the data on this card is encrypted and can be decrypted with the nfc key
* store keys on several places (this 
** flash memory (saved during compile time) - unique for compile
** eeprom (saved during first start up) - unique for hardware device
** sd card (saved during first data save) - unique for sd card
** nfc card (saved during initiliazation. The data on this card must be decrypted by the nfc key) - unique for user/nfc card
** user password (never saved!) - unique for user
** CRYPTO_KEY = FLASH+EEPROM+SD+NFC+USERPASS
* a better touchscreen
** more precise touchscreen
** key keyboard size adapts by the screen size and screen rotation
* let the user keyboard rotate. Therefore it isn't so easy for an atacker to map the grease spots to the keyboard.
* substitute some keyboard keys. Therefore it isn't so easy for an atacker to map the grease spots to the keyboard.


Needed Testcases
========

* Click Trough 
* Fuzzy testing on all interfaces (Touchscreen, SD, NFC, EEPROM, Passwords/Keys)
* Fuzzy testing all used libraries/classes
* Testing the state pattern (program internal - see loop())
* static code analysis: e.g. splint(.py)/cppcheck/cpplint


Possible targets to hack the devices
========

* steal the data on the nfc card
* steal the nfc key
** decrypt the data
** write some other data to the nfc and exploit a bug (e.g. buffer overflow)
** fake a user on the nfc card
* steal the sd card
** steal the sd card key
** decrypt the data
** write some other data to the sd card and exploit a bug (e.g. buffer overflow)
* steal the whole device
** read the key from the eeprom
** write some other data to the eeprom and exploit a bug (e.g. buffer overflow)
** read the key from the flash memory (sketch)
** write some other data to the flash memory and exploit a bug (e.g. buffer overflow)
* steal the user password
* exploit the device with the touchscreen
* read the hardware pins an sniff the crypto key (CRYPTO_KEY)

