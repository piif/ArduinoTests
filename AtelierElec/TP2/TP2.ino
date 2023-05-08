#include <Arduino.h>

void setup() {
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
}

void loop() {
    digitalWrite(2, !digitalRead(4));
    digitalWrite(3, !digitalRead(5));
}