#include <Arduino.h>
// #include <avr/boot.h>

// for debug purpose (my target nano serial port is out of order)
#define HAVE_SERIAL

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
#define S_A  6 // PD6
#define S_B  7 // PD7
#define S_C 10 // PB2
#define S_D 11 // PB3
#define S_E 12 // PB4
#define S_F  8 // PB0
#define S_G  9 // PB1
#define S_P  2 // PD2

// set segment bit to 0
#define MASK_A(Bbits, DBits)  Dbits &= ~(1 << 6)
#define MASK_B(Bbits, DBits)  Dbits &= ~(1 << 7)
#define MASK_C(Bbits, DBits)  Bbits &= ~(1 << 2)
#define MASK_D(Bbits, DBits)  Bbits &= ~(1 << 3)
#define MASK_E(Bbits, DBits)  Bbits &= ~(1 << 4)
#define MASK_F(Bbits, DBits)  Bbits &= ~(1 << 0)
#define MASK_G(Bbits, DBits)  Bbits &= ~(1 << 1)
#define MASK_P(Bbits, DBits)  Dbits &= ~(1 << 2)

/* digit selection pins (+) */
#define D0  5 // PD5
#define D1  4 // PD4
#define D2  3 // PD3

// set digit bit to 1
#define MASK_D0(Bbits, DBits)  Dbits |= (1 << 5)
#define MASK_D1(Bbits, DBits)  Dbits |= (1 << 4)
#define MASK_D2(Bbits, DBits)  Dbits |= (1 << 3)

// digit bits
#define B_DIGITS 0x00
#define D_DIGITS 0x38

// segment bits
#define B_SEGMENTS 0x1F
#define D_SEGMENTS 0xC4

// other bits
#define B_OTHER 0xE0
#define D_OTHER 0x03

#define BUTTON_UP      A0
#define BUTTON_DOWN    A1

#define SPEAKER 13

// current status
enum {
    OFF,
    RUNNING,
    RINGING,
    STOPPING
} status = OFF;

#define TIMER_STEP 5 // 30

// value currently displayed (-1 = off)
int display = -1;
byte displaySegments[] = { 0, 0, 0 };
byte displayPortB[] = { 0, 0, 0 };
byte displayPortD[] = { 0, 0, 0 };

// next millis value for display change
unsigned long timerNext=0;

// count ring periods
byte ring = 0;
#define RING_LENGTH 500

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
    0b00000001, // 10 = '.'
    0, 0, 0, 0, 0 // 11..15 : nothing.
};

void setDisplay(int value) {
    display = value;
    byte segments[] = { 0, 0, 0 };

    // TODO : precompute B & D instead of segments
    if (value == -1) { // off
        segments[0] = 0;
        segments[1] = 0;
        segments[2] = 0;
    } else if (value == -2) { // dots only
        segments[0] = mapSegments[10];
        segments[1] = mapSegments[10];
        segments[2] = mapSegments[10];
    } else if (value == -3) { // all on
        segments[0] = 0xFF;
        segments[1] = 0xFF;
        segments[2] = 0xFF;
    } else {
        segments[0] = mapSegments[(display/100) % 10] | 1;
        segments[1] = mapSegments[(display/10) % 10];
        segments[2] = mapSegments[display % 10];
    }
    prepareDigit(0, segments[0]);
    prepareDigit(1, segments[1]);
    prepareDigit(2, segments[2]);
}

void prepareDigit(byte digit, byte segments) {
    byte Bbits = B_SEGMENTS, Dbits = (PORTD & D_OTHER) | D_SEGMENTS;

    if (segments & 0x80) MASK_A(Bbits, DBits);
    if (segments & 0x40) MASK_B(Bbits, DBits);
    if (segments & 0x20) MASK_C(Bbits, DBits);
    if (segments & 0x10) MASK_D(Bbits, DBits);
    if (segments & 0x08) MASK_E(Bbits, DBits);
    if (segments & 0x04) MASK_F(Bbits, DBits);
    if (segments & 0x02) MASK_G(Bbits, DBits);
    if (segments & 0x01) MASK_P(Bbits, DBits);

    switch(digit) {
        case 0: MASK_D0(Bbits, DBits);
        break;
        case 1: MASK_D1(Bbits, DBits);
        break;
        case 2: MASK_D2(Bbits, DBits);
        break;
    }
    displayPortB[digit] = Bbits;
    displayPortD[digit] = Dbits;
}

void outputDigit(byte digit) {
    PORTB = (PORTB & B_OTHER) | displayPortB[digit];
    PORTD = (PORTD & D_OTHER) | displayPortD[digit];
}

void updateDisplay() {
    if (status == OFF || display == -1) {
        // PORTB &= ~B_DIGITS; useless since B_DIGITS==0
        PORTD &= ~D_DIGITS;
        return;
    }

    static byte digit = 0;
    outputDigit(digit);
    digit = (digit + 1) % 3;
}

void updateRing(unsigned long now) {
    if (ring == 0) {
        noTone(SPEAKER);
        status = STOPPING;
        timerNext = now + 2500;
        setDisplay(-2);       
    } else {
        if (ring & 1) {
            tone(SPEAKER, 1000);
            setDisplay(-3);
        } else {
            noTone(SPEAKER);
            setDisplay(-1);
        }
        timerNext = now + RING_LENGTH;
        ring--;
    }
}

void updateButtons(unsigned long now) {
    boolean mustBeep=0;
    if (buttonLastChanged==0 || now>buttonLastChanged+BounceDelay) {
        buttonLastChanged = now;
        byte newBDown = !digitalRead(BUTTON_DOWN);
        byte newBUp = !digitalRead(BUTTON_UP);
        if (newBUp != BUp) {
            BUp = newBUp;
            if (BUp==1) {
                // button UP pressed
                mustBeep=1;
                if (status == RUNNING) {
                    if (display < 999 - TIMER_STEP) {
                        setDisplay(display + TIMER_STEP);
                    }
                } else if (status == OFF || status == STOPPING) {
                    setDisplay(TIMER_STEP);
                    status = RUNNING;
                    timerNext = millis() + 1000;
                } else if (status == RINGING) {
                    ring = 0;
                    updateRing(now);
                }
            }
        }
        if (newBDown != BDown) {
            BDown = newBDown;
            if (BDown==1) {
                // button DOWN pressed
                mustBeep=1;
                if (status == RUNNING) {
                    if (display > TIMER_STEP) {
                        setDisplay(display - TIMER_STEP);
                    } else if (display > 0) {
                        setDisplay(0);
                        status = STOPPING;
                        timerNext = now + 2500;
                    }
                } else if (status == RINGING) {
                    ring = 0;
                    updateRing(now);
                }
            }
        }
        if (mustBeep) {
            tone(SPEAKER, 880, 5);
        }
    }
}

void setup() {
#ifdef HAVE_SERIAL
    Serial.begin(115200);
#endif
	pinMode(D0, OUTPUT);
	pinMode(D1, OUTPUT);
	pinMode(D2, OUTPUT);

	pinMode(S_A, OUTPUT);
	pinMode(S_B, OUTPUT);
	pinMode(S_C, OUTPUT);
	pinMode(S_D, OUTPUT);
	pinMode(S_E, OUTPUT);
	pinMode(S_F, OUTPUT);
	pinMode(S_G, OUTPUT);
	pinMode(S_P, OUTPUT);

	pinMode(BUTTON_UP,   INPUT_PULLUP);
	pinMode(BUTTON_DOWN, INPUT_PULLUP);

	pinMode(SPEAKER, OUTPUT);
#ifdef HAVE_SERIAL
	// Serial.print("Lfuse "); Serial.println(boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS), HEX);
	// Serial.print("Hfuse "); Serial.println(boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS), HEX);
	// Serial.print("Efuse "); Serial.println(boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS), HEX);
    Serial.println("Setup OK");
#endif
}

// TODO : drive thru timer interrupts any 10 millis ?
void loop() {
    unsigned long now = millis();
    if (timerNext != 0 && now >= timerNext) {
#ifdef HAVE_SERIAL
    Serial.print("timerNext ");Serial.println(timerNext);
    Serial.print("status    ");Serial.println(status);
    if (status == RINGING) {
        Serial.print("ring      ");Serial.println(ring);
    }
    Serial.print("display   ");Serial.println(display);
#endif
        if (status == RUNNING) {
            setDisplay(display-1);
            if (display == 0) {
                ring = 5;
                status = RINGING;
                updateRing(now);
            } else {
                timerNext = now + 1000;
            }
        } else if (status == RINGING) {
            updateRing(now);
        } else if (status == STOPPING) {
            setDisplay(-1);
            timerNext = 0;
            // TODO : sleep mode
        }
    }
    updateButtons(now);
    updateDisplay();
}
