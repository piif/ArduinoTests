#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
	#include "Lego/Lego.h"
	#include "serialInput/serialInput.h"
#else
	// other includes with short pathes
	#include "serialInput.h"
	#include "Lego.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

Lego lego;

//void readSensor(byte number) {
//	Serial.print("Reading sensor ");Serial.println(lego.sensors[number]);
//	for(int i = 0; i < 30; i++) {
//		int v = analogRead(lego.sensors[number]);
//		//bool b = lego.readSensor(number);
//		Serial.println(v);//Serial.print(" => ");Serial.println(b);
//		delay(500);
//	}
//}

void help() {
	Serial.println("m [0..3] [-255..255] : motor speed");
	Serial.println("l [0..3] 0 / 1 light off / on");
	Serial.println("s [0..3] read sensor");
}

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);
	lego.begin(Serial);
	help();
}

char buffer[50];
byte index = 0;

bool interpret() {
	buffer[index] = '\0';
	Serial.print("interpret ");Serial.println(buffer);
	// must have [mls]\s*\d(\s+\d+)
	if (index ==0) {
		return false;
	}
	char command = buffer[0];
	if (command != 'm' && command != 'l' && command != 's') {
		return false;
	}
	int i = 1;
	while(buffer[i] == ' ' && i < index) {
		i++;
	}
	if (i == index || (buffer[i] < '0' && buffer[i] > '3')) {
		return false;
	}
	byte number = buffer[i] - '0';
	if (command == 's') {
//		readSensor(number);
		bool b = lego.readSensor(number);
		Serial.print(" => ");Serial.println(b);
		return true;
	}
	i++;
	while(buffer[i] == ' ' && i < index) {
		i++;
	}
	if (i >= index) {
		return false;
	}
	if (command == 'l') {
		if (buffer[i] == '0') {
			lego.setLight(number, LOW);
			return true;
		} else if (buffer[i] == '1') {
			lego.setLight(number, HIGH);
			return true;
		} else {
			return false;
		}
	}
	int value = 0;
	bool neg = false;
	if (buffer[i] == '-') {
		neg = true;
		i++;
	}
	while(i < index && buffer[i] >= '0' && buffer[i] <= '9') {
		value = value * 10 + buffer[i] - '0';
		i++;
	}
	if (neg) {
		value = -value;
	}
	lego.setMotor(number, value);
}

void handleInput() {
	if (Serial.available()) {
		int c = Serial.read();
		if (c == -1 || c == '\n' || c == '\r') {
			interpret();
			index = 0;
		} else {
			buffer[index] = c;
			index++;
			if (index == 50) {
				interpret();
				index = 0;
			}
		}
	}
}

void loop() {
	handleInput();
//	sensor = digitalRead(SENSOR);
//	digitalWrite(LED, sensor);
//	delay(100);
}
