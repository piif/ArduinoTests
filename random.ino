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

byte randByte() {
	pinMode(A0, INPUT);
	byte res = 0;
	byte bits = 0;
	while(bits < 8) {
		bool b1 = analogRead(A0);
		bool b2 = analogRead(A0);
		if (b1 != b2) {
			res <<= 1;
			res |= b2;
			bits++;
		}
	}
	return res;
}

byte occurs[256] = { 0, };

void setup(void) {
	pinMode(A0, INPUT);
	byte res = randByte();
	Serial.begin(DEFAULT_BAUDRATE);
	Serial.print("init : ");
	Serial.print(res, HEX);
	Serial.print(" / ");
	Serial.println(rand(), HEX);

	unsigned long before = millis();
	for (int i = 0; i < 500; i++) {
		occurs[randByte()]++;
		if (i % 100 == 0) {
			Serial.print('.');
		}
	}
	unsigned long after = millis();
	Serial.println();
	Serial.print("time : "); Serial.println(after - before);
	for (int i = 0; i < 256; i+=16) {
		for (int j = 0; j < 16; j++) {
			Serial.print(occurs[i + j], HEX);
		}
		Serial.println();
	}
}

void loop() {
	delay(1000);

	byte res = randByte();
	Serial.print(res, HEX);
	Serial.print(" / ");
	Serial.println(rand(), HEX);
}
