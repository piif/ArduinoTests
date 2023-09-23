#include <Arduino.h>

#ifdef TEST_SERIAL
	#ifndef DEFAULT_BAUDRATE
		#ifdef __AVR_ATmega644P__
			#define DEFAULT_BAUDRATE 19200
		#else
			#define DEFAULT_BAUDRATE 115200
		#endif
	#endif
#endif

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
	#define ARDUINO_TINY
#elif defined(__AVR_MEGA__)
	#define ARDUINO_UNO
#endif

// #define TEST_SERIAL
#ifdef ARDUINO_TINY
	#define TEST_TIMER
#endif

//TODO : comprendre de ou sort le CORE_DIR dans le makefile de arddude/target/sanguino

#if defined(ARDUINO_TINY)
	#define LED 4
	#warning "Tiny"
#elif defined(ARDUINO_RASPBERRY_PI_PICO)
	#define LED LED_BUILTIN
	#warning "Pico"
#else
	#define LED 13
	#warning "Arduino"
#endif

#define PERIOD 250

void setup(void) {
#ifdef TEST_SERIAL
	Serial.begin(DEFAULT_BAUDRATE);
#endif

	pinMode(LED, OUTPUT);
	pinMode(1, OUTPUT);

#ifdef TEST_TIMER
	// set Clock to 1MHz instead of 8
	cli(); CLKPR=0x80 ; CLKPR=0x03; sei();

    TCCR1 = (1 << CTC1) | (1 << PWM1A) | (1 << COM1A0) | 0xF; // reset on OCR1A match | prescale 16384
    OCR1A = 30; // interrupt before counter reset
    OCR1C = 61; // -> ~1s
    GTCCR = 0;
    TIMSK = 1 << OCIE1A;
#endif

	digitalWrite(LED, LOW);
#ifdef TEST_SERIAL
	Serial.println("ready.");
#endif
}

volatile bool clockTick = 0;

#ifdef TEST_TIMER
ISR(TIMER1_COMPA_vect) {
    clockTick = 1;
}
#endif

int count = 0;
void loop() {
#ifdef TEST_TIMER
	static bool led = 1;
	if (clockTick == 1) {
		led = !led;
		digitalWrite(LED, led);
		clockTick = 0;
	}
#else
	delay(PERIOD);
	digitalWrite(LED, HIGH);
#ifdef TEST_SERIAL
	Serial.print("count = ");Serial.println(count++);
#endif
	delay(PERIOD*3);
	digitalWrite(LED, LOW);
#endif
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
