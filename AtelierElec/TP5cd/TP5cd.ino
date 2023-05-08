#include <Arduino.h>

#define A0   2
#define A1   3
#define CAS  4
#define RAS  5
#define WE   6
#define DATA 7
#define ADDRESS_MASK 0b00001100

#define SET_ADDRESS(adr) PORTD = (PORTD & ~ADDRESS_MASK) | ((adr)<< 2)

bool example1[16] = {
    1, 1, 1, 1,
    1, 1, 1, 0,
    1, 1, 0, 0,
    1, 0, 0, 0
};

bool example2[16] = {
    1, 0, 0, 1,
    0, 0, 0, 0,
    1, 0, 0, 1,
    0, 1, 1, 0
};

void setup() {
    Serial.begin(115200);

    pinMode(A0  , OUTPUT);
    pinMode(A1  , OUTPUT);
    pinMode(CAS , OUTPUT);
    pinMode(RAS , OUTPUT);
    pinMode(WE  , OUTPUT);

    Serial.println("After boot");
    dumpMemory();
    Serial.println("Write example1");
    writeMemory(example1);
    dumpMemory();
    Serial.println("Write example2");
    writeMemory(example2);
    dumpMemory();
    Serial.println("Wait 3s");
    delay(3000);
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

// address 0x0000abcd -> row ab , column cd
void writeData(byte address, bool data) {
    // S'assurer que RAS et DAS sont à 1 , et DATA en sortir
    pinMode(DATA, OUTPUT);

    digitalWrite(RAS, 1);
    digitalWrite(CAS, 1);

    // Positionner WE à 0 et DATA à data
    digitalWrite(WE , 0);
    digitalWrite(DATA , data);

    // Positionner une adresse de ligne
    SET_ADDRESS((address >> 2) & 0x03);

    // Passer RAS à 0
    digitalWrite(RAS, 0);

    // Positionner une adresse de colonne
    SET_ADDRESS(address & 0x03);

    // Passer CAS à 0
    digitalWrite(CAS, 0);

    // et c'est tout
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

void writeMemory(bool data[]) {
    for (byte adr=0; adr<16; adr++) {
        writeData(adr, data[adr]);
    }
}

void loop() {
}