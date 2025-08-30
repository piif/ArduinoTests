// standard libs
#include <Arduino.h>
#include <avr/wdt.h>
#include "strop.h"

// custom external libs
#include "serialInput.h"

// local libs
#include "pins.h"
#include "project.h"
#include "myClock.h"
#include "stepper.h"
#include "fork.h"

#ifndef DEFAULT_BAUDRATE
    #define DEFAULT_BAUDRATE 115200
#endif

Stepper motorX, motorY;
Fork forkX, forkY;

#define PAPER_SENSOR USER_1
#define PAPER_SENSOR_INTR USER_1_INTR
#define PAPER_SENSOR_INTR_NUM USER_1_INTR_NUM

#define HEAD_SENSOR USER_3
#define HEAD_SENSOR_INTR USER_3_INTR
#define HEAD_SENSOR_INTR_NUM USER_3_INTR_NUM

#define X_FORK_A USER_4
#define X_FORK_A_INTR USER_4_INTR
#define X_FORK_A_INTR_NUM USER_4_INTR_NUM

#define X_FORK_B USER_0
#define X_FORK_B_INTR USER_0_INTR
#define X_FORK_B_INTR_NUM USER_0_INTR_NUM

#define Y_FORK_A Y_MIN_PIN
#define Y_FORK_A_INTR Y_MIN_INTR
#define Y_FORK_A_INTR_NUM Y_MIN_INTR_NUM

#define Y_FORK_B Z_MIN_PIN
#define Y_FORK_B_INTR Z_MIN_INTR
#define Y_FORK_B_INTR_NUM Z_MIN_INTR_NUM


#ifdef COMPUTE_ISR_DURATION
volatile unsigned long pinChangeNbIsrCall = 0;
volatile unsigned long pinChangeAllIsrCall = 0;
volatile unsigned long pinChangeMaxIsrCall = 0;
#endif

void myClockCallback(unsigned long clock) {
    stepperDoStep(&motorX);
    stepperDoStep(&motorY);
}

void pinChangeCallback() {
#ifdef COMPUTE_ISR_DURATION
    unsigned long tic = micros();
#endif

    forkUpdate(&forkX);
    forkUpdate(&forkY);
    byte paper = digitalRead(PAPER_SENSOR);
    byte head  = digitalRead(HEAD_SENSOR);

#ifdef COMPUTE_ISR_DURATION
    unsigned long duration = micros() - tic;
    pinChangeNbIsrCall++;
    pinChangeAllIsrCall += duration;
    if (duration > pinChangeMaxIsrCall) {
        pinChangeMaxIsrCall = duration;
    }
#endif
}

ISR(PIN_PCINT_vect(PAPER_SENSOR_INTR_NUM)) {
    pinChangeCallback();
}
#if HEAD_SENSOR_INTR_NUM != PAPER_SENSOR_INTR_NUM
ISR(PIN_PCINT_vect(HEAD_SENSOR_INTR_NUM)) {
    pinChangeCallback();
}
#endif
#if X_FORK_A_INTR_NUM != PAPER_SENSOR_INTR_NUM \
 && X_FORK_A_INTR_NUM != HEAD_SENSOR_INTR_NUM 
ISR(PIN_PCINT_vect(X_FORK_A_INTR_NUM)) {
    pinChangeCallback();
}
#endif
#if X_FORK_B_INTR_NUM != PAPER_SENSOR_INTR_NUM \
 && X_FORK_B_INTR_NUM != HEAD_SENSOR_INTR_NUM \
 && X_FORK_B_INTR_NUM != X_FORK_A_INTR_NUM
ISR(PIN_PCINT_vect(X_FORK_B_INTR_NUM)) {
    pinChangeCallback();
}
#endif
#if Y_FORK_A_INTR_NUM != PAPER_SENSOR_INTR_NUM \
 && Y_FORK_A_INTR_NUM != HEAD_SENSOR_INTR_NUM \
 && Y_FORK_A_INTR_NUM != X_FORK_A_INTR_NUM \
 && Y_FORK_A_INTR_NUM != X_FORK_B_INTR_NUM
ISR(PIN_PCINT_vect(Y_FORK_A_INTR_NUM)) {
    pinChangeCallback();
}
#endif
#if Y_FORK_B_INTR_NUM != PAPER_SENSOR_INTR_NUM \
 && Y_FORK_B_INTR_NUM != HEAD_SENSOR_INTR_NUM \
 && Y_FORK_B_INTR_NUM != X_FORK_A_INTR_NUM \
 && Y_FORK_B_INTR_NUM != X_FORK_B_INTR_NUM \
 && Y_FORK_B_INTR_NUM != Y_FORK_A_INTR_NUM
ISR(PIN_PCINT_vect(Y_FORK_B_INTR_NUM)) {
    pinChangeCallback();
}
#endif

void moveX(int v) {
    stepperMoveOf(&motorX, v);
}
void moveY(int v) {
    stepperMoveOf(&motorY, v);
}

void status() {
    byte paper = digitalRead(PAPER_SENSOR);
    byte head  = digitalRead(HEAD_SENSOR);

    Serial
        << F("X fork ") << forkX.position << F(" (") << forkX.errors
        << F(") , motor ") << motorX.toMove << F(" (") << digitalRead(motorX.enable_pin)
        << F(")\nY fork ") << forkY.position << F(" (") << forkY.errors
        << F(") , motor ") << motorY.toMove << F(" (") << digitalRead(motorY.enable_pin)
        << F(")\npaper ") << paper << F(" , head ") << head << EOL;

#ifdef COMPUTE_ISR_DURATION
    Serial
        << myClockNbIsrCall << F(" timer calls , avg ")
        << (myClockNbIsrCall == 0 ? 0 : (myClockAllIsrCall/myClockNbIsrCall)) << F(" us , max = ")
        << myClockMaxIsrCall << F(" us") << EOL;
    Serial
        << pinChangeNbIsrCall << F(" pin change calls , avg ")
        << (pinChangeNbIsrCall==0 ? 0 : (pinChangeAllIsrCall/pinChangeNbIsrCall))
        << F(" us , max = ") << pinChangeMaxIsrCall << F(" us") << EOL;
#endif
}

InputItem inputs[] = {
    { '?', 'f', (void *)status },
    { 'x', 'I', (void *)moveX },
    { 'y', 'I', (void *)moveY },
};

void setup() {
    pinMode(DEBUG_PIN, OUTPUT);
    digitalWrite(DEBUG_PIN, 1);

    Serial.begin(DEFAULT_BAUDRATE);

    set_sleep_mode(SLEEP_MODE_IDLE);

    stepperInit(&motorX, X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN);
    stepperInit(&motorY, Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN);
    forkInit(&forkX, X_FORK_A, X_FORK_B);
    forkInit(&forkY, Y_FORK_A, Y_FORK_B);
    pinMode(PAPER_SENSOR, INPUT_PULLUP);
    pinMode(HEAD_SENSOR, INPUT_PULLUP);

    PIN_PCMSK(PAPER_SENSOR_INTR_NUM) |= (1 << PAPER_SENSOR_INTR);
    PIN_PCMSK(HEAD_SENSOR_INTR_NUM)  |= (1 << HEAD_SENSOR_INTR);
    PIN_PCMSK(X_FORK_A_INTR_NUM)     |= (1 << X_FORK_A_INTR);
    PIN_PCMSK(X_FORK_B_INTR_NUM)     |= (1 << X_FORK_B_INTR);
    PIN_PCMSK(Y_FORK_A_INTR_NUM)     |= (1 << Y_FORK_A_INTR);
    PIN_PCMSK(Y_FORK_B_INTR_NUM)     |= (1 << Y_FORK_B_INTR);
    PCICR |= (1 << PIN_PCICR(PAPER_SENSOR_INTR_NUM))
           | (1 << PIN_PCICR(HEAD_SENSOR_INTR_NUM))
           | (1 << PIN_PCICR(X_FORK_A_INTR_NUM))
           | (1 << PIN_PCICR(X_FORK_B_INTR_NUM))
           | (1 << PIN_PCICR(Y_FORK_A_INTR_NUM))
           | (1 << PIN_PCICR(Y_FORK_B_INTR_NUM));

    registerInput(sizeof(inputs), inputs);

    initMyClock(100);
    startMyClock();

    status();
    Serial << F("setup Ok") << EOL;
}

void loop() {
    sleep_mode();
    handleInput();
}
