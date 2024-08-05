#include <Arduino.h>

#ifndef DEFAULT_BAUDRATE
	#ifdef __AVR_ATmega644P__
		#define DEFAULT_BAUDRATE 19200
	#else
		#define DEFAULT_BAUDRATE 115200
	#endif
#endif

#define DATA_0  2
#define DATA_1  3
#define CARD    4
#define HOLD    5
#define LED    13

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(LED, OUTPUT);
	pinMode(DATA_0, INPUT);
	pinMode(DATA_1, INPUT);
	pinMode(CARD  , INPUT);
	pinMode(HOLD  , OUTPUT);

	digitalWrite(LED , LOW);
	digitalWrite(HOLD, LOW);
}

bool data0=1, data1=1;
bool card =1;

void loop() {
	bool d0 = digitalRead(DATA_0);
	bool d1 = digitalRead(DATA_1);
	bool c  = digitalRead(CARD);
	if (data0 != d0 || data1 != d1 || card != c) {
		data0=d0;
		data1=d1;
		card = c;
		Serial.print(data0); Serial.print(data1); Serial.print(card);
	} 
}
