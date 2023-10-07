#include <Arduino.h>

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
	#define ARDUINO_TINY
#elif defined(__AVR_MEGA__)
	#define ARDUINO_UNO
	#define HAVE_SERIAL
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define IR_1 A5
#define IR_2 A4
// OCR2A = D11
#define GATE       11
#define GATE_LED   13
#define GATE_DOWN  20
#define GATE_UP    35
#define GATE_DELAY 1200
#define OUTGOING_DELAY 1000

void setGate(byte gatePosition) {
	OCR2A = gatePosition;
	digitalWrite(GATE_LED, gatePosition == GATE_UP);
	pinMode(GATE, OUTPUT);
	delay(GATE_DELAY);
	pinMode(GATE, INPUT);
}

void setup(void) {
#ifdef HAVE_SERIAL
	Serial.begin(DEFAULT_BAUDRATE);
#endif

	// setup IR sensors
	pinMode(IR_1, INPUT);
	pinMode(IR_2, INPUT);
#ifdef HAVE_SERIAL
	Serial.println("Setup ok");
#endif

	// setup servo
	pinMode(GATE, INPUT); // set to Z to cut servo motor
	pinMode(GATE_LED, OUTPUT);

	// 1MHz /256 / 64 =~ 1/16ms
	// CS2 = 100 -> clock/64
	// WGM2 = 011 -> fast PWM mode, reset on TOP
	// COM2A = 10 -> clear on OCR2A , set on bottom
	TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20); // 0b10000011;
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // 0b00000100;
	setGate(GATE_DOWN);
}

bool ir1Cut, ir2Cut;

enum { VEHICULE_OUT, VEHICULE_INCOMING, VEHICULE_IN, VEHICULE_OUTGOING } state = VEHICULE_OUT;
unsigned long stateTimestamp = 0;

void loop() {
	bool newIr1 = analogRead(IR_1) > 1000;
	bool newIr2 = analogRead(IR_2) > 1000;
	if (ir1Cut != newIr1 || ir2Cut != newIr2) {
		ir1Cut = newIr1;
		ir2Cut = newIr2;
#ifdef HAVE_SERIAL
		Serial.print("sensors = ");Serial.print(ir1Cut);Serial.print('\t');Serial.println(ir2Cut);
#endif

		// interesting cases :
		// vehicule out, at least one sensor cut -> vehicule incoming, must open
		// vehicule incoming, no sensor cut -> vehicule in
		// vehicule in, at least one sensor cut -> vehicule outgoing
		// vehicule outgoing, no sensor cut -> vehicule out, must close
		bool cut = ir1Cut | ir2Cut;

		switch(state) {
			case VEHICULE_OUT:
				if (cut) {
					state = VEHICULE_INCOMING;
					stateTimestamp = millis();
					setGate(GATE_UP);
				}
			break;
			case VEHICULE_INCOMING:
				if (!cut) {
					state = VEHICULE_IN;
					stateTimestamp = millis();
				}
			break;
			case VEHICULE_IN:
				if (cut) {
					state = VEHICULE_OUTGOING;
					stateTimestamp = millis();
				}
			break;
			case VEHICULE_OUTGOING:
				if (!cut && millis() > stateTimestamp + OUTGOING_DELAY) {
					setGate(GATE_DOWN);
					state = VEHICULE_OUT;
					stateTimestamp = millis();
				}
			break;
		}
#ifdef HAVE_SERIAL
		Serial.print("state = ");Serial.println(state);
#endif

		// // for test pursoses : open when at least one sensor cut
		// if (cut) {
		// 	setGate(GATE_UP);
		// } else {
		// 	setGate(GATE_DOWN);
		// }
	}
}

// int ir1, ir2;

// #define THRESHOLD 20

// void loop() {
// 	int newIr1 = analogRead(IR_1);
// 	int newIr2 = analogRead(IR_2);
// 	if (abs(ir1 - newIr1) > THRESHOLD || abs(ir2 - newIr2) > THRESHOLD) {
// 		ir1 = newIr1;
// 		ir2 = newIr2;
// #ifdef HAVE_SERIAL
// 		Serial.print(ir1);Serial.print('\t');Serial.println(ir2);
// #endif
// 	}
// }