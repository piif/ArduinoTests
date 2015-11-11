#include <Arduino.h>
#include "myMillis/myMillis.h"

void setup() {
	Serial.begin(115200);
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);

	initTimerMillis(0, 20, 8 /*255, 64*/);

	Serial.println("ready");
}

void loop() {
	Serial.print("clock_ticks ");Serial.println(clock_ticks);
	Serial.print("TCNT0 ");Serial.println(TCNT0);
	Serial.print("tic ");Serial.println(myMillis());
	myDelay(1000);
	digitalWrite(13, LOW);
	Serial.print("tac ");Serial.println(myMillis());
	myDelay(1000);
	digitalWrite(13, HIGH);
}
