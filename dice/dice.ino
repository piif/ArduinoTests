#include <Arduino.h>
#include <avr/sleep.h>

#define setA 1
#define setB 2
#define setC 4
#define setD 8

#define outA 4
#define outB 5
#define outC 6
#define outD 7

#define BUTTON 2

void dice2output(byte dice) {
    byte outputs;
    // TODO : convert this with bit mask on output register
    switch(dice) {
        case 1: outputs = setD;
        break;
        case 2: outputs = setC;
        break;
        case 3: outputs = setC | setD;
        break;
        case 4: outputs = setA | setC;
        break;
        case 5: outputs = setA | setC | setD;
        break;
        case 6: outputs = setA | setB | setC;
        break;
        default: outputs = 0;
    }

    digitalWrite(outA, outputs & setA);
    digitalWrite(outB, outputs & setB);
    digitalWrite(outC, outputs & setC);
    digitalWrite(outD, outputs & setD);
}

enum { PAUSED, PRESSED, TO_ROLL, ROLLING } state = PAUSED;
unsigned long seed;
unsigned long tick = 0;

void roll() {
    byte dice = random(1, 7);
    dice2output(dice);
    for(int i=random(20); i<30; i++) {
        if (state == PAUSED) {
            dice2output(0);
            return;
        }
        if (i<26) {
            delay(20);
        } else {
            delay(100);
        }
        dice = (dice + random(1, 5)) % 6 + 1;
        dice2output(dice);
    }
}

void button() {
    byte b = digitalRead(BUTTON);
    if (b == 0) {
        if (state == ROLLING && millis() - tick > 50) {
            state = PAUSED;
        } else {
            state = PRESSED;
        }
        seed += micros();
        dice2output(0);
    } else {
        if (state == PRESSED) {
            state = TO_ROLL;
            tick = millis();
            seed += micros();
            randomSeed(seed);
        }
    }
}

void setup() {
	pinMode(outA, OUTPUT);
	pinMode(outB, OUTPUT);
	pinMode(outC, OUTPUT);
	pinMode(outD, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(2), button, CHANGE);
}

void loop() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();

    if (state == TO_ROLL) {
        state = ROLLING;
        roll();
        state = PAUSED;
        tick = 0;
    }
}
