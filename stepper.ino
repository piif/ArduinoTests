// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoTools
#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include "serialInput.h"
#else
	#include "serialInput.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

//// A2..A5 == PORTC 2..5
//#define SET_MOTOR(bits) PORTC = (bits) << 2
// D8..D11 == PORTB 0..3
#define SET_MOTOR(bits) PORTB = (bits)

#define STEP_A 0x08
#define STEP_B 0x02
#define STEP_C 0x04
#define STEP_D 0x01

#define STEP_AB STEP_A|STEP_B
#define STEP_BC STEP_B|STEP_C
#define STEP_CD STEP_C|STEP_D
#define STEP_DA STEP_D|STEP_A

byte fullSteps[] = { STEP_A, STEP_B, STEP_C, STEP_D  };
byte halfSteps[] = { STEP_A, STEP_AB, STEP_B, STEP_BC, STEP_C, STEP_CD, STEP_D, STEP_DA  };

byte nbFullSteps = sizeof(fullSteps);
byte nbSteps = sizeof(halfSteps);

bool rotateRight = true;
bool remanent = false;
int remaining = 0;
int stepDelay = 1000;
unsigned long nextStep = 0;
bool full = true;
byte step = 0;
byte toSend = 0;

void setNumber(int d) {
	if (d > 0) {
		remaining = d;
		rotateRight = true;
	} else {
		remaining = -d;
		rotateRight = false;
	}
}

void setSpeed(int d) {
	stepDelay = 1000 - d;
}

void setHalf() {
	full = false;
}

void setFull() {
	full = true;
}

void setRemanent() {
	remanent = !remanent;
	if (remanent) {
		SET_MOTOR(toSend);
	} else {
		SET_MOTOR(0);
	}

}

void status() {
	Serial.println("Commands :");
	Serial.println("? : status");
	Serial.println("n v: launch v steps (<0 for backward");
	Serial.println("s v: set speed (0 = 1s per step to 1000 = 0ms per step)");
	Serial.println("r : set/unset remanent");
	Serial.println("h : set half");
	Serial.println("f : set full");
	if (remanent) {
		Serial.print("remanent ");
	}
	Serial.print(full ? "full" : "half");
	Serial.println(" steps");
	Serial.print("speed ");
	Serial.println(1000 - stepDelay);
}

void doStep() {
	if (remaining == 0) {
		return;
	}
	unsigned long now = millis();
	if (now >= nextStep) {
		step = step + (rotateRight ? 1 : -1);
		byte toSend;
		if (full) {
			step = step & (nbFullSteps - 1);
			toSend = fullSteps[step];
		} else {
			step = step & (nbSteps - 1);
			toSend = halfSteps[step];
		}
		Serial.print(step);
		Serial.print(" -> ");
		Serial.println(toSend);
		SET_MOTOR(toSend);

		remaining--;
		if (remaining == 0) {
			if (!remanent) {
				delay(stepDelay >> 1);
				SET_MOTOR(0);
			}
			return;
		}

		nextStep = now + stepDelay;
	}
}

InputItem inputs[] = {
	{ '?', 'f', (void *)status },
	{ 'n', 'I', (void *)setNumber },
	{ 's', 'I', (void *)setSpeed },
	{ 'r', 'f', (void *)setRemanent },
	{ 'h', 'f', (void *)setHalf },
	{ 'f', 'f', (void *)setFull }
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

//	pinMode(A2, OUTPUT);
//	pinMode(A3, OUTPUT);
//	pinMode(A4, OUTPUT);
//	pinMode(A5, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();
	doStep();
}
