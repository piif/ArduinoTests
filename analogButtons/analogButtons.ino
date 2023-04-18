#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
	#define ARDUINO_TINY
#elif defined(__AVR_MEGA__)
	#define ARDUINO_UNO
    #define HAVE_SERIAL
#endif

#include "Arduino.h"

#ifdef ARDUINO_UNO
	#define BUTTONS A0
#else
	#define BUTTONS 3
	// #define DEBUG_PORT 0
	// #define TICK_PORT 1

	// #define DEBUG_START 0b10101110
	// #define DEBUG_END   0b01110101

	// void sendDebug(byte port, byte value) {
	// 	for(byte mask = 0x80 ; mask >>= 1; mask) {
	// 		digitalWrite(port, (value & mask) != 0);
	// 	}
	// }
#endif

void setup() {
	pinMode(BUTTONS, INPUT);
#ifdef ARDUINO_UNO
	Serial.begin(115200);
	Serial.println("Ready");
#else
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
    TCCR1 = (1 << PWM1A) | (1 << COM1A1) | 0x6; // reset on top | match output on OC1A = out 1 | prescale 32
    OCR1A = 128;
    GTCCR = 0;

	// pinMode(DEBUG_PORT, OUTPUT);
	// pinMode(TICK_PORT, OUTPUT);
#endif
}

byte decode(int v) {
	if (v > 950) {
		return 3;
	}
	if (v > 800) {
		return 2;
	}
	if (v > 700) {
		return 1;
	}
	return 0;
}

byte button = 0;

int v = 0;

void loop() {
#ifdef ARDUINO_UNO
	v =  analogRead(BUTTONS);
	Serial.println(v);
	byte newButton = decode(v);
	if (newButton != button) {
		delay(100);
		v =  analogRead(BUTTONS);
		newButton = decode(v);
		if (newButton != button) {
			button = newButton;
			Serial.println(button);
		}
	}
	delay(500);
#else
	v =  analogRead(BUTTONS);
	OCR1A = (byte)(v >> 2);
	byte button = decode(v);
	digitalWrite(0, button & 1);
	digitalWrite(2, button & 2);

	// static long counter = 0;
	// static bool tick = 0;
	// counter++;
	// if (counter == 1000) {
	// 	counter = 0;
	// 	tick = !tick;
	// 	digitalWrite(TICK_PORT, tick);
	// 	v =  analogRead(BUTTONS);
	// }
	// sendDebug(DEBUG_PORT, DEBUG_START);
	// sendDebug(DEBUG_PORT, (byte)(v >> 8));
	// sendDebug(DEBUG_PORT, (byte)(v & 0xFF));
	// sendDebug(DEBUG_PORT, DEBUG_END);
#endif
}
