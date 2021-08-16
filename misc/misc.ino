#include "Arduino.h"

void setup() {
	pinMode(13, OUTPUT);
	pinMode(A5, INPUT_PULLUP);
	pinMode(A3, INPUT);
	Serial.begin(115200);
	Serial.println("Ready");
}

void loop() {
//	int v =  analogRead(A5);
	int v =  digitalRead(A5);
	digitalWrite(13, v);
//	Serial.println(v);
}
