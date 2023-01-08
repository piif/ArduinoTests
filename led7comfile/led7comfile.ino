#include <Arduino.h>
#include <Wire.h>


#include "csg4m.h"

/**
 * Test with comfile CSG-4M 4 digits display
 */

CSG4M *display = 0;

void setup() {
	display = new CSG4M(0x70);

	display->display("1234");

//	Serial.begin(115200);
//	Serial.println("Ready");
}

void loop() {
//	int v = Serial.read();
//	if (v != -1) {
//		char c = (char)v;
//		Serial.println(c);
//		display->shift(c);
//	}
}
