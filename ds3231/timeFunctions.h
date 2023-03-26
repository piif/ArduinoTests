
typedef struct _TimeStruct {
    byte seconds;    /* 00 - 59 */
    byte minutes;    /* 00 - 59 */
    byte hours;      /* 00 - 23 */
    byte dayOfWeek;  /*  1 (monday) -  7 (sunday) */
    byte dayOfMonth; /*  1 - 31 */
    byte month;      /*  1 - 12 */
    byte year;       /*  0 - 99 */
} TimeStruct;

char *shortDays[] = {
    "xxx",
    "Lun",
    "Mar",
    "Mer",
    "Jeu",
    "Ven",
    "Sam",
    "Dim"
};

const byte lastDay[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

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
