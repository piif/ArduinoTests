#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include "serialInput.h"
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
	// example : #include "led7.h"
	#include "serialInput.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define PORT_1A 11
#define PORT_1B 10
#define PORT_2A 3
#define PORT_2B 9
#define PORT_3A 12
#define PORT_3B 13
#define PORT_4A 8
#define PORT_4B 2

#define PORT_5A 1
#define PORT_5B 0
#define PORT_6A A2
#define PORT_6B A3
#define PORT_7A A6
#define PORT_7B A1
#define PORT_8A A7
#define PORT_8B A0

#define PORT_M1_POWER 6
#define PORT_M1_DIR 7
#define PORT_M2_POWER 5
#define PORT_M2_DIR 4

#define GRIP_SPEED 255
#define Y_SPEED 50
#define X_SPEED 255
#define STEP_DURATION 20

void grip(int duration) {
	byte closing = 1;
	if (duration < 0) {
		closing = 0;
		duration = -duration;
	}
	if (closing) {
		Serial.print("closing grip");
		digitalWrite(PORT_M1_DIR, HIGH);
	} else {
		Serial.print("opening grip");
		digitalWrite(PORT_M1_DIR, LOW);
	}
	analogWrite(PORT_M1_POWER, GRIP_SPEED);
	delay(duration);
	analogWrite(PORT_M1_POWER, 0);
	Serial.println(".");
}

int speed = STEP_DURATION;

void moveX(int steps) {
	byte dir = 1;
	if (steps < 0) {
		dir = 0;
		steps = -steps;
	}
	if (dir) {
		Serial.print("clockwise");
		digitalWrite(PORT_M2_DIR, HIGH);
	} else {
		Serial.print("counterclock");
		digitalWrite(PORT_M2_DIR, LOW);
	}
	digitalWrite(PORT_1A, dir);
	delay(5);
	while (steps--) {
		digitalWrite(PORT_1B, HIGH);
		delay(speed);
		digitalWrite(PORT_1B, LOW);
		delay(speed);
	}
	Serial.println(".");
}

void moveY(int duration) {
	byte closing = 1;
	if (duration < 0) {
		closing = 0;
		duration = -duration;
	}
	if (closing) {
		Serial.print("up");
		digitalWrite(PORT_M2_DIR, HIGH);
	} else {
		Serial.print("down");
		digitalWrite(PORT_M2_DIR, LOW);
	}
	analogWrite(PORT_M2_POWER, Y_SPEED);
	delay(duration);
	analogWrite(PORT_M2_POWER, 0);
	Serial.println(".");
}

void status() {
	Serial.println("Todo ...");
}

InputItem list[] = {
	{ '?', 'f', (void *)status },
	{ 'g', 'f', (void *)grip },
	{ 'x', 'f', (void *)moveX },
	{ 'y', 'f', (void *)moveY },
	{ 's', 'i', (void *)&speed }
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);
	registerInput(sizeof(list), list);

	pinMode(PORT_M1_POWER, OUTPUT);
	pinMode(PORT_M1_DIR, OUTPUT);
	pinMode(PORT_M2_POWER, OUTPUT);
	pinMode(PORT_M2_DIR, OUTPUT);
	pinMode(PORT_1A, OUTPUT);
	pinMode(PORT_1B, OUTPUT);
	Serial.println("Ready...\n");
}

void loop() {
	handleInput();
}
