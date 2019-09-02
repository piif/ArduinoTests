#include <Arduino.h>
#include "setInterval.h"

// setInterval(callback)
// callBack => case 1/2/3 , set A,B,C + other
// use a global 6bits mask
// main or another setInterval to play changes
// test speed limits ...

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

#define LED_1  0x20
#define LED_2  0x10
#define LED_3  0x08
#define LED_1b 0x04
#define LED_2b 0x02
#define LED_3b 0x01

#define OUT_A 0x08
#define OUT_B 0x10
#define OUT_C 0x20

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

//	if (leds == 1) {
//		leds = 0x20;
//	} else {
//		leds >>= 1;
//	}
}

void displayStep(void *userData, long late, int missed) {
	static byte step  = 0;
	byte portd = PORTD & 0xC7, ddrd = DDRD  & 0xC7; // clear bits 3, 4 & 5 to copy to real PORTD and DDRD
	char A,B,C;

	switch(step) {
	case 0:
		// A = '1';
		portd |= OUT_A; ddrd |= OUT_A;
		// B = (leds & 0x20) ? '0' : 'z';
		if (leds & LED_1) {
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
		if (leds & LED_2) {
			ddrd |= OUT_C;
		}
	break;
	case 2:
		// A = (leds & 0x08) ? '0' : 'z';
		if (leds & LED_3) {
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
//	parseBit(A,3);
//	parseBit(B,4);
//	parseBit(C,5);

	step = (step + 1) % 3;
}

void setup() {
	Serial.begin(115200);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);

	displayTimer = setInterval(7, displayStep, NULL);
	animationTimer = setInterval(250, animationStep, NULL);

	Serial.println("ready");
}

int parseInt(char *buffer) {
	int result = 0;
	for(char *c = buffer; *c; c++) {
		result = result * 10 + *c - '0';
	}
	return result;
}

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
	parseBit(buffer[0], 3);
	parseBit(buffer[1], 4);
	parseBit(buffer[2], 5);
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
			return parseInt(buffer);
		}
	} else if (index == MAX_BUFFER - 2) {
		buffer[index + 1] = '\0';
		index = 0;
		return parseMask(buffer);
	} else {
		index++;
	}

}

int readInt() {
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
			return parseInt(buffer);
		}
	} else if (index == MAX_BUFFER - 2) {
		buffer[index + 1] = '\0';
		index = 0;
		return parseInt(buffer);
	} else {
		index++;
	}
	return -1;
}

void loop() {
	readMask();
	setIntervalStep();
}
