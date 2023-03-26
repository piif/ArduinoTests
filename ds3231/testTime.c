#ifndef ARDUINO

#include <stdio.h>

typedef short byte;

#include "timeFunctions.h"

TimeStruct tests[] = {
    { 12, 34, 22, 7, 31, 12, 2022 },
    { 12, 34, 23, 7, 31, 12, 2022 },
    { 12, 34,  0, 1,  1,  1, 2023 },
    { 12, 34, 23, 2, 28,  2, 2023 },
    { 12, 34, 10, 6, 29, 10, 2022 },
    { 12, 34, 23, 6, 29, 10, 2022 },
    { 12, 34,  1, 1, 30, 10, 2022 },
    { 12, 34, 10, 1, 30, 10, 2022 },
    { 12, 34, 10, 2, 31, 10, 2022 },
    { 12, 34, 10, 6, 25,  3, 2023 },
    { 12, 34, 23, 6, 25,  3, 2023 },
    { 12, 34,  1, 1, 26,  3, 2023 },
    { 12, 34, 10, 1, 26,  3, 2023 }
};
#define NB_TESTS (sizeof(tests) / sizeof(TimeStruct))

void toStr(TimeStruct *time, char *str) {
    sprintf(str, "%3s % 2u/%02u/%4u : %2u:%2u:%2u",
        shortDays[time->dayOfWeek],
        time->dayOfMonth, time->month, time->year,
        time->hours, time->minutes, time->seconds);
}

int main(int argc, char **argv) {
    char strGmt[40], strLocal[40];
    for(int i = 0; i < NB_TESTS; i++) {
        TimeStruct *gmt = &tests[i];
        TimeStruct local = *gmt;
        toLocal(&local);
        toStr(gmt, strGmt);
        toStr(&local, strLocal);
        printf("test % 2u : %s\t->  %s\n", i, strGmt, strLocal);
    }
}

#endif