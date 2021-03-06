//#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
//	// other includes with full pathes
//	// example : #include "led7/led7.h"
//#else
//	// other includes with short pathes
//	// example : #include "led7.h"
//#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif


#define SCLK 2
#define SDIO 3
#define PD 4

#define REG_PRODUCT_ID 0x00
#define REG_REVISION_ID 0x01
#define REG_MOTION 0x02
#define REG_DELTA_X 0x03
#define REG_DELTA_Y 0x04
#define REG_SQUAL 0x05
#define REG_AVERAGE_PIXEL 0x06
#define REG_MAXIMUM_PIXEL 0x07
#define REG_CONFIG_BITS 0x0A
#define REG_DATA_OUT_LOWER 0x0C
#define REG_DATA_OUT_UPPER 0x0D
#define REG_SHUTTER_LOWER 0x0E
#define REG_SHUTTER_UPPER 0x0F
#define REG_FRAME_PERIOD_LOWER 0x10
#define REG_FRAME_PERIOD_UPPER 0x11

int dumpWidth = 256; // Number of pixels to read for each frame.
byte frame[256];

byte readRegister(byte address) {
	pinMode(SDIO, OUTPUT);

	for (byte i = 128; i > 0; i >>= 1) {
		digitalWrite(SCLK, LOW);
		digitalWrite(SDIO, (address & i) != 0 ? HIGH : LOW);
		digitalWrite(SCLK, HIGH);
	}

	pinMode(SDIO, INPUT);

	delayMicroseconds(100); // tHOLD = 100us min.

	byte res = 0;
	for (byte i = 128; i > 0; i >>= 1) {
		digitalWrite(SCLK, LOW);
		digitalWrite(SCLK, HIGH);
		if (digitalRead(SDIO) == HIGH)
			res |= i;
	}

	return res;
}

void writeRegister(byte address, byte data) {
	address |= 0x80; // MSB indicates write mode.
	pinMode(SDIO, OUTPUT);

	for (byte i = 128; i > 0; i >>= 1) {
		digitalWrite(SCLK, LOW);
		digitalWrite(SDIO, (address & i) != 0 ? HIGH : LOW);
		digitalWrite(SCLK, HIGH);
	}

	for (byte i = 128; i > 0; i >>= 1) {
		digitalWrite(SCLK, LOW);
		digitalWrite(SDIO, (data & i) != 0 ? HIGH : LOW);
		digitalWrite(SCLK, HIGH);
	}

	delayMicroseconds(100); // tSWW, tSWR = 100us min.
}

void reset() {
	pinMode(SCLK, OUTPUT);
	pinMode(SDIO, INPUT);
	pinMode(PD, OUTPUT);
	digitalWrite(SCLK, LOW);
	digitalWrite(PD, HIGH);
	delayMicroseconds(1);
	digitalWrite(PD, LOW);
}

void dumpFrame() {
	byte config = readRegister(REG_CONFIG_BITS);
	config |= B00001000; // PixDump
	writeRegister(REG_CONFIG_BITS, config);

	int count = 0;
	do {
		byte data = readRegister(REG_DATA_OUT_LOWER);
		if ((data & 0x80) == 0) { // Data is valid
			frame[count++] = data;
		}
	} while (count != dumpWidth);

	config = readRegister(REG_CONFIG_BITS);
	config &= B11110111;
	writeRegister(REG_CONFIG_BITS, config);

	Serial.print("FRAME:");
	for (int i = 0; i < dumpWidth; i++) {
		byte pix = frame[i];
		if (pix < 0x10)
			Serial.print("0");
		Serial.print(pix, HEX);
	}
	Serial.println();
}

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	reset();
	byte productId = readRegister(REG_PRODUCT_ID);
	byte revisionId = readRegister(REG_REVISION_ID);
	Serial.print("Found productId ");
	Serial.print(productId, HEX);
	Serial.print(", rev. ");
	Serial.print(revisionId, HEX);
	Serial.println(
			productId == 0x02 ? " OK." : " Unknown productID. Carry on.");

	byte config = readRegister(REG_CONFIG_BITS);
	config |= B00000001; // Don't sleep (LED always powered on).
	writeRegister(REG_CONFIG_BITS, config);
}

void loop() {
	// Allows to set the dump window by sending the number of lines to read via the serial port.
	if (Serial.available() > 0) {
		dumpWidth = 16 * Serial.read();
		dumpWidth = constrain(dumpWidth, 0, 256);
	}

	readRegister(REG_MOTION); // Freezes DX and DY until they are read or MOTION is read again.
	char dx = readRegister(REG_DELTA_X);
	char dy = readRegister(REG_DELTA_Y);
	Serial.print("DELTA:");
	Serial.print(dx, DEC);
	Serial.print(" ");
	Serial.println(dy, DEC);

	if (dumpWidth > 0)
		dumpFrame();
}
