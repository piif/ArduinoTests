// PIF_TOOL_CHAIN_OPTION: UPLOAD_OPTIONS := -c "raw,cr"
// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs ArduinoTools

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	// other includes with full pathes
	// example : #include "led7/led7.h"
#ifdef INTERACTIVE
	#include "serialInput/serialInput.h"
#endif
#else
	// other includes with short pathes
	// example : #include "led7.h"
#ifdef INTERACTIVE
	#include "serialInput.h"
#endif
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

// Sensor pineout
#define SENSOR_OUT 2
#define SENSOR_ENABLE 3
#define S0 4
#define S1 5
#define S2 6
#define S3 7

#define READ_DELAY 5

typedef enum {
	RED = 0, WHITE, BLUE, GREEN
} SENSOR_COLOR;

typedef enum {
	PRESCALE_OFF = 0, PRESCALE_20, PRESCALE_2, PRESCALE_1
} SENSOR_PRESCALE;

void setSensorColor(SENSOR_COLOR color) {
	switch (color) {
	case 'R': case 'r': color = RED  ; break;
	case 'G': case 'g': color = GREEN; break;
	case 'B': case 'b': color = BLUE ; break;
	case 'W': case 'w': color = WHITE; break;
	}
	digitalWrite(S2, color & 1);
	digitalWrite(S3, color & 2);
}
SENSOR_PRESCALE currentPrescale;

void setSensorPrescale(SENSOR_PRESCALE prescale) {
	currentPrescale = prescale;
	digitalWrite(S0, prescale & 1);
	digitalWrite(S1, prescale & 2);
}

char *getColorName(SENSOR_PRESCALE prescale,
		unsigned long r, unsigned long g,
		unsigned long b, unsigned long w) {
	unsigned long threshold;
	switch(prescale) {
	case PRESCALE_OFF:
		return "off";
	case PRESCALE_1:
		threshold = 2000;
		break;
	case PRESCALE_2:
		threshold = 1000;
		break;
	case PRESCALE_20:
		threshold = 100;
		break;
	}
	byte rgbMask =
			((r < threshold ? 1 : 0) << 2) |
			((g < threshold ? 1 : 0) << 1) |
			 (b < (threshold * 0.9) ? 1 : 0);
	switch (rgbMask) {
	case 0: return "black";
	case 1: return "blue";
	case 2: return "green";
	case 3: return "cyan";
	case 4: return "red";
	case 5: return "magenta";
	case 6: return "yellow";
	case 7: return "white";
	}
}

void readColor() {
//	digitalWrite(SENSOR_ENABLE, LOW);
//	delay(1); // datasheet says 100ns between enable and correct output

	unsigned long r, g, b, w;

	setSensorColor(WHITE);
	delay(READ_DELAY);
	w = pulseIn(SENSOR_OUT, LOW);

	setSensorColor(RED);
	delay(READ_DELAY);
	r = pulseIn(SENSOR_OUT, LOW);

	setSensorColor(GREEN);
	delay(READ_DELAY);
	g = pulseIn(SENSOR_OUT, LOW);

	setSensorColor(BLUE);
	delay(READ_DELAY);
	b = pulseIn(SENSOR_OUT, LOW);

//	digitalWrite(SENSOR_ENABLE, HIGH);

	Serial.print(w);

	Serial.print('\t');
	Serial.print(r);

	Serial.print('\t');
	Serial.print(g);

	Serial.print('\t');
	Serial.print(b);

	Serial.print('\t');
	Serial.print(getColorName(currentPrescale, r, g, b, w));
}

#ifdef INTERACTIVE
bool doLoop = false;
#else
bool doLoop = true;
#endif
void loopRead() {
	doLoop = !doLoop;
}

#ifdef INTERACTIVE
InputItem list[] = {
	{ 'i', 'f', (void *)setSensorPrescale},
	{ 'c', 'f', (void *)setSensorColor },
	{ 'r', 'f', (void *)readColor },
	{ 'l', 'f', (void *)loopRead }
};
#endif


void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);
	pinMode(SENSOR_ENABLE, OUTPUT);
	digitalWrite(SENSOR_ENABLE, LOW);
	pinMode(SENSOR_OUT, INPUT);
	setSensorPrescale(PRESCALE_2);
#ifdef INTERACTIVE
	registerInput(sizeof(list), list);
#endif
	Serial.println("Ready");
}

void loop() {
#ifdef INTERACTIVE
	handleInput();
#endif
	if (doLoop) {
		readColor();
		delay(500);
	}
}
