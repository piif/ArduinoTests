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

// on ADNS5020EN
// 1    SDIO      Serial Port Data Input and Output
// 2    XY_LED    LED Control
// 3    NRESET    Reset Pin (active low input)
// 4    NCS       Chip Select (active low input)
// 5    VDD5      Supply Voltage
// 6    GND       Ground
// 7    REGO      Regulator Output
// 8    SCLK      Serial Clock Input


#define SCLK 2
#define SDIO 3
#define NCS 4
#define NRESET 5

#define REG_PRODUCT_ID 0x00
#define REG_REVISION_ID 0x01
#define REG_MOTION 0x02
#define REG_DELTA_X 0x03
#define REG_DELTA_Y 0x04
#define REG_SQUAL 0x05
#define REG_SHUTTER_LOWER 0x06
#define REG_SHUTTER_UPPER 0x07
#define REG_MAXIMUM_PIXEL 0x08
#define REG_SUM_PIXEL 0x09
#define REG_MINIMUM_PIXEL 0x0A
#define REG_PIXEL_GRAB 0x0B
// 0C reserved
#define REG_MOUSE_CONTROL 0x0D
// 0E-39 reserved
#define REG_CHIP_RESET 0x3A
// 3B-3E reserved
#define REG_INV_REV_ID 0x3F
// 40-62 reserved
#define REG_MOTION_BURST 0x63

#define MAP_SIZE (15*15)

int dumpWidth = 0; // Number of pixels to read for each frame.
byte frame[MAP_SIZE];

// TODO : refaire ça en manipulant les registres en direct
// pour avoir un timing constant et donc connu
// idéalement, le refaire en assembleur en s'inspirant de la lib LEDMatrix ?
byte readRegister(byte address) {
	pinMode(SDIO, OUTPUT);
	digitalWrite(NCS, LOW);
	delayMicroseconds(1);

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

	delayMicroseconds(100); // tSWW, tSWR = 100us min.
//	delayMicroseconds(1);
	digitalWrite(NCS, HIGH);

	return res;
}

void writeRegister(byte address, byte data) {
	digitalWrite(NCS, LOW);
	delayMicroseconds(1);

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
//	delayMicroseconds(1);
	digitalWrite(NCS, HIGH);
}

void reset() {
//	pinMode(SCLK, OUTPUT);
//	pinMode(SDIO, INPUT);
//	digitalWrite(SCLK, LOW);
	digitalWrite(NRESET, LOW);
	delayMicroseconds(1);
	digitalWrite(NRESET, HIGH);
}

void dumpFrame() {
	writeRegister(REG_PIXEL_GRAB, 42);

	int count = 0;
	do {
		byte data = readRegister(REG_PIXEL_GRAB);
		if ((data & 0x80) != 0) { // Data is valid
			frame[count++] = data;
		}
	} while (count != dumpWidth);
// TODO : faire une somme et en déduire un niveau de gris global à dumper à part

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

	pinMode(SCLK, OUTPUT);
	pinMode(NRESET, OUTPUT);
	pinMode(NCS, OUTPUT);

	reset();
	byte productId = readRegister(REG_PRODUCT_ID);
	byte revisionId = readRegister(REG_REVISION_ID);
	Serial.print("Found productId ");
	Serial.print(productId, HEX);
	Serial.print(", rev. ");
	Serial.print(revisionId, HEX);
	Serial.println(
			productId == 0x02 ? " OK." : " Unknown productID. Carry on.");

}

void loop() {
	// Allows to set the dump window by sending the number of lines to read via the serial port.
	if (Serial.available() > 0) {
		int c = Serial.read();
		if (c >= '0' && c <= '9') {
			c = c - '0';
			dumpWidth = 15 * c;
		} else if (c >= 'a' && c <= 'f') {
			c = c - 'a' + 10;
			dumpWidth = 15 * c;
		}
	}


	if (dumpWidth > 0) {
		dumpFrame();
	}
	if (readRegister(REG_MOTION) & 0x80) {
		char dx = readRegister(REG_DELTA_X);
		char dy = readRegister(REG_DELTA_Y);
		writeRegister(REG_MOTION, 0);
		if (dx != 0 || dy != 0) {
			Serial.print("DELTA:");
			Serial.print(dx, DEC);
			Serial.print(" ");
			Serial.println(dy, DEC);
		}
	}
}
