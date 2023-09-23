#include <Arduino.h>

/* digit selection pins (+) */
#define POWER_TRIGGER 4
#define BLINK_LED 13
#define BUTTON_LED 5
#define BUTTON 6


void setup() {
	pinMode(POWER_TRIGGER, OUTPUT);
	pinMode(BLINK_LED, OUTPUT);
	pinMode(BUTTON_LED, OUTPUT);
	pinMode(BUTTON, INPUT);
}

void loop() {
    digitalWrite(POWER_TRIGGER, HIGH);
    for(byte i=0; i<5; i++) {
        digitalWrite(BLINK_LED, HIGH);
        delay(500);
        digitalWrite(BLINK_LED, LOW);
        delay(500);
    }
    digitalWrite(BLINK_LED, HIGH);
	// pinMode(POWER_TRIGGER, INPUT);
    digitalWrite(POWER_TRIGGER, LOW);
    delay(3000);
}
