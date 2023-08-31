#include <Arduino.h>
#include "serialInput.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define STEP_A  8
#define STEP_B  9
#define STEP_C 10
#define STEP_D 11
#define ENABLE_1 3
#define ENABLE_2 3

byte fullSteps[] = {
	0b0001, 0b0010, 0b0100, 0b1000
};
byte halfSteps[] = {
	0b0001, 0b0011, 0b0010, 0b0110,
	0b0100, 0b1100, 0b1000, 0b1001
};

byte quarterSteps[] = {
	0b10010010, 0b00000010, 0b01000010, 0b01010010,
	0b01001010, 0b00001000, 0b00011010, 0b01011010,
	0b00011000, 0b00100000, 0b01101000, 0b01011000,
	0b01100000, 0b10000000, 0b10010000, 0b01010000,
};
byte heighthSteps[] = {
	// TODO : 0 = low, 1= pwm 33%, 2= pwm 66%, 3= high
};

enum { FULL, HALF, QUARTER, HEIGHTH } mode = FULL;     // send half/full steps
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

void setFull() {
	mode = FULL;
	steps = fullSteps;
	maxSteps = sizeof(fullSteps) - 1;
}

void setHalf() {
	mode = HALF;
	steps = halfSteps;
	maxSteps = sizeof(halfSteps) - 1;
}

void setQuarter() {
	mode = QUARTER;
	steps = quarterSteps;
	maxSteps = sizeof(quarterSteps) - 1;
}

void setHeighth() {
	mode = HEIGHTH;
	steps = heighthSteps;
	maxSteps = sizeof(heighthSteps) - 1;
}

void doStep() {
	unsigned long now = millis();
	if (stepsToDo == 0 && (speed == 0 || now < nextStep)) {
		return;
	}
	// Serial.print("Step ");Serial.print(steps[step], BIN);Serial.print(" / ");Serial.print(maxSteps);Serial.print(" @ ");Serial.println(now);
	nextStep = now + speed;

	if (mode == FULL || mode == HALF) {
		digitalWrite(STEP_A, steps[step] & 1);
		digitalWrite(STEP_B, steps[step] & 2);
		digitalWrite(STEP_C, steps[step] & 4);
		digitalWrite(STEP_D, steps[step] & 8);
	} else {
		digitalWrite(STEP_A, steps[step] & 0x02);
		digitalWrite(STEP_B, steps[step] & 0x08);
		if (steps[step] & 0x10) {
			analogWrite(STEP_C, 128);
		} else {
			digitalWrite(STEP_C, steps[step] & 0x20);
		}
		if (steps[step] & 0x40) {
			analogWrite(STEP_D, 128);
		} else {
			digitalWrite(STEP_D, steps[step] & 0x80);
		}
	}

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
	{ 'f', 'f', (void *)setFull },
	{ 'h', 'f', (void *)setHalf },
	{ '4', 'f', (void *)setQuarter },
	{ '8', 'f', (void *)setHeighth }
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
