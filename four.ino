// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoTools

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	// other includes with full pathes
	#include "serialInput.h"
#else
	// other includes with short pathes
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define F_STROBE 3
#define F_CLOCK  5
#define F_DATA   7

#define NB_BITS 320
#define NB_BYTES (NB_BITS >> 3)
byte bits[NB_BYTES] = { 0xFF, }; // 40 * 8 = 320 bits
word current = 0; // current bit to flip

void update() {
	digitalWrite(F_STROBE, HIGH);
	for (char i = NB_BYTES-1; i >= 0; i--) {
		for (byte mask = 0x01; mask; mask <<= 1) {
			digitalWrite(F_DATA, bits[i] & mask ? HIGH : LOW);
			digitalWrite(F_CLOCK, HIGH);
			digitalWrite(F_CLOCK, LOW);
		}
	}
	digitalWrite(F_STROBE, LOW);
}

void toggle() {
	for (byte i = 0; i < NB_BYTES; i++) {
		bits[i] = ~bits[i];
	}
	update();
}
void flip() {
	byte index = current >> 3;
	byte mask = 0x80 >> (current & 7);
	bits[index] ^= mask;
	update();
}
void move(int i) {
	current = i % NB_BITS;
}

void flipNext() {
	current = (current + 1) % NB_BITS;
	flip();
}
void flipPred() {
	current = (current + (NB_BITS-1)) % NB_BITS;
	flip();
}

void set(byte v) {
	for (byte i = 0; i < NB_BYTES; i++) {
		bits[i] = v;
	}
	update();
}
void clearAll() {
	set(0);
}
void setAll() {
	set(0xFF);
}
void setHalf() {
	set(0x55);
}

void help() {
	Serial.print("bits : ");
	for (byte i = 0; i < NB_BYTES; i++) {
		Serial.print(bits[i], BIN);
		Serial.print(' ');
	}
	Serial.println();
	Serial.print("current = ");Serial.println(current);
	Serial.println("0 : clearAll");
	Serial.println("1 : setAll");
	Serial.println("2 : setHalf");
	Serial.println("+ : flipNext");
	Serial.println("- : flipPred");
	Serial.println(". : flip current");
	Serial.println();
}

InputItem list[] = {
	{ 'm', 'I', (void *)move },
	{ 'v', 'I', (void *)set },
	{ '0', 'f', (void *)clearAll },
	{ '1', 'f', (void *)setAll },
	{ '2', 'f', (void *)setHalf },
	{ '+', 'f', (void *)flipNext },
	{ '-', 'f', (void *)flipPred },
	{ '.', 'f', (void *)flip },
	{ 't', 'f', (void *)toggle },
	{ '?', 'f', (void *)help },
};


void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(F_STROBE, OUTPUT);
	pinMode(F_CLOCK, OUTPUT);
	pinMode(F_DATA, OUTPUT);

	digitalWrite(F_STROBE, LOW);
	digitalWrite(F_CLOCK, LOW);
	digitalWrite(F_DATA, LOW);

	registerInput(sizeof(list), list);
	Serial.println("Ready");
}

void loop() {
	handleInput();
//	update();
//	delay(49);
}
