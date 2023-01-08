#include <Arduino.h>

/*
TODO :
voir https://github.com/piif/ArduinoTests/blob/master/bell.ino pour sleep mode et interruptions
- interruption sur bouton UP
- sleep mode
- interruption réveille tout et fait timerValue=30
- on décompte et +/- fonds +30 / -30
- zéro => bip, puis mise en veille

ensuite :
- revoir cablage pour le nano ? : les pattes D0 à D12 sont alignées => on peut mettre le 7 segments directement en face
  voire le souder dessus
- est-ce qu'on arrive à dialoguer avec ? via les pattes rx/tx ?
  sinon, revoir https://docs.google.com/document/d/1PdzK6jrWJHqXbHkuemFGcNKQm6GtFs1EnQDyhPDC-Q4/edit : c'est peut être récupérable en fait ?

- mettre sur pile et check la conso (si le serial est mort, peut être qu'il consomme plus rien ...)
  sinon, désouder sa patte + , et la led power ?
*/

/* segment selection pins (-) */
#define Sa  2
#define Sb  3
#define Sc  4
#define Sd  5
#define Se  6
#define Sf  7
#define Sg  8
#define Sp 13

/* digit selection pins (+) */
#define D0  9
#define D1 10
#define D2 11
#define D3 12

#define ButtonUp   A0
#define ButtonDown A1

#define Speaker    A5

byte mapSegments[] = {
	//tRrblLmp
	0b11111100, // 0
	0b01100000, // 1
	0b11011010, // 2
	0b11110010, // 3
	0b01100110, // 4
	0b10110110, // 5
	0b10111110, // 6
	0b11100000, // 7
	0b11111110, // 8
	0b11110110, // 9
    0, 0, 0, 0, 0, 0
};

void outputDigit(byte value, byte digit, boolean point) {
    byte map = mapSegments[value];
    if (point) {
        map |= 1;
    }

    digitalWrite(D0, 0);
    digitalWrite(D1, 0);
    digitalWrite(D2, 0);
    digitalWrite(D3, 0);
    digitalWrite(Sa, !(map & 0x80));
    digitalWrite(Sb, !(map & 0x40));
    digitalWrite(Sc, !(map & 0x20));
    digitalWrite(Sd, !(map & 0x10));
    digitalWrite(Se, !(map & 0x08));
    digitalWrite(Sf, !(map & 0x04));
    digitalWrite(Sg, !(map & 0x02));
    digitalWrite(Sp, !(map & 0x01));

    switch(digit) {
        case 0: digitalWrite(D0, 1);
        break;
        case 1: digitalWrite(D1, 1);
        break;
        case 2: digitalWrite(D2, 1);
        break;
        case 3: digitalWrite(D3, 1);
        break;
    }
}

int display=1;

void updateDisplay(unsigned long now) {
    static byte digit = 0;
    byte value;
    switch(digit) {
        case 3: value=display % 10;
        break;
        case 2: value=(display/10) % 10;
        break;
        case 1: value=(display/100) % 10;
        break;
        case 0: value=(display/1000) % 10;
        break;
    }
    outputDigit(value, digit, 0);
    digit = (digit + 1) & 3;
}

byte BDown = 0, BUp = 0;
unsigned long buttonLastChanged=0;
#define BounceDelay 100

void updateButtons(unsigned long now) {
    boolean mustBeep=0;
    if (buttonLastChanged==0 || now>buttonLastChanged+BounceDelay) {
        byte newBDown = !digitalRead(ButtonDown);
        byte newBUp = !digitalRead(ButtonUp);
        if (newBDown != BDown) {
            BDown = newBDown;
            if (BDown==1) {
                mustBeep=1;
                display--;
            }
        }
        if (newBUp != BUp) {
            BUp = newBUp;
            if (BUp==1) {
                mustBeep=1;
                display++;
            }
        }
        if (mustBeep) {
            tone(Speaker, 880, 5);
        }
    }
}

unsigned long tic;

void setup() {
	pinMode(D0, OUTPUT);
	pinMode(D1, OUTPUT);
	pinMode(D2, OUTPUT);
	pinMode(D3, OUTPUT);

	pinMode(Sa, OUTPUT);
	pinMode(Sb, OUTPUT);
	pinMode(Sc, OUTPUT);
	pinMode(Sd, OUTPUT);
	pinMode(Se, OUTPUT);
	pinMode(Sf, OUTPUT);
	pinMode(Sg, OUTPUT);
	pinMode(Sp, OUTPUT);

	pinMode(ButtonDown, INPUT_PULLUP);
	pinMode(ButtonUp,   INPUT_PULLUP);

	pinMode(Speaker, OUTPUT);

    tic= millis();
}


void loop() {
    // outputDigit(8, 1, 1);
    unsigned long tac = millis();
    updateButtons(tac);
    updateDisplay(tac);
    // if (tac > tic + 500) {
    //     tic= tac;
    //     display++;
    //     if (display > 9999) {
    //         display = 1;
    //     }
    // }
}