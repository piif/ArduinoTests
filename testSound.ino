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

const int noteFreqs[] = {
//   C    C#   D    D#   E    F    F#   G    G#   A    A#   B
	262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, // octave 0
	523, 554, 587, 622, 659, 699, 740, 784, 831, 880, 932, 988  // octave 1
};
const char noteNames[] = "cCdDefFgGaAb";

int freqForNote(char name, short octave) {
	const char *nn = noteNames;
	while(*nn && *nn != name) *nn++;
	if (*nn == '\0') {
		return 0;
	}
	return noteFreqs[(nn - noteNames) + 12 * octave];
}

int frequency = 440, volume = 50, tempo = 120;
boolean soundOn = false;

void play() {
	unsigned long compFrequency = frequency;
	word top, prescale;
	computePWM(FREQUENCY_PWM, compFrequency, prescale, top);

	unsigned long volumeTop = (long)top * volume / 100;

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

void mute() {
	setPWM(FREQUENCY_PWM, 0,
		COMPARE_OUTPUT_MODE_NONE, 0,
		COMPARE_OUTPUT_MODE_NONE, 0,
		0, 0);
}

void setNote(char *notes) {
	Serial.print("Playing "); Serial.println(notes);
	while(*notes) {
		Serial.print(" -> "); Serial.println(*notes);
		if (*notes == '.') {
			// continue preceding note
		} else if (*notes == ' ') {
			// silence
			mute();
		} else {
			int octave = 0;
			if (*notes == '+') {
				octave = 1;
				notes++;
			}
			frequency = freqForNote(*notes, octave);
			play();
			if (tempo == 0) {
				// tempo 0 => play first note forever.
				return;
			}
		}
		delay(60L * 1000 / tempo);
		notes++;
	}
	mute();
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
	{ 'n', 'S', (void *)setNote },
	{ 't', 'i', (void *)tempo },
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
