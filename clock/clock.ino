#include <Arduino.h>
#include "cs1694.h"
#include "dvdPanel.h"
#include "ds3231.h"

// some tests with CS16940E component, display/keys handler

CS1694 cs1694;
DVDPanel dvdPanel(&cs1694);
DS3231 rtc;

void setup() {
    Serial.begin(115200);

    // initialize external interrupt on pin 2 (INT0)
    EICRA |= 0x03;
    EIMSK |= 0x01;

    // sendCommand(0b00000010); // 6 Grids,11 Segs mode
    cs1694.sendCommand(0b00000011); // 7 Grids,10 Segs mode
    cs1694.sendCommand(0b10001011); // leds on, brightness 10/16

    dvdPanel.clearPanel();
    // dvdPanel.setDigit(0, 1);
    // dvdPanel.setDigit(1, 2);
    // dvdPanel.setDigit(2, 3);
    // dvdPanel.setDigit(3, 4);
    // dvdPanel.setDigit(4, 5);
    // dvdPanel.setDigit(5, 6);
    // dvdPanel.setDigit(6, 7);
    // dvdPanel.setSegment(dvdPanel.diskMap[1]);
    // dvdPanel.setSegment(dvdPanel.diskMap[3]);
    // dvdPanel.setSegment(dvdPanel.diskMap[5]);
    // dvdPanel.setSegment(dvdPanel.diskMap[7]);
    dvdPanel.flush();
    Serial.println("Setup OK");
}

word buttons = 0;

void displayTime() {
    TimeStruct time;
    rtc.getTime(&time);
    toLocal(&time);

    dvdPanel.clearPanel();
    if (buttons & BUTTON_5) {
        // display date
        dvdPanel.setDigit(0, time.dayOfWeek);
        dvdPanel.setDigit(1, time.dayOfMonth/10);
        dvdPanel.setDigit(2, time.dayOfMonth%10);
        dvdPanel.setDigit(3, time.month/10);
        dvdPanel.setDigit(4, time.month%10);
        dvdPanel.setDigit(5, time.year/10);
        dvdPanel.setDigit(6, time.year%10);
    } else {
        // display hour
        dvdPanel.setSegment(dvdPanel.colonMap[0]);
        dvdPanel.setSegment(dvdPanel.colonMap[1]);
        dvdPanel.setDigit(1, time.hours/10);
        dvdPanel.setDigit(2, time.hours%10);
        dvdPanel.setDigit(3, time.minutes/10);
        dvdPanel.setDigit(4, time.minutes%10);
        dvdPanel.setDigit(5, time.seconds/10);
        dvdPanel.setDigit(6, time.seconds%10);
    }
    dvdPanel.flush();
}

volatile bool clockTick = 0;

ISR(INT0_vect) {
    clockTick = 1;
}


void loop() {
    word newButtons = dvdPanel.checkButtons();
    if (newButtons != 0xFFFF) {
        buttons = newButtons;
        Serial.print("Buttons ");
        if (buttons & BUTTON_1) {
            Serial.print('A');
        }
        if (buttons & BUTTON_2) {
            Serial.print('B');
        }
        if (buttons & BUTTON_3) {
            Serial.print('C');
        }
        if (buttons & BUTTON_4) {
            Serial.print('D');
        }
        if (buttons & BUTTON_5) {
            Serial.print('E');
        }
        Serial.println();
    }
    if (clockTick) {
        displayTime();
        clockTick = 0;
    }
}
