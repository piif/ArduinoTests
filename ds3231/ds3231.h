#ifndef DS3231_H
#define DS3231_H

#include <Wire.h>
#include "timeFunctions.h"

#define DS3231_I2C_ADDRESS 0x68
#define DS3231_REG_NUM  0x13

class DS3231 {
public:
#ifdef DS3231_DEBUG
    static char *registerContents[0x13];

    void dumpAllRegisters() {
        Wire.beginTransmission(DS3231_I2C_ADDRESS);
        Wire.write(0);
        Wire.endTransmission();

        Wire.requestFrom(DS3231_I2C_ADDRESS, DS3231_REG_NUM);

        byte registerValues[DS3231_REG_NUM];
        for (byte i=0; i< DS3231_REG_NUM; i++) {
            registerValues[i] = Wire.read();
        }
        for (byte i=0; i< DS3231_REG_NUM; i++) {
            Serial.print(registerContents[i]);
            Serial.print('\t');
            Serial.println(registerValues[i], HEX);
        }
    }
#endif
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

    DS3231() {
        Wire.begin();
    }

    byte decToBcd(byte val) {
    return (val/10 << 4) | (val%10);
    }
    byte bcdToDec(byte val) {
    return (((val & 0xF0) >> 4)*10) + (val & 0x0F);
    }

    void getTime(TimeStruct *time) {
        Wire.beginTransmission(DS3231_I2C_ADDRESS);
        Wire.write(0);
        Wire.endTransmission();

        Wire.requestFrom(DS3231_I2C_ADDRESS, 7);

        time->seconds     = bcdToDec(Wire.read() & 0x7f);
        time->minutes     = bcdToDec(Wire.read());
        time->hours       = bcdToDec(Wire.read() & 0x3f);
        time->dayOfWeek   = bcdToDec(Wire.read());
        time->dayOfMonth  = bcdToDec(Wire.read());
        time->month       = bcdToDec(Wire.read());
        time->year        = bcdToDec(Wire.read());
    }

    int getFullTemp() {
        Wire.beginTransmission(DS3231_I2C_ADDRESS);
        Wire.write(0x11);
        Wire.endTransmission();

        Wire.requestFrom(DS3231_I2C_ADDRESS, 2);
        int result = Wire.read() << 2;
        result |= Wire.read() >> 6;
        if (result & 0x200) {
            result |= 0xFE00;
        }
        return result;
    }

    short getShortTemp() {
        Wire.beginTransmission(DS3231_I2C_ADDRESS);
        Wire.write(0x11);
        Wire.endTransmission();

        Wire.requestFrom(DS3231_I2C_ADDRESS, 1);
        return (short)(Wire.read());
    }

    void setTime(TimeStruct *time) {
        Wire.beginTransmission(DS3231_I2C_ADDRESS);
        Wire.write(0);
        Wire.write(decToBcd(time->seconds));
        Wire.write(decToBcd(time->minutes));
        Wire.write(decToBcd(time->hours));
        Wire.write(decToBcd(time->dayOfWeek));
        Wire.write(decToBcd(time->dayOfMonth));
        Wire.write(decToBcd(time->month));
        Wire.write(decToBcd(time->year));
        Wire.endTransmission();
    }

    void setControl(byte control, byte status) {
        Wire.beginTransmission(DS3231_I2C_ADDRESS);
        Wire.write(0x0E);
        Wire.write(control);
        Wire.write(status);
        Wire.endTransmission();
    }
};

#ifdef DS3231_DEBUG
static char *DS3231::registerContents[0x13] = {
    /* 0x00 */ "Seconds",
    /* 0x01 */ "Minutes",
    /* 0x02 */ "Hours",
    /* 0x03 */ "Day",
    /* 0x04 */ "Date",
    /* 0x05 */ "Month",
    /* 0x06 */ "Year",
    /* 0x07 */ "A1_Seconds",
    /* 0x08 */ "A1_Minutes",
    /* 0x09 */ "A1_Hours",
    /* 0x0A */ "A1_Date",
    /* 0x0B */ "A2_Minutes",
    /* 0x0C */ "A2_Hours",
    /* 0x0D */ "A2_Date",
    /* 0x0E */ "Control",
    /* 0x0F */ "Status",
    /* 0x10 */ "Offset",
    /* 0x11 */ "MSB_Temp",
    /* 0x12 */ "LSB_Temp"
};
#endif

#endif