#include <Arduino.h>
#define HAVE_SERIAL

#include "ledMatrix.h"

// test MAX7219 chained led matrix 8x8xN

#define MATRIX_CLK A2
#define MATRIX_CS  A1
#define MATRIX_DIN A4

#define WIDTH 12

LedMatrix ledMatrix(WIDTH, MATRIX_CLK, MATRIX_CS, MATRIX_DIN, 2);

byte testBuffer[] = {
    // 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x81
    0x03, 0x0C, 0x30, 0xC0, 0x11, 0x22, 0x44, 0x88
};

void setup() {
#ifdef HAVE_SERIAL
    Serial.begin(115200);
#endif

    for (byte i=0; i<8; i++) {
        ledMatrix.sendCommand(i+1, testBuffer[i], WIDTH);
    }
    ledMatrix.flush();
}

char buffer[25];
byte i = 0;

void loop() {
    if (i >= 25) {
        i = 0;
    }
    int c = Serial.read();
    if (c == -1) {
        return;
    }
    if (c == '\n' || c == '\r') {
        buffer[i] = 0;
        Serial.print("[");Serial.print(buffer);Serial.println("]");
        ledMatrix.clear();
        ledMatrix.drawString(0, buffer);
#ifdef HAVE_SERIAL
        ledMatrix.dumpMatrix(Serial);
#endif
        ledMatrix.flush();
        i = 0;
        return;
    }
    buffer[i] = c;
    i++;
}
