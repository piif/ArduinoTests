#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
#else
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define SERVO 3
void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	// COM2A = 0 , COM2B = 2 , WGM = 3 , CS = 7
	TCCR2A = 0x23;
	TCCR2B = 0x07;
	OCR2B = 96;

	pinMode(SERVO, OUTPUT);

	Serial.println("setup ok");
}

void loop() {
	int v= Serial.parseInt();
	if (v != 0) {
		OCR2B = v;
	}
}
