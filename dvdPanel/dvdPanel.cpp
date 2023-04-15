#include "Arduino.h"
#include "dvdPanel.h"

static byte DVDPanel::diskMap[8] = { 0xD1, 0x10, 0x30, 0xB0, 0xD0, 0x70, 0x50, 0x90 };
static byte DVDPanel::symbolsMap[9] = {
    0x11, /* dvd */
    0x31, /* vcd */
    0x51, /* mp3 */
    0x91, /* play */
    0xB1, /* pause */
    0x71, /* pbc */
    0x06, /* loop */
    0x86, /* dts */
    0x66, /* ddd */
};
static byte DVDPanel::colonMap[2] = { 0x27, 0x47 };

static byte DVDPanel::digitMap[7][7] = {
    { 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0 },
    { 0x01, 0x21, 0x41, 0x61, 0x81, 0xA1, 0xC1 },
    { 0x07, 0x27, 0x47, 0x67, 0x87, 0xA7, 0xC7 },
    { 0x02, 0x22, 0x42, 0x62, 0x82, 0xA2, 0xC2 },
    { 0x03, 0x23, 0x43, 0x63, 0x83, 0xA3, 0xC3 },
    { 0x04, 0x24, 0x44, 0x64, 0x84, 0xA4, 0xC4 },
    { 0x05, 0x25, 0x45, 0x65, 0x85, 0xA5, 0xC5 }
};

// static byte DVDPanel::digitSegments[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };

// segment map for each of the characters listed in mapChars
static byte DVDPanel::charSegments[42] = {
	// mLlbrRt
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9
	0b00001000, // .
	0b00000000, // ' '
	0b01110111, // a
	0b01111100, // b
	0b00111001, // c
	0b01011110, // d
	0b01111001, // e
	0b01110001, // f
	0b00111101, // g
	0b01110100, // h
	0b00000100, // i
	0b00001110, // j
	0b01111000, // k
	0b00111000, // l
	0b00110111, // m
	0b01010100, // n
	0b01011100, // o
	0b01110011, // p
	0b01100111, // q
	0b01010000, // r
	0b00101101, // s
	0b01110000, // t
	0b00011100, // u
	0b00111110, // v
	0b01111110, // w
	0b01110110, // x
	0b01101110, // y
	0b00011011, // z
	0b01010011, // ?
	0b01000000, // -
	0b00001000, // _
	0b00100000, // '
};
static byte *DVDPanel::letterSegments = DVDPanel::charSegments+12;
static byte *DVDPanel::digitSegments = DVDPanel::charSegments;
