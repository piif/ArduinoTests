#include <Arduino.h>
#include "timeFunctions.h"

const byte lastDay[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

char *shortDays[8] = {
    "",
    "Lun",
    "Mar",
    "Mer",
    "Jeu",
    "Ven",
    "Sam",
    "Dim"
};

char *shortMonthes[13] = {
    "",
    "jan", "fev" , "mar" , "avr" ,
    "mai", "juin", "juil", "aout",
    "sep", "oct" , "nov" , "dec"
};

void nextDay(TimeStruct *time) {
    if (time->dayOfWeek == 7) {
        time->dayOfWeek = 1;
    } else {
        time->dayOfWeek++;
    }
    byte last = lastDay[time->month];
    if (time->month == 2 && time->year%4 == 0) {
        last++; // correct from 1901 to 2099, should be enough ;-)
    }
    if (time->dayOfMonth == last) {
        time->dayOfMonth = 1;
        if (time->month == 12) {
            time->year++;
            time->month = 1;
        } else  {
            time->month++;
        }
    } else {
        time->dayOfMonth++;
    }
}

void toLocal(TimeStruct *time) {
    byte offset = 1;
    byte dayOfWeek = time->dayOfWeek % 7; // convert to 0(sunday)-6(saturday) value
    if (
        ( time->month > 3 && time->month < 10 ) // from april to september
            ||
        ( // after last synday of march
            ( time->month == 3 && (time->dayOfMonth - dayOfWeek) >= 25 )
                &&
            ! ( time->dayOfMonth >= 25 && dayOfWeek == 0 )
        )
            ||
        ( // before last sunday of october
            ( time->month == 10 && (time->dayOfMonth - dayOfWeek) < 25 )
                &&
            ! ( time->dayOfMonth >= 25 && dayOfWeek == 0 )
        )
            ||
        ( // on last sunday of october before 1:00 AM , or of april after 0:00
            ( (time->month == 10 && time->hours < 1) || (time->month == 3 && time->hours > 0) )
                &&
            ( time->dayOfMonth >= 25 && dayOfWeek == 0 )
        )
    ) {
        offset++;
    }
    time->hours += offset;
    if (time->hours > 23) {
        time->hours -= 24;
        nextDay(time);
    }
}
