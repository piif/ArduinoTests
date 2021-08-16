#include <Arduino.h>

#include <MozziGuts.h>
#include <Oscil.h>
#include <EventDelay.h>
#include <ADSR.h>
#include <tables/sin8192_int8.h>
#include <mozzi_midi.h>

Oscil <8192, AUDIO_RATE> aOscil(SIN8192_DATA);;

// for triggering the envelope
EventDelay noteDelay;

ADSR <AUDIO_RATE, AUDIO_RATE> envelope;

// input potentiometers
#define PLUS_1 2   // gray
#define PLUS_2 3   // yellow
#define MINUS_1 4  // green
#define MINUS_2 5  // blue

#define INPUT_1 A0 // black
#define INPUT_2 A1 // red
#define INPUT_3 A2 // brown
#define INPUT_4 A3 // orange


void setup() {
	Serial.begin(115200);
	pinMode(PLUS_1 , INPUT);
	pinMode(PLUS_2 , INPUT);
	pinMode(MINUS_1, INPUT);
	pinMode(MINUS_2, INPUT);
	pinMode(INPUT_1, INPUT);
	pinMode(INPUT_2, INPUT);
	pinMode(INPUT_3, INPUT);
	pinMode(INPUT_4, INPUT);

	noteDelay.set(2000); // 2 second countdown

	/* 33 = A0 , A1 = 45 (+12) , A3(440Hz) = 69 */
	int freq = (int)mtof(69);
	aOscil.setFreq(freq);
	Serial.print("setFreq ");
	Serial.println(freq);

	envelope.setLevels(250, 200, 200, 50);
	envelope.setTimes(250, 250, 250, 250);

	startMozzi();
}

int pots[8] = { 0, };

/*
 * 0 = play note
 * 1 = activate first pot set
 * 2 = read first pot set
 * 3 = activate second pot set
 * 4 = read second pot set
 */
byte state = 0;

void updateControl() {
	if (noteDelay.ready()) {
//		Serial.println("noteDelay.ready");
//		envelope.noteOn();
//		noteDelay.start(1200);
//		return;

		state = (state + 1) % 5;
		Serial.println(state);
		switch (state) {
		case 1:
			// read potentiometers
			pinMode(PLUS_1 , OUTPUT);
			pinMode(MINUS_1, OUTPUT);
			pinMode(PLUS_2 ,  INPUT);
			pinMode(MINUS_2,  INPUT);
			digitalWrite(PLUS_1 , HIGH);
			digitalWrite(MINUS_1, LOW);
			noteDelay.start(5);
			break;

		case 2:
			pots[0] = analogRead(INPUT_1);
			pots[1] = analogRead(INPUT_2);
			pots[2] = analogRead(INPUT_3);
			pots[3] = analogRead(INPUT_4);
			noteDelay.start(1);
			break;

		case 3:
			pinMode(PLUS_1 ,  INPUT);
			pinMode(MINUS_1,  INPUT);
			pinMode(PLUS_2 , OUTPUT);
			pinMode(MINUS_2, OUTPUT);
			digitalWrite(PLUS_2 , HIGH);
			digitalWrite(MINUS_2, LOW);
			noteDelay.start(5);
			break;

		case 4:
			pots[4] = analogRead(INPUT_1);
			pots[5] = analogRead(INPUT_2);
			pots[6] = analogRead(INPUT_3);
			pots[7] = analogRead(INPUT_4);
			noteDelay.start(1);
			break;

		case 0:
			for(int i=0; i<8; i++) {
				Serial.print(pots[i]); Serial.print('\t');
			}
			Serial.println();

			// set envelope
			envelope.setLevels(pots[0] >> 2, pots[2] >> 2, pots[4] >> 2, pots[6] >> 2);
//			envelope.setADLevels(pots[4], pots[6]);
			envelope.setTimes(pots[1], pots[3], pots[5], pots[7]);

			int duration = pots[1] + pots[3] + pots[5] + pots[7] + 200;
			Serial.println(duration);

			envelope.noteOn();
			noteDelay.start(duration);
			break;
		}
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

