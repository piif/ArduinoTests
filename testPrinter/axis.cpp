#include "axis.h"
// #include "tools.h"
#include "strop.h"

// current head position , 0 = left of carriage but may be different when initialisation is not still completed
volatile long X_pos = 0;
// speed = PWM level , 0 = stop
volatile byte X_speed = 0;
// dir = +1 (right) or -1 (left)
// when motor is stopped, dir stay on it's last value to be able to update position if inertia make head move after motor stop
volatile int  X_dir = 0;

// current paper position , 0 = bottom of page when paper is detected
volatile long Y_pos = 0;
volatile int  Y_speed = 0;
volatile int  Y_dir = 0;

volatile byte sensor_bits = 0;

volatile byte paper_present = 0;
volatile byte head_max = 0;

inline void axis_x_update() {
    X_pos += X_dir;
}

inline void axis_y_update() {
    Y_pos += Y_dir;
}

void axis_paper_detect(byte state) {
    paper_present = state;
}

void axis_head_max_detect(byte state) {
    head_max = state;
    if (head_max) {
        // stop X motor, force X to X_MAX
        axis_x_set_speed(0);
        X_pos = X_MAX;
    }
}

void axis_begin() {
    pinMode(M_X_EN, OUTPUT);
    pinMode(M_X_A, OUTPUT);
    pinMode(M_X_B, OUTPUT);

    pinMode(M_Y_EN, OUTPUT);
    pinMode(M_Y_A, OUTPUT);
    pinMode(M_Y_B, OUTPUT);

    pinMode(FORK_X_A, INPUT);
    pinMode(FORK_X_B, INPUT);

    pinMode(FORK_Y_A, INPUT);
    pinMode(FORK_Y_B, INPUT);

    pinMode(FORK_P, INPUT_PULLUP);
    pinMode(FORK_H, INPUT_PULLUP);

    // initialize sensors status
    sensor_bits = SENSOR_BITS;
    axis_paper_detect((sensor_bits & FORK_P_BITS) == 0);
    axis_head_max_detect((sensor_bits & FORK_H_BITS) == 0);

    // Listen PCINT 8,9,10,11,12,13 == interrupts on PC0,1,2,3,4,5 == A0,1,2,3,4,5
    PCMSK0 = 0;
    PCMSK1 = _BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10) | _BV(PCINT11) | _BV(PCINT12) | _BV(PCINT13);
    PCMSK2 = 0;
    PCICR = _BV(PCIE1); // listen for PCINT[14:8]
}

void axis_x_set_speed(int v) {
    if (v == 0) {
        X_speed = 0;
        digitalWrite(M_X_EN, 0);
        return;
    }

    if (v > 0) {
        X_speed = v;
        X_dir = 1;
#ifdef PWM_ON_ENABLE
        analogWrite(M_X_EN, X_speed);
        digitalWrite(M_X_A, 1);
        digitalWrite(M_X_B, 0);
#else
        analogWrite(M_X_A, X_speed);
        digitalWrite(M_X_B, 0);
        digitalWrite(M_X_EN, 1);
#endif
    } else {
        X_speed = -v;
        X_dir = -1;
#ifdef PWM_ON_ENABLE
        analogWrite(M_X_EN, X_speed);
        digitalWrite(M_X_A, 0);
        digitalWrite(M_X_B, 1);
#else
        digitalWrite(M_X_A, 0);
        analogWrite(M_X_B, X_speed);
        digitalWrite(M_X_EN, 1);
#endif
    }
}

void axis_y_set_speed(int v) {
    if (v == 0) {
        Y_speed = 0;
        digitalWrite(M_Y_EN, 0);
        return;
    }

    if (v > 0) {
        Y_speed = v;
        Y_dir = 1;
#ifdef PWM_ON_ENABLE
        analogWrite(M_Y_EN, Y_speed);
        digitalWrite(M_Y_A, 1);
        digitalWrite(M_Y_B, 0);
#else
        analogWrite(M_Y_A, Y_speed);
        digitalWrite(M_Y_B, 0);
        digitalWrite(M_Y_EN, 1);
#endif
    } else {
        Y_speed = -v;
        Y_dir = -1;
#ifdef PWM_ON_ENABLE
        analogWrite(M_Y_EN, Y_speed);
        digitalWrite(M_Y_A, 0);
        digitalWrite(M_Y_B, 1);
#else
        digitalWrite(M_Y_A, 0);
        analogWrite(M_Y_B, Y_speed);
        digitalWrite(M_Y_EN, 1);
#endif
    }
}

void axis_stop() {
    axis_x_set_speed(0);
    axis_y_set_speed(0);
}

void axis_x_move_of(int delta) {

}

void axis_y_move_of(int delta) {

}

ISR(PCINT1_vect) {
    cli();
    byte new_sensor_bits = SENSOR_BITS;
    byte changes = new_sensor_bits ^ sensor_bits;
    sensor_bits = new_sensor_bits;

    if (changes & FORK_X_BITS) {
        // update x position on any sensor change
        axis_x_update();
    }
    if ((changes & FORK_Y_BITS) && (sensor_bits & FORK_Y_BITS) == 0) {
        // update y position only when sensor value is 0
        axis_y_update();
    }
    if (changes & FORK_P_BITS) {
        // paper detection change
        axis_paper_detect((sensor_bits & FORK_P_BITS) == 0);
    }
    if (changes & FORK_H_BITS) {
        axis_head_max_detect((sensor_bits & FORK_H_BITS) == 0);
    }
    sei();
}

void axis_status() {
	Serial.print("sensors="); Serial.print(SENSOR_BITS, BIN); Serial.print('/'); Serial.println(sensor_bits, BIN);
	Serial << "X : speed=" << X_speed
           << "\tdir="     << X_dir
           << "\tpos="     << X_pos
           << "\tmax="     << (head_max ? "yes" : "no")
           << EOL;
	Serial << "Y : speed=" << Y_speed
           << "\tdir="     << Y_dir
           << "\tpos="     << Y_pos
           << "\tpaper="   << (paper_present ? "yes" : "no")
           << EOL;
    return 0;
}

/*
#define USE_LOW_HIGH_SPEED

void Axis::moveOf(int delta) {
    long target = position + delta;
    if (delta == 0 || target > positionMax || target < positionMin) {
        Serial.print(target); Serial.println(" none or out of range");
        return;
    }
#ifdef USE_LOW_HIGH_SPEED
    if (delta > 200 || delta < -200) {
        long preTarget = position + (delta * .8);
        Serial << "going from " << position << EOL;
        Serial << " to " << preTarget << EOL;
        Serial << " then  " << target << EOL;
        if (delta > 0) {
            setHighSpeed(1);
            if(WAIT_FOR(position >= preTarget)) {
                return;
            }
        } else {
            setHighSpeed(-1);
            if (WAIT_FOR(position <= preTarget)) {
                return;
            }
        }
    } else {
        Serial << "going from " << position << EOL;
        Serial << " to " << target << EOL;
    }

    if (delta > 0) {
        setLowSpeed(1);
        if (WAIT_FOR(position >= target)) {
            return;
        }
        stop();
    } else {
        setLowSpeed(-1);
        if (WAIT_FOR(position <= target)) {
            return;
        }
        stop();
    }
#else
    if (delta > 0) {
        setHighSpeed(1);
        if (WAIT_FOR(position >= target)) {
            return;
        }
        setHighSpeed(-1);
        delay(20);
        stop();
    } else {
        setHighSpeed(-1);
        if (WAIT_FOR(position <= target)) {
            return;
        }
        setHighSpeed(1);
        delay(20);
        stop();
    }
#endif
}
*/