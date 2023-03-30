#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define BUTTON_INPUT A0

#define DEBOUNCE_DELAY 50
#define NO_BUTTON_CHANGE 255

class Buttons {
public:
    Buttons() {
        pinMode(BUTTON_INPUT, INPUT);
    }

    byte read() {
        int v =  analogRead(BUTTON_INPUT);
        byte newButton = decode(v);
        if (newButton != button) {
            delay(DEBOUNCE_DELAY);
            v =  analogRead(BUTTON_INPUT);
            newButton = decode(v);
            if (newButton != button) {
                button = newButton;
                return button;
            }
        }
        return NO_BUTTON_CHANGE;
    }

private:
    byte button = 0;

    byte decode(int v) {
        if (v > 800) {
            return 3;
        }
        if (v > 600) {
            return 2;
        }
        if (v > 400) {
            return 1;
        }
        return 0;
    }

};

#endif