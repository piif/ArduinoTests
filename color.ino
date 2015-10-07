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

// RGB Led pineout
#define RED 9
#define GREEN 10
#define BLUE 11

#define THRESHOLD 800
#define READ_DELAY 20

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);

	pinMode(A5, INPUT_PULLUP);
	pinMode(2, INPUT);

//	captor.reset();
//	byte productId;
//	byte revisionId;
//	captor.getId(productId, revisionId);
//
//	Serial.print("* Found productId ");
//	Serial.print(productId, HEX);
//	Serial.print(", rev. ");
//	Serial.print(revisionId, HEX);
//	Serial.println(productId == 0x02 ? " OK." : " Unknown productID. Carry on.");
}

const char *colors[] = {
	"black",
	"red",
	"green",
	"yellow",
	"blue",
	"magenta",
	"cyan",
	"white"
};

byte color = 0;

int count;

void loop() {
	delay(20*READ_DELAY);

	digitalWrite(RED, 1);
	delay(READ_DELAY);
	int r = analogRead(A5);

	digitalWrite(RED, 0);
	digitalWrite(GREEN, 1);
	delay(READ_DELAY);
	int g = analogRead(A5);

	digitalWrite(GREEN, 0);
	digitalWrite(BLUE, 1);
	delay(READ_DELAY);
	int b = analogRead(A5);

	digitalWrite(BLUE, 0);
	delay(READ_DELAY);
	int k = analogRead(A5);

	Serial.print(r);
	Serial.print('\t');
	Serial.print(g);
	Serial.print('\t');
	Serial.print(b);
	Serial.print('\t');
	Serial.println(k);

	byte newColor;

//	if (r > 800 && g > 800 && b > 800) {
//		newColor = 0;
//	} else if (r < 250) {
//		if (g < 620) {
//			newColor = 3;
//		} else {
//			newColor = 1;
//		}
//	} else if (g < 620) {
//		newColor = 2;
//	} else if (b < 700) {
//		newColor = 4;
//	}

	newColor =
		((r < THRESHOLD) ? 1 : 0) |
		((g < THRESHOLD) ? 2 : 0) |
		((b < THRESHOLD) ? 4 : 0);

	if (color != newColor) {
		color = newColor;
		Serial.println(colors[color]);
	}

//	if(count++ % 10 == 0) {
//	Serial.print(digitalRead(2));
//	Serial.print(" ");
//	Serial.println(analogRead(A5));
//	delay(500);
//	}
}
