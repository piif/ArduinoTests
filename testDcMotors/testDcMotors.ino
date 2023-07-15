#include <Arduino.h>
#include <serialInput.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define EN_1 4
#define M1_A 5
#define M1_B 6

int speed = 0;

void setSpeed(int v) {
    speed = v;
    if (speed == 0) {
        digitalWrite(EN_1, 0);
    } else {
        if (speed > 0) {
            digitalWrite(M1_B, 0);
            analogWrite(M1_A, speed);
        } else {
            analogWrite(M1_B, -speed);
            digitalWrite(M1_A, 0);
        }
        digitalWrite(EN_1, 1);
    }
}

InputItem inputs[] = {
//	{ '?', 'f', (void *)status },
	{ 's', 'I', (void *)setSpeed },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);
	pinMode(EN_1, OUTPUT);
	pinMode(M1_A, OUTPUT);
	pinMode(M1_B, OUTPUT);

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();
}
