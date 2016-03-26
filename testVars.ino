#include <Arduino.h>

#define A 42

void setup() {
	Serial.begin(115200);
	for (int i = A; i < 3 * A; i+= A) {
		Serial.println(i);
	}

}

void loop() {
}
