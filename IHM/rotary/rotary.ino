#include <Arduino.h>

#define IN_A 2   // White
#define COMMON 3 // Red
#define IN_B 4   // Black

void setup() {
	Serial.begin(115200);

	pinMode(COMMON, OUTPUT);
	digitalWrite(COMMON, LOW);
	pinMode(IN_A, INPUT_PULLUP);
	pinMode(IN_B, INPUT_PULLUP);

	Serial.println("Setup OK");
}

byte state = 0;
short dir = 0;
byte counter = 0;

// TODO : Handle logarithmic increment

void loop() {
	byte newState = 0;
	newState |= digitalRead(IN_A);
	newState |= digitalRead(IN_B) << 1;
	if (state != newState) {
		delay(50);
		newState |= digitalRead(IN_A);
		newState |= digitalRead(IN_B) << 1;
	}
	if (state != newState) {
		switch((state << 2) | newState) {
		case 0b0001:
		case 0b0111:
		case 0b1110:
		case 0b1000:
			dir=1;
			break;
		case 0b0010:
		case 0b1011:
		case 0b1101:
		case 0b0100:
			dir=-1;
			break;
		default:
			break;
		}

		counter += dir;
		state = newState;
		Serial.print(state);
		Serial.print('\t');
		Serial.println(counter);
	}
}
