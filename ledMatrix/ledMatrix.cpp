#include <Arduino.h>
#include "ledMatrix.h"
#include "font7.h"

LedMatrix::LedMatrix(byte _width, byte _clk, byte _cs, byte _din, byte _intensity = 2) {
    width = _width;
    clk = _clk;
    cs = _cs;
    din = _din;
    matrix = new byte[8 * width];

    pinMode(clk, OUTPUT);
    pinMode(cs , OUTPUT);
    pinMode(din, OUTPUT);
    digitalWrite(cs , HIGH);
    digitalWrite(clk, HIGH);

    clear();

    sendCommand(0x9, 0, width); // clear decode mode
    sendCommand(0xA, _intensity, width); // set intensity
    sendCommand(0xB, 7, width); // set scan limit
    sendCommand(0xC, 1, width); // clear shutdown 
}

void LedMatrix::sendCommand(byte address, byte value, byte iterations = 1) {
    digitalWrite(cs, LOW);
#ifdef HAVE_SERIAL
    Serial.print(address, HEX); Serial.print(' '); Serial.println(value, BIN);
#endif
    while(iterations--) {
        shiftOut(din, clk, MSBFIRST, address); 
        shiftOut(din, clk, MSBFIRST, value); 
    }
    digitalWrite(cs, HIGH);
}

void LedMatrix::sendData(byte address, byte *values, byte len) {
#ifdef HAVE_SERIAL
    Serial.print(address, HEX); 
    for (byte i=0; i<len; i++) {
        Serial.print(' '); Serial.print(values[i], BIN);
    }
    Serial.println();
#endif
    digitalWrite(cs, LOW);
    while(len--) {
        shiftOut(din, clk, MSBFIRST, address);
        shiftOut(din, clk, MSBFIRST, *values);
        values++;
    }
    digitalWrite(cs, HIGH);
}

void LedMatrix::clear() {
    memset(matrix, 0, 8*width);
}

void LedMatrix::flush() {
    byte *ptr = matrix;
    for(byte y=1; y<=8; y++) {
        sendData(y, ptr, width);
        ptr += width;
    }
}

byte LedMatrix::charWidth(char c) {
    return ::charWidth(c);
}

// Given an array of 8xN bytes containing N matrices (left to right) of 8 bytes (top to bottom) of 8 bits (LSB = right)
// Draw a char into the array, at position X (thus from bit 8-X%8 into matrix X/8)
// return next X position (Depends on character width)
int LedMatrix::drawChar(int X, char c) {
    return ::drawChar(matrix, width, X, c);
}

int LedMatrix::drawString(int X, char *str) {
    return ::drawString(matrix, width, X, str);
}

void LedMatrix::dumpMatrix(Stream &stream) {
    for(byte y=0; y<8; y++) {
        for(byte x=0; x<width; x++) {
            for(byte mask = 0x80; mask; mask >>= 1) {
                stream.print(matrix[x+y*width] & mask ? '#' : ' ');
            }
        }
        stream.println();
    }
}
