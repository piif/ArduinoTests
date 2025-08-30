#ifndef HAVE_STEPPER_H
#define HAVE_STEPPER_H

#include <Arduino.h>

typedef struct _stepper {
    byte enable_pin, dir_pin, step_pin;
    long toMove;
} Stepper;

// initialize pins for stepper
void stepperInit(Stepper *s, byte enable_pin, byte dir_pin, byte step_pin);

// prepare for v steps , if v<0, direction is inverted
void stepperMoveOf(Stepper *s, int v);

// do one step , return 1 if a step was actually done
byte stepperDoStep(Stepper *s);

#define stepperMoving(s) ((s)->toMove != 0)

// initialize a stepper pair. Works also if enabled pin is shared
void stepperInit(
        Stepper *X, byte x_enable_pin, byte x_dir_pin, byte x_step_pin,
        Stepper *Y, byte y_enable_pin, byte y_dir_pin, byte y_step_pin);

void stepperMoveOf(Stepper *X, Stepper *Y,
        int dx, int dy);

byte stepperDoStep(Stepper *X, Stepper *Y);


// prepare a move sequence
void stepperPrepare(Stepper *X, Stepper *Y, boolean backwardX, boolean backwardY);

// do one step on X and/or Y
void stepperOneStep(Stepper *X, Stepper *Y, boolean stepX, boolean stepY);

// end a move sequence
void stepperEnd(Stepper *X, Stepper *Y);

#endif // HAVE_STEPPER_H
