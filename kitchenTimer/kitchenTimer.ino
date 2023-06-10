#include <Arduino.h>
#include <avr/sleep.h>

#ifdef ARDUINO_AVR_UNO
#define USE_SLEEP_MODE
#define USE_13
#define HAVE_SERIAL
// #define HAVE_SPEAKER
#endif

#ifdef ARDUINO_AVR_NANO
#define USE_SLEEP_MODE
#define USE_13
#define HAVE_SPEAKER
// #define TEST_SPEAKER

// for debug purpose (my target nano serial port is out of order)
// #define HAVE_SERIAL
#endif

/*
à vide, en 3V (2 piles AA), 7mA
dès qu'on appuie sur un bouton, l'écran s'allume, on passe à 15-20mA, mais ça reboote
=> à tester avec 3 piles

tests avec une alim 5V externe , et le haut parleur sur la patte 3V2 à travers un transistor
- en veille : 15mA
- pendant le décompte : 50 à 70mA
- pendant le bip : 150mA
après activation du sleep mode + désoudage (arrachage ...) de la puce USB et du régulateur de tension
- en veille : 19µA
- pendant le décompte : 36mA
- pendant le bip : 150mA
*/

/* segment selection pins (-) */
#define S_A  6 // PD6
#define S_B  7 // PD7
#define S_C 10 // PB2
#define S_D 11 // PB3
#define S_E 12 // PB4
#define S_F  8 // PB0
#define S_G  9 // PB1
#define S_P  2 // PD1

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
#define D2  3 // PD2

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

#define BUTTON_DOWN    A0
#define BUTTON_UP      A1

#define SPEAKER A3

// set speaker port on/off
#define SPEAKER_ON()   PORTC |=  (1 << 3)
#define SPEAKER_OFF()  PORTC &= ~(1 << 3)

// special display combinations
#define DISPLAY_OFF    -1
#define DISPLAY_DOTS   -2
#define DISPLAY_ALL_ON -3
#define TIMER_MAX 599 // = 5'59"

// current status
enum {
    OFF,
    RUNNING,
    RINGING,
    STOPPING
} status = OFF;

#define TIMER_STEP 5 // 30

// value currently displayed
// current display is :
// -1 for display off
// -2 for display dots only
// -3 for display all segments
// a value between 0 and 599, then timer is running = the time remaining between 0 and 9'59"
int display = DISPLAY_OFF;

byte displaySegments[] = { 0, 0, 0 };
byte displayPortB[] = { 0, 0, 0 };
byte displayPortD[] = { 0, 0, 0 };

// next millis value look for (display change, button debounce, bip, ..)
unsigned long timerNext=0;

// count ring periods
byte ring = 0;
#define RING_LENGTH 500

// timestamp when click sound must stop
unsigned long clicking=0;

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

void (*resetFunc) (void) = 0; // declare reset function at address 0

void inline soundOn() {
    TIMSK2 |= 2; // enable OCIE2A = TIMER2_COMPA
}

void inline soundOff() {
    TIMSK2 &= ~2; // disable OCIE2A = TIMER2_COMPA
}

volatile unsigned long myMillis=0;

ISR(TIMER2_OVF_vect) {
    myMillis++;
    SPEAKER_OFF();
}

ISR(TIMER2_COMPA_vect) {
    SPEAKER_ON();
}

// change current display by a value according to current value :
// +/-5 under 15", +/-15 under 1', +/-30" after
// if upward is thru, add value, substract otherwise
int changeDisplay(bool upward) {
    int delta = 30;
    if (display <= 10) {
        delta = 5;
    } else if (display <= 45) {
        delta = 15;
    }
    int newValue = display;
    if (upward) {
        if (display <=0) {
            newValue = delta;
        } else if (display+delta < TIMER_MAX) {
            newValue += delta;
        }
    } else {
        if (display <= delta) {
            newValue = 0;
        } else {
            newValue -= delta;
        }
    }
    setDisplay(newValue);
    return newValue;
}

// set current display to given value
// then updates displayPortB/D arrays for current display
void setDisplay(int value) {
    display = value;
    byte segments[] = { 0, 0, 0 };

    // TODO : precompute B & D instead of segments
    if (value == DISPLAY_OFF) {
        segments[0] = 0;
        segments[1] = 0;
        segments[2] = 0;
    } else if (value == DISPLAY_DOTS) {
        segments[0] = mapSegments[10];
        segments[1] = mapSegments[10];
        segments[2] = mapSegments[10];
    } else if (value == DISPLAY_ALL_ON) { 
        segments[0] = 0xFF;
        segments[1] = 0xFF;
        segments[2] = 0xFF;
    } else {
        if (display >= 60) {
            segments[0] = mapSegments[(display / 60)] | mapSegments[10]; // minutes + 'dot'
        }
        if (display >= 10) {
            segments[1] = mapSegments[(display % 60) / 10];
        }
        segments[2] = mapSegments[display % 10];
    }
    prepareDigit(0, segments[0]);
    prepareDigit(1, segments[1]);
    prepareDigit(2, segments[2]);
}

// set displayPortB/D arrays for one digit, for given segments
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

// outputs one digit
void outputDigit(byte digit) {
    PORTB = (PORTB & B_OTHER) | displayPortB[digit];
    PORTD = (PORTD & D_OTHER) | displayPortD[digit];
}

// each call to this function cycles between 3 digits, and output one of them
void updateDisplay() {
    if (status == OFF || display == DISPLAY_OFF) {
        // PORTB &= ~B_DIGITS; useless since B_DIGITS==0
        PORTD &= ~D_DIGITS;
        return;
    }

    static byte digit = 0;
    outputDigit(digit);
    digit = (digit + 1) % 3;
}

// each call to this function make beep on/off untl 'ring' is 0
void updateRing(unsigned long now) {
    if (ring == 0) {
        soundOff();
        status = STOPPING;
        timerNext = now + 2500;
        setDisplay(DISPLAY_DOTS);       
    } else {
        if (ring & 1) {
            soundOn();
            setDisplay(DISPLAY_ALL_ON);
        } else {
            soundOff();
            setDisplay(DISPLAY_OFF);
        }
        timerNext = now + RING_LENGTH;
        ring--;
    }
}

// read button state, debounce it, and deduce action from current status and buttons state
void updateButtons(unsigned long now) {
    boolean mustClick = 0;
    if (buttonLastChanged==0 || now>buttonLastChanged+BounceDelay) {
        buttonLastChanged = now;
        byte newBDown = digitalRead(BUTTON_DOWN);
        byte newBUp = digitalRead(BUTTON_UP);
        if (newBUp != BUp) {
            BUp = newBUp;
            if (BUp==1) {
                // button UP pressed
                mustClick=1;
                if (status == RUNNING) {
                    changeDisplay(true);
                } else if (status == OFF || status == STOPPING) {
                    changeDisplay(true);
                    status = RUNNING;
#ifdef USE_13
                    digitalWrite(13, HIGH);
#endif
#ifdef HAVE_SERIAL
                    Serial.println("RUNNING");
#endif
                    timerNext = now + 1000;
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
                mustClick=1;
                if (status == RUNNING) {
                    if (changeDisplay(false) == 0) {
                        status = STOPPING;
                        timerNext = now + 2500;
                    }
                } else if (status == RINGING) {
                    ring = 0;
                    updateRing(now);
                }
            }
        }
        if (mustClick) {
            soundOn();
            clicking = now+5;
        }
    }
}

ISR(PCINT1_vect) {
    // do nothing, but interrupt will awake from deep sleep mode
}
// ISR(WDT_vect) {}

#ifdef USE_SLEEP_MODE
void sleepMode() {
    // disable unused stuffs
    ADCSRA &= ~_BV(ADEN); // turn off ADC
	ACSR |= _BV(ACD);     // disable the analog comparator
	MCUCR |= _BV(BODS) | _BV(BODSE) | _BV(PUD); // turn off the brown-out detector and pullup
#ifdef HAVE_SERIAL
    // turn off timers, TWI/SPI, Serial, analog converter
    PRR = _BV(PRTWI) | _BV(PRSPI) | _BV(PRADC);
#else
    PRR = _BV(PRTIM0) | _BV(PRTIM1) | _BV(PRTIM2) | _BV(PRTWI) | _BV(PRSPI) | _BV(PRUSART0) | _BV(PRADC);
#endif

    // listen Pin change on 9 (A1)
    PCMSK0 = 0;
    PCMSK1 = _BV(PCINT9);
    PCMSK2 = 0;
    PCICR = _BV(PCIE1); // listen for PCINT[14:8]

#ifdef USE_13
	digitalWrite(13, LOW);
#endif
#ifdef HAVE_SERIAL
    Serial.println("before sleep");Serial.println(PCI1count);
    delay(500);
#endif
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();

    // we woke up because of button press, thus assert it's still pressed
    // BUp = 1;

#ifdef USE_13
	digitalWrite(13, HIGH);
#endif
#ifdef HAVE_SERIAL
    Serial.println("wake up");Serial.println(PCI1count);
#endif

    // disable PCINT interrupt
    PCICR = 0;
    // wake up timer 2 (the only one used by code)
	PRR &= ~_BV(PRTIM2);
}
#endif // USE_SLEEP_MODE

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

	pinMode(BUTTON_UP,   INPUT);
	pinMode(BUTTON_DOWN, INPUT);

	pinMode(SPEAKER, OUTPUT);

/* frequency is 16MHz
    clock select = 4 => prescale 64 , thus 250KHz = 4µs per tick
    Mode 5 = Phase Correct PWM, toggle output COM2B half way

    OCR2A  = 125 => 0->0CR2A then OCR2A->0 -> 2*500µs = 1ms
    OCR2B  = 63  => toggle output half way
    OCIE2B = 1   => to raise interrupt at Bottom => every 1ms
    COM2A  = 2   => toggle every 500ms -> frequency 1000Hz
*/
    OCR2A  = 125;
    OCR2B  = 63;
    TCCR2A = 0b00000001; // 0b00100001; // COM1A = 0 , COM1B = 10 , WGM x01 / 0b00000001 => OC1A off 
    TCCR2B = 0b00001100; // WGM 1xx , Clock Select 100
    TIMSK2 = 0b00000001; // TOIE2 , 0b0011 = TOIE2 + OCIE2A when beeping

#ifdef HAVE_SERIAL
	// Serial.print("Lfuse "); Serial.println(boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS), HEX);
	// Serial.print("Hfuse "); Serial.println(boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS), HEX);
	// Serial.print("Efuse "); Serial.println(boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS), HEX);
    Serial.println("Setup OK");
    delay(500);
#endif

#ifdef USE_13
	pinMode(13, OUTPUT);
	// digitalWrite(13, HIGH);
    // delay(1000);
	// digitalWrite(13, LOW);
    // delay(1000);
#endif

#ifdef USE_SLEEP_MODE
    sleepMode();
#endif
}

#ifdef TEST_SPEAKER
void loop() {
    static byte BUp = 0;
    byte newBUp = digitalRead(BUTTON_UP);
    if (BUp != newBUp) {
        BUp = newBUp;
        if (BUp) {
            soundOn();
#ifdef HAVE_SERIAL
            Serial.print("myMillis "); Serial.println(myMillis);
            Serial.print("TCCR2A "); Serial.println(TCCR2A, HEX);
            Serial.print("TCCR2B "); Serial.println(TCCR2B, HEX);
            Serial.print("PRR    "); Serial.println(PRR   , HEX);
            Serial.print("ASSR   "); Serial.println(ASSR  , HEX);
            Serial.print("TIMSK2 "); Serial.println(TIMSK2, HEX);
            Serial.print("OCR2A  "); Serial.println(OCR2A);
            Serial.print("OCR2B  "); Serial.println(OCR2B);
#endif
        } else {
            soundOff();
        }
    }
}
#else // TEST_SPEAKER

// TODO : drive thru timer interrupts any 10 millis ?
void loop() {
    unsigned long now = myMillis;
    if (clicking != 0 && now >= clicking) {
        soundOff();
        clicking = 0;
    }
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
            setDisplay(DISPLAY_OFF);
            timerNext = 0;
            status = OFF;
#ifdef USE_13
            digitalWrite(13, LOW);
#endif
#ifdef HAVE_SERIAL
            Serial.println("OFF");
#endif
       }
    }
    updateButtons(now);
    updateDisplay();
#ifdef USE_SLEEP_MODE
    if (status == OFF) {
        resetFunc();
    }
#endif
}
#endif // else TEST_SPEAKER