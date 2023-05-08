#include <Arduino.h>

#define A0   2
#define A1   3
#define CAS  4
#define RAS  5
#define WE   6
#define DATA 7
#define ADDRESS_MASK 0b00001100

#define SET_ADDRESS(adr) PORTD = (PORTD & ~ADDRESS_MASK) | ((adr)<< 2)

void setup() {
    Serial.begin(115200);

    pinMode(A0  , OUTPUT);
    pinMode(A1  , OUTPUT);
    pinMode(CAS , OUTPUT);
    pinMode(RAS , OUTPUT);
    pinMode(WE  , OUTPUT);

    dumpMemory();
}

// address 0x0000abcd -> row ab , column cd
bool readData(byte address) {
    // S'assurer que RAS et DAS sont à 1 , et DATA en entrée
    pinMode(DATA, INPUT);

    digitalWrite(RAS, 1);
    digitalWrite(CAS, 1);

    // Positionner WE à 1
    digitalWrite(WE , 1);

    // Positionner une adresse de ligne
    SET_ADDRESS((address >> 2) & 0x03);

    // Passer RAS à 0
    digitalWrite(RAS, 0);

    // Positionner une adresse de colonne
    SET_ADDRESS(address & 0x03);

    // Passer CAS à 0
    digitalWrite(CAS, 0);

    // Lire DOUT
    return digitalRead(DATA);
}

void dumpMemory() {
    Serial.println("Dump :");
    for (byte row=0; row<4; row++) {
        Serial.print("  ");
        for (byte col=0; col<4; col++) {
            byte adr = (row << 2) | col;
            bool data = readData(adr);
            Serial.print(data);
        }
        Serial.println();
    }
    Serial.println();
}

void loop() {
}