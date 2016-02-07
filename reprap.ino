#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
#endif

#ifndef DEFAULT_BAUDRATE
	#ifdef __AVR_ATmega644P__
		#define DEFAULT_BAUDRATE 19200
	#else
		#define DEFAULT_BAUDRATE 115200
	#endif
#endif

// FROM https://github.com/kliment/Sprinter/blob/master/Sprinter/pins.h#L301
// x axis
#define X_STEP_PIN 15
#define X_DIR_PIN 18
#define X_MIN_PIN 20
#define X_ENABLE_PIN 24 //actually uses Y_enable_pin
#define X_MAX_PIN -1

// y axes
#define Y_STEP_PIN 23
#define Y_DIR_PIN 22
#define Y_MIN_PIN 25
#define Y_ENABLE_PIN 24 //shared with X_enable_pin
#define Y_MAX_PIN -1

// z axes
#define Z_STEP_PIN 27
#define Z_DIR_PIN 28
#define Z_MIN_PIN 30
#define Z_ENABLE_PIN 29
#define Z_MAX_PIN -1

//extruder pins
#define E_STEP_PIN 12
#define E_DIR_PIN 17
#define E_ENABLE_PIN 3
#define HEATER_0_PIN 16
#define TEMP_0_PIN 0
//TODO : comprendre de ou sort le CORE_DIR dans le makefile de arddude/target/sanguino

// LOW should open mofset linked to FAN connector
#define FAN_PIN 1

#define LED 4

#define NB_X 100
#define X_DELAY 1

#define NB_Y 20
#define Y_DELAY 20

#define NB_Z 1000
#define Z_DELAY 1

#define NB_E 150
#define E_DELAY 1

void setup(void) {
//	Serial.begin(DEFAULT_BAUDRATE);
	pinMode(LED, OUTPUT);

	digitalWrite(LED, LOW);

	pinMode(X_MIN_PIN, INPUT);
	pinMode(X_STEP_PIN, OUTPUT);
	pinMode(X_DIR_PIN, OUTPUT);
	pinMode(X_ENABLE_PIN, OUTPUT);
	digitalWrite(X_ENABLE_PIN, HIGH);

	pinMode(Y_MIN_PIN, INPUT);
	pinMode(Y_STEP_PIN, OUTPUT);
	pinMode(Y_DIR_PIN, OUTPUT);
	pinMode(Y_ENABLE_PIN, OUTPUT);
	digitalWrite(Y_ENABLE_PIN, HIGH);

	pinMode(Z_MIN_PIN, INPUT);
	pinMode(Z_STEP_PIN, OUTPUT);
	pinMode(Z_DIR_PIN, OUTPUT);
	pinMode(Z_ENABLE_PIN, OUTPUT);
	digitalWrite(Z_ENABLE_PIN, HIGH);

	pinMode(E_STEP_PIN, OUTPUT);
	pinMode(E_DIR_PIN, OUTPUT);
	pinMode(E_ENABLE_PIN, OUTPUT);
	digitalWrite(E_ENABLE_PIN, HIGH);

	digitalWrite(Y_ENABLE_PIN, LOW);
}

int x = 0, y = NB_Y, z = 0, e = NB_E;
bool dx = true, dy = false, dz = true, de = false;

int tx = 0, ty = 0, tz = 0, te = 0;

void loop() {

	if (tx == 0) {
		tx = X_DELAY;

		if (dx) {
			x++;
			if (x == NB_X) {
				dx = false;
			}
		} else {
			x--;
			if (x == 0) {
				dx = true;
			}
		}
		digitalWrite(X_DIR_PIN, dx);
//		digitalWrite(X_ENABLE_PIN, LOW);
		digitalWrite(X_STEP_PIN, HIGH);
		delay(1);
		digitalWrite(X_STEP_PIN, LOW);
		delay(1);
//		digitalWrite(X_ENABLE_PIN, HIGH);
	} else {
		tx--;
	}

	if (ty == 0) {
		ty = Y_DELAY;

		if (dy) {
			y++;
			if (y == NB_Y) {
				dy = false;
			}
		} else {
			y--;
			if (y == 0) {
				dy = true;
			}
		}
		digitalWrite(Y_DIR_PIN, dy);
//		digitalWrite(Y_ENABLE_PIN, LOW);
		digitalWrite(Y_STEP_PIN, HIGH);
		delay(1);
		digitalWrite(Y_STEP_PIN, LOW);
		delay(1);
//		digitalWrite(Y_ENABLE_PIN, HIGH);
	} else {
		ty--;
	}

	if (tz == 0) {
		tz = Z_DELAY;

		if (dz) {
			z++;
			if (z == NB_Z) {
				dz = false;
			}
		} else {
			z--;
			if (z == 0) {
				dz = true;
			}
		}
		digitalWrite(Z_DIR_PIN, dz);
		digitalWrite(Z_ENABLE_PIN, LOW);
		digitalWrite(Z_STEP_PIN, HIGH);
		delay(1);
		digitalWrite(Z_STEP_PIN, LOW);
		delay(1);
		digitalWrite(Z_ENABLE_PIN, HIGH);
	} else {
		tz--;
	}

	if (te == 0) {
		te = E_DELAY;

		if (de) {
			e++;
			if (e == NB_E) {
				de = false;
			}
		} else {
			e--;
			if (e == 0) {
				de = true;
			}
		}
		digitalWrite(E_DIR_PIN, de);
		digitalWrite(E_ENABLE_PIN, LOW);
		digitalWrite(E_STEP_PIN, HIGH);
		delay(1);
		digitalWrite(E_STEP_PIN, LOW);
		delay(1);
		digitalWrite(E_ENABLE_PIN, HIGH);
	} else {
		te--;
	}

	delay(1);

//
//	for(int i = 0; i < NB_STEPS; i++) {
//		digitalWrite(Z_STEP_PIN, HIGH);
//		delay(STEP_DELAY);
//		digitalWrite(Z_STEP_PIN, LOW);
//		delay(STEP_DELAY);
//	}
//
//	digitalWrite(Z_ENABLE_PIN, HIGH);
//	delay(400);
//
//	digitalWrite(Z_DIR_PIN, HIGH);
//	digitalWrite(Z_ENABLE_PIN, LOW);
//	for(int i = 0; i < NB_STEPS; i++) {
//		digitalWrite(Z_STEP_PIN, HIGH);
//		delay(STEP_DELAY);
//		digitalWrite(Z_STEP_PIN, LOW);
//		delay(STEP_DELAY);
//	}
//	digitalWrite(Z_ENABLE_PIN, HIGH);
//	delay(400);

}
