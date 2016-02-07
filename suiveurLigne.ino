#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
#else
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define RECIEVER A0

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(RECIEVER, INPUT);

	Serial.println("Setup ok");
}

void loop() {
	int r = analogRead(RECIEVER);
	Serial.println(r);
	delay(500);
}
