// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include <serialInput/serialInput.h>
	#include <MsTimer2/MsTimer2.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
	// example : #include "led7.h"
	#include <serialInput.h>
	#include <MsTimer2.h>
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

// IR receiver must be connected on pind 3, 4 and 5
// 3 = data / 4 = GND -> set to LOW / 5 = Vcc -> set to HIGH
#define LEFT_EN 4
#define LEFT_PWM 5
#define RIGHT_EN 7
#define RIGHT_PWM 6

#define LEFT_SENSOR A1
#define RIGHT_SENSOR A0
#define POWER_SENSOR A2

#define WITHOUT_SERIAL

void setup() {
#if !defined(WITHOUT_SERIAL)
	Serial.begin(DEFAULT_BAUDRATE);
#endif
	pinMode(13, OUTPUT);

	pinMode(LEFT_EN, OUTPUT);
	pinMode(LEFT_PWM, OUTPUT);
	pinMode(RIGHT_EN, OUTPUT);
	pinMode(RIGHT_PWM, OUTPUT);
	pinMode(LEFT_SENSOR, INPUT);
	pinMode(RIGHT_SENSOR, INPUT);
	pinMode(POWER_SENSOR, INPUT);

	digitalWrite(LEFT_EN, LOW);
	analogWrite(LEFT_PWM, 0);

	digitalWrite(RIGHT_EN, LOW);
	analogWrite(RIGHT_PWM, 0);


#if !defined(WITHOUT_SERIAL)
	Serial.println("setup ok");
#endif
}

int l, r, p;

void loop() {
	p = analogRead(POWER_SENSOR);

	if (p < 900) {
#if !defined(WITHOUT_SERIAL)
		Serial.print("Power : ");Serial.println(p);
#else
		digitalWrite(13, LOW);
#endif
		digitalWrite(LEFT_EN, LOW);
		analogWrite(LEFT_PWM, 0);

		digitalWrite(RIGHT_EN, LOW);
		analogWrite(RIGHT_PWM, 0);

		delay(1000);
		return;
	}
#if defined(WITHOUT_SERIAL)
	digitalWrite(13, HIGH);
#endif

	l = analogRead(LEFT_SENSOR);
	r = analogRead(RIGHT_SENSOR);

	if (l < 100) {
		digitalWrite(RIGHT_EN, HIGH);
		analogWrite(RIGHT_PWM, 100);
	} else {
		digitalWrite(RIGHT_EN, LOW);
		analogWrite(RIGHT_PWM, 150);
	}
	if (r < 100) {
		digitalWrite(LEFT_EN, HIGH);
		analogWrite(LEFT_PWM, 100);
	} else {
		digitalWrite(LEFT_EN, LOW);
		analogWrite(LEFT_PWM, 150);
	}
	delay(500);

#if !defined(WITHOUT_SERIAL)
	Serial.print("Left  : ");Serial.println(l);
	Serial.print("Right : ");Serial.println(r);
	Serial.print("Power : ");Serial.println(p);
#endif
}
