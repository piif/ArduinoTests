#include <Arduino.h>

#define BUTTON_CLEAR  4
#define BUTTON_CLOCK  2
#define BUTTON_DATA   3

#define OUTPUT_CLEAR  8
#define OUTPUT_CLOCK  9
#define OUTPUT_DATA  10

void setup() {
	Serial.begin(115200);

	pinMode(BUTTON_CLEAR, INPUT_PULLUP);
	pinMode(BUTTON_CLOCK, INPUT_PULLUP);
	pinMode(BUTTON_DATA , INPUT_PULLUP);

	pinMode(OUTPUT_CLEAR, OUTPUT);
	pinMode(OUTPUT_CLOCK, OUTPUT);
	pinMode(OUTPUT_DATA, OUTPUT);
}

byte readButton(byte button, byte oldValue) {
	byte input = !digitalRead(button);
	if (input != oldValue) {
		delay(10);
		input = !digitalRead(button);
		if (input != oldValue) {
			Serial.print(button);
			Serial.print(" - > ");
			Serial.print(input);
			return input;
		}
	}
	return oldValue;
}

byte clear = 0, data = 0, clock = 0;

void loop() {
	clear= readButton(BUTTON_CLEAR, clear);
	digitalWrite(OUTPUT_CLEAR, !clear);

	clock= readButton(BUTTON_CLOCK, clock);
	digitalWrite(OUTPUT_CLOCK, clock);

	data = readButton(BUTTON_DATA, data);
	digitalWrite(OUTPUT_DATA, data);

	delay(100);
}
