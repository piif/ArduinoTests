#include <Arduino.h>
#include <serialInput.h>

#ifndef DEFAULT_BAUDRATE
	#ifdef __AVR_ATmega644P__
		#define DEFAULT_BAUDRATE 19200
	#else
		#define DEFAULT_BAUDRATE 115200
	#endif
#endif

#define LEFT  5
#define RIGHT 6

byte left = 128, right = 128, speed = 200;

void status() {
	Serial.print("Left  : ");Serial.println(left);
	Serial.print("Right : ");Serial.println(right);
	Serial.print("Delay : ");Serial.println(speed);
}

// void setLeft(int v) {
// 	left = v;
// 	analogWrite(LEFT, left);
// 	status();
// }

// void setRight(int v) {
// 	right = v;
// 	analogWrite(RIGHT, right);
// 	status();
// }

InputItem inputs[] = {
	{ 'l', 'b', (void *)&left },
	{ 'r', 'b', (void *)&right },
	{ 'd', 'b', (void *)&speed },
	{ '?', 'f', (void *)status },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(LEFT , OUTPUT);
	pinMode(RIGHT, OUTPUT);
	// analogWrite(LEFT, left);
	// analogWrite(RIGHT, right);

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();
	if (speed==0) {
		digitalWrite(RIGHT, 0);
		digitalWrite(LEFT, 0);
	} else {
		digitalWrite(RIGHT, 0);
		analogWrite(LEFT, left);
		delay(speed);
		digitalWrite(LEFT, 0);
		analogWrite(RIGHT, right);
		delay(speed);
	}
}
