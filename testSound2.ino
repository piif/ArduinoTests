#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include "ArduinoTools.h"
	#include "pwm/pwm.h"
	#include "serialInput/serialInput.h"
//	#include "arduino-toneac/toneAC.h"
#else
	#include "ArduinoTools.h"
	#include "pwm.h"
#endif

//#define WITH_TONE_AC

#include "toneAC/toneAC.h"

// Duty for linear volume control. from toneAC
//static byte volumeScales[] = { /*200,*/ 100, 67, 50, 40, 33, 29, 22, 11, 5, 2 };
static int volumeScales[] = { /*5,*/ 10, 30, 20, 25, 30, 33, 45, 90, 200, 500 };

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define OUT_FREQ_1 PWM_1_A
#define OUT_FREQ_2 PWM_1_B
// Uno : PWM_1_B = D10, PWM_2_B = D3
#define FREQUENCY_PWM  1
#define VOLUME_MAX 20
#define VOLUME_PWM 2
#define OUT_VOLUME PWM_2_B

//const int noteFreqs[] = {
////   C    C#   D    D#   E    F    F#   G    G#   A    A#   B
//	262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, // octave 0
//	523, 554, 587, 622, 659, 699, 740, 784, 831, 880, 932, 988  // octave 1
//};
//const char noteNames[] = "cCdDefFgGaAb";

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

int frequency = 440, tempo = 120;
byte volume = VOLUME_MAX;
int dutyFactor = 500;
char soundMode = 'E'; // H = half, F = full, T = toneAC lib

void play() {
	if (volume == 0 || frequency == 0) {
		return;
	}

	if (soundMode == 'T') {
		toneAC(frequency, volume, 0, 1);
		return;
	}

	unsigned long compFrequency = frequency;
//	if (full) {
//		compFrequency <<= 1; // phase => double frequency
//	}
	word top, prescale;
	computePWM(FREQUENCY_PWM, compFrequency, prescale, top);

	// volume max = 100, but duty cycle max = 50% => volume / 100 / 2
	word volumeTopA;
	word volumeTopB;
	if (soundMode == 'E') {
		volumeTopA = volumeTopB = top >> 1;
	} else {
		volumeTopA = (unsigned long)top * dutyFactor / 1000L;
		if (soundMode == 'F') {
			// set B same time than A, but at end of duty cycle
			// implies to divide duration by 2
			volumeTopA >> 1;
			volumeTopB = top - volumeTopA;
		} else {
			volumeTopB = 0;
		}
	}

	Serial.print("asked frequency : "); Serial.println(frequency);
	Serial.print("comp. frequency : "); Serial.println(compFrequency);
	Serial.print("prescale : "); Serial.println(prescale);
	Serial.print("top : "); Serial.println(top);
	Serial.print("volume : "); Serial.println(volume);
	Serial.print("dutyFactor : "); Serial.println(dutyFactor);
	Serial.print("volumeTopA : "); Serial.println(volumeTopA);
	Serial.print("volumeTopB : "); Serial.println(volumeTopB);

	if (TCNT1 > top) {
        // Counter over the top, not good, put within range.
		TCNT1 = top;
	}
	if (soundMode == 'E') {
		setPWM(FREQUENCY_PWM, top,
			COMPARE_OUTPUT_MODE_NORMAL, volumeTopA,
			COMPARE_OUTPUT_MODE_INVERTED, volumeTopB ,
			WGM_1_FAST_ICR, prescale);
	} else if (soundMode == 'F') {
		setPWM(FREQUENCY_PWM, top,
			COMPARE_OUTPUT_MODE_NORMAL, volumeTopA,
			COMPARE_OUTPUT_MODE_INVERTED, volumeTopB ,
			WGM_1_FAST_ICR, prescale);
	} else {
		digitalWrite(OUT_FREQ_2, LOW);
		setPWM(FREQUENCY_PWM, top,
			COMPARE_OUTPUT_MODE_NORMAL, volumeTopA,
			COMPARE_OUTPUT_MODE_NONE, 0,
			WGM_1_FAST_ICR, prescale);
	}
}

void play(int duration, short effect) {
	if (effect == 0) {
		play();
		delay(duration);
	} else {
		duration /= 10;
		if (soundMode == 'E') {
			play();
			if (effect == -1) {
				byte step = VOLUME_MAX;
				while(step > VOLUME_MAX/2) {
					OCR2B = step;
					delay(duration);
					step -= 1;
				}
			} else {
				byte step = 0;
				do {
					step += 1;
					OCR2B = step;
					delay(duration);
				} while(step < 10);
			}
		} else {
			int backup = dutyFactor;
			if (effect == -1) {
				byte step = 10;
				do {
					step--;
					dutyFactor = volumeScales[step];
					play();
					delay(duration);
				} while(step > 0);
			} else {
				byte step = 0;
				while(step < 10) {
					dutyFactor = volumeScales[step];
					play();
					delay(duration);
					step++;
				}
			}
			dutyFactor = backup;
		}
	}
}

void initVolume() {
	if (volume == 0) {
		if (soundMode == 'T') {
			noToneAC();
		} else {
			setPWM(FREQUENCY_PWM, 0,
				COMPARE_OUTPUT_MODE_NONE, 0,
				COMPARE_OUTPUT_MODE_NONE, 0,
				0, 0);
			setPWM(VOLUME_PWM, 0,
					COMPARE_OUTPUT_MODE_NONE, 0,
					COMPARE_OUTPUT_MODE_NONE, 0,
					0, 0);
			if (soundMode == 'E' || soundMode == 'F') {
				digitalWrite(OUT_FREQ_1, HIGH);
				digitalWrite(OUT_FREQ_2, HIGH);
				digitalWrite(OUT_VOLUME, LOW);
			} else {
				digitalWrite(OUT_FREQ_1, LOW);
				digitalWrite(OUT_FREQ_2, LOW);
				digitalWrite(OUT_VOLUME, LOW);
			}
		}
	} else {
		if (soundMode == 'E') {
			setPWM(VOLUME_PWM, 0,
					COMPARE_OUTPUT_MODE_NONE, VOLUME_MAX,
					COMPARE_OUTPUT_MODE_NORMAL, 0,
					WGM_2_FAST_OCRA, PWM2_PRESCALER_8);
		} else {
			// T, F and H modes don't use enable pin => set to 1.
			digitalWrite(OUT_VOLUME, HIGH);
		}
	}
}

void setVolume(int v) {
	volume = constrain(v, 0, VOLUME_MAX);
	initVolume();

	if (soundMode == 'E') {
		OCR2B = volume;
	} else {
		dutyFactor = volumeScales[volume - 1];
	}
}

void setDutyFactor(int f) {
	dutyFactor = constrain(f, 0, 500);
	play();
}

byte oldVolume = volume;
void mute() {
	oldVolume = volume;
	setVolume(0);
}
void unmute() {
	setVolume(oldVolume);
	play();
}

void setNote(char *n) {
	frequency = freqForNote(*n, 0);
	unmute();
	play();
}

void setFrequency(int f) {
	frequency = f;
	unmute();
	play();
}

void setMode(char *mode) {
	switch(mode[0]) {
	case 'T':
	case 'H':
	case 'F':
	case 'E':
		soundMode = mode[0];
	}
	initVolume();
	if (volume > 0) {
		play();
	}
}

void playNotes(char *notes) {
	Serial.print("Playing "); Serial.println(notes);
	short duration = 1;
	short effect = 0; // -1 for > / 0 for flat / 1 for <
	unmute();
	while(*notes) {
//		Serial.print(" -> "); Serial.println(*notes);
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
			unmute();
		} else {
			int octave = 0;
			if (*notes == '+') {
				octave = 1;
				notes++;
			}
			frequency = freqForNote(*notes, octave);

			if (tempo == 0) {
				// tempo 0 => play first note forever.
				play();
				return;
			} else {
				play((60L * 1000 / tempo) * duration, effect);
			}
		}
		duration = 1;
		effect = 0;
		notes++;
	}
	Serial.println("End.");
	mute();
}

void music(int index) {
	tempo = samples[index].tempo;
	playNotes(samples[index].score);
}

void test(int t) {
	mute();
	if (t & 1) {
		Serial.println("Enable ON");
		digitalWrite(OUT_VOLUME, HIGH);
	} else {
		Serial.println("Enable OFF");
		digitalWrite(OUT_VOLUME, LOW);
	}
	if (t & 2) {
		Serial.println("Freq 1 ON");
		digitalWrite(OUT_FREQ_1, HIGH);
	} else {
		Serial.println("Freq 1 OFF");
		digitalWrite(OUT_FREQ_1, LOW);
	}
	if (t & 4) {
		Serial.println("Freq 2 ON");
		digitalWrite(OUT_FREQ_2, HIGH);
	} else {
		Serial.println("Freq 2 OFF");
		digitalWrite(OUT_FREQ_2, LOW);
	}
}

void help() {
#ifdef WITH_TONE_AC
	Serial.println("WITH ToneAC VERSION");
#else
	Serial.println("HOME MADE VERSION");
#endif
	Serial.println("f frequency");
	Serial.println("n note name");
	Serial.println("v volume [0..10]");
	Serial.println("d duty factor [0..500]");
	Serial.println("M H/F/T : mode Full/Half/ToneAC lib");
	Serial.println("1 (one) sound on");
	Serial.println("0 (zero) sound off");

	Serial.println("N note name list");
	Serial.println("t tempo (notes per minute)");
	Serial.println("m music index");

	Serial.print("mode   "); Serial.println(soundMode);
	Serial.print("volume "); Serial.println(volume);
	Serial.print("dutyFactor "); Serial.println(dutyFactor);
	Serial.print("freq   "); Serial.println(frequency);
}

InputItem inputs[] = {
	{ 'M', 'S', (void *)setMode },
	{ 'f', 'I', (void *)setFrequency },
	{ 'v', 'I', (void *)setVolume },
	{ 'n', 'S', (void *)setNote },
	{ '1', 'S', (void *)unmute },
	{ '0', 'S', (void *)mute },
	{ 'd', 'I', (void *)setDutyFactor },
	{ 'T', 'I', (void *)test },

	{ 'N', 'S', (void *)playNotes },
	{ 't', 'i', (void *)(&tempo) },
	{ 'm', 'I', (void *)music },

	{ '?', 'I', (void *)help }
};

//ISR(TIMER1_COMPA_vect) {
//	OCR2A = 0;
//	OCR2B = 0;
//}
//ISR(TIMER1_OVF_vect) {
//	OCR2A = VOLUME_MAX;
//	OCR2B = VOLUME_MAX - currentVolume;
//}
//volatile long ticks;
//ISR(TIMER0_OVF_vect) {
//	ticks++;
//}

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);
	pinMode(OUT_FREQ_1, OUTPUT);
	pinMode(OUT_FREQ_2, OUTPUT);
	pinMode(OUT_VOLUME, OUTPUT);

	mute();

//	TIMSK1 = (1<<OCIE1A) | (1<<TOIE1);
//	TIMSK0 = (1<<TOIE0);

	registerInput(sizeof(inputs), inputs);
	help();
}

void loop() {
	handleInput();
}
