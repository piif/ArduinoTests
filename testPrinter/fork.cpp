
#include "fork.h"

byte forkRead(Fork *f) {
    return (digitalRead(f->pinA) << 1) | digitalRead(f->pinB);
}

void forkInit(Fork *f, byte pinA, byte pinB) {
    f->pinA = pinA;
    f->pinB = pinB;
    f->position = 0;
    f->errors = 0;
    f->state = forkRead(f);
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
}

short forkUpdate(Fork *f) {
    byte new_state = forkRead(f);
    if (new_state == f-> state) {
        return 0;
    }
    byte state_change = new_state | (f->state << 2);
    f->state = new_state;
    switch (state_change) {
        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            f->position++;
            return 1;

        case 0b0100:
        case 0b1101:
        case 0b1011:
        case 0b0010:
            f->position--;
            return -1;

        default:
            f->errors++;
            return 0;
    }
}

void forkSetPosition(Fork *f, long p) {
    f->position = p;
}
