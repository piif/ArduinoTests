#include <Arduino.h>

#include "axis.h"
#include "strop.h"
#include "tools.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

int init_head() {
    // if head already on sensor, leave it
    if (head_max) {
        Serial << "init_head already ok";
    }

    // // move to left until head touch sensor
    // Serial.println("WAIT_FOR(head sensor == 1)");
    // axis_x_set_speed(M_X_SPEED);
    // if (WAIT_FOR(xAxis.sensorState == 1)) {
    //     return 1;
    // }
    // xAxis.stop();
    // Serial << "<- ok " << xAxis.position << EOL;
    // xAxis.position = 0;

    // Serial.println("WAIT_FOR(xAxis.position >= X_MAX)");
    // xAxis.setHighSpeed(1);
    // if (WAIT_FOR(xAxis.position >= xAxis.positionMax)) {
    //     return 1;
    // }
    // xAxis.stop();
    // Serial << "==> ok " << xAxis.position << EOL;

    // Serial.println("WAIT_FOR(xAxis.position <= 0 || xAxis.sensorState == 1)");
    // xAxis.setHighSpeed(-1);
    // if (WAIT_FOR(xAxis.position <= 500 || xAxis.sensorState == 1)) {
    //     return 1;
    // }
    // xAxis.setLowSpeed(-1);
    // if (WAIT_FOR(xAxis.position <= 0 || xAxis.sensorState == 1)) {
    //     return 1;
    // }
    // xAxis.stop();
    // Serial << "<== ok " << xAxis.position << EOL;

    // // if xAxis.sensorState == 1 while X far from 0, it's a problem
    // status();
    return 1;
}

void feed_paper() {
    Serial.println("feed_paper"); axis_status();

    if (!head_max) {
        axis_x_set_speed(M_X_SPEED);
        WAIT_FOR(head_max);
        Serial.println("after 1st move right"); axis_status();
    }
    axis_x_set_speed(-M_X_SPEED);
    WAIT_FOR(X_pos <= X_MAX - 100);
    axis_stop();

    axis_x_set_speed(M_X_SPEED);
    WAIT_FOR(head_max);

    // if paper detected, eject it until bottom border
    if (paper_present) {
        axis_y_set_speed(-M_Y_SPEED);
        WAIT_FOR(!paper_present);
        axis_stop();
    }
    // then, feed until it's detected
    axis_y_set_speed(M_Y_SPEED);
    WAIT_FOR(paper_present);
    axis_stop();

    // this is position for bottom of page
    Y_pos = 0;

    Serial.println("bottom of page"); axis_status();

    // find top border
    axis_y_set_speed(M_Y_SPEED);
    WAIT_FOR(!paper_present);
    axis_stop();

    Serial.println("top of page"); axis_status();

    // go back to bottom
    axis_y_set_speed(-M_Y_SPEED);
    WAIT_FOR(Y_pos <= 100);
    axis_stop();
    if (!paper_present) {
        Serial.println("end of paper before bottom of page");
        return;
    }

    // look for left border
    axis_x_set_speed(-M_X_SPEED);
    WAIT_FOR(!paper_present);
    axis_stop();

    Serial.println("left of page"); axis_status();

    // // if no paper present, abort
    // if (yAxis.sensorState == 1) {
    //     Serial.println("No paper");
    //     return 1;
    // }

    // // feed paper until yAxis.sensorState == 1 (end of sheet)
    // yAxis.position=0;
    // Serial.println("WAIT_FOR(yAxis.sensorState == 1)");
    // yAxis.setHighSpeed(1);
    // if (WAIT_FOR(yAxis.sensorState == 1 || yAxis.position >= yAxis.positionMax)) {
    //     return 1;
    // }
    // Serial.println("ok");

    // if (yAxis.position >= yAxis.positionMax) {
    //     yAxis.stop();
    //     Serial.println("can't find end of paper ?");
    //     return 1;
    // }

    // // move paper back until page bottom
    // Serial.println("yAxis.position <= Y_MARGIN");
    // yAxis.setHighSpeed(-1);
    // if (WAIT_FOR(yAxis.position <= Y_MARGIN)) { // todo : or yAxis.sensorState==1 , but needs to rollback until 0 before
    //     return 1;
    // }
    // yAxis.stop();
    // Serial.println("ok");

    // status();

    // // consider this as new 0
    // yAxis.position=0;
    return 1;
}

InputItem inputs[] = {
	{ '0', 'f', (void *)axis_stop },
	{ 's', 'f', (void *)axis_stop },
	{ '?', 'f', (void *)status },
	{ 'x', 'I', (void *)axis_x_set_speed },
	{ 'y', 'I', (void *)axis_y_set_speed },
	{ 'X', 'I', (void *)axis_x_move_of },
	{ 'Y', 'I', (void *)axis_y_move_of },
	{ 'f', 'f', (void *)feed_paper },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

    axis_begin();

    set_sleep_mode(SLEEP_MODE_IDLE);

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
    status();
}

void status() {
    axis_status();
}

void loop() {
    sleep_mode();
	handleInput();
}
