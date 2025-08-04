#include <Arduino.h>

#define M_X_EN 11 // white
#define M_X_A   9 // blue
#define M_X_B  10 // gray
#define M_X_SPEED 60
#define X_MAX 5974

#define M_Y_EN  3 // orange
#define M_Y_A   6 // green
#define M_Y_B   5 // yellow
#define M_Y_SPEED 60
#define Y_MAX 100000
#define Y_MARGIN 8000 // distance between paper entry and pen

#define SENSOR_BITS (PINC & 0x3F)

#define FORK_X_A A0
#define FORK_X_B A1
#define FORK_X_BITS 0x03

#define FORK_Y_A A2
#define FORK_Y_B A3
#define FORK_Y_BITS 0x0C

#define FORK_P A4 // green , 1 means paper is present , 0 = no paper
#define FORK_P_BITS 0x10
#define FORK_H A5 // 1 means head is at left end
#define FORK_H_BITS 0x20

#define PAPER_LED 13

extern volatile byte paper_present;
extern volatile byte head_max;

void axis_begin();

void axis_x_set_speed(int v);
void axis_y_set_speed(int v);
void axis_stop();

void axis_x_move_of(int delta);
void axis_y_move_of(int delta);
void axis_status();
