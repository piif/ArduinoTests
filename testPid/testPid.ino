#include <Arduino.h>
#include "strop.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#include "PID_v1.h"

double xp_input = 0, xp_output = 0, xp_setpoint = -1;
#define XP_PID_P 1
#define XP_PID_I 0.0
#define XP_PID_D 0.0
PID xp_pid(&xp_input, &xp_output, &xp_setpoint, XP_PID_P, XP_PID_I, XP_PID_D, DIRECT);

long x_prev;

void testMove() {
    xp_setpoint = 100;
    
    for(byte i = 0; i < 120; i+=12) {
        xp_input = i;
        xp_pid.Compute();
        Serial << "xp_input=" << xp_input << " xp_output=" << xp_output << EOL;
        delay(1000);
    }
}

void setup() {
    Serial.begin(DEFAULT_BAUDRATE);
    xp_pid.SetMode(AUTOMATIC);
    testMove();
}

void loop () {}