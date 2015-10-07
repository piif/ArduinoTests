#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include "serialInput/serialInput.h"
	#include "pwm/pwm.h"
#else
	#include "serialInput.h"
	#include "pwm.h"
//	// other includes with full pathes
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define BELL 10
// = PWM_1_B
#define GND 11
// = PWM_2_A

int frequency = 440;

void bell(int f) {
	unsigned long frequency = f;
	word top, prescale;
	computePWM(1, frequency, prescale, top);

	setPWM(1, 0,
			COMPARE_OUTPUT_MODE_NONE, top,
			COMPARE_OUTPUT_MODE_NORMAL, top / 2,
			WGM_1_FAST_OCRA, prescale);
	int volume = 255;
	while(volume--) {
		delay(5);
		setPWM(2, 255,
				COMPARE_OUTPUT_MODE_NORMAL, volume,
				COMPARE_OUTPUT_MODE_NONE, 0,
				WGM_2_FAST_MAX, PWM2_PRESCALER_1);
	}
	setPWM(1, 0,
			COMPARE_OUTPUT_MODE_NONE, 0,
			COMPARE_OUTPUT_MODE_NONE, 0,
			WGM_NORMAL, 0);
	setPWM(2, 0,
			COMPARE_OUTPUT_MODE_NONE, 0,
			COMPARE_OUTPUT_MODE_NONE, 0,
			WGM_NORMAL, 0);
}

InputItem inputs[] = {
	{ 'b', 'I', (void *)bell }
};


void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(BELL, OUTPUT);
	pinMode(GND, OUTPUT);
	digitalWrite(BELL, LOW);
	digitalWrite(GND, LOW);

	registerInput(sizeof(inputs), inputs);
}

void loop() {
	handleInput();
}
