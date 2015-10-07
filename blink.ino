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

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(13, OUTPUT);

	digitalWrite(13, LOW);
}

void loop() {
	delay(5);
	digitalWrite(13, HIGH);
	delay(5);
	digitalWrite(13, LOW);
}
