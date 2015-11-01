/**
 * "partition"
 * mi do ré sol(x3) sol ré mi do(x3)
 * tempo 120
 * fréquences :
 * sol: 392
 * do : 523
 * ré : 587
 * mi : 659
 */

#define SOL_3 392
#define DO_4  523
#define RE_4  587
#define MI_4  659

/**
 * Principe :
 * - on ne touche pas au mode du timer 0 pour garder millis() & delay()
 * - on touche à son OCRA pour simuler une sortie analogique sur OC0A = D0
 *   => controle du volume
 * - on manipule timer 1 pour établir une fréquence, à 50% de PWM
 *   => controle de la note
 * - utilisation de l'interruption INT0 pour détecter l'appui sur un bouton (D2)
 */

//#define USE_INTERRUPT_INPUT_HANDLER_0 1
//#define USE_INTERRUPT_TIMER_HANDLER_0 1

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include "ArduinoTools.h"
	#include "pwm/pwm.h"
#else
	#include "ArduinoTools.h"
	#include "pwm.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define BUTTON 2
#ifdef __AVR_ATtinyX5__
#define OUT_FREQ PWM_1_A
// ATTiny : D2 = Pin 6
#define BELL_VOL PWM_0_A
// ATTiny : D1 = Pin 5
#else
#define OUT_FREQ PWM_1_B
// Uno : D10
#define BELL_VOL 13
// Uno : Ground.
#endif

#define VOLUME_PWM  0
#define FREQUENCY_PWM  1

uint8_t _tAC_volume[] = {
		255, 100, 80, 67, 60,
		50, 45, 40, 35, 31,
		27, 22, 20, 16, 13,
		11, 7, 5, 4, 3, 2
}; // Duty for linear volume control.

void bell(int f, int duration) {
	if (f == 0) {
		delay(duration);
		return;
	}

	unsigned long frequency = f;
	word top, prescale;
	computePWM(FREQUENCY_PWM, frequency, prescale, top);

	int vd = duration / 20;
	int volume = 20;

	TCNT1 = top;
#ifdef __AVR_ATtinyX5__
	setPWM(FREQUENCY_PWM, top,
			COMPARE_OUTPUT_MODE_1_CLEAR_NONE, top / _tAC_volume[volume],
			COMPARE_OUTPUT_MODE_1_NONE_NONE, 0,
			SET_PWM_1_A, prescale);
#else
	setPWM(FREQUENCY_PWM, 0,
			COMPARE_OUTPUT_MODE_NONE, top,
			COMPARE_OUTPUT_MODE_NORMAL, top / _tAC_volume[volume],
			WGM_1_FAST_OCRA, prescale);
#endif

//	delay(duration);
	while(volume--) {
		delay(vd);
#ifdef __AVR_ATtinyX5__
		OCR1A = top / _tAC_volume[volume];
		setPWM(FREQUENCY_PWM, top,
			COMPARE_OUTPUT_MODE_1_CLEAR_NONE, top / _tAC_volume[volume],
			COMPARE_OUTPUT_MODE_1_NONE_NONE, 0,
			SET_PWM_1_A, prescale);
#else
		OCR1B = top / _tAC_volume[volume];
//		setPWM(FREQUENCY_PWM, 0,
//			COMPARE_OUTPUT_MODE_NONE, top,
//			COMPARE_OUTPUT_MODE_NORMAL, top / _tAC_volume[volume],
//			WGM_1_FAST_OCRA, prescale);
#endif
//		setPWM(VOLUME_PWM, 255,
//				COMPARE_OUTPUT_MODE_NORMAL, volume,
//				COMPARE_OUTPUT_MODE_NONE, 0,
//				1, PWMx_PRESCALER_1);
	}
	delay(duration % 20);

#ifdef __AVR_ATtinyX5__
	setPWM(FREQUENCY_PWM, 0,
			COMPARE_OUTPUT_MODE_1_NONE_NONE, 0,
			COMPARE_OUTPUT_MODE_1_NONE_NONE, 0,
			SET_PWM_1_NONE, 0);
#else
	setPWM(FREQUENCY_PWM, 0,
			COMPARE_OUTPUT_MODE_NONE, 0,
			COMPARE_OUTPUT_MODE_NONE, 0,
			0, prescale);
#endif
}

volatile boolean led = HIGH;

void toggleLed() {
	digitalWrite(BELL_VOL, led);
	led = !led;
}

ISR(INT0_vect) {
//void onButton(int data) {
//	toggleLed();
}

void setup() {
	pinMode(BUTTON, INPUT_PULLUP);
	pinMode(OUT_FREQ, OUTPUT);
	pinMode(BELL_VOL, OUTPUT);
	digitalWrite(OUT_FREQ, LOW);
	digitalWrite(BELL_VOL, LOW);

////	OCR0A = 128;
//	setPWM(VOLUME_PWM, 255,
//			COMPARE_OUTPUT_MODE_NORMAL, 128,
//			COMPARE_OUTPUT_MODE_NONE, 255,
//			WGM_0_FAST_MAX, PWM0_PRESCALER_8);

	toggleLed();
	delay(1000);
	toggleLed();
	delay(100);

//	setPWM(VOLUME_PWM, 255,
//			COMPARE_OUTPUT_MODE_NONE, 255,
//			COMPARE_OUTPUT_MODE_NONE, 255,
//			WGM_0_FAST_MAX, PWM0_PRESCALER_1024);
}

void loop() {
	enableInputInterrupt(INTERRUPT_FOR_PIN(BUTTON), LOW);
	sleepNow(SLEEP_MODE_PWR_DOWN);
	disableInputInterrupt(INTERRUPT_FOR_PIN(BUTTON));

	toggleLed();
	delay(200);
	toggleLed();
	delay(200);

	bell(MI_4,  500); toggleLed();
	bell(DO_4,  500); toggleLed();
	bell(RE_4,  500); toggleLed();
	bell(SOL_3,1000); toggleLed();
	bell(0  ,   500); toggleLed();
	bell(SOL_3, 500); toggleLed();
	bell(RE_4,  500); toggleLed();
	bell(MI_4,  500); toggleLed();
	bell(DO_4, 1000);

//	setPWM(VOLUME_PWM, 255,
//			COMPARE_OUTPUT_MODE_NONE, 128,
//			COMPARE_OUTPUT_MODE_NONE, 255,
//			WGM_0_FAST_OCRA, PWM0_PRESCALER_1024);
//	enableTimerInterrupt(VOLUME_PWM, TIMER_COMPARE_A);
//	sleepNow(SLEEP_MODE_PWR_SAVE);

//	setPWM(VOLUME_PWM, 0,
//			COMPARE_OUTPUT_MODE_NORMAL, 128,
//			COMPARE_OUTPUT_MODE_NONE, 0,
//			WGM_0_FAST_MAX, PWM0_PRESCALER_1024);
//	delay(1000);
//	digitalWrite(BELL_IN, LOW);
//	delay(100);
//	bell(440);
//	delay(1000);
//	bell(220);
//	delay(1000);
}
