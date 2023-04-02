#include <Arduino.h>
#include "ds3231.h"
#include "ledMatrix.h"
#include "buttons.h"

#define HAVE_SERIAL

#define DS_SDA A4
#define DS_SCL A5
#define DS_SQW A3

#define BUTTONS_PIN  A0

#define MATRIX_CLK A2
#define MATRIX_CS  A1
// #define MATRIX_DIN A4
#define MATRIX_DIN 7

DS3231 rtc;

// int buttonThreshold[] = { 600, 450, 300 };  // value wuth 22k / 10k / 10k
int buttonThreshold[] = { 750, 650, 550 };  // value wuth 56k / 10k / 10k
Buttons buttons = Buttons(BUTTONS_PIN, 3, buttonThreshold);

LedMatrix ledMatrix = LedMatrix(12, MATRIX_CLK, MATRIX_CS, MATRIX_DIN, 3);

void setup() {
#ifdef HAVE_SERIAL
    Serial.begin(115200);
#endif

    // initialize external interrupt on button pin A0 (PCINT8) + clock pin A3 (PCINT11)
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT11);
    PCICR  |= 1 << PCIE1;

#ifdef HAVE_SERIAL
    Serial.println("Setup OK");
#endif
}

char *appendStr(char* dst, char* src) {
    while (*src) {
        *dst++ = *src++;
    }
    return dst;
}
char *appendChar(char* dst, char src) {
    *dst++ = src;
    return dst;
}

void displayTime(TimeStruct *time, byte offset, byte maxLen) {
    char str[9] = { 0, }, *ptr = str;
    if (time->hours > 10) {
        ptr = appendChar(ptr, '0' + time->hours / 10);
    }
    ptr = appendChar(ptr, '0' + time->hours % 10);
    ptr = appendChar(ptr, ':');
    ptr = appendChar(ptr, '0' + time->minutes / 10);
    ptr = appendChar(ptr, '0' + time->minutes % 10);
    ptr = appendChar(ptr, ':');
    ptr = appendChar(ptr, '0' + time->seconds / 10);
    ptr = appendChar(ptr, '0' + time->seconds % 10);
    byte len = ledMatrix.stringWidth(str) - 1;
    ledMatrix.drawString(offset + (maxLen-len)/2, str);

#ifdef HAVE_SERIAL
    Serial.print(time->hours); Serial.print(':');
    Serial.print(time->minutes); Serial.print(':');
    Serial.print(time->seconds); Serial.println();
#endif
}

void displayDate(TimeStruct *time, byte offset, byte maxLen) {
    char str[16] = { 0, }, *ptr = str;
    ptr = appendStr(ptr, shortDays[time->dayOfWeek]);
    ptr = appendChar(ptr, ' ');
    if (time->dayOfMonth > 10) {
        ptr = appendChar(ptr, '0' + time->dayOfMonth / 10);
    }
    ptr = appendChar(ptr, '0' + time->dayOfMonth % 10);
    ptr = appendChar(ptr, ' ');
    ptr = appendStr(ptr, shortMonthes[time->month]);
    ptr = appendStr(ptr, " 20");
    ptr = appendChar(ptr, '0' + time->year / 10);
    ptr = appendChar(ptr, '0' + time->year % 10);
    byte len = ledMatrix.stringWidth(str) - 1;
    ledMatrix.drawString(offset + (maxLen-len)/2, str);

#ifdef HAVE_SERIAL
    Serial.print(shortDays[time->dayOfWeek]); Serial.print(' ');
    Serial.print(time->dayOfMonth); Serial.print(' ');
    Serial.print(shortMonthes[time->month]); Serial.print(' ');
    Serial.print(time->year); Serial.print(' ');
#endif
}

volatile bool clockTick = 0;
volatile bool buttonChange = 0;

ISR(PCINT1_vect) {
    clockTick = !digitalRead(DS_SQW);
    buttonChange = 1;
}

byte button = 0;

void updateDisplay() {
    TimeStruct time;
    rtc.getTime(&time);
    toLocal(&time);

    ledMatrix.clear();
    displayTime(&time, 0, 32);
    displayDate(&time, 32, 64);

    // debug buttons by displaying a pixel in the corner
    switch (button) {
        case 1:
            ledMatrix.drawPixel(0, 7, 1);
        break;
        case 2:
            ledMatrix.drawPixel(2, 7, 1);
        break;
        case 3:
            ledMatrix.drawPixel(4, 7, 1);
        break;
    }
    ledMatrix.flush();
}

void loop() {
    if (buttonChange) {
        byte newButton = buttons.read();
        if (newButton != NO_BUTTON_CHANGE) {
            button = newButton;
#ifdef HAVE_SERIAL
            Serial.print("Button "); Serial.println(button);
#endif
        } else {
            buttonChange = 0;
        }
    }
    if (clockTick || buttonChange) {
// #ifdef HAVE_SERIAL
//         Serial.print("Button value "); Serial.println(analogRead(BUTTONS_PIN));
// #endif
        updateDisplay();
        clockTick = 0;
        buttonChange = 0;
    }
}
