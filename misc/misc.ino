#include "Arduino.h"

void setup() {
	pinMode(A0, INPUT);
	Serial.begin(115200);
	Serial.println("Ready");
}

byte decode(int v) {
	if (v > 800) {
		return 3;
	}
	if (v > 600) {
		return 2;
	}
	if (v > 400) {
		return 1;
	}
	return 0;
}

byte button = 0;

void loop() {
	int v =  analogRead(A0);
	Serial.println(v);
	byte newButton = decode(v);
	if (newButton != button) {
		delay(100);
		v =  analogRead(A0);
		newButton = decode(v);
		if (newButton != button) {
			button = newButton;
			Serial.println(button);
		}
	}
	delay(500);
}
