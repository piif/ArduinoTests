#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
#else
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define IN_1 A0
#define IN_2 A1

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(IN_1, INPUT_PULLUP);
	pinMode(IN_2, INPUT_PULLUP);

	Serial.println("Setup ok");
}

void loop() {
	int i1 = analogRead(IN_1);
	int i2 = analogRead(IN_2);
	Serial.print(i1);
	Serial.print('\t');
	Serial.println(i2);
	delay(500);
}
