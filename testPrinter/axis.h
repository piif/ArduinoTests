#include <Arduino.h>

#define M_X_EN 11 // white
#define M_X_A   9 // blue
#define M_X_B  10 // gray

// minimal PWN for X axis
#define M_X_SPEED 40
// Position when head touch "end of carriage" sensor
#define X_MAX 5950

// X position of paper sensor relative to pen position (center of head) (to be verified once pen is positioned)
// this means that when paper sensor see left border of paper, we have to move of X_PAPER_SENSOR_DELTA to set pen on this border
#define X_PAPER_SENSOR_DELTA -1100

// Position of left and right A4 paper border
#define X_PAPER_LEFT   800
#define X_PAPER_RIGHT 5800

#define M_Y_EN  3 // orange
#define M_Y_A   6 // green
#define M_Y_B   5 // yellow

// minimal PWN for X axis
#define M_Y_SPEED 60
// Y position of paper sensor relative to pen position (to be verified once pen is positioned)
#define Y_PAPER_SENSOR_DELTA 100

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
extern volatile long X_pos;
extern volatile long Y_pos;

void axis_begin();

void axis_x_set_speed(int v);
void axis_y_set_speed(int v);
void axis_stop();

void axis_x_move_of(int delta);
void axis_y_move_of(int delta);
void axis_status();
