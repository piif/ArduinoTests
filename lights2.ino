#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
	// example : #include "led7.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define BLUE   6
#define GREEN  5
#define COMMON 4
#define RED    3

#define POWER 128

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(BLUE  , OUTPUT);
	pinMode(GREEN , OUTPUT);
	pinMode(RED   , OUTPUT);
	pinMode(COMMON, OUTPUT);

	digitalWrite(COMMON, LOW);
	analogWrite (RED, POWER);
	analogWrite (GREEN, POWER);
	analogWrite (BLUE, POWER);
}

int M = 10;
long int k;

void loop() {
	int c = Serial.read();
	if (c == '0') {
		analogWrite (RED, 0);
		analogWrite (GREEN, 0);
		analogWrite (BLUE, 0);
	} else if (c == '1') {
		analogWrite (RED, POWER);
		analogWrite (GREEN, POWER);
		analogWrite (BLUE, POWER);
	}
//	delay(250);
	k = 0;
	for (int i = 0; i < M; i++) {
		delay(5);
		k += analogRead(A0);
	}
	k /= M;
	Serial.println(k);
}
