// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include <Servo/src/Servo.h>
	#include <serialInput/serialInput.h>
	#include <MsTimer2/MsTimer2.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
	// example : #include "led7.h"
	#include <Servo.h>
	#include <serialInput.h>
	#include <MsTimer2.h>
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define DEBUG

#define LEFT_FOOT 11
#define RIGHT_FOOT 10
#define LEFT_LEG 8
#define RIGHT_LEG 9

class MyServo {
public:
	// Servo object attached
	Servo motor;

	// value to set for position "zero"
	int zero;
	// default speed, in ms/° (thus it's more a delay than a speed ...)
	unsigned int speed;

	// current angle
	int currentAngle;
	// target angle
	int startAngle, targetAngle;
	// delay to match target
	unsigned long startTime, targetTime;

	void init(byte pin, int _zero, int _speed = 10) {
		zero = _zero;
		speed = _speed;
		currentAngle = 0;
		targetTime = 0;
		pinMode(pin, OUTPUT);
		motor.attach(pin);
		motor.write(zero);
	}
	void update() {
		// rule of thumb between target-current and ttl
		if (targetTime == 0) {
			// nothing to do.
			return;
		}
		unsigned long now = millis();
		int a;
		if (now > targetTime) {
			// ttl expired -> force to target.
			a = targetAngle;
			targetTime = 0;
		} else {
			long t = now - startTime;
			a = startAngle + (targetAngle - startAngle) * t / (long)(targetTime - startTime);
		}
		if (a != currentAngle) {
#ifdef DEBUG
			Serial.print("@ "); Serial.println(now);
			Serial.print("update to "); Serial.println(currentAngle);
#endif
			currentAngle = a;
			motor.write(currentAngle + zero);
		}
	}
	void setAngle(int angle, unsigned long delay = 0) {
		startAngle = currentAngle;
		targetAngle = angle;
		if (delay == 0) {
			if (targetAngle > currentAngle) {
				delay = (targetAngle - currentAngle) * speed;
			} else {
				delay = (currentAngle - targetAngle) * speed;
			}
		}
		startTime = millis();
		targetTime = startTime + delay;
#ifdef DEBUG
		Serial.print("setAngle "); Serial.print(angle);
		Serial.print(", "); Serial.println(delay);
		Serial.print("=> "); Serial.print(currentAngle);
		Serial.print(" to "); Serial.println(targetAngle);
		Serial.print("in "); Serial.print(startTime);
		Serial.print(" to "); Serial.println(targetTime);
#endif
	}
	void status(Stream &s) {
		s.print(currentAngle);s.print(" + ");s.println(zero);
	}
};

MyServo leftFoot, rightFoot;
MyServo leftLeg, rightLeg;

void setLf(int angle) {
	leftFoot.setAngle(angle);
}
void setRf(int angle) {
	rightFoot.setAngle(angle);
}
void setLl(int angle) {
	leftLeg.setAngle(angle);
}
void setRl(int angle) {
	rightLeg.setAngle(angle);
}
void setSpeed(int speed) {
	leftFoot.speed = speed;
	rightFoot.speed = speed;
	leftLeg.speed = speed;
	rightLeg.speed = speed;
}

void waitIdle() {
	while(leftFoot.targetTime != 0 || rightFoot.targetTime != 0
			|| leftFoot.targetTime != 0 || rightFoot.targetTime != 0) {
		delay(5);
	}
}

void zero() {
	leftFoot.setAngle(0);
	rightFoot.setAngle(0);
	leftLeg.setAngle(0);
	rightLeg.setAngle(0);
}

void bow(int dir) {
	setLf(-45);
	setRf(45);
	waitIdle();
	if (dir > 0) {
		setRf(-45);
		waitIdle();
		setLf(0);
	} else {
		setLf(45);
		waitIdle();
		setRf(0);
	}
}

void walk(int nbSteps) {
	if (nbSteps > 0) {
		while(nbSteps --) {
			setLf(20);setRf(20);
			waitIdle();
			setLl(30);setRl(30);
			waitIdle();
			setLf(-20);setRf(-20);
			waitIdle();
			setLl(-30);setRl(-30);
			waitIdle();
		}
	} else {
		while(nbSteps ++) {
			setLf(20);setRf(20);
			waitIdle();
			setLl(-20);setRl(-20);
			waitIdle();
			setLf(-20);setRf(-20);
			waitIdle();
			setLl(20);setRl(20);
			waitIdle();
		}
	}
	zero();
}

void status() {
	Serial.println("left  foot :"); leftFoot.status(Serial);
	Serial.println("right foot :"); rightFoot.status(Serial);
	Serial.println("left  leg  :"); leftLeg.status(Serial);
	Serial.println("right leg  :"); rightLeg.status(Serial);
}

InputItem inputs[] = {
	{ '?', 'f', (void *)status },
	{ 's', 'I', (void *)setSpeed },
	{ 'l', 'I', (void *)setLf },
	{ 'r', 'I', (void *)setRf },
	{ 'L', 'I', (void *)setLl },
	{ 'R', 'I', (void *)setRl },
	{ 'z', 'I', (void *)zero },
	{ 'b', 'I', (void *)bow },
	{ 'w', 'I', (void *)walk }
};

void updateMotors() {
	leftFoot.update();
	rightFoot.update();
	leftLeg.update();
	rightLeg.update();
}

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	leftFoot.init(LEFT_FOOT, 40);
	rightFoot.init(RIGHT_FOOT, 50);
	leftLeg.init(LEFT_LEG, 55);
	rightLeg.init(RIGHT_LEG, 75);

	MsTimer2::set(10, updateMotors); // 500ms period
	MsTimer2::start();

	zero();

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();
}
