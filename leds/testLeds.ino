#include <Arduino.h>
#include "setInterval.h"

// Essayer avec 6 valeurs sur 4 bits pour gérer un pseudo PWM
// une boucle à 1ms qui compte de 0 à 15 et pour chaque valeur on passe à 0 ou 1 selon qu'on a passé la valeur
// + une fonction callback d'animation appelée pendant les 16 tours, pour préparer les valeurs de l'itération suivante
// et qui reçoit un compteur en paramètre
// => il faut driver la boucle rapide par des interruptions pour que la callback puisse se dérouler entre les itérations

/**
 * à revoir en PWM, sur les sortie 9, 10, 11 = portb bits 1, 2 et 3
 *  9 = OCR1A
 * 10 = OCR1B
 * 11 = OCR2A
 * - prescaler à 1
 * - fast pwm jusque max pour timer 2, 256 pour timer 1 (16 bits)
 * - output sur 1A, 1B et 2A
 * COM1A1:0 = 11 , COM1B1:0 = 11 , WGM13:0 = 1010
 * TCCR1A = 1111xx00 , TCCR1B = 00x00001 , TCCR1C = 11xxxxxx
 *
 * COM2A1:0 = 11 , COM2B1:0 = 00 , WGM23:0 = 1010
 * TCCR2A = 1100xx00 , TCCR2B = 11xx0001
 *
 * TODO : tester les sorties PWM "en dur" avec une valeur donnée
 */


setIntervalTimer animationTimer, displayTimer;

// undef to use on/off mode , def to use variable intensity thru PWM
#define WITH_PWM

#define MAX_INTENSITY 255
// each led intensity required by animation steps
// values must be between 0 and MAX_INTENSITY
byte leds[6] = { 0, };

#define PIN_A 9
#define PIN_B 10
#define PIN_C 11

#define OUT_A 0x02
#define OUT_B 0x04
#define OUT_C 0x08

#define OCR_OUT_A OCR1A
#define OCR_OUT_B OCR1B
#define OCR_OUT_C OCR2A

#define OUT_NOT (~(OUT_A | OUT_B | OUT_C))

typedef struct animation {
	bool (*callback)(long, byte leds[]);
	long delay;
	long nbSteps;
} Animation;

void rotateRight() {
	byte tmp = leds[5];
	leds[5] = leds[4];
	leds[4] = leds[3];
	leds[3] = leds[2];
	leds[2] = leds[1];
	leds[1] = leds[0];
	leds[0] = tmp;
}

bool aStep(long step, byte leds[]) {
	static byte led = 0;

	if (step == 0) {
		leds[0] = MAX_INTENSITY;
		leds[1] = MAX_INTENSITY >> 3;
		leds[2] = 0;
		leds[3] = 0;
		leds[4] = 0;
		leds[5] = 0;
		return true;
	}
	rotateRight();

//	leds[led] = 0;
//	if (led == 5) {
//		led = 0;
//	} else {
//		led++;
//	}
//	leds[led] = MAX_INTENSITY;

	return true;
}

bool bStep(long step, byte leds[]) {
	static byte led = 0;
	static byte on = MAX_INTENSITY;

	if (step == 0) {
		leds[0] = 0;
		leds[1] = 0;
		leds[2] = 0;
		leds[3] = 0;
		leds[4] = 0;
		leds[5] = 0;
		return true;
	}

	leds[led] = on;

	if (led == 5) {
		led = 0;
		on = (on == MAX_INTENSITY) ? 0 : MAX_INTENSITY;
	} else {
		led++;
	}
	return true;
}

bool cStep(long step, byte leds[]) {
	if (step == 0) {
		leds[0] = 0;
		leds[1] = 0;
		leds[2] = 0;
		leds[3] = 0;
		leds[4] = 0;
		leds[5] = 0;
		return true;
	}

	leds[0]++;
	leds[1] = leds[0];
	leds[2] = leds[0];
	leds[3] = leds[0];
	leds[4] = leds[0];
	leds[5] = leds[0];

//	if (leds[0] == 0) {
//		Serial.println(millis());
//	}
	return true;
}

bool dStep(long step, byte leds[]) {
	if (step == 0) {
		leds[0] = 0;
		leds[1] = MAX_INTENSITY >> 4;
		leds[2] = MAX_INTENSITY >> 2;
		leds[3] = MAX_INTENSITY;
		leds[4] = MAX_INTENSITY >> 2;
		leds[5] = MAX_INTENSITY >> 4;
	} else {
		rotateRight();
	}
	return true;
}

bool eStep(long step, byte leds[]) {
	if (step == 0) {
		leds[0] = 0;
		leds[1] = 20;
		leds[2] = 40;
		leds[3] = 60;
		leds[4] = 80;
		leds[5] = 100;
		return true;
	}

	leds[0]++;
	leds[1]++;
	leds[2]++;
	leds[3]++;
	leds[4]++;
	leds[5]++;

//	if (leds[0] == 0) {
//		Serial.println(millis());
//	}
	return true;
}

Animation animations[] = {
	{ aStep, 250,   6 },
	{ bStep, 100,  12 },
	{ cStep,   5, 256 },
	{ dStep, 100,   6 },
	{ eStep,   5, 256 }
};
int currentAnim = -1;
#define NB_ANIMATIONS (sizeof(animations) / sizeof(Animation))

long animationStep = 0;
long displaySubStep = 0;

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
	animationStep = 0;
	displaySubStep = 0;
	animationTimer = changeInterval(animationTimer, animations[index].delay);
}


void animationStepCallback(void *userData, long late, int missed) {
	if (currentAnim >= 0) {
		(animations[currentAnim].callback)(animationStep, leds);
		animationStep++;
	}
}

void displayStepCallback_6(void *userData, long late, int missed) {
	byte port = PORTB & OUT_NOT, ddr = DDRB & OUT_NOT; // clear bits 3, 4 & 5 to copy to real PORTD and DDRD

	switch(displaySubStep) {
	case 0:
		// A = '1';
		port |= OUT_A; OCR_OUT_A = 0; ddr |= OUT_A;

		// B = leds[0] ? '0' : 'z';
		if (leds[0] > 0) {
			ddr |= OUT_B;
#ifdef WITH_PWM
			OCR_OUT_B = leds[0];
#endif
		} // else ddrd already cleared

		displaySubStep = 1;
	break;
	case 1:
		// A = '1';
		port |= OUT_A; OCR_OUT_A = 0; ddr |= OUT_A;

		// C = leds[5] ? '0' : 'z';
		if (leds[5] > 0) {
			ddr |= OUT_C;
#ifdef WITH_PWM
			OCR_OUT_C = leds[5];
#endif
		}
		displaySubStep = 2;
	break;
	case 2:
		// B = '1';
		port |= OUT_B; OCR_OUT_B = 0; ddr |= OUT_B;

		// A = leds[1] ? '0' : 'z';
		if (leds[1] > 0) {
			ddr |= OUT_A;
#ifdef WITH_PWM
			OCR_OUT_A = leds[1];
#endif
		}

		displaySubStep = 3;
	break;
	case 3:
		// B = '1';
		port |= OUT_B; OCR_OUT_B = 0; ddr |= OUT_B;

		// C = leds[2] ? '0' : 'z';
		if (leds[2] > 0) {
			ddr |= OUT_C;
#ifdef WITH_PWM
			OCR_OUT_C = leds[2];
#endif
		}
		displaySubStep = 4;
	break;
	case 4:
		// C = '1';
		port |= OUT_C; OCR_OUT_C = 0; ddr |= OUT_C;

		// A = leds[4] ? '0' : 'z';
		if (leds[4] > 0) {
			ddr |= OUT_A;
#ifdef WITH_PWM
			OCR_OUT_A = leds[4];
#endif
		}

		displaySubStep = 5;
	break;
	case 5:
		// C = '1';
		port |= OUT_C; OCR_OUT_C = 0; ddr |= OUT_C;

		// B = leds[3] ? '0' : 'z';
		if (leds[3] > 0) {
			ddr |= OUT_B;
#ifdef WITH_PWM
			OCR_OUT_B = leds[3];
#endif
		}

		displaySubStep = 0;
	break;
	}
	PORTB = port;
	DDRB  = ddr;
}

void displayStepCallback_3(void *userData, long late, int missed) {
	byte port = PORTB & OUT_NOT, ddr = DDRB & OUT_NOT; // clear bits 3, 4 & 5 to copy to real PORTD and DDRD

	switch(displaySubStep) {
	case 0:
		// A = '1';
		port |= OUT_A; OCR_OUT_A = 0; ddr |= OUT_A;

		// B = leds[0] ? '0' : 'z';
		if (leds[0] > 0) {
			ddr |= OUT_B;
#ifdef WITH_PWM
			OCR_OUT_B = leds[0];
#endif
		} // else ddrd already cleared

		// C = leds[5] ? '0' : 'z';
		if (leds[5] > 0) {
			ddr |= OUT_C;
#ifdef WITH_PWM
			OCR_OUT_C = leds[5];
#endif
		}
		displaySubStep = 1;
	break;
	case 1:
		// A = leds[1] ? '0' : 'z';
		if (leds[1] > 0) {
			ddr |= OUT_A;
#ifdef WITH_PWM
			OCR_OUT_A = leds[1];
#endif
		}

		// B = '1';
		port |= OUT_B; OCR_OUT_B = 0; ddr |= OUT_B;

		// C = leds[2] ? '0' : 'z';
		if (leds[2] > 0) {
			ddr |= OUT_C;
#ifdef WITH_PWM
			OCR_OUT_C = leds[2];
#endif
		}
		displaySubStep = 2;
	break;
	case 2:
		// A = leds[4] ? '0' : 'z';
		if (leds[4] > 0) {
			ddr |= OUT_A;
#ifdef WITH_PWM
			OCR_OUT_A = leds[4];
#endif
		}

		// B = leds[3] ? '0' : 'z';
		if (leds[3] > 0) {
			ddr |= OUT_B;
#ifdef WITH_PWM
			OCR_OUT_B = leds[3];
#endif
		}

		// C = '1';
		port |= OUT_C; OCR_OUT_C = 0; ddr |= OUT_C;

		displaySubStep = 0;
	break;
	}
	PORTB = port;
	DDRB  = ddr;
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
	break;
	case '1':
	case '+':
		pinMode(pin, OUTPUT);
		digitalWrite(pin, 1);
	break;
	default:
		pinMode(pin, INPUT);
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

	parseBit(buffer[0], PIN_A);
	parseBit(buffer[1], PIN_B);
	parseBit(buffer[2], PIN_C);
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
	animationTimer = setInterval(SET_INTERVAL_PAUSED, animationStepCallback, NULL);
	displayTimer = setInterval(2, displayStepCallback_6, NULL);

#ifdef WITH_PWM
	TCCR1A = 0xF1; TCCR1B = 0x09; TCCR1C = 0xC0;
	TCCR2A = 0xC3; TCCR2B = 0xC1;
//	OCR_OUT_A = 50;
//	OCR_OUT_B = 128;
//	OCR_OUT_C = 200;
//	DDRB |= OUT_A | OUT_B | OUT_C;
#endif
	startAnim(0);

	Serial.println("ready");
}

void loop() {
	readMask();
	if (currentAnim >= 0) {
		setIntervalStep();
	}
}
