// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include <Servo/src/Servo.h>
	#include <serialInput/serialInput.h>
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

#define SERVO 11
#define SERVO_RANGE
//, 850, 2400
Servo motor;

void set(int v) {
	motor.write(v);
}

InputItem inputs[] = {
//	{ '?', 'f', (void *)status },
	{ 's', 'I', (void *)set },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(SERVO, OUTPUT);

	motor.attach(SERVO SERVO_RANGE);

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();
}
