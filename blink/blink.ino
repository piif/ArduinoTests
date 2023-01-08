#include <Arduino.h>

#ifndef DEFAULT_BAUDRATE
	#ifdef __AVR_ATmega644P__
		#define DEFAULT_BAUDRATE 19200
	#else
		#define DEFAULT_BAUDRATE 115200
	#endif
#endif


//TODO : comprendre de ou sort le CORE_DIR dans le makefile de arddude/target/sanguino

#if defined __AVR_ATtinyX5__ || defined __AVR_ATmega644P__
#define LED 4
#else
#define LED 13
#endif

#define PERIOD 250

void setup(void) {
#if !defined __AVR_ATtinyX5__
	Serial.begin(DEFAULT_BAUDRATE);
#endif

	pinMode(LED, OUTPUT);

	digitalWrite(LED, LOW);
}

int count = 0;
void loop() {
	delay(PERIOD);
	digitalWrite(LED, HIGH);
//	Serial.print("count = ");Serial.println(count++);
	delay(PERIOD*3);
	digitalWrite(LED, LOW);
}

/*
TODO : revoir la ligne de commande depuis l'IDE Pour "Arduino as ISP"
et voir comment l'intégrer dans ArdDude

./opt/arduino-1.6.5-r5/hardware/tools/avr/bin/avrdude
  -C/opt/arduino-1.6.5-r5/hardware/tools/avr/etc/avrdude.conf
  -v
  -pattiny85
  -cstk500v1
  -P/dev/ttyACM0
  -b19200
  -Uflash:w:/tmp/build2267963526209929137.tmp/Blink.cpp.hex:i

*/
