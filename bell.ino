
// TODO :
// - faire marcher la sonnette en Uno sur timer 2
// - idem sur timer 0 => manips sur delay (comment surcharger TIMER0_OVF ?)
// - ensuite sur tiny

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
//	#include <avr/wdt.h>
	#include "ArduinoTools.h"
	#include "pwm/pwm.h"
//	#include <avr/pgmspace.h>
#else
	#include "ArduinoTools.h"
	#include "pwm.h"
#endif


#ifdef __AVR_ATtinyX5__
	#define FREQUENCY_PWM  1
	#define VOLUME_PWM 0
	#define OUT_FREQ ??
	// ATTiny : ?? D1 = Pin 6
	#define OUT_VOLUME ??
	// ATTiny : ??
	#define LED 0
	#define BUTTON 2
#else
	#define FREQUENCY_PWM  1
	#define VOLUME_PWM 2
	#define OUT_FREQ PWM_1_B
	// Uno : D10
	#define OUT_VOLUME PWM_2_B
	// Uno : D3
	#define REGISTER_VOLUME_TOP OCR2B
	#define LED 13
	#define BUTTON 2
#endif

#define VOLUME_MAX 100

int freqForNote(char name, short octave) {
	int f = 0;
	switch(name) {
		case 'c': f = 262; break;
		case 'C': f = 277; break;
		case 'd': f = 294; break;
		case 'D': f = 311; break;
		case 'e': f = 330; break;
		case 'f': f = 349; break;
		case 'F': f = 370; break;
		case 'g': f = 392; break;
		case 'G': f = 415; break;
		case 'a': f = 440; break;
		case 'A': f = 466; break;
		case 'b': f = 494; break;
	}
	if (octave > 0) {
		f <<= octave;
	} else if (octave < 0) {
		f >>= (-octave);
	}
	return f;
}

// TODO : put in prog mem
const struct _sample {
	int tempo;
	char *score;
} samples[] = {
	// Big Ben
	{ 120, ">+e>+c>+d>_g >g>+d>+e>_+c" },
	// Hedwge theme
	{ 360, "_b__+e+g_+F___+e_+b_____+a_____+F__+e+g_+F___+D_+f__b>_b" },
	// PopCorn
	{ 480, ">+b >+a >+b >+F >+d >+F >b   >+b >+a >+b >+F >+d >+F >b" },
	// Imperial March
	{ 400, "__g>g__g>g__g>g_e>eb__g>g_e>eb__g>g" }
};
#define NB_SAMPLES (sizeof(samples) / sizeof(struct _sample))

int frequency = 440, tempo = 120;
volatile int currentVolume = 0;

void play(int frequency, int duration, short effect) {
//	wdt_reset();
	unsigned long compFrequency = frequency;
	word top, prescale;
	computePWM(FREQUENCY_PWM, compFrequency, prescale, top);

#ifdef __AVR_ATtinyX5__
	setPWM(FREQUENCY_PWM, top,
			COMPARE_OUTPUT_MODE_1_CLEAR_NONE, top / 2,
			COMPARE_OUTPUT_MODE_1_NONE_NONE, 0,
			SET_PWM_1_A, prescale);
#else
	setPWM(FREQUENCY_PWM, 0,
			COMPARE_OUTPUT_MODE_NONE, top,
			COMPARE_OUTPUT_MODE_NORMAL, top / 2,
			WGM_1_FAST_OCRA, prescale);
#endif

	if (effect == 0) {
		currentVolume = VOLUME_MAX;
		delay(duration);
	} else {
		byte step, delta;
		if (effect == -1) {
			currentVolume = VOLUME_MAX;
			step = 20;
			delta = -VOLUME_MAX / step;
		} else {
			currentVolume = 0;
			step = 10;
			delta = VOLUME_MAX / step;
		}
		duration /= step;

		while(step --) {
			currentVolume += delta;
			delay(duration);
		}
	}

}

void mute() {
#ifdef __AVR_ATtinyX5__
	setPWM(FREQUENCY_PWM, 0,
			COMPARE_OUTPUT_MODE_1_NONE_NONE, 0,
			COMPARE_OUTPUT_MODE_1_NONE_NONE, 0,
			0, 0);
#else
	setPWM(FREQUENCY_PWM, 0,
		COMPARE_OUTPUT_MODE_NONE, 0,
		COMPARE_OUTPUT_MODE_NONE, 0,
		0, 0);
#endif
	currentVolume = 0;
}

void playNotes(char *notes) {
	short duration = 1;
	short effect = 0; // -1 for > / 0 for flat / 1 for <
	while(*notes) {
		if (*notes == '>') {
			effect = -1;
			notes++;
			continue;
		} else if (*notes == '<') {
			effect = 1;
			notes++;
			continue;
		} else if (*notes == '_') {
			// add one time to next note
			duration++;
			notes++;
			continue;
		} else if (*notes == ' ') {
			// silence
			mute();
			delay((60L * 1000 / tempo) * duration);
		} else {
			int octave = 0;
			if (*notes == '+') {
				octave = 1;
				notes++;
			}
			play(freqForNote(*notes, octave), (60L * 1000 / tempo) * duration, effect);
			if (tempo == 0) {
				// tempo 0 => play first note forever.
				return;
			}
		}
		duration = 1;
		effect = 0;
		notes++;
	}
	mute();
}

volatile boolean led = HIGH;

void toggleLed() {
	digitalWrite(LED, led);
	led = !led;
}

// stop volume pwm when frequency pwm is low
#ifdef __AVR_ATtinyX5__
ISR(TIMER1_COMPA_vect) {
#else
ISR(TIMER1_COMPB_vect) {
#endif
	OCR2A = 0;
	OCR2B = 0;
}
// launch volume pwm when frequency pwm is high
#ifdef __AVR_ATtinyX5__
ISR(TIMER1_OVF_vect) {
#else
ISR(TIMER1_COMPA_vect) {
#endif
	OCR2A = VOLUME_MAX;
	OCR2B = VOLUME_MAX - currentVolume;
}
//volatile long ticks;
//ISR(TIMER0_OVF_vect) {
//	ticks++;
//}

ISR(INT0_vect) {}

void setup() {
	pinMode(BUTTON, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
	pinMode(OUT_FREQ, OUTPUT);
	digitalWrite(OUT_FREQ, LOW);
	pinMode(OUT_VOLUME, OUTPUT);
	digitalWrite(OUT_VOLUME, LOW);

	mute();

	ADCSRA &= ~(1<<ADEN);				//turn off ADC
	ACSR |= _BV(ACD);					//disable the analog comparator
	MCUCR |= _BV(BODS) | _BV(BODSE);	//turn off the brown-out detector

//	wdt_enable(7);

#ifdef __AVR_ATtinyX5__
	TIMSK1 = (1<<OCIE1A) | (1<<TOIE1);
#else
	TIMSK1 = (1<<OCIE1A) | (1<<OCIE1B);
#endif
//	TIMSK0 = (1<<TOIE0);

	toggleLed();
	delay(2000);
	toggleLed();
}

int index = 0;

void loop() {
//	wdt_reset();
//	wdt_enable(7);
//	// prepare to program watchdog timer
//	__asm__ __volatile__ ("wdr"); // wdt_reset();
//	WDTCR |= (1<<WDCE) | (1<<WDE);
//	// make it reset after 2 sec.
//	WDTCR = (1<<WDP2) | (1<<WDP1) | (1<<WDP0) | (1<<WDE);

	// disable timers
#if defined PRUSI
	PRR = PRTIM0 | PRTIM1 | PRUSI | PRADC;
#else
	PRR = PRTIM0 | PRTIM1 | PRADC;
#endif

	enableInputInterrupt(INTERRUPT_FOR_PIN(BUTTON), LOW);

	// wait ...
	sleepNow(SLEEP_MODE_PWR_DOWN);

	// disable watchdog (or reset may occur before the end of the job)
//	wdt_reset();
//	__asm__ __volatile__ ("wdr");
//	wdt_disable();
//	WDTCR |= (1<<WDCE) | (1<<WDE);
//	WDTCR = 0;

	// disable interrupt to avoid repeated calls
	disableInputInterrupt(INTERRUPT_FOR_PIN(BUTTON));

	// enable timers (needed for sound an delay())
#if defined PRUSI
	PRR = PRUSI | PRADC;
#else
	PRR = PRADC;
#endif

	// do the job
	toggleLed();
	delay(200);
	toggleLed();

	tempo = samples[index].tempo;
	playNotes(samples[index].score);
	index = (index + 1) % NB_SAMPLES;

	// loop back to start
}
