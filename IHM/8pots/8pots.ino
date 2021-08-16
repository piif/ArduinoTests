#include <Arduino.h>

#include <MozziGuts.h>
#include <Oscil.h>
#include <EventDelay.h>
#include <ADSR.h>
#include <tables/sin8192_int8.h>
#include <mozzi_midi.h>

#include "serialInput.h"

Oscil <8192, AUDIO_RATE> aOscil(SIN8192_DATA);;

// for triggering the envelope
EventDelay noteDelay;

ADSR <AUDIO_RATE, AUDIO_RATE> envelope;

byte attackLevel = 250, decayLevel = 220, sustainLevel = 220, releaseLevel = 10;
int attackMs = 50, decayMs = 100, sustainMs = 300, releaseMs = 50;

/* 33 = A0 , A1 = 45 (+12) , A3(440Hz) = 69 */
byte note;
int freq;

void setNote(byte n) {
	note = n;
	freq = (int)mtof(note);
	aOscil.setFreq(freq);
	Serial.print("setFreq ");
	Serial.println(freq);
}

InputItem inputs[] = {
	{ 'a', 'b', (void *)&attackLevel  },
	{ 'd', 'b', (void *)&decayLevel   },
	{ 's', 'b', (void *)&sustainLevel },
	{ 'r', 'b', (void *)&releaseLevel },
	{ 'A', 'i', (void *)&attackMs     },
	{ 'D', 'i', (void *)&decayMs      },
	{ 'S', 'i', (void *)&sustainMs    },
	{ 'R', 'i', (void *)&releaseMs    },
	{ 'n', 'B', (void *)setNote  }
};

void playNote() {
	// set envelope
	envelope.setLevels(attackLevel, decayLevel, sustainLevel, releaseLevel);
	envelope.setTimes(attackMs, decayMs, sustainMs, releaseMs);

	int duration = attackMs + decayMs + sustainMs + releaseMs + 200;
	Serial.println(duration);

	envelope.noteOn();
	noteDelay.start(duration);
}

void setup() {
	Serial.begin(115200);
	registerInput(sizeof(inputs), inputs);

	setNote(69);

	startMozzi();
	playNote();
}

void updateControl() {
	handleInput();
	if (noteDelay.ready()) {
		Serial.print(attackLevel); Serial.print('\t');
		Serial.print(decayLevel); Serial.print('\t');
		Serial.print(sustainLevel); Serial.print('\t');
		Serial.println(releaseLevel);

		Serial.print(attackMs); Serial.print('\t');
		Serial.print(decayMs); Serial.print('\t');
		Serial.print(sustainMs); Serial.print('\t');
		Serial.println(releaseMs);

		playNote();
	}
}

AudioOutput_t updateAudio(){
  envelope.update();
//  return aOscil.next();
  return MonoOutput::from16Bit((int) (envelope.next() * aOscil.next()));
}


void loop(){
  audioHook(); // required here
}

