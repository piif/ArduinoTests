#include <Arduino.h>
#include <Wire.h>
#include "ds3231.h"

void setup() {
    Serial.begin(115200);
    Wire.begin();

    EICRA |= 0x03;
    EIMSK |= 0x01;

// #define SET_TIME TRUE
#ifdef SET_TIME
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
#endif

    /* output square wave + oscillator continue counting when power off + no oscillator output */
    setControl(0b00000000 , 0b10000000);
    /* output nothing */
    // setControl(0b10000100 , 0b10000000);
    /* output nothing but continue counting */
    // setControl(0b00000100 , 0b10001000);

    Serial.println("dumpAllRegisters :");
    dumpAllRegisters();
}

void displayTime() {
    TimeStruct time;
    getTime(&time);
    Serial.print("dayOfWeek : "); Serial.println(time.dayOfWeek);
    Serial.print("dayOfMonth: "); Serial.println(time.dayOfMonth);
    Serial.print("month     : "); Serial.println(time.month);
    Serial.print("year      : "); Serial.println(time.year);
    Serial.print("hours     : "); Serial.println(time.hours);
    Serial.print("minutes   : "); Serial.println(time.minutes);
    Serial.print("seconds   : "); Serial.println(time.seconds);

    float fullTemp = getFullTemp() / 4.0;
    short temp = getShortTemp();
    Serial.print("full temp : "); Serial.println(fullTemp);
    Serial.print("temp      : "); Serial.println(temp);
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