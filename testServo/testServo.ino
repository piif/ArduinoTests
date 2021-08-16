// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

#include <Arduino.h>
#include <serialInput.h>

#undef WITH_PWM_1
#define WITH_PWM_2

#if defined(WITH_PWM_1)
	#define SERVO 10
#elif defined(WITH_PWM_2)
	#define SERVO 3
#else
	#define SERVO 3 // 10
	#define SERVO_RANGE, 100, 2500
	Servo motor;
	#include <Servo.h>
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif


void set(int v) {
#if defined(WITH_PWM_1)
	OCR1B = 2*v;
#elif defined(WITH_PWM_2)
	OCR2B = v;
#else
	motor.write(v);
#endif
}

void attach(int v) {
#if defined(WITH_PWM_1)
	TCCR1A = 0x23;
#elif defined(WITH_PWM_2)
	TCCR2A = 0x21;
#else
	motor.attach(SERVO SERVO_RANGE);
#endif
}

void detach(int v) {
#if defined(WITH_PWM_1)
	TCCR1A = 0x03;
#elif defined(WITH_PWM_2)
	TCCR2A = 0x00;
#else
	motor.detach();
#endif
}

InputItem inputs[] = {
//	{ '?', 'f', (void *)status },
	{ 's', 'I', (void *)set },
	{ 'a', 'I', (void *)attach },
	{ 'd', 'I', (void *)detach },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);
	pinMode(SERVO, OUTPUT);
	pinMode(11, OUTPUT);

#if defined(WITH_PWM_1)
	// OC1A disabled , OC1B enabled / clear on compare match
	// WGM 15 = Fast PWM , top = OCR1A
	// prescale 8
	TCCR1A = 0x23;
	TCCR1B = 0x08;
	OCR1A  = 40000; // 16Mhz / 8 * 40000 = 20ms
	OCR1B = 2000;
#elif defined(WITH_PWM_2)
	// OC2A disabled , OC2B enabled / Set OC2A on Compare Match when up-counting. Clear OC2A on
	// Compare Match when down-counting.
	// WGM 7 = PWM phase correct (to divide f by 2), top = OCRA
	// prescale 8
//	TCCR2A = 0x73;
//	TCCR2B = 0x07;
	TCCR2A = 0x21;
	TCCR2B = 0x0F;
	OCR2A  = 156; // 16Mhz * 1024 * 156 * 2 = 19.96ms
	OCR2B = 4; // 1 = 128 µs
#else
	motor.attach(SERVO SERVO_RANGE);
#endif

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();
}
