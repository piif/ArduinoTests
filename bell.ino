#include <Arduino.h>
#include "ArduinoTools.h"
#include "pwm/pwm.h"
#ifdef WITHOUT_MILLIS_FUNCTIONS
#include "myMillis/myMillis.h"
#define delay myDelay
#endif

#define RANDOM_ADC

#ifdef __AVR_ATtinyX5__
	#define LED 0
	#define BUTTON 2

	#define FREQUENCY_PWM  1
	#define OUT_FREQ_1 PWM_1_B
	#define OUT_FREQ_2 PWM_NOT_1_B
	// ATTiny : D4 = pin 3 & D3 = pin 2

	#define VOLUME_PWM 0
	#define VOLUME_OCRxB OCR0B
	#define VOLUME_TCCRxx TCCR0A
	#define OUT_VOLUME PWM_0_B
	// ATTiny : D1 = pin 6
#else
	#define LED 13
	#define BUTTON 2
	#define FREQUENCY_PWM  1
	#define OUT_FREQ_1 PWM_1_A
	#define OUT_FREQ_2 PWM_1_B
	// Uno : D9 & 10
#ifdef WITHOUT_MILLIS_FUNCTIONS
	#define VOLUME_PWM 0
	#define VOLUME_OCRxB OCR0B
	#define VOLUME_TCCRxx TCCR0A
	#define OUT_VOLUME PWM_0_B
#else
	#define VOLUME_PWM 2
	#define VOLUME_OCRxB OCR2B
	// Uno : D5
	#define VOLUME_TCCRxx TCCR2A
	#define OUT_VOLUME PWM_2_B
#endif
#endif

#define VOLUME_MAX 32

volatile boolean led = HIGH;

void toggleLed() {
	digitalWrite(LED, led);
	led = !led;
}

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
//	// test
//	{ 60, ">a" },
	// Big Ben
	{ 100, ">+e>+c>+d>_g >g>+d>+e>_+c" },
	// Hedwige theme
	{ 360, "_b__+e+g_+F___+e_+b_____+a_____+F__+e+g_+F___+D_+f__b>_b" },
	// PopCorn
	{ 480, ">+b >+a >+b >+F >+d >+F >b   >+b >+a >+b >+F >+d >+F >b" },
	// Imperial March
	{ 400, "__g>g__g>g__g>g_e>eb__g>g_e>eb__g>g" },
	// Jingle Bell
	{ 400, ">_b>_b>___b>_b>_b>___b>_b>_+d>__ga>___b" }
};
#define NB_SAMPLES (sizeof(samples) / sizeof(struct _sample))

int frequency = 0, tempo = 0;
volatile int currentVolume = VOLUME_MAX;

byte volumes[] = {
	32, 32, 32, 31, 31,
	31, 30, 29, 29, 28,
	27, 26, 24, 23, 21,
	19, 17, 14, 10, 3
};

void setVolumePWM(int volume) {
	if (VOLUME_OCRxB == volume) return;
	VOLUME_OCRxB = volume;
	if (volume == 0) {
		VOLUME_TCCRxx = (VOLUME_TCCRxx & 0x0f)
				| (COMPARE_OUTPUT_MODE_NONE << COM0A0)
				| (COMPARE_OUTPUT_MODE_NONE << COM0B0);
		digitalWrite(OUT_VOLUME, 0);
	} else {
		VOLUME_TCCRxx = (VOLUME_TCCRxx & 0x0f)
						| (COMPARE_OUTPUT_MODE_NONE << COM0A0)
						| (COMPARE_OUTPUT_MODE_NORMAL << COM0B0);
	}
}

void mute() {
	setVolumePWM(0);
	// TODO : shutdown FREQUENCY_PWM also ? or at least set frequency to 0 ?
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
	digitalWrite(OUT_FREQ_1, LOW);
	digitalWrite(OUT_FREQ_2, LOW);
}

void play(int frequency, int duration, short effect) {
//	wdt_reset();
	unsigned long compFrequency = frequency;
	word top, prescale;
	computePWM(FREQUENCY_PWM, compFrequency, prescale, top);

#ifdef __AVR_ATtinyX5__
	setPWM(FREQUENCY_PWM, top,
			COMPARE_OUTPUT_MODE_1_NONE_NONE, 0,
			COMPARE_OUTPUT_MODE_1_CLEAR_NONE, top / 2,
			SET_PWM_1_B, prescale);
#else
	setPWM(FREQUENCY_PWM, top,
		COMPARE_OUTPUT_MODE_NORMAL, top / 2,
		COMPARE_OUTPUT_MODE_INVERTED, top / 2,
		WGM_1_FAST_ICR, prescale);
#endif
	if (TCNT1 > top) {
        // Counter over the top, not good, put within range.
		TCNT1 = top;
	}

//	Serial.print("f ");Serial.print(frequency);
//	Serial.print("e ");Serial.println(effect);

	if (effect == 0) {
		setVolumePWM(VOLUME_MAX);
		delay(duration);
	} else {
		byte step;
		step = 20;
		duration /= step;
		if (effect == -1) {
			while(step--) {
				setVolumePWM(volumes[19 - step]);
				delay(duration);
			}
		} else {
			byte v = 0;
			while(step--) {
				v += VOLUME_MAX / 20;
				setVolumePWM(v);
				delay(duration);
			}
		}
	}
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
			toggleLed();
			mute();
			delay((60L * 1000 / tempo) * duration);
			toggleLed();
		} else {
			int octave = 0;
			if (*notes == '+') {
				octave = 1;
				notes++;
			} else if (*notes == '-') {
				octave = -1;
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

// button interrupt must be present to avoid reset
//ISR(INT0_vect) {}

#ifdef RANDOM_THRU_JITTER
// https://gist.github.com/endolith/2568571#file-readme-md
// doesn't work ?

// Setup of the watchdog timer.
void initRandom() {
  cli();
  MCUSR = 0;

  /* Start timed sequence */
  WDTCR |= _BV(WDCE) | _BV(WDE);

  /* Put WDT into interrupt mode */
  /* Set shortest prescaler(time-out) value = 2048 cycles (~16 ms) */
  WDTCR = _BV(WDIE);

  sei();
}

volatile byte sample;
volatile bool sample_waiting = false;

// Watchdog Timer Interrupt Service Routine
ISR(WDT_vect)
{
  sample = TCNT0; // Ignore higher bits
  sample_waiting = true;
}

byte randByte() {
	byte res = 0;
	byte bits = 0;
	while(bits < 8) {
		while(!sample_waiting);
		sample_waiting = false;
		bool b1 = sample & 1;
		while(!sample_waiting);
		sample_waiting = false;
		bool b2 = sample & 1;
		if (b1 != b2) {
			res |= b2;
			res <<= 1;
			bits++;
		}
	}
	return res;
}
#endif

#ifdef RANDOM_ADC
void initRandom() {
}
byte randByte() {
	byte dummy;

	pinMode(5, INPUT);
	byte res = 0;
	byte msk = 0x01;

	ADCSRA =
		_BV(ADEN) |
		//_BV(ADSC) |
		//_BV(ADATE) |
		//_BV(ADIF) | _BV(ADIE) |
		/*_BV(ADPS2) |*/ _BV(ADPS1) | _BV(ADPS0);

	ADCSRB = 0;
	// BIN ACME IPR – – ADTS2 ADTS1 ADTS0

	ADMUX =
		/*_BV(REFS2) | _BV(REFS1) | _BV(REFS0) |*/
		//_BV(ADLAR) |
		_BV(MUX3) | /*_BV(MUX2) | _BV(MUX1) |*/ _BV(MUX0); // PB5 *20

	while(msk) {
		ADCSRA |= _BV(ADSC);
		while (ADCSRA & _BV(ADSC)) PORTB ^= _BV(5);
		bool b1 = ADCL & 1;
		dummy = ADCH;

		ADCSRA |= _BV(ADSC);
		while (ADCSRA & _BV(ADSC)) PORTB ^= _BV(5);
		bool b2 = ADCL & 1;
		dummy = ADCH;

		if (b1 != b2) {
			if (b2) res |= msk;
			msk <<= 1;
		}
	}
	return res;
}
#endif

int index;

void setup() {
	// everything in input pullup -> should avoid
	// strange behavior with reset pin ...
#ifdef __AVR_ATtinyX5__
	DDRB = 0; PORTB = 0xFF;
#endif

	// do it at startup to hope that pins are still in weird states
//	initRandom();
//	index = randByte() % NB_SAMPLES;
	index=0;

//	ADCSRA &= ~(1<<ADEN);				//turn off ADC
//	ACSR |= _BV(ACD);					//disable the analog comparator
//	MCUCR |= _BV(BODS) | _BV(BODSE);	//turn off the brown-out detector

//#ifndef __AVR_ATtinyX5__
//	Serial.begin(115200);
//	Serial.print("play ");Serial.println(index);
//#endif

//	pinMode(BUTTON, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
	pinMode(OUT_FREQ_1, OUTPUT);
	digitalWrite(OUT_FREQ_1, LOW);
	pinMode(OUT_FREQ_2, OUTPUT);
	digitalWrite(OUT_FREQ_2, LOW);
	pinMode(OUT_VOLUME, OUTPUT);
	digitalWrite(OUT_VOLUME, LOW);

#ifdef __AVR_ATtinyX5__
	// set clock frequency prescaler to 1 => 8MHz
	CLKPR = 0x80; // set high bit to enabled prescale write
	CLKPR = 0x00; // set prescale to 0 = /1
#endif

#ifdef WITHOUT_MILLIS_FUNCTIONS
	initTimerMillis(WGM_0_FAST_OCRA, VOLUME_MAX, 8);
#endif

#ifdef WITHOUT_MILLIS_FUNCTIONS
	VOLUME_TCCRxx = (VOLUME_TCCRxx & 0x0f)
			| (COMPARE_OUTPUT_MODE_NONE << COM0A0)
			| (COMPARE_OUTPUT_MODE_NONE << COM0B0);

#else
	setPWM(VOLUME_PWM, 0,
			COMPARE_OUTPUT_MODE_NONE, VOLUME_MAX,
			COMPARE_OUTPUT_MODE_NONE, 0,
			WGM_0_FAST_OCRA, PWM0_PRESCALER_8);
#endif

//	wdt_enable(7);

	mute();

	toggleLed();
	delay(100);
	toggleLed();
	delay(100);
	toggleLed();
	delay(100);
	toggleLed();
	delay(100);

	tempo = samples[index].tempo;
	playNotes(samples[index].score);
	mute();

	// TODO : send "off" signal to empty capacitor
}

void loop() {
////	wdt_reset();
////	wdt_enable(7);
////	// prepare to program watchdog timer
////	__asm__ __volatile__ ("wdr"); // wdt_reset();
////	WDTCR |= (1<<WDCE) | (1<<WDE);
////	// make it reset after 2 sec.
////	WDTCR = (1<<WDP2) | (1<<WDP1) | (1<<WDP0) | (1<<WDE);
//
//	// disable timers
//
//#if defined PRUSI
//	PRR = PRTIM0 | PRTIM1 | PRUSI | PRADC;
//#else
//	PRR = PRTIM0 | PRTIM1 | PRADC;
//#endif
//
//	enableInputInterrupt(INTERRUPT_FOR_PIN(BUTTON), LOW);
//
//	// wait ...
//	sleepNow(SLEEP_MODE_PWR_DOWN);
//
//	// disable watchdog (or reset may occur before the end of the job)
////	wdt_reset();
////	__asm__ __volatile__ ("wdr");
////	wdt_disable();
////	WDTCR |= (1<<WDCE) | (1<<WDE);
////	WDTCR = 0;
//
//	// disable interrupt to avoid repeated calls
//	disableInputInterrupt(INTERRUPT_FOR_PIN(BUTTON));
//
//	// enable timers (needed for sound an delay())
//#if defined PRUSI
//	PRR = PRUSI | PRADC;
//#else
//	PRR = PRADC;
//#endif
//
//	// do the job
//	toggleLed();
//	delay(200);
//	toggleLed();
//
//	tempo = samples[index].tempo;
//	playNotes(samples[index].score);
//	index = (index + 1) % NB_SAMPLES;
//
//	// loop back to start
}
