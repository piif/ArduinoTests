#ifndef TIMEFUNTIONS_H
#define TIMEFUNTIONS_H

#ifdef ARDUINO
#include <Arduino.h>
#endif
typedef struct _TimeStruct {
    byte seconds;    /* 00 - 59 */
    byte minutes;    /* 00 - 59 */
    byte hours;      /* 00 - 23 */
    byte dayOfWeek;  /*  1 (monday) -  7 (sunday) */
    byte dayOfMonth; /*  1 - 31 */
    byte month;      /*  1 - 12 */
    byte year;       /*  0 - 99 */
} TimeStruct;

extern char *shortDays[8];
extern char *shortMonthes[13];

void toLocal(TimeStruct *time);

#endif