#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include "serialInput/serialInput.h"
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
	#include "serialInput.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif


#define LED 13
#define SENSOR A0
#define MOTOR_DIGIT 2
#define MOTOR_PWM 3


void setMotor(int s) {
	if (s == 0) {
		digitalWrite(MOTOR_DIGIT, LOW);
		digitalWrite(MOTOR_PWM, LOW);
		Serial.println("motor off");
	} else if (s > 0) {
		digitalWrite(MOTOR_DIGIT, LOW);
		analogWrite(MOTOR_PWM, s);
		Serial.print("motor ");Serial.println(s);
	} else {
		digitalWrite(MOTOR_DIGIT, HIGH);
		analogWrite(MOTOR_PWM, ~(-s));
		Serial.print("motor ");Serial.println(s);
	}
}

void setLight(int s) {
	// TODO
}

void readSensor() {
	Serial.println("Reading sensor");
	for(int i = 0; i < 20; i++) {
		int v = analogRead(SENSOR);
		Serial.println(v);
		delay(1000);
	}
}

InputItem inputs[] = {
	{ 'm', 'I', (void *)setMotor },
	{ 'l', 'I', (void *)setLight },
	{ 's', 'S', (void *)readSensor }
};

void help() {
	Serial.println("m [-255..255] : motor speed");
	Serial.println("l 0 / 1 light off / on");
	Serial.println("s read sensor");
}

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(LED, OUTPUT);
	pinMode(MOTOR_DIGIT, OUTPUT);
	pinMode(MOTOR_PWM, OUTPUT);
	pinMode(SENSOR, INPUT);

	setMotor(0);
	setLight(0);

	registerInput(sizeof(inputs), inputs);
	help();
}

bool sensor = LOW;

void loop() {
	handleInput();
//	sensor = digitalRead(SENSOR);
//	digitalWrite(LED, sensor);
//	delay(100);
}
