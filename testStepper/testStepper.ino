#include <Arduino.h>
#include "serialInput.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define STEP_A  5
#define STEP_B  6
#define STEP_C  9
#define STEP_D 10
#define ENABLE_1 4
#define ENABLE_2 8

byte fullSteps[] = { 0x1, 0x2, 0x4, 0x8 };
byte halfSteps[] = { 0x1, 0x3, 0x2, 0x6, 0x4, 0xc, 0x8, 0x9  };

bool full;     // send half/full steps
byte maxSteps; // how many steps combinations
byte *steps;   // current step list
byte step;     // current step

unsigned long nextStep = 0;
int speed = 0; // delay (in ms) between steps
int stepsToDo = 0;
bool clockwise = 1;

void setSpeed(int s) {
	if (s == 0) {
		speed = 0;
		digitalWrite(ENABLE_1, 0);
		digitalWrite(ENABLE_2, 0);
	} else {
		digitalWrite(ENABLE_1, 1);
		digitalWrite(ENABLE_2, 1);
	}
	if (s < 0) {
		clockwise = 0;
		speed = -s;
	} else {
		clockwise = 1;
		speed = s;
	}
	nextStep = 0;
}

void setHalf() {
	full = false;
	steps = halfSteps;
	maxSteps = sizeof(halfSteps) - 1;
}

void setFull() {
	full = true;
	steps = fullSteps;
	maxSteps = sizeof(fullSteps) - 1;
}

void doStep() {
	unsigned long now = millis();
	if (stepsToDo == 0 && (speed == 0 || now < nextStep)) {
		return;
	}
	// Serial.print("Step ");Serial.print(steps[step], BIN);Serial.print(" / ");Serial.print(maxSteps);Serial.print(" @ ");Serial.println(now);
	nextStep = now + speed;

	digitalWrite(STEP_A, steps[step] & 1);
	digitalWrite(STEP_B, steps[step] & 2);
	digitalWrite(STEP_C, steps[step] & 4);
	digitalWrite(STEP_D, steps[step] & 8);

	if (clockwise) {
		step = (step + 1) & maxSteps;
	} else {
		step = (step + maxSteps) & maxSteps;
	}
}

void doNStep(int n) {
	setSpeed(10);
	if (n < 0) {
		stepsToDo = -n;
		clockwise = 0;
	} else {
		stepsToDo = n;
		clockwise = 1;
	}
	Serial.print("Running ");Serial.print(stepsToDo);Serial.println(" steps");
	for(;stepsToDo > 0; stepsToDo--) {
		delay(speed);
		doStep();
	}
	setSpeed(0);
	Serial.println("End");
}

InputItem inputs[] = {
	{ 's', 'I', (void *)setSpeed  },
	{ 'n', 'I', (void *)doNStep  },
	{ 'h', 'f', (void *)setHalf },
	{ 'f', 'f', (void *)setFull }
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(STEP_A,   OUTPUT);
	pinMode(STEP_B,   OUTPUT);
	pinMode(STEP_C,   OUTPUT);
	pinMode(STEP_D,   OUTPUT);
	pinMode(ENABLE_1, OUTPUT);
	pinMode(ENABLE_2, OUTPUT);

	setFull();
	setSpeed(0);
	registerInput(sizeof(inputs), inputs);

	Serial.println("setup ok");
}

void loop() {
	handleInput();
	doStep();
}
