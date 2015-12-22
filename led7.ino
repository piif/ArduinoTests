#include <Arduino.h>
#include <led7/xx164.h>

/**
 * Test with 74LS164 and 7 segments common anode display
 */

#include <MsTimer2/MsTimer2.h>

xx164 *display = 0;

void send() {
	display->send();
}

volatile byte pins[] = { 4 };

void setup() {
	Serial.begin(115200);
	display = new xx164(1, pins);

	MsTimer2::set(10, send);
	MsTimer2::start();

	Serial.println("Ready");
}

void loop() {
	int v = Serial.read();
	if (v != -1) {
		char s[2] = { (char)v, 0 };
		Serial.println(s);
		display->display(s);
	}
//	display->send();
//	delay(500);

//	Serial.println(display->current);
//	Serial.println(display->toSend[0]);
}
