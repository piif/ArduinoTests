#ifndef CS1694_H
#define CS1694_H

#include <Arduino.h>

// some tests with CS16940E component, display/keys handler

#define CS_STROBE A3
#define CS_CLOCK  A2
#define CS_DATA   A0


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

#define CS_COMMAND_MODE 0x00
#define CS_COMMAND_ADDR 0xC0

#define CS_COMMAND_READ 0x42
#define CS_COMMAND_WRIT 0x40
#define CS_COMMAND_FIXD 0x04 // flag to "or" with READ/WRIT
#define CS_COMMAND_INCR 0x00 // flag to "or" with READ/WRIT

#define CS_COMMAND_CONF 0x10
#define CS_COMMAND_DPON 0x01 // flag to "or" with CONF
#define CS_COMMAND_DPOF 0x00 // flag to "or" with CONF

class CS1694 {
public:
    CS1694();

    byte display[14] = { 0, };

    void beginCommand();
    void endCommand();

    void sendCommand(byte command);
    void readData(byte address, byte len, byte *buffer);
    void writeData(byte address, byte len, byte *buffer);

    void flushDisplay();

private:
    void writeByte(byte value);
    byte readByte();
};

#endif