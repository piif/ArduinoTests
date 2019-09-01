// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoTools

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	// other includes with full pathes
#else
	// other includes with short pathes
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define T_IN  A1

// single char mappings
byte mapSegments[] = {
	//tLmRrlbp
	0b11011110, // 0
	0b00011000, // 1
	0b10110110, // 2
	0b10111010, // 3
	0b01111000, // 4
	0b11101010, // 5
	0b11101110, // 6
	0b10011000, // 7
	0b11111110, // 8
	0b11111010, // 9
	0b00100000, // '-'
	0b00000001, // .
	0b00000000, // ' '
};

#define DATA 2
#define CLOCK 3

#define DIGIT_1 8
#define DIGIT_2 9
#define DIGIT_3 10

#define BUTTON 7

void outputChar(byte map) {
	for(byte mask = 0x01; mask; mask <<= 1) {
		digitalWrite(DATA, mask & map ? LOW : HIGH);
		digitalWrite(CLOCK, HIGH);
		digitalWrite(CLOCK, LOW);
	}
}

byte digit = 0;
byte currentDisplay[3] = { 0, };

void updateDisplay(int instant, int avg) {
	static int lastVal = 0, newVal;
	bool button = digitalRead(BUTTON);

	if (button == LOW) {
		newVal = avg;
	} else {
		newVal = instant;
	}

	if (newVal != lastVal) {
		lastVal = newVal;
		currentDisplay[0] = mapSegments[newVal / 100];
		currentDisplay[1] = mapSegments[(newVal / 10) % 10];
		currentDisplay[2] = mapSegments[newVal % 10];
//			Serial.print(currentDisplay[0], BIN);Serial.print(' ');
//			Serial.print(currentDisplay[1], BIN);Serial.print(' ');
//			Serial.print(currentDisplay[2], BIN);Serial.println(' ');
	}

	if (button == LOW) {
		currentDisplay[2] |= 0x01;
	} else {
		currentDisplay[2] &= 0xFE;
	}

	PORTB |= 0x07; // set bits 0, 1 and 2
	outputChar(currentDisplay[digit]); // prepare output
	PORTB &= ~(1 << digit); // set it to current digit

	if (digit == 2) {
		digit = 0;
	} else {
		digit++;
	}
}

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(T_IN, INPUT);

	pinMode(DATA, OUTPUT);
	pinMode(CLOCK, OUTPUT);
	pinMode(DIGIT_1, OUTPUT);
	pinMode(DIGIT_2, OUTPUT);
	pinMode(DIGIT_3, OUTPUT);
	pinMode(BUTTON, INPUT_PULLUP);

	digitalWrite(DATA, LOW);
	digitalWrite(CLOCK, LOW);
	digitalWrite(DIGIT_1, HIGH);
	digitalWrite(DIGIT_2, HIGH);
	digitalWrite(DIGIT_3, HIGH);

	Serial.println("Ready");
}

byte i = 0;

int instant = 0, avg = 0;

#define AVG_SIZE 20
int lastT[AVG_SIZE] = { 0, };
byte lastIndex = 0;
byte nbLast = 0;

void loop() {
	i++;

	if (i==200) {
		instant = analogRead(T_IN);
		Serial.println(instant);
		i= 0;
		lastT[lastIndex] = instant;
		lastIndex++;
		if (lastIndex == AVG_SIZE) {
			lastIndex = 0;
		}
		if (nbLast < AVG_SIZE) {
			nbLast++;
		}
		long sum = 0;
		for(byte j = 0; j < nbLast; j++) {
			sum += lastT[j];
		}
		avg = sum / nbLast;
	}

	updateDisplay(instant, avg);

	delay(5);
}
