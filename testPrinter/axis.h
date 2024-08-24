#include <Arduino.h>

#define ERR_LEN 64

class Axis {
    public:
        // motor :
        byte motorPinEnable; // Must be able to output PWM
        byte motorPinA, motorPinB;

        // position sensor :
        byte forkPinA, forkPinB;
        // end of track sensor :
        byte sensorPin, sensorMode;
        // output pin to echo end of track sensor (-1 if none)
        short echoPin = -1;

        int lowSpeed, highSpeed;

        Axis(
            byte motorPinEnable, byte motorPinA, byte motorPinB,
            byte forkPinA, byte forkPinB,
            byte sensorPin, byte sensorMode = INPUT_PULLUP, short echoPin = -1);

        int speed = 0;
        long positionMax, positionMin;

        volatile byte forkState;
        volatile byte sensorState;
        volatile long position;

        volatile byte err[ERR_LEN];
        volatile byte errIdx = 0;

        void begin();
        void setSpeed(int v);
        void stop() {
            setSpeed(0);
        }
        void setHighSpeed(short dir) {
            setSpeed(dir == -1 ? -highSpeed : highSpeed);
        }
        void setLowSpeed(short dir) {
            setSpeed(dir == -1 ? -lowSpeed : lowSpeed);
        }
        void moveOf(int delta);
        void updateState();
        void status();
};
