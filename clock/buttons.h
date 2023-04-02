#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define DEBOUNCE_DELAY 50
#define NO_BUTTON_CHANGE 255

class Buttons {
public:
    Buttons(byte _input, byte _buttonsNumber, int *_thresholds) {
        input = _input;
        buttonsNumber = _buttonsNumber;
        thresholds = _thresholds;
        pinMode(input, INPUT);
    }

    byte read() {
        int v =  analogRead(input);
        byte newButton = decode(v);
        if (newButton != button) {
            delay(DEBOUNCE_DELAY);
            v =  analogRead(input);
            newButton = decode(v);
            if (newButton != button) {
                button = newButton;
                return button;
            }
        }
        return NO_BUTTON_CHANGE;
    }

private:
    byte input;
    byte buttonsNumber;
    int *thresholds;
    byte button = 0;

    byte decode(int v) {
        for (byte b = 0; b < buttonsNumber; b++) {
            if (v > thresholds[b]) {
                return b+1;
            }
        }
        return 0;
    }
};

#endif