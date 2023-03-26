#include <Arduino.h>

// some tests with CS16940E component, display/keys handler

#define CS_STROBE A3
#define CS_CLOCK  A2
#define CS_DATA   A0

#define CS_STROBE_MSK 0x8
#define CS_CLOCK_MSK  0x4
#define CS_DATA_MSK   0x1
#define CS_BITS       0xD

// commands :
// 00----10 = Display mode setting 10 = 6 Grids, 11 Segs
// 00----11 = Display mode setting 11 = 7 Grids, 10 Segs
//
// 01-- b3 b2 00 = Write data to display RAM
// 01-- b3 b2 10 = Read key data
// b2 = Address automatic plus 1 mode setting : 0 The address is automatically incremented by 1 after data is written , 1 Address is fixed
// b3 = Circuit mode selection : 0 normal work , 1 Test mode
//
// 10--b3 b2 b1 b0
// b3 = display setting : 0 Display is off, key scan continues ,  Display Open
// b2 b1 b0 = Brightness setting : 000=low -> 111=bright


#define NOP asm volatile("nop")
#define NOPNOP asm volatile("rjmp .+0")

void setupCS1694() {
    // set strobe, clock and data as output
    DDRC  |= CS_BITS;
    PORTC &= ~CS_BITS;
}

void writeByte(byte value) {
    // shiftOut(CS_DATA, CS_CLOCK, LSBFIRST, value);
    for (byte b=8; b; b--) {
        // Set clock to 0 => prepare b0
        PORTC &= ~CS_CLOCK_MSK;
        PORTC = PORTC & ~CS_DATA_MSK | (value & 1);
        // Set clock to 1 => b0 is read by component
        PORTC |= CS_CLOCK_MSK;
        value >>= 1;
    }
}

byte readByte() {
    // return shiftIn(CS_DATA, CS_CLOCK, LSBFIRST);
    byte value = 0;
    for (byte msk=0x01; msk; msk<<=1) {
        digitalWrite(CS_CLOCK, HIGH);
        // PORTC |= CS_CLOCK_MSK;
        NOPNOP;
        // read data
        if (digitalRead(CS_DATA)) {
            value |= msk;
        }
        // value |= PORTC & CS_DATA_MSK;
        // set clock to 0
        digitalWrite(CS_CLOCK, LOW);
        // PORTC &= ~CS_CLOCK_MSK;
    }
    return value;
}

void beginCommand() {
    // Set data to output
    DDRC |= CS_DATA_MSK;
    // Strobe = 1 and clock = 1
    PORTC = (PORTC & ~CS_BITS) | CS_STROBE_MSK | CS_CLOCK_MSK;
    NOPNOP;
    // Set strobe to 0
    PORTC &= ~CS_STROBE_MSK;
}

void endCommand() {
    // Set strobe to 1
    PORTC = PORTC & ~CS_STROBE_MSK;
}

void sendCommand(byte command) {
    beginCommand();
    writeByte(command);
    endCommand();
}

void readData(byte address, byte len, byte *buffer) {
    beginCommand();
    // set address
    writeByte(0b11000000 | (address&0x0F));
    endCommand();

    beginCommand();
    writeByte(0b01000010);

    // Set data to input
    pinMode(CS_DATA, INPUT);
    // PORTC &= ~CS_DATA_MSK;
    // DDRC  &= ~CS_DATA_MSK;
    // “Twait” (?)
    delay(1);

    // at end of writeByte, clock is already 1
    // -> for each byte of buffer
    while (len--) {
        *buffer = readByte();
        buffer++;
    }
    endCommand();
}

void writeData(byte address, byte len, byte *buffer) {
    beginCommand();
    // set address
    writeByte(0b11000000 | (address&0x0F));
    endCommand();

    beginCommand();
    // send write command with auto increment
    writeByte(0b01000000);
    // send bytes
    while (len--) {
        writeByte(*buffer);
        buffer++;
    }
    endCommand();
}

void setup() {
    Serial.begin(115200);
    setupCS1694();

    sendCommand(0b10001011); // leds on, brightness 10/16

    // byte buffer[14] = {
    //     0xFF, 0xFF,
    //     0xFF, 0xFF,
    //     0xFF, 0xFF,
    //     0xFF, 0xFF,
    //     0xFF, 0xFF,
    //     0xFF, 0xFF,
    //     0xFF, 0xFF
    // };
    // writeData(0, 14, buffer); // send random data in display

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

void loop() {
    static byte buffer[14];
    readData(0, 5, buffer);
    Serial.print("Buffer =");
    for(byte i = 0; i < 5; i++) {
        Serial.print(' '); Serial.print(buffer[i], HEX);
    }
    Serial.println();
    delay(2000);
    if (Serial.available()) {
        Serial.print("read data : ");
        for (byte b=0; b<14; b++) {
            byte msb = Serial.read();
            byte lsb = Serial.read();
            buffer[b] = fromHex(msb) << 4 | fromHex(lsb);
            Serial.print(buffer[b], HEX);Serial.print(' ');
        }
        Serial.println();
        writeData(0, 14, buffer);
    }
}
