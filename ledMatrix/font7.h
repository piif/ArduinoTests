#ifndef FONT7_H
#define FONT7_H

#include <Arduino.h>

// return width of character
byte charWidth(char c);

// Given an array of 8xN bytes containing N matrices (left to right) of 8 bytes (top to bottom) of 8 bits (LSB = right)
// Draw a char into the array, at position X (thus from bit 8-X%8 into matrix X/8)
// return next X position (Depends on character width)
int drawChar(byte *matrix, int maxX, int X, char c);

int drawString(byte *matrix, int maxX, int X, char *str);

#endif