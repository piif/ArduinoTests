#include "axis.h"
// #include "tools.h"
#include "strop.h"

// current head position , 0 = left of carriage but may be different when initialisation is not still completed
volatile long X_pos = 0;
volatile long X_pos_err = 0;
// speed = PWM level , 0 = stop
volatile byte X_speed = 0;
// dir = +1 (right) or -1 (left)
// when motor is stopped, dir stay on it's last value to be able to update position if inertia make head move after motor stop
volatile int  X_dir = 0;

// current paper position , 0 = bottom of page when paper is detected
volatile long Y_pos = 0;
volatile long Y_pos_err = 0;
volatile byte Y_speed = 0;
volatile int  Y_dir = 0;

volatile byte sensor_bits = 0;

volatile byte paper_present = 0;
volatile byte head_max = 0;

volatile byte sensor_X = 0;
volatile byte sensor_Y = 0;

inline void axis_x_update() {
    sensor_X = ( (sensor_X & FORK_X_BITS) << 2 ) | (sensor_bits & FORK_X_BITS);
    switch (sensor_X) {
        case 0b0001 << FORK_X_BIT_SHIFT:
        case 0b0111 << FORK_X_BIT_SHIFT:
        case 0b1110 << FORK_X_BIT_SHIFT:
        case 0b1000 << FORK_X_BIT_SHIFT:
            X_pos++;
            break;
        case 0b0100 << FORK_X_BIT_SHIFT:
        case 0b1101 << FORK_X_BIT_SHIFT:
        case 0b1011 << FORK_X_BIT_SHIFT:
        case 0b0010 << FORK_X_BIT_SHIFT:
            X_pos--;
            break;
        default:
            X_pos_err++;
    }
    // X_pos += X_dir;
}

inline void axis_y_update() {
#ifdef ONE_BIT_FORK
    Y_pos += Y_dir;
#else
    sensor_Y = ( (sensor_Y & FORK_Y_BITS) << 2 ) | (sensor_bits & FORK_Y_BITS);
    switch (sensor_Y) {
        case 0b0001 << FORK_Y_BIT_SHIFT:
        case 0b0111 << FORK_Y_BIT_SHIFT:
        case 0b1110 << FORK_Y_BIT_SHIFT:
        case 0b1000 << FORK_Y_BIT_SHIFT:
            Y_pos++;
            break;
        case 0b0100 << FORK_Y_BIT_SHIFT:
        case 0b1101 << FORK_Y_BIT_SHIFT:
        case 0b1011 << FORK_Y_BIT_SHIFT:
        case 0b0010 << FORK_Y_BIT_SHIFT:
            Y_pos--;
            break;
        default:
            Y_pos_err++;
    }
#endif
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
#ifndef ONE_BIT_FORK
    pinMode(FORK_Y_B, INPUT);
#endif
    pinMode(FORK_P, INPUT_PULLUP);
    pinMode(FORK_H, INPUT_PULLUP);

    // initialize sensors status
    sensor_bits = SENSOR_BITS;
    axis_paper_detect((sensor_bits & FORK_P_BITS) == 0);
    axis_head_max_detect((sensor_bits & FORK_H_BITS) == 0);

    // Listen PCINT 8,9,10,11,12,13 == interrupts on PC0,1,2,3,4,5 == A0,1,2,3,4,5
    PCMSK0 = 0;
#ifdef ONE_BIT_FORK
    PCMSK1 = _BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10) | _BV(PCINT12) | _BV(PCINT13);
#else
    PCMSK1 = _BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10) | _BV(PCINT11) | _BV(PCINT12) | _BV(PCINT13);
#endif
    PCMSK2 = 0;
    PCICR = _BV(PCIE1); // listen for PCINT[14:8]
}

void axis_x_set_speed(int v) {
    if (v == 0) {
        X_speed = 0;
        digitalWrite(M_X_EN, 0);
        digitalWrite(M_X_A, 0);
        digitalWrite(M_X_B, 0);
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
        digitalWrite(M_X_EN, 1);
        analogWrite(M_X_A, X_speed);
        digitalWrite(M_X_B, 0);
#endif
    } else {
        X_speed = -v;
        X_dir = -1;
#ifdef PWM_ON_ENABLE
        analogWrite(M_X_EN, X_speed);
        digitalWrite(M_X_A, 0);
        digitalWrite(M_X_B, 1);
#else
        digitalWrite(M_X_EN, 1);
        analogWrite(M_X_A, ~X_speed);
        digitalWrite(M_X_B, 1);
#endif
    }
}

void axis_y_set_speed(int v) {
    if (v == 0) {
        Y_speed = 0;
        digitalWrite(M_Y_EN, 0);
        digitalWrite(M_Y_A, 0);
        digitalWrite(M_Y_B, 0);
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
        digitalWrite(M_Y_EN, 1);
        analogWrite(M_Y_A, Y_speed);
        digitalWrite(M_Y_B, 0);
#endif
    } else {
        Y_speed = -v;
        Y_dir = -1;
#ifdef PWM_ON_ENABLE
        analogWrite(M_Y_EN, Y_speed);
        digitalWrite(M_Y_A, 0);
        digitalWrite(M_Y_B, 1);
#else
        digitalWrite(M_Y_EN, 1);
        analogWrite(M_Y_A, ~Y_speed);
        digitalWrite(M_Y_B, 1);
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

// set this define to compute max and average duration of ISR in order to estimate max possible calls/s
// on Arduino Uno with default frequency (8MHz), ISR take from 5 to 20µs
// #define COMPUTE_ISR_DURATION
#ifdef COMPUTE_ISR_DURATION
volatile unsigned long nb_isr_call = 0;
volatile unsigned long all_isr_call = 0;
volatile unsigned long max_isr_call = 0;
#endif

ISR(PCINT1_vect) {
    cli();
#ifdef COMPUTE_ISR_DURATION
    unsigned long tic = micros();
#endif
    byte new_sensor_bits = SENSOR_BITS;
    byte changes = new_sensor_bits ^ sensor_bits;
    sensor_bits = new_sensor_bits;

    if (changes & FORK_X_BITS) {
        // update x position on any sensor change
        axis_x_update();
    }
#ifdef ONE_BIT_FORK
    if ((changes & FORK_Y_BITS) && (sensor_bits & FORK_Y_BITS) == 0) {
#else
    if (changes & FORK_Y_BITS) {
#endif
        // update y position only when sensor value is 0
        axis_y_update();
    }
    if (changes & FORK_P_BITS) {
        // paper detection change
        axis_paper_detect((sensor_bits & FORK_P_BITS) != 0);
    }
    if (changes & FORK_H_BITS) {
        axis_head_max_detect((sensor_bits & FORK_H_BITS) == 0);
    }
#ifdef COMPUTE_ISR_DURATION
    unsigned long duration = micros() - tic;
    nb_isr_call++;
    all_isr_call += duration;
    if (duration > max_isr_call) {
        max_isr_call = duration;
    }
#endif
    sei();
}

void axis_status() {
	Serial.print("sensors="); Serial.print(sensor_bits, BIN);
    Serial.print('/'); Serial.println(sensor_X, BIN);
    Serial.print('/'); Serial.println(sensor_Y, BIN);
	Serial << "X : speed=" << X_speed
           << "\tdir="     << X_dir
           << "\tpos="     << X_pos << "\terr=" << X_pos_err
           << "\tmax="     << (head_max ? "yes" : "no")
           << EOL;
	Serial << "Y : speed=" << Y_speed
           << "\tdir="     << Y_dir
           << "\tpos="     << Y_pos << "\terr=" << Y_pos_err
           << "\tpaper="   << (paper_present ? "yes" : "no")
           << EOL;
#ifdef COMPUTE_ISR_DURATION
	Serial << nb_isr_call << " ISR calls , avg " << (all_isr_call/nb_isr_call) << " µs , max = " << max_isr_call << " µs" << EOL;
#endif
    return 0;
}
