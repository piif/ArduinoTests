/**
 */

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include "ArduinoTools.h"
	#include "pwm/pwm.h"
	#include "serialInput/serialInput.h"
#else
	#include "ArduinoTools.h"
	#include "pwm.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define OUT_FREQ PWM_2_B
// Uno : PWM_1_B = D10, PWM_2_B = D3
#define FREQUENCY_PWM  2
#define REGISTER_VOLUME_TOP OCR2B

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

struct sample {
	int tempo;
	char *score;
} samples[] = {
	// Big Ben
	{ 120, ">+e>+c>+d>_g >g>+d>+e>_+c" },
	// Hedwge theme
	{ 360, "_b__+e+g_+F___+e_+b_____+a_____+F__+e+g_+F___+D_+f__b>_b" }
};

int frequency = 440, volume = 100, tempo = 120;
boolean soundOn = false;

void play() {
	unsigned long compFrequency = frequency;
	word top, prescale;
	computePWM(FREQUENCY_PWM, compFrequency, prescale, top);

	// volume is in [ 0..100 ], but OCRxB must be in [ 0..ACRxA / 2 ]
	// thus, volumeTop = top * (volume / 2)%
	unsigned long volumeTop = (long)top * volume / 200;

	Serial.print("asked frequency : "); Serial.println(frequency);
	Serial.print("comp. frequency : "); Serial.println(compFrequency);
	Serial.print("prescale : "); Serial.println(prescale);
	Serial.print("top : "); Serial.println(top);
	Serial.print("volume : "); Serial.println(volume);
	Serial.print("volumeTop : "); Serial.println(volumeTop);

	setPWM(FREQUENCY_PWM, 0,
			COMPARE_OUTPUT_MODE_NONE, top,
			COMPARE_OUTPUT_MODE_NORMAL, volumeTop,
			WGM_3_FAST_OCRA, prescale);
}

void play(int duration, short effect) {
	unsigned long compFrequency = frequency;
	word top, prescale;
	computePWM(FREQUENCY_PWM, compFrequency, prescale, top);

	setPWM(FREQUENCY_PWM, 0,
			COMPARE_OUTPUT_MODE_NONE, top,
			COMPARE_OUTPUT_MODE_NORMAL, 0,
			WGM_3_FAST_OCRA, prescale);

	if (effect == 0) {
		REGISTER_VOLUME_TOP = (long)top * volume / 200;
		delay(duration);
	} else {
		unsigned long v, step, delta;
		if (effect == -1) {
			v = 100;
			step = 25;
			delta = -4;
		} else {
			v = 0;
			step = 25;
			delta = 4;
		}
		duration /= step;

		while(step --) {
			REGISTER_VOLUME_TOP = (long)top * v / 200;
			delay(duration);
			v += delta;
		}
	}

}

void mute() {
	setPWM(FREQUENCY_PWM, 0,
		COMPARE_OUTPUT_MODE_NONE, 0,
		COMPARE_OUTPUT_MODE_NONE, 0,
		0, 0);
}

void playNotes(char *notes) {
	Serial.print("Playing "); Serial.println(notes);
	short duration = 1;
	short effect = 0; // -1 for > / 0 for flat / 1 for <
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
		} else {
			int octave = 0;
			if (*notes == '+') {
				octave = 1;
				notes++;
			}
			frequency = freqForNote(*notes, octave);
			play((60L * 1000 / tempo) * duration, effect);
			if (tempo == 0) {
				// tempo 0 => play first note forever.
				return;
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

void setFrequency(int f) {
	frequency = f;
	if (soundOn) {
		play();
	}
}

void setVolume(int v) {
	volume = v;
	if (soundOn) {
		play();
	}
}

void setSoundOff() {
	soundOn = false;
	mute();
}

void setSoundOn() {
	soundOn = true;
	play();
}

InputItem inputs[] = {
	{ 'f', 'I', (void *)setFrequency },
	{ 'v', 'I', (void *)setVolume },
	{ 'n', 'S', (void *)playNotes },
	{ 't', 'i', (void *)(&tempo) },
	{ 'm', 'I', (void *)music },
	{ '1', 'S', (void *)setSoundOn },
	{ '0', 'S', (void *)setSoundOff }
};

void help() {
	Serial.println("f frequency");
	Serial.println("v volume [0..100]");
	Serial.println("n note name list");
	Serial.println("t tempo (notes per minute)");
	Serial.println("1 (one) sound on");
	Serial.println("0 (zero) sound off");
}

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);
	pinMode(OUT_FREQ, OUTPUT);
	digitalWrite(OUT_FREQ, LOW);

	registerInput(sizeof(inputs), inputs);
	help();
}

void loop() {
	handleInput();
}
