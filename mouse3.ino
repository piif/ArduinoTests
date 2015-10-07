#define USE_INTERRUPT_TIMER_HANDLER_2
#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include <events/events.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
	// example : #include "led7.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

/******/

// motor and scan speed
#define SPEED 50

/****** MOUSE CAPTOR PART ******/

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

//#define HORIZONTAL
#define MAP_SIZE 15
// mid value for total gray level of frame
#define THRESHOLD (MAP_SIZE * 0x7f / 2)

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

long dumpFrame() {
	writeRegister(REG_PIXEL_GRAB, 42);

	byte count = 0;
	long total = 0;
#ifdef HORIZONTAL
	byte i= 0;
	do {
		byte data = readRegister(REG_PIXEL_GRAB);
		if ((data & 0x80) != 0) { // Data is valid
			if (count % MAP_SIZE == 0) {
				total += data & 0x7f;
			}
			count++;
		}
	} while (count < MAP_SIZE * (MAP_SIZE - 1));
#else
	do {
		byte data = readRegister(REG_PIXEL_GRAB);
		if ((data & 0x80) != 0) { // Data is valid
			total += data & 0x7f;
			count++;
		}
	} while (count < MAP_SIZE);
#endif

	return total;
}

/****** STEPPER MOTOR PART ******/

// D8..D11 == PORTB 0..3
//#define SET_MOTOR(bits) PORTB &= (0xf0 | (bits))
#define SET_MOTOR(bits) PORTB = (bits)

#define STEP_A 0x08
#define STEP_B 0x02
#define STEP_C 0x04
#define STEP_D 0x01

#define STEP_AB STEP_A|STEP_B
#define STEP_BC STEP_B|STEP_C
#define STEP_CD STEP_C|STEP_D
#define STEP_DA STEP_D|STEP_A

byte steps[] = { STEP_A, STEP_AB, STEP_B, STEP_BC, STEP_C, STEP_CD, STEP_D, STEP_DA };
//byte steps[] = { STEP_A, STEP_B, STEP_C, STEP_D };
byte nbSteps = sizeof(steps);

byte motorStep = 0;

/*************/

Events::eventHandler *mouseTimer, *motorTimer;

bool dumping = false;
// currently on dark zone on not
bool dark;
// number of iteration since last darkness change
unsigned long count;
unsigned long stepCount;

void mouseCallback(byte detail, void *data) {
	if (!dumping) {
		return;
	}

	int value = dumpFrame();
	if ((dark && value > THRESHOLD) || (!dark && value < THRESHOLD)) {
		Serial.print(dark ? '-' : '+'); Serial.println(count);
		dark = !dark;
		count = 1;
	} else {
		count++;
	}
}

void motorCallback(byte detail, void *data) {
	if (!dumping) {
		return;
	}

	SET_MOTOR(steps[motorStep]);
	motorStep = (motorStep + 1) % nbSteps;
	stepCount++;
}

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(SCLK, OUTPUT);
	pinMode(NRESET, OUTPUT);
	pinMode(NCS, OUTPUT);

	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);

	reset();
	byte productId = readRegister(REG_PRODUCT_ID);
	byte revisionId = readRegister(REG_REVISION_ID);
	Serial.print("Found productId ");
	Serial.print(productId, HEX);
	Serial.print(", rev. ");
	Serial.print(revisionId, HEX);
	Serial.println(
			productId == 0x02 ? " OK." : " Unknown productID. Carry on.");

	Scheduler.begin();
	mouseTimer = Scheduler.registerInterval(SPEED, mouseCallback, NULL);
	Serial.print("mouseTimer ");
	Serial.println((int)mouseTimer);
	motorTimer = Scheduler.registerInterval(SPEED, motorCallback, NULL);
	Serial.print("motorTimer ");
	Serial.println((int)motorTimer);
	Serial.print(nbSteps);
	Serial.println(" steps");
}

void loop() {
	// Allows to set the dump window by sending the number of lines to read via the serial port.
	if (Serial.available() > 0) {
		Serial.read();
		dumping = !dumping;
		Serial.println(dumping ? "ON" : "OFF");
		if (!dumping) {
			SET_MOTOR(0);
			Serial.println(stepCount);
		}
		count = 0;
		stepCount = 0;
		dark = true;
	}

	TODO : lire delta. si < 0, dump, calcul moyenne, et en déduire une bande
	1 - dump x, moyenne => voir le résultat
	2 - déduire de x et moyenne les bascules et donc les longueur de bandes blanches
	3 - isoler et convertir les nombres
	4 - essayer plus vite
//	if (readRegister(REG_MOTION) & 0x80) {
//		char dx = readRegister(REG_DELTA_X);
//		char dy = readRegister(REG_DELTA_Y);
//		writeRegister(REG_MOTION, 0);
//		if (dx != 0 || dy != 0) {
//			Serial.print("DELTA:");
//			Serial.print(dx, DEC);
//			Serial.print(" ");
//			Serial.println(dy, DEC);
//		}
//	}

	Scheduler.waitNext();
}
