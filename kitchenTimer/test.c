#include <Arduino.h>

void test() {
    ICR1 = 0x1234;  // should be 500 ??
    OCR1A = 0x5678;
    // OCR1AH = 0x9A;   // should be 250 ??
    // OCR1AL = 0xBC; // should be 250 ??
    TCCR1A = 0b00000010; // 0b10000010; // COM1A = 10 , COM1B = 0 , WGM xx10 / 0b00000010 => OC1A off 
    TCCR1B = 0b00010010; // WGM 10xx , Clock Select 010
    TCCR1C = 0;
    TIMSK1 = 1; // TOV1
}