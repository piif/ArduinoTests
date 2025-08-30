#include "stepper.h"

void stepperInit(Stepper *s, byte enable_pin, byte dir_pin, byte step_pin) {
    s->toMove = 0;
    s->step_pin = step_pin;
    s->dir_pin = dir_pin;
    s->enable_pin = enable_pin;

    pinMode(s->enable_pin, OUTPUT);
    digitalWrite(s->enable_pin, 1);
    pinMode(s->step_pin, OUTPUT);
    digitalWrite(s->step_pin, 0);
    pinMode(s->dir_pin, OUTPUT);
}

void stepperMoveOf(Stepper *s, int v) {
    digitalWrite(s->dir_pin, (v < 0));
    digitalWrite(s->enable_pin, 0);

    s->toMove = v;
}

// do one step , return 1 If a step was needed
byte stepperDoStep(Stepper *s) {
    if (s->toMove == 0) {
        return 0;
    } else if (s->toMove > 0) {
        s->toMove--;
    } else {
        s->toMove++;
    }

    digitalWrite(s->step_pin, 1);
    delay(1);
    // delayMicroseconds(250);
    digitalWrite(s->step_pin, 0);
    delay(1);
    // delayMicroseconds(250);

    if (s->toMove == 0) {
        digitalWrite(s->enable_pin, 1);
    }
    return 1;
}


void stepperInit(
        Stepper *X, byte x_enable_pin, byte x_dir_pin, byte x_step_pin,
        Stepper *Y, byte y_enable_pin, byte y_dir_pin, byte y_step_pin) {
    stepperInit(X, x_enable_pin, x_dir_pin, x_step_pin);
    stepperInit(Y, y_enable_pin, y_dir_pin, y_step_pin);
}

void stepperMoveOf(Stepper *X, Stepper *Y, int dx, int dy) {
    digitalWrite(X->enable_pin, 0);
    if (X->enable_pin != Y->enable_pin) {
        digitalWrite(Y->enable_pin, 0);
    }
    stepperMoveOf(X, dx);
    stepperMoveOf(Y, dy);
}

// do one step , return 1 If a step was needed
byte stepperDoStep(Stepper *X, Stepper *Y) {
    if (!stepperMoving(X) && !stepperMoving(Y)) {
        digitalWrite(X->enable_pin, 1);
        if (X->enable_pin != Y->enable_pin) {
            digitalWrite(Y->enable_pin, 1);
        }
        return 0;
    }
    if (X->toMove != 0) {
        digitalWrite(X->step_pin, 1);
        delay(1);
        digitalWrite(X->step_pin, 0);
        delay(1);
        if (X->toMove > 0) {
            X->toMove--;
        } else {
            X->toMove++;
        }
    }

    if (Y->toMove != 0) {
        digitalWrite(Y->step_pin, 1);
        delay(1);
        digitalWrite(Y->step_pin, 0);
        delay(1);
        if (Y->toMove > 0) {
            Y->toMove--;
        } else {
            Y->toMove++;
        }
    }

//    digitalWrite(X->enable_pin, 0);
//    if (X->enable_pin != Y->enable_pin) {
//        digitalWrite(Y->enable_pin, 0);
//    }
//    delay(1);
//    digitalWrite(X->enable_pin, 1);
//    if (X->enable_pin != Y->enable_pin) {
//        digitalWrite(Y->enable_pin, 1);
//    }

    return 1;
}

// prepare a move sequence
void stepperPrepare(Stepper *X, Stepper *Y, boolean backwardX, boolean backwardY) {
    digitalWrite(X->dir_pin, backwardX);
    digitalWrite(Y->dir_pin, backwardY);

    digitalWrite(X->enable_pin, 0);
    if (X->enable_pin != Y->enable_pin) {
        digitalWrite(Y->enable_pin, 0);
    }
}

// do one step on X and/or Y
void stepperOneStep(Stepper *X, Stepper *Y, boolean stepX, boolean stepY) {
    if (stepX) {
        digitalWrite(X->step_pin, 1);
    }
    if (stepY) {
        digitalWrite(Y->step_pin, 1);
    }
    delay(1);
    if (stepX) {
        digitalWrite(X->step_pin, 0);
    }
    if (stepY) {
        digitalWrite(Y->step_pin, 0);
    }
    delay(1);
}

// end a move sequence
void stepperEnd(Stepper *X, Stepper *Y) {
    digitalWrite(X->enable_pin, 1);
    if (X->enable_pin != Y->enable_pin) {
        digitalWrite(Y->enable_pin, 1);
    }
}

