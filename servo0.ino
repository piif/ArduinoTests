// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

#include <Arduino.h>
#include <serialInput.h>
#include <Servo.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define SERVO 9
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
