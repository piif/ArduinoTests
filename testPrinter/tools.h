#include <avr/sleep.h>
#include <serialInput.h>

// #define PWM_ON_ENABLE

// TODO : add "set idle" + handle serial input ?
#define WAIT_FOR(condition) ({            \
    short __r = 0;                        \
    for(;;) {                             \
        set_sleep_mode(SLEEP_MODE_IDLE);  \
        sleep_mode();                     \
        if (condition) {                  \
            break;                        \
        }                                 \
        if (Serial.available()) {         \
            __r = handleInput();          \
            if (__r) {                    \
                break;                    \
            }                             \
        }                                 \
    }                                     \
    __r;                                  \
})
