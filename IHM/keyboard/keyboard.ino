#include <Arduino.h>
#include "serialInput.h"

#define K_OUT_CLOCK  8  // blue
//#define K_OUT_CLEAR  9  // white
//#define K_OUT_DATA  10  // gray

#define K_IN_C 5    // orange
#define K_IN_B 6    // yellow
#define K_IN_A 7    // green
#define K_IN_G 3    // brown
#define K_DATA 4    // red

void sendBuffer(byte value) {
	pinMode(K_DATA , OUTPUT);
//	for(byte mask = 0x80; mask; mask >>= 1) {
//		digitalWrite(K_OUT_CLOCK, LOW);
////		Serial.println((mask & value));
//		digitalWrite(K_DATA, mask & value);
//		digitalWrite(K_OUT_CLOCK, HIGH);
//	}
//	digitalWrite(K_OUT_CLOCK, LOW);
	digitalWrite(K_OUT_CLOCK, LOW);
	shiftOut(K_DATA, K_OUT_CLOCK, MSBFIRST, value);
}

byte readInput() {
	pinMode(K_DATA , INPUT_PULLUP);
	digitalWrite(K_IN_G, LOW);
	byte result = digitalRead(K_DATA);
	// must disable output just after, to avoid to send data value to shift register (has data pins are shared)
	digitalWrite(K_IN_G, HIGH);
	return result;
}

byte readInput(byte address) {
	digitalWrite(K_IN_A, address & 1);
	digitalWrite(K_IN_B, address & 2);
	digitalWrite(K_IN_C, address & 4);
	return readInput();
}

char *mapping[8][8] = {
	{	"cx",	"X",	"W",	"sp",	"#",	"9",	"6",	"3"	},
	{	"fc",	"S",	"Q",	"Ct",	"J",	"K",	"L",	"M"	},
	{	"gd",	"Z",	"A",	"sm",	"U",	"I",	"O",	"P"	},
	{	"B",	"C",	"V",	"N",	"0",	"8",	"5",	"2"	},
	{	".",	"es",	",",	"'",	"su",	"rt",	"en",	"rp"	},
	{	"G",	"D",	"F",	"H",	"*",	"7",	"4",	"1"	},
	{	"T",	"E",	"R",	"Y",	";",	"-",	":",	"?"	},
	{	"^",	"cr",	"an",	"v",	"sh",	"<",	">",	"Rt"	}
};

byte scanLine() {
	for(byte adr = 0; adr < 8; adr++) {
		if (readInput(adr) == 0) {
			return adr;
		}
	}
	return 0xFF;
}

byte findLine() {
	sendBuffer(~0xF0);
	if (readInput()) {
		// 0,1,2 or 3
		sendBuffer(~0xC);
		if (readInput()) {
			// 0 or 1
			sendBuffer(~0x2);
			if (readInput()) {
				return 0;
			} else {
				return 1;
			}
		} else {
			// 2 or 3
			sendBuffer(~0x8);
			if (readInput()) {
				return 2;
			} else {
				return 3;
			}
		}
	} else {
		// 4,5,6 or 7
		sendBuffer(~0xC0);
		if (readInput()) {
			// 4 or 5
			sendBuffer(~0x20);
			if (readInput()) {
				return 4;
			} else {
				return 5;
			}
		} else {
			// 6 or 7
			sendBuffer(~0x80);
			if (readInput()) {
				return 6;
			} else {
				return 7;
			}
		}
	}
}

byte scanFirst() {
	sendBuffer(0);
	byte adr = scanLine();
	if (adr == 0xFF) {
		return 0xFF;
	}
	delay(2);
	adr = scanLine();
	if (adr == 0xFF) {
		return 0xFF;
	}
	return (findLine() << 4) | adr;
}

void scanAll() {
	for(byte mask = 0x80; mask; mask >>= 1) {
		sendBuffer(~mask);
		for(byte adr = 0; adr < 8; adr++) {
			Serial.print(readInput(adr));
		}
		Serial.println();
	}
}

void scanAddress(byte address) {
	Serial.println(readInput(address));
}

void scan() {
	for(byte adr = 0; adr < 8; adr++) {
		Serial.print(readInput(adr));
	}
	Serial.println();
}

InputItem inputs[] = {
	{ 'o', 'B', (void *)sendBuffer },
	{ 'i', 'B', (void *)scanAddress },
	{ 's', 'f', (void *)scan },
	{ 'a', 'f', (void *)scanAll }
};

void setup() {
	Serial.begin(115200);

	pinMode(K_OUT_CLOCK, OUTPUT);
//	pinMode(K_OUT_CLEAR, OUTPUT);
//	pinMode(K_OUT_DATA , OUTPUT);
 	pinMode(K_IN_A     , OUTPUT);
 	pinMode(K_IN_B     , OUTPUT);
 	pinMode(K_IN_C     , OUTPUT);
 	pinMode(K_IN_G     , OUTPUT);
// 	pinMode(K_IN_DATA  , INPUT_PULLUP);

 	digitalWrite(K_IN_G, HIGH);
 	digitalWrite(K_OUT_CLOCK, HIGH);
// 	digitalWrite(K_OUT_CLEAR, HIGH);

 	registerInput(sizeof(inputs), inputs);

 	Serial.print(mapping[2][2]);
 	Serial.print(mapping[0][7]);
 	Serial.print(mapping[7][0]);
 	Serial.println(" => A3^ ?");

 	Serial.println("Setup OK");
}

//char *key = 0;
byte key = 0xFF;
void loop() {
	unsigned long before = micros();
	byte newKey = scanFirst();
	unsigned long after = micros();
	if (newKey != key) {
		key = newKey;
		if (key != 0xFF) {
			Serial.print(key, HEX);
			Serial.print('\t');
			Serial.println(mapping[key >> 4][key & 7]);
			Serial.println(after-before);
		} else {
			Serial.println("--");
		}
	}

//	handleInput();
//	scan();
//	delay(500);
}
