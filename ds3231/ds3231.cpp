#define HAVE_SERIAL

#include <Arduino.h>
#include <Wire.h>
#include "ds3231.h"

DS3231::DS3231() {
    Wire.begin();
}

#ifdef DS3231_DEBUG
void DS3231::dumpAllRegisters() {
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

byte DS3231::decToBcd(byte val) {
    return (val/10 << 4) | (val%10);
}
byte DS3231::bcdToDec(byte val) {
    return (((val & 0xF0) >> 4)*10) + (val & 0x0F);
}

void DS3231::getTime(TimeStruct *time) {
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

int DS3231::getFullTemp() {
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

short DS3231::getShortTemp() {
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x11);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_I2C_ADDRESS, 1);
    return (short)(Wire.read());
}

void DS3231::setTime(TimeStruct *time) {
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

void DS3231::setControl(byte control, byte status) {
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x0E);
    Wire.write(control);
    Wire.write(status);
    Wire.endTransmission();
}

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
