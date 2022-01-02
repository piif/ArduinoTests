// test code to dump a 27C256-10 ROM
// from datasheet https://pdf1.alldatasheet.com/datasheet-pdf/view/74503/MCNIX/27C256-10.html

#include <Arduino.h>

/**
 * wiring :
 *  A0-A7 = Q0-Q7
 *  A0 -  A5 -> D14 - D19 = PC0-5
 *  A5 - A11 -> D2  - D7  = PD2-7
 * A12 - A14 -> D8  - D10 = PB0-2
 * OE = D11 = PB3
 * CE = D12 = PB4
 */

byte readData(word address) {
	// switch A0-A7 to output
	DDRC |= 0x3F; // bits 0-5 output
	DDRD |= 0x03; // bits 2-7 output
	PORTB = PORTB | 0x18; // set CE and OE

	// fill addresss
	PORTC = (PORTC & ~0x3F) | ( address        & 0x3F);
	PORTD = (PORTD & ~0xFC) | ((address >>  4) & 0xFC); // >> 6 and << 2
	PORTB = (PORTB & ~0x07) | ((address >> 12) & 0x07);

	PORTB = PORTB & ~0x18; // clear CE and OE

	// switch A0-A7 to input
	PORTC &= ~0x3F;
	DDRC &= ~0x3F; // bits 0-5 input without pull-up
	PORTD &= ~0x03;
	DDRD &= ~0x03; // bits 2-7 input without pull-up

	return (PORTC & 0x3F) | ((PORTD & 0x03) << 6);
}

void setup() {
	DDRC |= 0x3F; // bits 0-5 output
	DDRD |= 0xFC; // bits 2-7 output
	DDRB |= 0x1F; // bits 0-4 output

	Serial.begin(115200);
	Serial.println("Setup OK");
}

void display16(word data) {
	Serial.print((data >> 12) & 0xF, HEX);
	Serial.print((data >>  8) & 0xF, HEX);
	Serial.print((data >>  4) & 0xF, HEX);
	Serial.print( data        & 0xF, HEX);
}

void dump8(word from) {
	display16(from);
	Serial.print(" :");
	for (byte count=8;  count; from++, count--) {
		byte data = readData(from);
		Serial.print(' ');
		Serial.print(data >> 4, HEX);
		Serial.print(data & 0xF, HEX);
	}
	Serial.println();
}
void loop() {
	// TODO : input
	// r addr => read one data
	// d addr => dump N data
	for(word addr = 0x1200; addr < 0x1220; addr +=8) {
		dump8(addr);
	}
	delay(5000);
}
