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
#define Sa  6 // PD6
#define Sb  7 // PD7
#define Sc 10 // PB2
#define Sd 11 // PB3
#define Se 12 // PB4
#define Sf  8 // PB0
#define Sg  9 // PB1
#define Sp  2 // PD2

// set segment bit to 0
#define MASK_A  Dbits &= ~(1 << 6)
#define MASK_B  Dbits &= ~(1 << 7)
#define MASK_C  Bbits &= ~(1 << 2)
#define MASK_D  Bbits &= ~(1 << 3)
#define MASK_E  Bbits &= ~(1 << 4)
#define MASK_F  Bbits &= ~(1 << 0)
#define MASK_G  Bbits &= ~(1 << 1)
#define MASK_P  Dbits &= ~(1 << 2)

/* digit selection pins (+) */
#define D0  5 // PD5
#define D1  4 // PD4
#define D2  3 // PD3

// set digit bit to 1
#define MASK_D0  Dbits |= (1 << 5)
#define MASK_D1  Dbits |= (1 << 4)
#define MASK_D2  Dbits |= (1 << 3)

// digit bits
#define B_DIGITS 0x00
#define D_DIGITS 0x38

// segment bits
#define B_SEGMENTS 0x1F
#define D_SEGMENTS 0xC4

// other bits
#define B_OTHER 0xE0
#define D_OTHER 0x03

#define ButtonUp      A0
#define ButtonDown    A1

#define Speaker    13

// current status
enum {
    OFF,
    RUNNING,
    RINGING,
    STOPPING
} status = OFF;

// value currently displayed (-1 = off)
int display = -1;
byte displayDigits[] = { 0, 0, 0 };

// next millis value for display change
unsigned long timerNext=0;

// buttons state
byte BDown = 0, BUp = 0;
// last time buttons state changed
unsigned long buttonLastChanged=0;
// button bounce timeout
#define BounceDelay 100

byte mapSegments[] = {
	//abcdefgp
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

void setDisplay(int value) {
    display = value;
    if (value == -1) {
        return;
    }
    displayDigits[2] = display % 10;
    displayDigits[1] = (display/10) % 10;
    displayDigits[0] = (display/100) % 10;
}

void outputDigit(byte value, byte digit, boolean point) {
    byte map = mapSegments[value];
    if (point) {
        map |= 1;
    }

    byte Bbits = (PORTB & B_OTHER) | B_SEGMENTS, Dbits = (PORTD & D_OTHER) | D_SEGMENTS;

    if (map & 0x80) MASK_A;
    if (map & 0x40) MASK_B;
    if (map & 0x20) MASK_C;
    if (map & 0x10) MASK_D;
    if (map & 0x08) MASK_E;
    if (map & 0x04) MASK_F;
    if (map & 0x02) MASK_G;
    if (map & 0x01) MASK_P;

    switch(digit) {
        case 0: MASK_D0;
        break;
        case 1: MASK_D1;
        break;
        case 2: MASK_D2;
        break;
    }
    PORTB = Bbits; PORTD = Dbits;
}

void updateDisplay(unsigned long now) {
    if (status == OFF) {
        // PORTB &= ~B_DIGITS; useless since B_DIGITS==0
        PORTD &= ~D_DIGITS;
        return;
    }

    static byte digit = 0;
    outputDigit(displayDigits[digit], digit, digit == 0);
    digit = (digit + 1) % 3;
}

void updateButtons(unsigned long now) {
    boolean mustBeep=0;
    if (buttonLastChanged==0 || now>buttonLastChanged+BounceDelay) {
        buttonLastChanged = now;
        byte newBDown = !digitalRead(ButtonDown);
        byte newBUp = !digitalRead(ButtonUp);
        if (newBDown != BDown) {
            BDown = newBDown;
            if (BDown==1) {
                mustBeep=1;
                if (display == 0) {
                    setDisplay(999);
                } else {
                    setDisplay(display-1);
                }
            }
        }
        if (newBUp != BUp) {
            BUp = newBUp;
            if (BUp==1) {
                mustBeep=1;
                if (display == 999) {
                    setDisplay(0);
                } else {
                    setDisplay(display+1);
                }
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

	pinMode(Sa, OUTPUT);
	pinMode(Sb, OUTPUT);
	pinMode(Sc, OUTPUT);
	pinMode(Sd, OUTPUT);
	pinMode(Se, OUTPUT);
	pinMode(Sf, OUTPUT);
	pinMode(Sg, OUTPUT);
	pinMode(Sp, OUTPUT);

	pinMode(ButtonDown,    INPUT_PULLUP);
	pinMode(ButtonUp,      INPUT_PULLUP);

	pinMode(Speaker, OUTPUT);

    tic= millis();

    status = RUNNING;
    setDisplay(0);
}

// TODO : drive thru timer interrupts any 10 millis ?
void loop() {
    // outputDigit(8, 1, 1);
    unsigned long tac = millis();
    updateButtons(tac);
    updateDisplay(tac);
}