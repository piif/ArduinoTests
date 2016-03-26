/**
 * Testing access to registers of DS12887A Real-Time Clock
 */

// PIF_TOOL_CHAIN_OPTION: UPLOAD_OPTIONS := -c "raw,cr"
// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

// TODO : read http://forum.arduino.cc/index.php?topic=138168.5

#include <Arduino.h>
#include "serialInput/serialInput.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define NOP do { __asm__ __volatile__ ( "nop\n" ); } while(0)

// delays : Uno at 16MHz => 1 instruction / 62.5ns

// dallas pins connections
// AD0 - AD6 => PORTD 2 to 7 = D2 - 7
// AD7 and 8 -> later
#define ADR_DATA_PORT PORTD
#define ADR_DATA_DIR DDRD

// D10, 11, 12, 13
#define ChipSelect PORTB,2
#define AddressStrobe PORTB,3
#define ReadWrite PORTB,4
#define DataStrobe PORTB,6

#define SET(RegBit) bitSet(RegBit)
#define CLEAR(RegBit) bitClear(RegBit)

/** registers :
 * 00 : seconds
 * 01 : seconds alarm
 * 02 : minutes
 * 03 : minutes alarm
 * 04 : hours
 * 05 : hours alarm
 * 06 : day of week
 * 07 : day of month
 * 08 : month
 * 09 : year
 * 0A : UIP DV2 DV1 DV0 RS3 RS2 RS1 RS0
 * 0B : SET PIE AIE UIE SQWE DM 24/12 DSE
 * 0C : IRQF PF AF UF 0 0 0 0 read only
 * 0D :  VRT 0 0 0 0 0 0 0 read only
 * 0E - 7F : RAM
 */

//A REVOIR EN MODE MOTOROLA ?

/**
 * MOT to GND or not connected => intel bus mode
 *
 * To read data :
 * set RW and DS
 * set AS
 * write address to AD5:0
 * clear CS
 * clear AS
 * delay ?
 *
 * clear DS
 * delay ?
 * set DS
 * read data from AD7:0
 * set CS
 *
 * To write data :
 * set RW and DS
 * set AS
 * write address to AD5:0
 * clear CS
 * clear AS
 * delay ?
 *
 * clear RW
 * write data to AD7:0
 * set RW
 * delay ?
 * set CS
 */

void setAdress(byte address) {
	// set ADR_DATA as output
	ADR_DATA_DIR |= 0xFC;
	SET(ReadWrite);
	SET(DataStrobe);
	// >= 20ns
	SET(AddressStrobe);
	ADR_DATA_PORT = (ADR_DATA_PORT & 0x03) | (address << 2);
	// >= 30ns
	CLEAR(ChipSelect);
	NOP;
	CLEAR(AddressStrobe);
	NOP;
	NOP;
}

byte readData(byte address) {
	setAdress(address);
	CLEAR(DataStrobe);
	// set ADR_DATA as input
	ADR_DATA_PORT = (ADR_DATA_PORT & 0x03);
	ADR_DATA_DIR &= 0x03;
	SET(DataStrobe);
	NOP;
	byte result = (ADR_DATA_PORT >> 2);
	SET(ChipSelect);
	NOP;
	return result;
}

void writeData(byte address, byte value) {
	setAdress(address);
	CLEAR(ReadWrite);
	NOP;
	ADR_DATA_PORT = (ADR_DATA_PORT & 0x03) | (address << 2);
	SET(ReadWrite);
	NOP;
	NOP;
	SET(ChipSelect);
	NOP;
}

void help() {
	Serial.println("s nn: set data to write");
	Serial.println("w nn: write data to address");
	Serial.println("r nn: read data from address");
}

byte data = 42;
void setData(byte d) {
	data = d;
}
void doRead(byte ad) {
	Serial.print("Read adr "); Serial.print(ad, HEX);
	Serial.print(" : "); Serial.println(readData(ad));
}
void doWrite(byte ad) {
	Serial.print("Write "); Serial.print(data, HEX);
	Serial.print(" to adr "); Serial.print(ad, HEX);
	writeData(ad, data);
}

InputItem inputs[] = {
	{ 's', 'I', (void *)setData },
	{ 'r', 'I', (void *)doRead },
	{ 'w', 'I', (void *)doWrite }
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	// control pins are outputs
	DDRB |= 0x5c;
	// data pins are outputs
	DDRD |= 0x3c;
	SET(ChipSelect);

	registerInput(sizeof(inputs), inputs);
	help();
}

void loop() {
	handleInput();
}
