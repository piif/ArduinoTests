#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define STEP_A 2
#define STEP_B 4
#define STEP_C 3
#define STEP_D 5

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(STEP_A, INPUT_PULLUP);
	pinMode(STEP_B, INPUT_PULLUP);
	pinMode(STEP_C, INPUT_PULLUP);
	pinMode(STEP_D, INPUT_PULLUP);

	Serial.println("setup ok");
}

void loop() {
}
