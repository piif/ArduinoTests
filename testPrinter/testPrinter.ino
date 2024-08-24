#include <Arduino.h>

#include "axis.h"
#include "strop.h"
#include "tools.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define M_X_EN  3
#define M_X_A  5
#define M_X_B  6
#define M_X_SPEED 200
#define X_MAX 6500

#define M_Y_EN 11
#define M_Y_A  9
#define M_Y_B 10
#define M_Y_SPEED 200
#define Y_MAX 100000
#define Y_MARGIN 8000 // distance between paper entry and pen

#define FORK_X_A A0
#define FORK_X_B A1

#define FORK_Y_A A2
#define FORK_Y_B A3

#define FORK_P A4 // 0 means paper is present , 1 = no paper
#define FORK_H A5 // 1 means head is at left end
#define PAPER_LED 13

Axis xAxis(
    M_X_EN, M_X_A, M_X_B,
    FORK_X_A, FORK_X_B,
    FORK_H, -1);
Axis yAxis(
    M_Y_EN, M_Y_A, M_Y_B,
    FORK_Y_A, FORK_Y_B,
    FORK_P, PAPER_LED); // INPUT, 

ISR(PCINT1_vect) {
    xAxis.updateState();
    yAxis.updateState();
}

int status() {
	Serial.print("X :"); xAxis.status();
	Serial.print("Y :"); yAxis.status();
    return 0;
}

int initHead() {
    // if head already on sensor, leave it
    if (xAxis.sensorState == 1) {
        xAxis.position = 0;
        Serial.println("WAIT_FOR(head sensor == 0)");
        xAxis.setHighSpeed(1);
        if (WAIT_FOR(xAxis.sensorState == 0)) {
            return 1;
        }
        xAxis.stop();
        Serial << "-> ok " << xAxis.position << EOL;
    }

    // move to left until head touch sensor
    Serial.println("WAIT_FOR(head sensor == 1)");
    xAxis.setHighSpeed(-1);
    if (WAIT_FOR(xAxis.sensorState == 1)) {
        return 1;
    }
    xAxis.stop();
    Serial << "<- ok " << xAxis.position << EOL;
    xAxis.position = 0;

    Serial.println("WAIT_FOR(xAxis.position >= X_MAX)");
    xAxis.setHighSpeed(1);
    if (WAIT_FOR(xAxis.position >= xAxis.positionMax)) {
        return 1;
    }
    xAxis.stop();
    Serial << "==> ok " << xAxis.position << EOL;

    Serial.println("WAIT_FOR(xAxis.position <= 0 || xAxis.sensorState == 1)");
    xAxis.setHighSpeed(-1);
    if (WAIT_FOR(xAxis.position <= 500 || xAxis.sensorState == 1)) {
        return 1;
    }
    xAxis.setLowSpeed(-1);
    if (WAIT_FOR(xAxis.position <= 0 || xAxis.sensorState == 1)) {
        return 1;
    }
    xAxis.stop();
    Serial << "<== ok " << xAxis.position << EOL;

    // if xAxis.sensorState == 1 while X far from 0, it's a problem
    status();
    return 1;
}

void feedPaper() {
    // if no paper present, abort
    if (yAxis.sensorState == 1) {
        Serial.println("No paper");
        return 1;
    }

    // feed paper until yAxis.sensorState == 1 (end of sheet)
    yAxis.position=0;
    Serial.println("WAIT_FOR(yAxis.sensorState == 1)");
    yAxis.setHighSpeed(1);
    if (WAIT_FOR(yAxis.sensorState == 1 || yAxis.position >= yAxis.positionMax)) {
        return 1;
    }
    Serial.println("ok");

    if (yAxis.position >= yAxis.positionMax) {
        yAxis.stop();
        Serial.println("can't find end of paper ?");
        return 1;
    }

    // move paper back until page bottom
    Serial.println("yAxis.position <= Y_MARGIN");
    yAxis.setHighSpeed(-1);
    if (WAIT_FOR(yAxis.position <= Y_MARGIN)) { // todo : or yAxis.sensorState==1 , but needs to rollback until 0 before
        return 1;
    }
    yAxis.stop();
    Serial.println("ok");

    status();

    // consider this as new 0
    yAxis.position=0;
    return 1;
}

// indirection to instance methods since we can't get pointers to them
void xAxisSetSpeed(int v) { xAxis.setSpeed(v); return 1; }
void yAxisSetSpeed(int v) { yAxis.setSpeed(v); return 1; }
void xAxisMoveOf(int v)   { xAxis.moveOf(v);   return 1; }
void yAxisMoveOf(int v)   { yAxis.moveOf(v);   return 1; }
void stop() {
    xAxis.stop();
    yAxis.stop();
    status();
    return 1;
}

InputItem inputs[] = {
	{ '0', 'f', (void *)stop },
	{ '?', 'f', (void *)status },
	{ 'x', 'I', (void *)xAxisSetSpeed },
	{ 'y', 'I', (void *)yAxisSetSpeed },
	{ 'X', 'I', (void *)xAxisMoveOf },
	{ 'Y', 'I', (void *)yAxisMoveOf },
	{ 'h', 'f', (void *)initHead  },
	{ 'f', 'f', (void *)feedPaper },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

    xAxis.lowSpeed = 180;  xAxis.highSpeed = 255;
    xAxis.positionMin = 0; xAxis.positionMax = X_MAX;

    yAxis.lowSpeed = 180;  yAxis.highSpeed = 255;
    yAxis.positionMin = 0; yAxis.positionMax = Y_MAX;

    // Listen PCINT 8,9,10,11,12,13 == interrupts on PC0,1,2,3,4,5 == A0,1,2,3,4,5
    PCMSK0 = 0;
    PCMSK1 = _BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10) | _BV(PCINT11) | _BV(PCINT12) | _BV(PCINT13);
    PCMSK2 = 0;
    PCICR = _BV(PCIE1); // listen for PCINT[14:8]

    set_sleep_mode(SLEEP_MODE_IDLE);

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
    status();
}

void loop() {
    sleep_mode();
	handleInput();
}
