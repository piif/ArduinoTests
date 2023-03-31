#include <Arduino.h>
#include "ds3231.h"
#include "ledMatrix.h"
#include "buttons.h"

#define HAVE_SERIAL

// some tests with CS16940E component, display/keys handler

DS3231 rtc;
Buttons buttons;

void setup() {
#ifdef HAVE_SERIAL
    Serial.begin(115200);
#endif

    // initialize external interrupt on pin 2 (INT0)
    EICRA |= 0x03;
    EIMSK |= 0x01;

    // initialize external interrupt on pin A0 (PCINT8)
    PCMSK1 |= 1 << PCINT8;
    PCICR  |= 1 << PCIE1;

#ifdef HAVE_SERIAL
    Serial.println("Setup OK");
#endif
}

void displayTime(TimeStruct *time) {

#ifdef HAVE_SERIAL
    Serial.print(time.hours); Serial.print(':');
    Serial.print(time.minutes); Serial.print(':');
    Serial.print(time.seconds); Serial.println();
#endif
}

void displayDay(TimeStruct *time) {

#ifdef HAVE_SERIAL
    Serial.print(shortDays[time.dayOfWeek]); Serial.print(' ');
    Serial.print(time.dayOfMonth); Serial.print(' ');
#endif
}

void displayMonth(TimeStruct *time) {
#ifdef HAVE_SERIAL
    Serial.print(shortMonthes[time.month]); Serial.print(' ');
    Serial.print(time.year); Serial.print(' ');
#endif
}

volatile bool clockTick = 0;

ISR(INT0_vect) {
    clockTick = 1;
}

volatile bool buttonChange = 0;

ISR(PCINT1_vect) {
    buttonChange = 1;
}

void updateDisplay() {
    TimeStruct time;
    rtc.getTime(&time);
    toLocal(&time);

    switch(button) {
        case 0:
            displayTime(&time);
        break;
        case 1:
            displayDay(&time);
        break;
        case 2:
            displayMonth(&time);
        break;
    }
}

void loop() {
#ifdef HAVE_SERIAL
    if (buttonChange) {
        buttonChange = 0;
        byte button = buttons.read();
        if (button != NO_BUTTON_CHANGE) {
            Serial.print("Button "); Serial.println(button);
            updateDisplay();
        }
#endif
    }
    if (clockTick && button == 0) {
        updateDisplay();
        clockTick = 0;
    }
}
