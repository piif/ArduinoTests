#include <Arduino.h>
#include "setInterval.h"

// Essayer avec 6 valeurs sur 4 bits pour gérer un pseudo PWM
// une boucle à 1ms qui compte de 0 à 15 et pour chaque valeur on passe à 0 ou 1 selon qu'on a passé la valeur
// + une fonction callback d'animation appelée Pendant les 16 tours, pour préparer les valeurs de l'itération suivante
// et qui reçoit un compteur en paramètre
// => il faut driver la boucle rapide par des interruptions pour que la callback puisse se dérouler entre les itérations


char *LedMaskToOutputMask[] = {
	"10z",
	"z10",
	"0z1",
	"01z",
	"z01",
	"1z0"
};

setIntervalTimer animationTimer, displayTimer;

// led   1    2    3    1'   2'   3'
// mask 0x20 0x10 0x08 0x04 0x02 0x01
byte leds = 0;

#define LED_1a 0x20
#define LED_1b 0x10
#define LED_2a 0x08
#define LED_2b 0x04
#define LED_3a 0x02
#define LED_3b 0x01

#define OUT_A 0x04
#define OUT_B 0x08
#define OUT_C 0x10
#define OUT_NOT (~(OUT_A | OUT_B | OUT_C))

long iter=0;

void animationStep(void *userData, long late, int missed) {
	static byte bit = 1;
	static bool on = 1;

	if (on) {
		leds |= bit;
	} else {
		leds &= ~bit;
	}

	if (bit == 0x20) {
		bit = 1;
		on = !on;
	} else {
		bit <<= 1;
	}

	Serial.println(iter);
//	if (leds == 1) {
//		leds = 0x20;
//	} else {
//		leds >>= 1;
//	}
}

void displayStep(void *userData, long late, int missed) {
	static byte step  = 0;
	byte portd = PORTD & OUT_NOT, ddrd = DDRD & OUT_NOT; // clear bits 3, 4 & 5 to copy to real PORTD and DDRD
	char A,B,C;

	iter++;

	switch(step) {
	case 0:
		// A = '1';
		portd |= OUT_A; ddrd |= OUT_A;
		// B = (leds & 0x20) ? '0' : 'z';
		if (leds & LED_1a) {
			ddrd |= OUT_B;
		} // else ddrd already cleared
		// C = (leds & 0x01) ? '0' : 'z';
		if (leds & LED_3b) {
			ddrd |= OUT_C;
		}
	break;
	case 1:
		// A = (leds & 0x04) ? '0' : 'z';
		if (leds & LED_1b) {
			ddrd |= OUT_A;
		}
		// B = '1';
		portd |= OUT_B; ddrd |= OUT_B;
		// C = (leds & 0x10) ? '0' : 'z';
		if (leds & LED_2a) {
			ddrd |= OUT_C;
		}
	break;
	case 2:
		// A = (leds & 0x08) ? '0' : 'z';
		if (leds & LED_3a) {
			ddrd |= OUT_A;
		}
		// B = (leds & 0x02) ? '0' : 'z';
		if (leds & LED_2b) {
			ddrd |= OUT_B;
		}
		// C = '1';
		portd |= OUT_C; ddrd |= OUT_C;
	break;
	}
	PORTD = portd;
	DDRD  = ddrd;

	step = (step + 1) % 3;
}

// This part of code is used to get input like "10z" and set pins to
// associated states

#define MAX_BUFFER 4
char buffer[MAX_BUFFER];
int index = 0;
bool anim = true;

void parseBit(char state, byte pin) {
	switch(state) {
	case '0':
	case '-':
		pinMode(pin, OUTPUT);
		digitalWrite(pin, 0);
		pinMode(pin + 3, OUTPUT);
		digitalWrite(pin + 3, 0);
	break;
	case '1':
	case '+':
		pinMode(pin, OUTPUT);
		digitalWrite(pin, 1);
		pinMode(pin + 3, OUTPUT);
		digitalWrite(pin + 3, 1);
	break;
	default:
		pinMode(pin, INPUT);
		pinMode(pin + 3, INPUT);
	break;
	}
}

void parseMask(char *buffer) {
	if (buffer[0]=='?') {
		Serial.println("ping...");
		return;
	} else if (buffer[0]=='a') {
		anim = !anim;
		return;
	}

	parseBit(buffer[0], 2);
	parseBit(buffer[1], 3);
	parseBit(buffer[2], 4);
}

void readMask() {
	if (!Serial.available()) {
		return -1;
	}
	buffer[index] = Serial.read();
	if (buffer[index] == '\n' || buffer[index] == '\r') {
		if (index == 0) {
			return -1;
		} else {
			buffer[index] = '\0';
			index = 0;
			parseMask(buffer);
		}
	} else if (index == MAX_BUFFER - 2) {
		buffer[index + 1] = '\0';
		index = 0;
		parseMask(buffer);
	} else {
		index++;
	}

}

void setup() {
	Serial.begin(115200);

	displayTimer = setInterval(2, displayStep, NULL);
	animationTimer = setInterval(500, animationStep, NULL);

	Serial.println("ready");
}

void loop() {
	readMask();
	if (anim) {
		setIntervalStep();
	}
}
