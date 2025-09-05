#ifndef HAVE_FORK_H
#define HAVE_FORK_H

#include <Arduino.h>

typedef struct _fork {
    byte pinA, pinB;
    volatile byte state;
    volatile long position;
    volatile long errors;
} Fork;

void forkInit(Fork *f, byte pinA, byte pinB);

// read fork state and update position according
// return +1/-1 if position changed forward/backward , or 0 if no change
short forkUpdate(Fork *f);

// set new position
void forkSetPosition(Fork *f, long p);

// read current fork state
byte forkRead(Fork *f);

#endif // HAVE_FORK_H
