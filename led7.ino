#include <Arduino.h>
#include <led7/xx164.h>

/**
 * Test with 74LS164 and 7 segments common anode display
 */

#define DATA 2
#define CLOCK 3

void send(byte value) {
	shiftOut(DATA, CLOCK, LSBFIRST, ~value);
}

xx164 *display = 0;

void setup() {
	Serial.begin(115200);
	display = new xx164();
	Serial.println("Ready");
}

void loop() {
	int v = Serial.read();
	if (v != -1) {
		char s[2] = { (char)v, 0 };
		Serial.println(s);
		display->display(s);
	}
}
