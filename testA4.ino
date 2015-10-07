#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
#else
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define EMITTER 2
#define RECIEVER 3
#define LED 13

#define MOTOR_ENABLE 4
#define MOTOR_DIR    5

bool send = HIGH;
long next = 0;

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(EMITTER, OUTPUT);
	pinMode(RECIEVER, INPUT);
	pinMode(LED, OUTPUT);

	digitalWrite(EMITTER, send);

	pinMode(MOTOR_ENABLE, OUTPUT);
	pinMode(MOTOR_DIR, OUTPUT);

	Serial.println("Setup ok");
}

const char *states[4] = {
	"motor off",
	"motor clockwise",
	"motor off",
	"motor counterclockwise"
};

byte state;

void loop() {
//	long now = millis();
//	if (now > next) {
//		next = now + 2000;
//
//		switch(state) {
//		case 0:
//			digitalWrite(MOTOR_ENABLE, LOW);
//			break;
//		case 1:
//			digitalWrite(MOTOR_DIR, LOW);
//			digitalWrite(MOTOR_ENABLE, HIGH);
//			break;
//		case 2:
//			digitalWrite(MOTOR_ENABLE, LOW);
//			break;
//		case 3:
//			digitalWrite(MOTOR_DIR, HIGH);
//			digitalWrite(MOTOR_ENABLE, HIGH);
//			break;
//		}
//		Serial.println(states[state]);
//	}

	bool r = digitalRead(RECIEVER);
	digitalWrite(LED, !r);

	if (Serial.available()) {
		Serial.read();

		state = (state + 1) % 4;
		digitalWrite(MOTOR_ENABLE, state & 1);
		digitalWrite(MOTOR_DIR, state & 2);
		Serial.print("motor state ");
		Serial.println(state);

		send = !send;
		Serial.print("IR emitter ");
		Serial.println(send ? "OFF" : "ON");
		digitalWrite(EMITTER, send);
	}
}
