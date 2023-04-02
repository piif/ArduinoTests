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

    // TODO : use internal counter instead of external tick
    /* DS3231 outputs nothing */
    rtc.setControl(0b10000100 , 0b10000000);

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
    char str[9], *ptr = str;
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
    ptr = appendChar(ptr, 0);
    byte len = ledMatrix.stringWidth(str) - 1;
    ledMatrix.drawString(offset + (maxLen-len)/2, str);

#ifdef HAVE_SERIAL
    Serial.print(time->hours); Serial.print(':');
    Serial.print(time->minutes); Serial.print(':');
    Serial.print(time->seconds); Serial.println();
#endif
}

void displayDate(TimeStruct *time, byte offset, byte maxLen) {
    char str[16], *ptr = str;
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
    ptr = appendChar(ptr, 0);
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

#define ST_DISPLAY         0
#define ST_SET_YEAR        1
#define ST_SET_MONTH       2
#define ST_SET_DAY_OF_WEEK 3
#define ST_SET_DAY         4
#define ST_SET_HOUR        5
#define ST_SET_MINUTES     6
#define ST_SET_SECONDS     7

#define ST_LAST            7

byte state = ST_DISPLAY;

char *prompts[] = {
    " : ",
    "annee",
    "mois",
    "jour semaine",
    "jour",
    "heure",
    "minute",
    "seconde"
};

void updateDisplay() {
    TimeStruct time;
    rtc.getTime(&time);
    toLocal(&time);

    ledMatrix.clear();

    if (state == ST_DISPLAY) {
        displayTime(&time, 0, 32);
        displayDate(&time, 32, 64);
    } else {
        char str[16], *ptr = str;
        ptr = appendStr(ptr, prompts[state]);
        ptr = appendStr(ptr, prompts[0]);
        ptr = appendChar(ptr, 0);
        ledMatrix.drawString(33, str);

        ptr = str;
        switch(state) {
            case ST_SET_YEAR:
                ptr = appendStr(ptr, "20");
                ptr = appendChar(ptr, '0' + time.year / 10);
                ptr = appendChar(ptr, '0' + time.year % 10);
            break;
            case ST_SET_MONTH:
                ptr = appendStr(ptr, shortMonthes[time.month]);
            break;
            case ST_SET_DAY_OF_WEEK:
                ptr = appendStr(ptr, shortDays[time.dayOfWeek]);
            break;
            case ST_SET_DAY:
                if (time.dayOfMonth > 10) {
                    ptr = appendChar(ptr, '0' + time.dayOfMonth / 10);
                }
                ptr = appendChar(ptr, '0' + time.dayOfMonth % 10);
            break;
            case ST_SET_HOUR:
                if (time.hours > 10) {
                    ptr = appendChar(ptr, '0' + time.hours / 10);
                }
                ptr = appendChar(ptr, '0' + time.hours % 10);
            break;
            case ST_SET_MINUTES:
                ptr = appendChar(ptr, '0' + time.minutes / 10);
                ptr = appendChar(ptr, '0' + time.minutes % 10);
            break;
            case ST_SET_SECONDS:
                ptr = appendChar(ptr, '0' + time.seconds / 10);
                ptr = appendChar(ptr, '0' + time.seconds % 10);
            break;
        }
        ptr = appendChar(ptr, 0);
        ledMatrix.drawString(0, str);
    }

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
            if (button == 1) {
                if (state == ST_LAST) {
                    state = ST_DISPLAY;
                } else {
                    state++;
                }
            }
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
