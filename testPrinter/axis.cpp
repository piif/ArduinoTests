#include "axis.h"
#include "tools.h"
#include "strop.h"

Axis::Axis(
        byte p_motorPinEnable, byte p_motorPinA, byte p_motorPinB,
        byte p_forkPinA, byte p_forkPinB,
        byte p_sensorPin, byte p_sensorMode = INPUT_PULLUP, short p_echoPin = -1):
    motorPinEnable(p_motorPinEnable),
    motorPinA(p_motorPinA),
    motorPinB(p_motorPinB),
    forkPinA(p_forkPinA),
    forkPinB(p_forkPinB),
    sensorPin(p_sensorPin),
    sensorMode(p_sensorMode),
    echoPin(p_echoPin)
{
    forkState = ((digitalRead(forkPinA) & 1) << 1) | (digitalRead(forkPinB) & 1);
    sensorState = digitalRead(sensorPin);
    if (echoPin != -1) {
    	digitalWrite(echoPin, sensorState);
    }
}

void Axis::begin() {
   	pinMode(motorPinEnable, OUTPUT);
	pinMode(motorPinA,      OUTPUT);
	pinMode(motorPinB,      OUTPUT);
	pinMode(forkPinA,       INPUT);
	pinMode(forkPinB,       INPUT);
	pinMode(sensorPin,      sensorMode);
    if (echoPin != -1) {
    	pinMode(echoPin, OUTPUT);
    }

    forkState = ((digitalRead(forkPinA) & 1) << 1) | (digitalRead(forkPinB) & 1);
    sensorState = digitalRead(sensorPin);
    if (echoPin != -1) {
    	digitalWrite(echoPin, sensorState);
    }
}

void Axis::setSpeed(int v) {
    speed = v;
#ifdef PWM_ON_ENABLE
    if (speed == 0) {
        digitalWrite(motorPinEnable, 0);
    } else {
        if (speed > 0) {
            analogWrite(motorPinEnable, speed);
            digitalWrite(motorPinA, 1);
            digitalWrite(motorPinB, 0);
        } else {
            analogWrite(motorPinEnable, -speed);
            digitalWrite(motorPinA, 0);
            digitalWrite(motorPinB, 1);
        }
    }
#else
    if (speed == 0) {
        digitalWrite(motorPinEnable, 0);
    } else {
        if (speed > 0) {
            analogWrite(motorPinA, speed);
            digitalWrite(motorPinB, 0);
        } else {
            digitalWrite(motorPinA, 0);
            analogWrite(motorPinB, -speed);
        }
        digitalWrite(motorPinEnable, 1);
   }
#endif

}

void Axis::moveOf(int delta) {
    long target = position + delta;
    if (delta == 0 || target > positionMax || target < positionMin) {
        Serial.print(target); Serial.println(" none or out of range");
        return;
    }
    if (delta > 200 || delta < -200) {
        long preTarget = position + (delta * .8);
        Serial << "going from " << position << EOL;
        Serial << " to " << preTarget << EOL;
        Serial << " then  " << target << EOL;
        if (delta > 0) {
            setHighSpeed(1);
            if(WAIT_FOR(position >= preTarget)) {
                return;
            }
            stop();
        } else {
            setHighSpeed(-1);
            if (WAIT_FOR(position <= preTarget)) {
                return;
            }
            stop();
        }
    } else {
        Serial << "going from " << position << EOL;
        Serial << " to " << target << EOL;
    }

    if (delta > 0) {
        setLowSpeed(1);
        if (WAIT_FOR(position >= target)) {
            return;
        }
        stop();
    } else {
        setLowSpeed(-1);
        if (WAIT_FOR(position <= target)) {
            return;
        }
        stop();
    }
}

void Axis::updateState() {
    byte newForkState = ((digitalRead(forkPinA) & 1) << 1) | (digitalRead(forkPinB) & 1);
    if (newForkState != forkState) {
        switch ((newForkState << 2) | forkState) {
            case 0b0001:
            case 0b0111:
            case 0b1110:
            case 0b1000:
                position++;
                break;
            case 0b0010:
            case 0b1011:
            case 0b1101:
            case 0b0100:
                position--;
                break;
            default:
                err[errIdx % ERR_LEN] = (newForkState << 2) | forkState;
                errIdx++;
                break;
        }
        forkState = newForkState;
    }

    byte s = digitalRead(sensorPin);
    if (s != sensorState) {
        sensorState = s;
        if (echoPin != -1) {
            digitalWrite(echoPin, sensorState);
        }
    }
}

void Axis::status() {
	Serial << "speed="    << speed
           << "\tfork="   << forkState
           << "\tsensor=" << sensorState
           << "\tpos="    << position
           << EOL;

    if (errIdx != 0) {
        Serial << errIdx << " errors :\n";
        for (byte i = 0; i<errIdx && i<ERR_LEN; i++) {
            Serial << ' ' << toBin(err[errIdx % ERR_LEN], 4);
        }
        Serial << EOL;
        errIdx = 0;
    }
}
