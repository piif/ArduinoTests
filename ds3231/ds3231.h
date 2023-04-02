#ifndef DS3231_H
#define DS3231_H

#include <Arduino.h>
#include "timeFunctions.h"

#define DS3231_I2C_ADDRESS 0x68
#define DS3231_REG_NUM  0x13

class DS3231 {
public:
    typedef struct _ControlRegister {
        byte EOSC:1;  /* 0 */
        byte BBSQW:1; /* 0 */
        byte CONV:1;  /* 0 */
        byte RS2:1;   /* 1 */
        byte RS1:1;   /* 1 */
        byte INTCN:1; /* 1 */
        byte A2IE:1;  /* 0 */
        byte A1IE:1;  /* 0 */
    } ControlRegister;

    typedef struct _StatusRegister {
        byte OSF:1;    /* 1 */
        byte _fill:3;
        byte EN32kHz:1;/* 1 */
        byte BSY:1;    /* 0 */
        byte A2F:1;    /* 0 */
        byte A1F:1;    /* 0 */
    } StatusRegister;

    DS3231();

    byte decToBcd(byte val);
    byte bcdToDec(byte val);

    void getTime(TimeStruct *time);

    int getFullTemp();

    short getShortTemp();

    void setTime(TimeStruct *time);

    void setControl(byte control, byte status);

#ifdef DS3231_DEBUG
    void dumpAllRegisters();
#endif

private:
#ifdef DS3231_DEBUG
    static char *DS3231::registerContents[0x13];
#endif

};

#endif