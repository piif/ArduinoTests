#ifdef DS3231_TEST

#define HAVE_SERIAL
#define DS3231_DEBUG

#include <Arduino.h>
#include <Wire.h>
#include "ds3231.h"
#include "timeFunctions.h"

DS3231 rtc;

void setup() {
#ifdef HAVE_SERIAL
    Serial.begin(115200);
#endif

    // initialize external interrupt on pin 2 (INT0)
    EICRA |= 0x03;
    EIMSK |= 0x01;

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
    setTime(&time);

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

volatile bool clockTick = 0;

ISR(INT0_vect) {
    clockTick = 1;
}

void loop() {
    if (clockTick) {
        displayTime();
        clockTick = 0;
    }
}

#endif