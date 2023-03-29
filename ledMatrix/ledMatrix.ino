#include <Arduino.h>
#include "font7.h"
#define HAVE_SERIAL

// test MAX7219 chained led matrix 8x8xN

#define MATRIX_CLK 5
#define MATRIX_CS  6
#define MATRIX_DIN 7

#define NOP asm volatile("nop")
#define NOPNOP asm volatile("rjmp .+0")

#define WIDTH 4

void sendCommand(byte address, byte value, byte iterations = 1) {
    digitalWrite(MATRIX_CS, LOW);
#ifdef HAVE_SERIAL
    Serial.print(address, HEX); Serial.print(' '); Serial.println(value, BIN);
#endif
    while(iterations--) {
        shiftOut(MATRIX_DIN, MATRIX_CLK, MSBFIRST, address); 
        shiftOut(MATRIX_DIN, MATRIX_CLK, MSBFIRST, value); 
    }
    digitalWrite(MATRIX_CS, HIGH);
}

void sendData(byte address, byte *values, byte len) {
#ifdef HAVE_SERIAL
    Serial.print(address, HEX); 
    for (byte i=0; i<len; i++) {
        Serial.print(' '); Serial.print(values[i], BIN);
    }
    Serial.println();
#endif
    digitalWrite(MATRIX_CS, LOW);
    while(len--) {
        shiftOut(MATRIX_DIN, MATRIX_CLK, MSBFIRST, address);
        shiftOut(MATRIX_DIN, MATRIX_CLK, MSBFIRST, *values);
        values++;
    }
    digitalWrite(MATRIX_CS, HIGH);
}

/*
void sendData(byte *buffer, int len) {
    digitalWrite(MATRIX_CS, LOW);
    while(len--) {
        byte b = *buffer;
        for(byte n=8; n; n--) {
            digitalWrite(MATRIX_DIN, b & 1);
            NOPNOP;
            digitalWrite(MATRIX_CLK, LOW);
            NOPNOP;
            digitalWrite(MATRIX_CLK, HIGH);
            NOPNOP;
            b >>= 1;
        }
        buffer++;
    }
    digitalWrite(MATRIX_CS, HIGH);
}
*/

byte testBuffer[] = {
    // 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x81
    0x03, 0x0C, 0x30, 0xC0, 0x11, 0x22, 0x44, 0x88
};

void setup() {
#ifdef HAVE_SERIAL
    Serial.begin(115200);
#endif

    pinMode(MATRIX_CLK, OUTPUT);
    pinMode(MATRIX_CS , OUTPUT);
    pinMode(MATRIX_DIN, OUTPUT);
    digitalWrite(MATRIX_CS, HIGH);
    digitalWrite(MATRIX_CLK, HIGH);

    sendCommand(0x9, 0, WIDTH); // clear decode mode
    sendCommand(0xA, 0, WIDTH); // set intensity
    sendCommand(0xB, 7, WIDTH); // set scan limit
    sendCommand(0xC, 1, WIDTH); // clear shutdown 

    for (byte i=0; i<8; i++) {
        sendCommand(i+1, testBuffer[i], WIDTH);
    }
}

byte matrix[8 * WIDTH] = { 0, };

void clear() {
    memset(matrix, 0, 8*WIDTH);
}

void flush() {
    byte *ptr = matrix;
    for(byte y=1; y<=8; y++) {
        sendData(y, ptr, WIDTH);
        ptr += WIDTH;
    }
}

#ifdef HAVE_SERIAL
void dumpMatrix() {
    for(byte y=0; y<8; y++) {
        for(byte x=0; x<WIDTH; x++) {
            for(byte mask = 0x80; mask; mask >>= 1) {
                Serial.print(matrix[x+y*WIDTH] & mask ? '#' : ' ');
            }
        }
        Serial.println();
    }
}
#endif

char *text[] = {
    "Lun 26",
    "Mar 14",
    "Mer 2",
    "Jeu 28",
    "Ven 30",
    "Sam 28",
    "Dim 31"

};
#define NB_TEXT 7

byte textEntry=0;

void loop() {
    delay(1000);
    clear();
    drawString(matrix, WIDTH, 0, text[textEntry]);
#ifdef HAVE_SERIAL
    dumpMatrix();
#endif
    textEntry = (textEntry+1) % NB_TEXT;
    flush();
}
