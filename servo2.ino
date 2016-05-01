// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include <Servo/src/Servo.h>
	#include <serialInput/serialInput.h>
	#include <MsTimer2/MsTimer2.h>
	#include <IRLib/IRLib.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#else
	// other includes with short pathes
	// example : #include "led7.h"
	#include <Servo.h>
	#include <serialInput.h>
	#include <MsTimer2.h>
	#include <IRLib.h>
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

// IR receiver must be connected on pind 3, 4 and 5
// 3 = data / 4 = GND -> set to LOW / 5 = Vcc -> set to HIGH
#define IR_PIN 3
#define IR_PIN_INTERRUPT 1
IRdecode My_Decoder;
IRrecvLoop My_Receiver(IR_PIN);


typedef struct _key { uint32_t code; char key; } Key;
Key remoteCodes[] = {
// makeblock remote
	{ 0xA25D, 'A' }, { 0x629D, 'B' }, { 0xE21D, 'C' },
	{ 0x22DD, 'D' }, { 0x02FD, 'u' }, { 0xC23D, 'E' },
	{ 0xE01F, 'l' }, { 0xA857, 'x' }, { 0x906F, 'r' },
	{ 0x6897, '0' }, { 0x9867, 'd' }, { 0xB04F, 'F' },
	{ 0x30CF, '1' }, { 0x18E7, '2' }, { 0x7A85, '3' },
	{ 0x10EF, '4' }, { 0x38C7, '5' }, { 0x5AA5, '6' },
	{ 0x42BD, '7' }, { 0x4AB5, '8' }, { 0x52AD, '9' },

// philips remote
	{ 0xC, 'X' },
	{ 0x31, 'S' }, { 0x30, 'p' },
	{ 0x2B, '<' }, { 0x2C, 'P' }, { 0x28, '>' },
	{ 0x6D, 'R' }, { 0x6E, 'G' }, { 0x6F, 'Y' }, { 0x70, 'B' },
	{ 0xCC, '?' }, { 0x54, 'H' }, { 0x90, 'A' },
	{ 0x58, 'u' },
	{ 0x4D, '-' }, { 0x5A, 'l' }, { 0x5C, 'k' }, { 0x5B, 'r' }, { 0x4C, '+' },
	{ 0x59, 'b' },
	{ 0x38, 's' }, { 0x0A, '^' }, { 0x40, 'O' },
	{ 0x11, 'v' }, { 0x0D, 'M' }, { 0x10, 'V' },
	{ 0x01, '1' }, { 0x02, '2' }, { 0x03, '3' },
	{ 0x04, '4' }, { 0x05, '5' }, { 0x06, '6' },
	{ 0x07, '7' }, { 0x08, '8' }, { 0x09, '9' },
	{ 0x4B, '_' }, { 0x00, '0' }, { 0x3C, 'T' },
	{ 0, 0 }
};

char lookUp(unsigned long code, Key *map) {
	while(map->key != 0) {
		if (code == map->code) {
			return map->key;
		}
		map++;
	}
	return '?';
}

//#define DEBUG

#define LEFT_FOOT 11
#define RIGHT_FOOT 8
#define LEFT_LEG 10
#define RIGHT_LEG 9

// range of 180° values for several kinds of servo
#define FUBATA_S3003 600, 2400
#define MG996R 630, 2620 // 850, 2400

// adapt following line to servo you actually use
#define LEFT_FOOT_RANGE FUBATA_S3003
#define RIGHT_FOOT_RANGE FUBATA_S3003
#define LEFT_LEG_RANGE MG996R
#define RIGHT_LEG_RANGE MG996R

// adapt following lines to actual servo alignment
#define LEFT_FOOT_ZERO 90
#define RIGHT_FOOT_ZERO 90
#define LEFT_LEG_ZERO 100
#define RIGHT_LEG_ZERO 90

class MyServo {
public:
	// Servo object attached
	Servo motor;

	// value to set for position "zero"
	// the idea consists in converting a 0..180° or 0..120° range to -90..90
	// thus "0" may be 90°, or 60 depending on motor amplitude
	int zero;
	// default speed, in ms/° (thus it's more a delay than a speed ...)
	unsigned int speed;

	// current angle
	int currentAngle;
	// target angle
	int startAngle, targetAngle;
	// delay to match target
	unsigned long startTime, targetTime;

	// min and max arguments are relative to 0 and 180 position in Servo lib space
	// user may have to fix a over-range value to handle 120° motor "as if" it is 180°
	// range
	void init(byte _pin, int _zero = 90, int _min = -1, int _max = -1) {
		zero = _zero;
		speed = 10;
		currentAngle = 0;
		targetTime = 0;
		pinMode(_pin, OUTPUT);
		if (_min == -1) {
			motor.attach(_pin);
		} else {
			motor.attach(_pin, _min, _max);
		}
		motor.write(zero);
	}
	void setSpeed(int _speed) {
		speed = _speed;
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
void setLl(int angle) {
	leftLeg.setAngle(angle);
}
// consider right members as "inverted" thus 30° is "in front"
// for legs and "down" for feet
void setRf(int angle) {
	rightFoot.setAngle(-angle);
}
void setRl(int angle) {
	rightLeg.setAngle(-angle);
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

void bow(int angle) {
	if (angle < 0) {
		setLf(angle);
		setRf(-2 * angle);
		waitIdle();
		setRf(angle);
	} else {
		setRf(-angle);
		setLf(2 * angle);
		waitIdle();
		setLf(-angle);
	}
}

void walk(int nbSteps) {
	if (nbSteps > 0) {
		while(nbSteps --) {
			setLf(45);setRf(-45);
			waitIdle();
			setLl(30);setRl(-30);
			waitIdle();
			setLf(-45);setRf(45);
			waitIdle();
			setLl(-30);setRl(30);
			waitIdle();
		}
	} else {
		while(nbSteps ++) {
			setLf(45);setRf(-45);
			waitIdle();
			setLl(-30);setRl(30);
			waitIdle();
			setLf(-45);setRf(45);
			waitIdle();
			setLl(30);setRl(-30);
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

//#define MODE_TEST
#define MODE_IR_ONLY

void setup() {
#if !defined(MODE_TEST) && !defined(MODE_IR_ONLY)
	Serial.begin(DEFAULT_BAUDRATE);
#endif

	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	digitalWrite(4, LOW);
	digitalWrite(5, HIGH);
	My_Receiver.enableIRIn();
	My_Receiver.blink13(true);


	leftFoot.init(LEFT_FOOT, LEFT_FOOT_ZERO, LEFT_FOOT_RANGE);
	rightFoot.init(RIGHT_FOOT, RIGHT_FOOT_ZERO, RIGHT_FOOT_RANGE);
	leftLeg.init(LEFT_LEG, LEFT_LEG_ZERO, LEFT_LEG_RANGE);
	rightLeg.init(RIGHT_LEG, RIGHT_LEG_ZERO, RIGHT_LEG_RANGE);

	MsTimer2::set(1, updateMotors);
	MsTimer2::start();

	zero();

#if !defined(MODE_TEST) && !defined(MODE_IR_ONLY)
	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
#endif
}

#ifdef MODE_TEST
void loop() {
	delay(3000);

	setLf(45);
	waitIdle();
	setLf(-45);
	waitIdle();

	zero();
	waitIdle();

	setRf(45);
	waitIdle();
	setRf(-45);
	waitIdle();

	zero();
	waitIdle();

	setLl(30);setRl(-30);
	waitIdle();
	setLl(-30);setRl(30);
	waitIdle();

	zero();
	waitIdle();

	setLl(30);setRl(-30);
	setLf(-45);setRf(45);
	waitIdle();

	setLl(-30);setRl(30);
	setLf(45);setRf(-45);
	waitIdle();

	zero();
	waitIdle();
}

#else // interactive mode

#define delta 5
int ll = 0, rl = 0, lf = 0, rf = 0;

void loop() {
#if !defined(MODE_IR_ONLY)
	handleInput();
#endif

	if (My_Receiver.GetResults(&My_Decoder)) {//wait till it returns true
		My_Decoder.decode();
//		My_Decoder.DumpResults();
		char code = lookUp(My_Decoder.value & 0xffff, remoteCodes);
#if !defined(MODE_IR_ONLY)
		Serial.println(code);
#endif
		switch(code) {
		case '-': ll += delta; setLl(ll);
		break;
		case '^': ll -= delta; setLl(ll);
		break;
		case 'H': rl += delta; setRl(rl);
		break;
		case '+': rl -= delta; setRl(rl);
		break;
		case 'l': lf += delta; setLf(lf);
		break;
		case 'b': lf -= delta; setLf(lf);
		break;
		case 'u': rf += delta; setRf(rf);
		break;
		case 'r': rf -= delta; setRf(rf);
		break;
		case 'k': ll = 0; rl = 0; lf = 0; rf = 0; zero();
		break;

		}
		My_Receiver.resume();
	}
}
#endif
