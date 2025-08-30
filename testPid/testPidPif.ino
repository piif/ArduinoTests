#ifdef VERSION_PIF

#include <Arduino.h>
#include "strop.h"

#include "PID_v1.h"

// pinout
#define MOTOR_ENABLE  3
#define MOTOR_PIN_A  10
#define MOTOR_PIN_B   5
#define SENSOR       A2

#define COMPUTE_SPEED 0
#define COMPUTE_SPEED_STR "0"

#define PID_P 2
#define PID_I 4
#define PID_D 0

// fork value
volatile byte sensor;

// current position
volatile unsigned long position = 0;

// to count sensor interrupt calls and their avg/max duration
volatile unsigned long nb_sensor_call = 0, all_sensor_call = 0, max_sensor_call = 0;

ISR(PCINT1_vect) {
    cli();
    unsigned long tic = micros();

    byte new_sensor = digitalRead(SENSOR);
    byte changes = new_sensor ^ sensor;
    sensor = new_sensor;

    if (changes && sensor) {
        position++;
    }

    // compute ISR duration
    unsigned long duration = micros() - tic;
    nb_sensor_call++;
    all_sensor_call += duration;
    if (duration > max_sensor_call) {
        max_sensor_call = duration;
    }

    sei();
}

// sample rate for PID and computation timer
#define PID_PERIOD 10

// position during previous timer call (to deduce actual speed)
volatile unsigned long prev_position = 0;

// input, output and target of PID loop
volatile double input = 0, output = 0, setpoint = 0;
PID pid(&input, &output, &setpoint, PID_P, PID_I, PID_D, P_ON_E, DIRECT);

volatile unsigned long my_clock = 0;

// to count timer calls and their avg/max duration
volatile unsigned long nb_timer_call = 0, all_timer_call = 0, max_timer_call = 0;
// to count actual PID::compute calls
volatile unsigned long nb_pid_call = 0;

#define BUF_MAX 100
int  buf_in[BUF_MAX];
byte buf_out[BUF_MAX];
volatile int buf_index = 0;

volatile long ring[5] =  { 0, };
volatile int  ring_idx=0;
volatile long ring_sum=0;

// call by timer0
ISR(TIMER0_COMPA_vect) {
    cli();
    if (my_clock % PID_PERIOD == 0) {
        unsigned long tic = micros();

        // update PWM value from actual speed (in step/s)
        unsigned long delta = (position - prev_position);
        prev_position = position;
        ring_sum -= ring[ring_idx];
        ring_sum += delta;
        ring[ring_idx] = delta;
        ring_idx = (ring_idx+1) % 5;

        input = ring_sum / 5;
        if (pid.Compute()) {
            nb_pid_call++;
        }
        analogWrite(MOTOR_PIN_A, output);

        // compute ISR duration
        unsigned long duration = micros() - tic;
        nb_timer_call++;
        all_timer_call += duration;
        if (duration > max_timer_call) {
            max_timer_call = duration;
        }
        if (buf_index < BUF_MAX) {
            buf_in[buf_index] = input;
            buf_out[buf_index] = output;
            buf_index++;
        }
    }
    my_clock++;
    sei();
}

void setup() {
    Serial.begin(115200);

    // timer 0 clock prescaler defaults to 8MHz/64 (used to update millis counter thru TOIE interrupt)
    // we can set TIMSK0 bit OCIE0A and set OCR0A to 125 to call OCIE0A every millisecond
    OCR0A = 125;

    // Listen PCINT 10 == interrupts on PC2 == A2
    PCMSK0 = 0;
    PCMSK1 = _BV(PCINT10);
    PCMSK2 = 0;
    PCICR  = _BV(PCIE1); // listen for PCINT[14:8]

    pinMode(MOTOR_ENABLE, OUTPUT);
    pinMode(MOTOR_PIN_A, OUTPUT);
    pinMode(MOTOR_PIN_B, OUTPUT);

    // enable H bridge, but low outputs at startup
    digitalWrite(MOTOR_ENABLE, 1);
    digitalWrite(MOTOR_PIN_A, 0);
    digitalWrite(MOTOR_PIN_B, 0);

    pinMode(SENSOR, INPUT);
    sensor = digitalRead(SENSOR);

#if COMPUTE_SPEED != 0
    Serial << "Compute speed at PWM=" COMPUTE_SPEED_STR "\n";
    for(byte i = 0; i < 5; i++) {
        unsigned long tic = micros();
        position = 0;
        analogWrite(MOTOR_PIN_A, COMPUTE_SPEED);
        delay(500);
        unsigned long long pos = position;
        unsigned long tac = micros();
        digitalWrite(MOTOR_PIN_A, 0);
        Serial << F(" => ") << (unsigned long)(pos * 1000000 / (tac-tic)) << F(" step/s\n");
    }
    // 120 => between 900 and 1300 step/s
    // 130 => ramp up (hardly) from 1000 to 10000 step/s
    // 140 => between 14000 and 15000 step/s
    // 160 => between 20000 and 21000 step/s
    // 180 => between 25000 and 26000 step/s
    // 200 => between 30000 and 31000 step/s
    analogWrite(MOTOR_PIN_A, 0);
    delay(1000);
#endif

    pid.SetSampleTime(PID_PERIOD);
    pid.SetOutputLimits(0, 255);
    setpoint = 100;
    output = 160;
    analogWrite(MOTOR_PIN_A, output);
    delay(100);
    pid.SetMode(AUTOMATIC);

    Serial << F("\nSetup OK\nLaunch timer\n");
    TIMSK0 |= (1 << OCIE0A);
}

void loop () {
    if (buf_index == BUF_MAX) {
        // stop motor and timer
        digitalWrite(MOTOR_ENABLE, 0);
        TIMSK0 &= ~(1 << OCIE0A);

        // dump collected PID data
        Serial << F("input\toutput\n");
        for(int i = 0; i < BUF_MAX; i++) {
            Serial << buf_in[i]  << '\t' << buf_out[i] << EOL;
        }
        Serial << nb_timer_call  << F(" Timer  calls , avg ") << (all_timer_call/nb_timer_call)   << F(" us , max = ") << max_timer_call  << F(" us") << EOL;
        Serial << nb_pid_call  << F(" PID  calls\n");
        Serial << nb_sensor_call << F(" Sensor calls , avg ") << (all_sensor_call/nb_sensor_call) << F(" us , max = ") << max_sensor_call << F(" us") << EOL;

        // die.
        for(;;){}
    } else {
        Serial << buf_index << EOL;
        delay(200);
    }

    // if (!Serial.available()) {
    //     static unsigned long prev = 0;
    //     static unsigned long now = millis();

    //     long p = position;
    //     long dx = p - prev;
    //     prev = p;
    //     unsigned long dt = millis() - now;
    //     now = millis();
    //     Serial << F("\nposition = ") << p << F(" , pwm = ") << output << F(" , speed = ") << (dx * 1000 / dt) << F(" step/s\n");

    //     Serial << nb_timer_call  << F(" Timer  calls , avg ") << (all_timer_call/nb_timer_call)   << F(" us , max = ") << max_timer_call  << F(" us") << EOL;
    //     Serial << nb_pid_call  << F(" PID  calls\n");
    //     Serial << nb_sensor_call << F(" Sensor calls , avg ") << (all_sensor_call/nb_sensor_call) << F(" us , max = ") << max_sensor_call << F(" us") << EOL;
    //     delay(200);
    // } else {
    //     digitalWrite(MOTOR_ENABLE, 0);
    // }
}

#endif