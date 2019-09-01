// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include <Servo/src/Servo.h>
	#include <serialInput.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
	// example : #include "led7.h"
	#include <Servo.h>
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define LEFT_FOOT 11
#define RIGHT_FOOT 10
#define LEFT_LEG 8
#define RIGHT_LEG 9

Servo leftFoot, rightFoot;
Servo leftLeg, rightLeg;

int leftFootAngle = 0, rightFootAngle = 0;
int leftLegAngle = 0, rightLegAngle = 0;

int leftFootZero = 40, rightFootZero = 50;
int leftLegZero = 55, rightLegZero = 75;

void setLf(int angle) {
	leftFootAngle = angle;
	leftFoot.write(leftFootZero + angle);
}
void setRf(int angle) {
	rightFootAngle = angle;
	rightFoot.write(rightFootZero + angle);
}
void setLl(int angle) {
	leftLegAngle = angle;
	leftLeg.write(leftLegZero + angle);
}
void setRl(int angle) {
	rightLegAngle = angle;
	rightLeg.write(rightLegZero + angle);
}

void zero() {
	setLf(0);
	setRf(0);
	setLl(0);
	setRl(0);
}

void bow(int dir) {
	if (dir > 0) {
		setLf(40);
		delay(200);
		setRf(40);
	} else {
		setRf(-40);
		delay(200);
		setLf(-40);
	}
}

void walk(int nbSteps) {
	if (nbSteps > 0) {
		while(nbSteps --) {
			setLf(20);setRf(20);
			delay(200);
			setLl(20);setRl(20);
			delay(400);
			setLf(-20);setRf(-20);
			delay(200);
			setLl(-20);setRl(-20);
			delay(400);
		}
	} else {
		while(nbSteps ++) {
			setLf(20);setRf(20);
			delay(200);
			setLl(-20);setRl(-20);
			delay(400);
			setLf(-20);setRf(-20);
			delay(200);
			setLl(20);setRl(20);
			delay(400);
		}
		setLl(0);setRl(0);
		setLf(0);setRf(0);
	}
}

InputItem inputs[] = {
//	{ '?', 'f', (void *)status },
	{ 'l', 'I', (void *)setLf },
	{ 'r', 'I', (void *)setRf },
	{ 'L', 'I', (void *)setLl },
	{ 'R', 'I', (void *)setRl },
	{ 'z', 'I', (void *)zero },
	{ 'b', 'I', (void *)bow },
	{ 'w', 'I', (void *)walk }
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(LEFT_FOOT, OUTPUT);
	pinMode(RIGHT_FOOT, OUTPUT);
	pinMode(LEFT_LEG, OUTPUT);
	pinMode(RIGHT_LEG, OUTPUT);

	leftFoot.attach(LEFT_FOOT);
	rightFoot.attach(RIGHT_FOOT);
	leftLeg.attach(LEFT_LEG);
	rightLeg.attach(RIGHT_LEG);

	zero();

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();
}
