#include <Arduino.h>

#define HAVE_SERIAL

#include "ds3231.h"
#include "timeFunctions.h"

#define DS_SQW A3

DS3231 rtc;

void displayTime() {
    TimeStruct time;
    rtc.getTime(&time);
    toLocal(&time);
#ifdef HAVE_SERIAL
    Serial.print("dayOfWeek : "); Serial.println(shortDays[time.dayOfWeek]);
    Serial.print("dayOfMonth: "); Serial.println(time.dayOfMonth);
    Serial.print("month     : "); Serial.println(time.month);
    Serial.print("year      : "); Serial.println(time.year);
    Serial.print("hours     : "); Serial.println(time.hours);
    Serial.print("minutes   : "); Serial.println(time.minutes);
    Serial.print("seconds   : "); Serial.println(time.seconds);

    float fullTemp = rtc.getFullTemp() / 4.0;
    short temp = rtc.getShortTemp();
    Serial.print("full temp : "); Serial.println(fullTemp);
    Serial.print("temp      : "); Serial.println(temp);
#endif
}

void setup() {
#ifdef HAVE_SERIAL
    Serial.begin(115200);
#endif

    // // initialize external interrupt on raising edge of pin 2 (INT0)
    // EICRA |= 0x03;
    // EIMSK |= 1 << INT0;

    // initialize external interrupt on pin A3 (PCINT11)
    PCMSK1 |= 1 << PCINT11;
    PCICR  |= 1 << PCIE1;

// #define INITIALIZE TRUE
#ifdef INITIALIZE
    TimeStruct time = {
        00, /* seconds */
        10, /* minutes */
        22, /* hours */
         6, /* dayOfWeek */
        25, /* dayOfMonth */
         3, /* month */
        23  /* year */
    };
    rtc.setTime(&time);

    /* output square wave + oscillator continue counting when power off + no oscillator output */
    rtc.setControl(0b00000000 , 0b10000000);
    /* output nothing */
    // setControl(0b10000100 , 0b10000000);
    /* output nothing but continue counting */
    // setControl(0b00000100 , 0b10001000);
#endif

#ifdef HAVE_SERIAL
    Serial.println("dumpAllRegisters :");
    rtc.dumpAllRegisters();
#endif
}

volatile bool clockTick = 0;

// ISR(INT0_vect) {
//     clockTick = 1;
// }
ISR(PCINT1_vect) {
    clockTick = digitalRead(DS_SQW);
}

void loop() {
    if (clockTick) {
        displayTime();
        clockTick = 0;
    }
}
