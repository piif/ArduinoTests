#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
	// example : #include "led7.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define BLUE   6
#define GREEN  5
#define COMMON 4
#define RED    3

#define POWER 128

int M = 10;

void readColors(int &k, int &r, int &g, int &b, int &y) {
	k = r = g = b = y = 0;

	digitalWrite(BLUE , LOW);
	digitalWrite(GREEN, LOW);
	digitalWrite(RED  , LOW);
	for (int i = 0; i < M; i++) {
		delay(5);
		k += analogRead(A0);
	}
	k /= M;

	analogWrite (RED  , POWER);
	for (int i = 0; i < M; i++) {
		delay(5);
		r += analogRead(A0);
	}
	r /= M;

	digitalWrite(RED  , LOW);
	analogWrite (GREEN, POWER);
	for (int i = 0; i < M; i++) {
		delay(5);
		g += analogRead(A0);
	}
	g /= M;

	analogWrite (BLUE , POWER);
	for (int i = 0; i < M; i++) {
		delay(5);
		y += analogRead(A0);
	}
	y /= M;

	digitalWrite(GREEN, LOW);
	for (int i = 0; i < M; i++) {
		delay(5);
		b += analogRead(A0);
	}
	b /= M;

	digitalWrite(BLUE  , LOW);
	digitalWrite(GREEN , LOW);
	digitalWrite(RED   , LOW);
}

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(BLUE  , OUTPUT);
	pinMode(GREEN , OUTPUT);
	pinMode(RED   , OUTPUT);
	pinMode(COMMON, OUTPUT);

	digitalWrite(COMMON, LOW);
	digitalWrite(BLUE  , LOW);
	digitalWrite(GREEN , LOW);
	digitalWrite(RED   , LOW);
}

int k, r, g, b, y;
void loop() {
	delay(250);

	readColors(k, r, g, b, y);

	Serial.print("k ");
	Serial.print(k);
	Serial.print("\tr ");
	Serial.print(r);
	Serial.print("\tg ");
	Serial.print(g);
	Serial.print("\tb ");
	Serial.print(b);
	Serial.print("\ty ");
	Serial.println(y);
}
