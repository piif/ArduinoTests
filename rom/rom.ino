// test code to dump a 27C256-10 ROM
// from datasheet https://pdf1.alldatasheet.com/datasheet-pdf/view/74503/MCNIX/27C256-10.html

#include <Arduino.h>
#include "serialInput.h"

/**
 * wiring :
 *  Output Enable (G) = A0 = PC0
 *  Chip Enable (E)   = A1 = PC1
 *  Q0-Q3 = D4-7 = PD4-7
 *  Q4-7  = D8-11 = PB0-3
 *  Adr0 -  Adr7 -> first 74164 outputs A-H
 *  Adr8 - Adr14 -> second 74164 outputs A-G
 *  first 74164 outputs H link to a+B inputs of second one
 *  74164 Clocks = A2 = PC2
 *  1st 74164 A+B inputs = A3 = PC3
 */

#define ADDR_CLOCK    A2
#define ADDR_DATA     A3
#define CHIP_ENABLE   A1
#define OUTPUT_ENABLE A0

void writeAddress(word addr) {
	PORTC |= 0x02;
	for(word mask = 0x8000; mask; mask >>= 1) {
		if (mask & addr) {
			PORTC |= 0x08;
		} else {
			PORTC &= ~0x08;
		}
		PORTC |= 0x04;
		_NOP();
		PORTC &= ~0x04;
	}
	PORTC &= ~0x02;
}

byte readData(word addr) {
	PORTC |= 0x01;
	writeAddress(addr);
	PORTC &= ~0x01;
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	byte data = ((PIND & 0xF0) >> 4) | ((PINB & 0x0F) << 4);
	PORTC |= 0x01;
	return data;
}

void display16(word data) {
	Serial.print((data >> 12) & 0xF, HEX);
	Serial.print((data >>  8) & 0xF, HEX);
	Serial.print((data >>  4) & 0xF, HEX);
	Serial.print( data        & 0xF, HEX);
}

word currentAddr = 0;

void read1(word from) {
	currentAddr = from;
	display16(from);
	Serial.print(" : ");

	byte data = readData(from);
	Serial.print(data >> 4, HEX);
	Serial.print(data & 0xF, HEX);

	Serial.println();
}

void dump16(word from) {
	currentAddr = from;

	display16(from);
	Serial.print(" :");
	for (byte count=16;  count; from++, count--) {
		byte data = readData(from);
		if (count == 8) {
			Serial.print(' ');
		}
		Serial.print(' ');
		Serial.print(data >> 4, HEX);
		Serial.print(data & 0xF, HEX);
	}
	Serial.println();
}

void dump(word count) {
	for(;;) {
		dump16(currentAddr);
		if (count < 16) {
			currentAddr += count;
			break;
		}
		currentAddr += 16;
		count -= 16;
	}
}

InputItem inputs[] = {
	{ 'r', 'I', (void *)read1 },
	{ 'd', 'I', (void *)dump16 },
	{ 'D', 'I', (void *)dump }
};

void setup() {
	Serial.begin(115200);

	DDRC |= 0x0F; // bits 0-4 output

	PORTD&= ~0xF0; // bits 4-7 input without pull-up
	DDRD &= ~0xF0;
	PORTB&= ~0x0F; // bits 0-3 input without pull-up
	DDRB &= ~0x0F;

	registerInput(sizeof(inputs), inputs);

	Serial.println("Setup OK");
}

void loop() {
	handleInput();
}
