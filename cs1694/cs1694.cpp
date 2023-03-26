#include <Arduino.h>
#include "cs1694.h"

#define CS_STROBE_MSK 0x08
#define CS_CLOCK_MSK  0x04
#define CS_DATA_MSK   0x01
#define CS_BITS       0x0D

#define NOP asm volatile("nop")
#define NOPNOP asm volatile("rjmp .+0")

CS1694::CS1694() {
    // set strobe, clock and data as output
    DDRC  |= CS_BITS;
    PORTC &= ~CS_BITS;
}

void CS1694::writeByte(byte value) {
    // shiftOut(CS_DATA, CS_CLOCK, LSBFIRST, value);
    for (byte msk=0x01; msk; msk<<=1) {
        // Set clock to 0 => prepare b0
        PORTC &= ~CS_CLOCK_MSK;
        if (value & msk) {
            PORTC |= CS_DATA_MSK;
        } else {
            PORTC &= ~CS_DATA_MSK;
        }
        // Set clock to 1 => b0 is read by component
        PORTC |= CS_CLOCK_MSK;
        NOPNOP;
    }
}

byte CS1694::readByte() {
    return shiftIn(CS_DATA, CS_CLOCK, LSBFIRST);
    // byte value = 0;
    // for (byte msk=0x01; msk; msk<<=1) {
    //     NOP;NOP;
    //     NOP;NOP;
    //     PORTC |= CS_CLOCK_MSK;
    //     NOP;NOP;
    //     NOP;NOP;
    //     // read data
    //     // if (digitalRead(CS_DATA)) {
    //     //     value |= msk;
    //     // }
    //     if (PINC & CS_DATA_MSK) { // this doesn't work, but digitalRead version works : WHY ?!?!?!
    //         value |= msk;
    //     } else {
    //         value &= ~msk;
    //     }
    //     PORTC &= ~CS_CLOCK_MSK;
    // }
    // return value;
}

void CS1694::beginCommand() {
    // Set data to output
    // pinMode(CS_DATA, OUTPUT);
    DDRC |= CS_DATA_MSK;
    // Strobe = 1 and clock = 1
    PORTC = (PORTC & ~CS_BITS) | CS_STROBE_MSK | CS_CLOCK_MSK;
    NOP;NOP;
    // Set strobe to 0
    PORTC &= ~CS_STROBE_MSK;
}

void CS1694::endCommand() {
    // Set strobe to 1
    PORTC |= CS_STROBE_MSK;
}

void CS1694::sendCommand(byte command) {
    beginCommand();
    writeByte(command);
    endCommand();
}

void CS1694::readData(byte address, byte len, byte *buffer) {
    // set address
    sendCommand(CS_COMMAND_ADDR | (address & 0x0F));

    beginCommand();
    writeByte(CS_COMMAND_READ | CS_COMMAND_INCR);

    // Set data to input
    // pinMode(CS_DATA, INPUT);
    PORTC &= ~CS_DATA_MSK;
    DDRC  &= ~CS_DATA_MSK;
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

void CS1694::writeData(byte address, byte len, byte *buffer) {
    // set address
    sendCommand(CS_COMMAND_ADDR | (address & 0x0F));

    beginCommand();
    // send write command with auto increment
    writeByte(CS_COMMAND_WRIT | CS_COMMAND_INCR);

    // send bytes
    while (len--) {
        writeByte(*buffer);
        buffer++;
    }
    endCommand();
}

void CS1694::flushDisplay() {
    writeData(0, 14, display);
}
