#include <Arduino.h>
#include "cs1694.h"
#include "dvdPanel.h"

// some tests with CS16940E component, display/keys handler

CS1694 cs1694;
DVDPanel dvdPanel(cs1694.display);

void setup() {
    Serial.begin(115200);

    // sendCommand(0b00000010); // 6 Grids,11 Segs mode
    cs1694.sendCommand(0b00000011); // 7 Grids,10 Segs mode
    cs1694.sendCommand(0b10001011); // leds on, brightness 10/16

    dvdPanel.clearPanel();
    dvdPanel.setDigit(0, 1);
    dvdPanel.setDigit(1, 2);
    dvdPanel.setDigit(2, 3);
    dvdPanel.setDigit(3, 4);
    dvdPanel.setDigit(4, 5);
    dvdPanel.setDigit(5, 6);
    dvdPanel.setDigit(6, 7);
    dvdPanel.setSegment(dvdPanel.diskMap[1]);
    dvdPanel.setSegment(dvdPanel.diskMap[3]);
    dvdPanel.setSegment(dvdPanel.diskMap[5]);
    dvdPanel.setSegment(dvdPanel.diskMap[7]);
    cs1694.flushDisplay();
    Serial.println("Setup OK");
}

byte fromHex(byte h) {
    if (h >= 'a' && h <= 'f') {
        return h + 10 - 'a';
    }
    if (h >= 'A' && h <= 'F') {
        return h + 10 - 'A';
    }
    if (h >= '0' && h <= '9') {
        return h - '0';
    }
    return 0;
}

long counter = 1234567;

void loop() {
    word buttons = dvdPanel.checkButtons();
    if (buttons != 0xFFFF) {
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
    delay(500);

    byte d= counter%8;
    dvdPanel.clearSegment(dvdPanel.diskMap[d]);
    dvdPanel.displayValue(counter++);
    d= counter%8;
    dvdPanel.setSegment(dvdPanel.diskMap[d]);
    cs1694.flushDisplay();

    // static byte buffer[14];
    // if (Serial.available()) {
    //     Serial.print("read data : ");
    //     for (byte b=0; b<14; b++) {
    //         byte msb = Serial.read();
    //         byte lsb = Serial.read();
    //         buffer[b] = fromHex(msb) << 4 | fromHex(lsb);
    //         Serial.print(buffer[b], HEX);Serial.print(' ');
    //     }
    //     Serial.println();
    //     // writeData(8, 6, buffer+8);
    //     cs1694.writeData(0, 14, buffer);
    // }
}
