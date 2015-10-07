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
byte volume = 10;

byte getPrescale(byte timer, int divider) {
#if defined __AVR_ATmega32U4__
	if (timer == 4) {
		switch (divider) {
		case     1: return PWM4_PRESCALER_1;
		case     2: return PWM4_PRESCALER_2;
		case     4: return PWM4_PRESCALER_4;
		case     8: return PWM4_PRESCALER_8;
		case    16: return PWM4_PRESCALER_16;
		case    32: return PWM4_PRESCALER_32;
		case    64: return PWM4_PRESCALER_64;
		case   128: return PWM4_PRESCALER_128;
		case   256: return PWM4_PRESCALER_256;
		case   512: return PWM4_PRESCALER_512;
		case  1024: return PWM4_PRESCALER_1024;
		case  2048: return PWM4_PRESCALER_2048;
		case  4096: return PWM4_PRESCALER_4096;
		case  8192: return PWM4_PRESCALER_8192;
		case 16384: return PWM4_PRESCALER_16384;
		}
	} else
#endif
	if (timer == 2) {
		switch (divider) {
		case    1: return PWM2_PRESCALER_1;
		case    8: return PWM2_PRESCALER_8;
		case   32: return PWM2_PRESCALER_32;
		case   64: return PWM2_PRESCALER_64;
		case  128: return PWM2_PRESCALER_128;
		case  256: return PWM2_PRESCALER_256;
		case 1024: return PWM2_PRESCALER_1024;
		}
	} else {
		switch (divider) {
		case 1   : return PWMx_PRESCALER_1;
		case 8   : return PWMx_PRESCALER_8;
		case 64  : return PWMx_PRESCALER_64;
		case 256 : return PWMx_PRESCALER_256;
		case 1024: return PWMx_PRESCALER_1024;
		}
	}
	return 0;
}

// compute nearest prescale and top value for given timer to achieve wanted frequency
// outputs rounded frequency
void computePWM(byte timer, unsigned long &frequency, word &prescale, word &top) {
	// compute ticks number
	unsigned long ticks = F_CPU / frequency;

#if defined __AVR_ATmega32U4__
	word timerMax = (timer == 1 || timer == 3) ? 65536 : 256;
#else
	word timerMax = (timer == 1) ? 65535 : 255;
#endif
	Serial.print("ticks "); Serial.println(ticks);
	Serial.print("timerSize "); Serial.println(timerMax);

	if (ticks <= timerMax) {
		prescale = getPrescale(timer, 1);
		top = ticks;
		return;
	}
	// compute nearest prescale
	prescale = 0;
	word p = 1;
	while (prescale == 0 || ticks > timerMax) {
		ticks >>= 1;
		p <<= 1;
		Serial.print("ticks "); Serial.println(ticks);
		Serial.print("p "); Serial.println(p);
		prescale = getPrescale(timer, p);
		Serial.print("prescale "); Serial.println(prescale);
	}
	top = ticks;
	frequency = (p * ticks) / F_CPU;
}

void bell(int f) {
	unsigned long frequency = f;
	word top, prescale;
	computePWM(1, frequency, prescale, top);

	Serial.print("freq "); Serial.println(frequency);
	Serial.print(" cs "); Serial.println(prescale);
	Serial.print(" top "); Serial.println(top);

	setPWM(1, 0,
			COMPARE_OUTPUT_MODE_NONE, top,
			COMPARE_OUTPUT_MODE_NORMAL, top / 2,
			WGM_1_FAST_OCRA, prescale);
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

// TODO : volume doit être un autre PWM, bcp plus rapide, avec un duty cycle
// = volume
// alors que ocra = icr/2 pour avoir une oscillation à f, et pas f/2
InputItem inputs[] = {
//	{ 'f', 'i', (void *)(&frequency) },
	{ 'v', 'i', (void *)(&volume) },
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
