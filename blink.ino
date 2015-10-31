#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif


#if defined __AVR_ATtinyX5__
#define LED 4
#else
#define LED 13
#endif

#define PERIOD 100

void setup(void) {
#if !defined __AVR_ATtinyX5__
	Serial.begin(DEFAULT_BAUDRATE);
#endif

	pinMode(LED, OUTPUT);

	digitalWrite(LED, LOW);
}

void loop() {
	delay(PERIOD);
	digitalWrite(LED, HIGH);
	delay(PERIOD);
	digitalWrite(LED, LOW);
}

/*
TODO : revoir la ligne de commande depuis l'IDE Pour "Arduino as ISP"
et voir comment l'intégrer dans ArdDude

./opt/arduino-1.6.5-r5/hardware/tools/avr/bin/avrdude
  -C/opt/arduino-1.6.5-r5/hardware/tools/avr/etc/avrdude.conf
  -v
  -pattiny85
  -cstk500v1
  -P/dev/ttyACM0
  -b19200
  -Uflash:w:/tmp/build2267963526209929137.tmp/Blink.cpp.hex:i

*/
