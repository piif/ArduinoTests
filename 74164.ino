// PIF_TOOL_CHAIN_OPTION: UPLOAD_OPTIONS := -c "raw,cr"
// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

#include <Arduino.h>

/* tests to handle a 7 segments 3 digits display with one 74LS164
 */

#ifdef PIF_TOOL_CHAIN
	#include "setInterval/setInterval.h"
#else
	#include "setInterval.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#ifdef __AVR_ATmega644P__
		#define DEFAULT_BAUDRATE 19200
	#else
		#define DEFAULT_BAUDRATE 115200
	#endif
#endif

#define DATA 2
#define CLOCK 3
#define LED 13

#define DIGIT_1 8
#define DIGIT_2 9
#define DIGIT_3 10

#define DELTA 5
#define PERIOD 1000

byte mapSegments[] = {
	//tRrblLmp
	0b11111100, // 0
	0b01100000, // 1
	0b11011010, // 2
	0b11110010, // 3
	0b01100110, // 4
	0b10110110, // 5
	0b10111110, // 6
	0b11100000, // 7
	0b11111110, // 8
	0b11110110, // 9
	0b00000001, // .
	0b00000000, // ' '
};

byte currentDisplay[3];
void displayChars(byte *map) {
	currentDisplay[0] = map[0];
	currentDisplay[1] = map[1];
	currentDisplay[2] = map[2];
}

void displayNumber(int value) {
	currentDisplay[0] = mapSegments[(value / 100) % 10];
	currentDisplay[1] = mapSegments[(value / 10) % 10];
	currentDisplay[2] = mapSegments[value % 10];
}

void outputChar(byte map) {
	for(byte mask = 0x01; mask; mask <<= 1) {
		digitalWrite(DATA, mask & map ? LOW : HIGH);
		digitalWrite(CLOCK, HIGH);
//		delay(DELTA);
		digitalWrite(CLOCK, LOW);
	}
}

byte digit = 0;
byte buttons = 0;

void updateDisplay(void *, long, int) {
	// read buttons state
////	PORTB |= 0x07;
//	DDRB &= 0xF8;
//	PORTB &= 0xF8;
//	digitalWrite(DATA, HIGH);
//	_NOP();
//	buttons = PINB & 0x07;
//	DDRB |= 0x07;
//	PORTB |= 0x07;
//	digitalWrite(DATA, LOW);
	pinMode(DATA, INPUT_PULLUP);
	_NOP();
	if (digitalRead(DATA)) {
		buttons &= ~(1 << digit);
	} else {
		buttons |= 1 << digit;
	}
	pinMode(DATA, OUTPUT);

	currentDisplay[2] = mapSegments[buttons];

	// set 'dot' segment like button states
	if (buttons & 0x04) {
		currentDisplay[0] |= 0x01;
	} else {
		currentDisplay[0] &= 0xFE;
	}
	if (buttons & 0x02) {
		currentDisplay[1] |= 0x01;
	} else {
		currentDisplay[1] &= 0xFE;
	}
	if (buttons & 0x01) {
		currentDisplay[2] |= 0x01;
	} else {
		currentDisplay[2] &= 0xFE;
	}

	// display one digit
	PORTB |= 0x07; // set bits 0, 1 and 2
	outputChar(currentDisplay[digit]); // prepare output
	PORTB &= ~ (1 << digit); // set it to current digit

	if (digit == 2) {
		digit = 0;
	} else {
		digit++;
	}
}

int count = 1;

void updateCounter(void *, long, int) {
	count++;
	displayNumber(count);
	if (count % 10 == 0) {
		tac();
	}

//	if (count == 9) {
//		count = 0;
//	} else {
//		count++;
//	}
//	displayChars(mapSegments+count);
}

void setup() {
	pinMode(DATA, OUTPUT);
	pinMode(CLOCK, OUTPUT);
	pinMode(LED, OUTPUT);

	pinMode(DIGIT_1, OUTPUT);
	pinMode(DIGIT_2, OUTPUT);
	pinMode(DIGIT_3, OUTPUT);

	digitalWrite(DATA, LOW);
	digitalWrite(CLOCK, LOW);
	digitalWrite(LED, LOW);

	digitalWrite(DIGIT_1, HIGH);
	digitalWrite(DIGIT_2, HIGH);
	digitalWrite(DIGIT_3, HIGH);

	setInterval(100, updateCounter, NULL);
	setInterval(3, updateDisplay, NULL);
}

void loop() {
	setIntervalStep();
}
