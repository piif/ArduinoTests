#include <Arduino.h>

#define M_X_EN 11 // white
#define M_X_A   9 // blue
#define M_X_B   6 // green

#define VCC 16

// minimal PWN for X axis
#if VCC == 32
#define M_X_SPEED 40
#endif
#if VCC == 24
#define M_X_SPEED 80
#endif
#if VCC == 16
#define M_X_SPEED 120
#endif
// Position when head touch "end of carriage" sensor
#define X_MAX 5950

// X position of paper sensor relative to pen position (center of head) (to be verified once pen is positioned)
// this means that when paper sensor see left border of paper, we have to move of X_PAPER_SENSOR_DELTA to set pen on this border
#define X_PAPER_SENSOR_DELTA -1100

// Position of left and right A4 paper border
#define X_PAPER_LEFT   800
#define X_PAPER_RIGHT 5800

#define M_Y_EN  3 // orange
#define M_Y_A  10 // gray
#define M_Y_B   5 // yellow

// minimal PWN for X axis
#if VCC == 32
#define M_Y_SPEED 60
#endif
#if VCC == 24
#define M_Y_SPEED 80
#endif
#if VCC == 16
#define M_Y_SPEED 170
#endif

// Y position of paper sensor relative to pen position (to be verified once pen is positioned)
#define Y_PAPER_SENSOR_DELTA 100

#define SENSOR_BITS (PINC & 0x3F)

#define FORK_X_A A0
#define FORK_X_B A1
#define FORK_X_BITS 0x03

#define ONE_BIT_FORK

#ifdef ONE_BIT_FORK
#define FORK_Y_A A2
#define FORK_Y_BITS 0x04
#else
#define FORK_Y_A A2
#define FORK_Y_B A3
#define FORK_Y_BITS 0x0C
#endif

#define FORK_P A4 // green , 1 means paper is present , 0 = no paper
#define FORK_P_BITS 0x10
// #define CAN_DETECT_LEFT_BORDER

#define FORK_H A5 // 1 means head is at left end
#define FORK_H_BITS 0x20

#define PAPER_LED 13

extern volatile byte paper_present, head_max;
extern volatile long X_pos, Y_pos;
extern volatile int X_dir, Y_dir;

void axis_begin();

void axis_x_set_speed(int v);
void axis_y_set_speed(int v);
void axis_stop();

void axis_x_move_of(int delta);
void axis_y_move_of(int delta);
void axis_status();
