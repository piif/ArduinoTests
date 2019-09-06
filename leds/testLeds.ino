#include <Arduino.h>
#include "setInterval.h"

// Essayer avec 6 valeurs sur 4 bits pour gérer un pseudo PWM
// une boucle à 1ms qui compte de 0 à 15 et pour chaque valeur on passe à 0 ou 1 selon qu'on a passé la valeur
// + une fonction callback d'animation appelée Pendant les 16 tours, pour préparer les valeurs de l'itération suivante
// et qui reçoit un compteur en paramètre
// => il faut driver la boucle rapide par des interruptions pour que la callback puisse se dérouler entre les itérations


setIntervalTimer animationTimer, displayTimer;

#define MAX_INTENSITY 15
// each led intensity required by animation steps
// values must be between 0 and MAX_INTENSITY
byte leds[6] = { 0, };

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

long iter=0, step = 0;

typedef struct animation {
	setIntervalFunction *callback;
	long delay;
	long nbSteps;
} Animation;

void aStep(void *userData, long late, int missed) {
	static byte led = 0;
	static byte on = MAX_INTENSITY;

	leds[led] = on;

	if (led == 5) {
		led = 0;
		on = (on == MAX_INTENSITY) ? 0 : MAX_INTENSITY;
	} else {
		led++;
	}
	Serial.println(iter);
}

void bStep(void *userData, long late, int missed) {
	static byte led = 0;
	leds[led] = 0;
	if (led == 5) {
		led = 0;
	} else {
		led++;
	}
	leds[led] = MAX_INTENSITY;
}


Animation animations[] = {
	{ aStep, 250, 12 },
	{ bStep, 250, 6 }
};
int currentAnim = -1;
#define NB_ANIMATIONS (sizeof(animations) / sizeof(Animation))

void startAnim(int index) {
	currentAnim = index;
	if (index == -1) {
		Serial.println("Stop anim");
		changeInterval(animationTimer, SET_INTERVAL_PAUSED);
		return;
	}
	Serial.print("Start anim ");
	Serial.println(index);
	index = index % NB_ANIMATIONS;
	animationTimer = changeInterval(animationTimer, animations[index].delay, animations[index].callback, NULL);
}

void displayStep(void *userData, long late, int missed) {
	static byte step  = 0;
	byte portd = PORTD & OUT_NOT, ddrd = DDRD & OUT_NOT; // clear bits 3, 4 & 5 to copy to real PORTD and DDRD

	iter++;

	switch(step) {
	case 0:
		// A = '1';
		portd |= OUT_A; ddrd |= OUT_A;
		// B = leds[0] ? '0' : 'z';
		if (leds[0]) {
			ddrd |= OUT_B;
		} // else ddrd already cleared
		// C = leds[5] ? '0' : 'z';
		if (leds[5]) {
			ddrd |= OUT_C;
		}
		step = 1;
	break;
	case 1:
		// A = leds[1] ? '0' : 'z';
		if (leds[1]) {
			ddrd |= OUT_A;
		}
		// B = '1';
		portd |= OUT_B; ddrd |= OUT_B;
		// C = leds[2] ? '0' : 'z';
		if (leds[2]) {
			ddrd |= OUT_C;
		}
		step = 2;
	break;
	case 2:
		// A = leds[4] ? '0' : 'z';
		if (leds[4]) {
			ddrd |= OUT_A;
		}
		// B = leds[3] ? '0' : 'z';
		if (leds[3]) {
			ddrd |= OUT_B;
		}
		// C = '1';
		portd |= OUT_C; ddrd |= OUT_C;
		step = 0;
	break;
	}
	PORTD = portd;
	DDRD  = ddrd;
}

// This part of code is used to get input like "10z" and set pins to
// associated states

#define MAX_BUFFER 4
char buffer[MAX_BUFFER];
int index = 0;

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
	} else if (buffer[0]>='a' && buffer[0]<'z') {
		startAnim(buffer[0]-'a');
		return;
	} else {
		startAnim(-1);
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

	// initialize animation timer with empty data
	animationTimer = setInterval(SET_INTERVAL_PAUSED, NULL, NULL);
	displayTimer = setInterval(2, displayStep, NULL);
	startAnim(0);

	Serial.println("ready");
}

void loop() {
	readMask();
	if (currentAnim >= 0) {
		setIntervalStep();
	}
}
