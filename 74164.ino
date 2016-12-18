#include <Arduino.h>

#ifdef PIF_TOOL_CHAIN
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
#endif

#ifndef DEFAULT_BAUDRATE
	#ifdef __AVR_ATmega644P__
		#define DEFAULT_BAUDRATE 19200
	#else
		#define DEFAULT_BAUDRATE 115200
	#endif
#endif

#define DATA 2
#define CLOCK 3
#define LED 13

#define DELTA 5
#define PERIOD 1000

void setup() {
	pinMode(DATA, OUTPUT);
	pinMode(CLOCK, OUTPUT);
	pinMode(LED, OUTPUT);

	digitalWrite(DATA, LOW);
	digitalWrite(CLOCK, LOW);
	digitalWrite(LED, LOW);
}

bool flag = HIGH;

void loop() {
	digitalWrite(LED, flag);
	digitalWrite(DATA, flag);
	digitalWrite(CLOCK, HIGH);
	delay(DELTA);
	digitalWrite(CLOCK, LOW);

	flag = !flag;

	delay(PERIOD);
}
