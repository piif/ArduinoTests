#include <Arduino.h>
#include <serialInput.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define M_X_EN  3
#define M_X_A  5
#define M_X_B  6
#define M_X_SPEED 250
#define X_MAX 5000

#define M_Y_EN 11
#define M_Y_A  9
#define M_Y_B 10
#define M_Y_SPEED 200
#define Y_MAX 100000
#define Y_MARGIN 8000 // distance between paper entry and pen

#define FORK_X_A A0
#define FORK_X_B A1

#define FORK_Y_A A2
#define FORK_Y_B A3

#define FORK_P A4 // 0 means paper is present , 1 = no paper
#define FORK_H A5 // 1 means head is at left end
#define PAPER_LED 13

// #define PWM_ON_ENABLE

// TODO : add "set idle" + handle serial input ?
#define WAIT_FOR(condition) do { } while(!(condition))

int speedX = 0, speedY = 0;
volatile byte forkX = 0, forkY = 0;
volatile long posX = 0, posY = 0;
volatile byte paperSensor = 0; // 0 means paper is present , 1 = no paper
volatile byte headSensor = 0;  // 1 means head is at left end


void setSpeedX(int v) {
    speedX = v;
#ifdef PWM_ON_ENABLE
    if (speedX == 0) {
        digitalWrite(M_X_EN, 0);
    } else {
        if (speedX > 0) {
            analogWrite(M_X_EN, speedX);
            digitalWrite(M_X_A, 1);
            digitalWrite(M_X_B, 0);
        } else {
            analogWrite(M_X_EN, -speedX);
            digitalWrite(M_X_A, 0);
            digitalWrite(M_X_B, 1);
        }
    }
#else
    if (speedX == 0) {
        digitalWrite(M_X_EN, 0);
    } else {
        if (speedX > 0) {
            analogWrite(M_X_A, speedX);
            digitalWrite(M_X_B, 0);
        } else {
            digitalWrite(M_X_A, 0);
            analogWrite(M_X_B, -speedX);
        }
        digitalWrite(M_X_EN, 1);
   }
#endif
}

void setSpeedY(int v) {
    speedY = v;
#ifdef PWM_ON_ENABLE
    if (speedY == 0) {
        digitalWrite(M_Y_EN, 0);
    } else {
        if (speedY > 0) {
            analogWrite(M_Y_EN, speedY);
            digitalWrite(M_Y_A, 1);
            digitalWrite(M_Y_B, 0);
        } else {
            analogWrite(M_Y_EN, -speedY);
            digitalWrite(M_Y_A, 0);
            digitalWrite(M_Y_B, 1);
        }
    }
#else
    if (speedY == 0) {
        digitalWrite(M_Y_EN, 0);
    } else {
        if (speedY > 0) {
            analogWrite(M_Y_A, speedY);
            digitalWrite(M_Y_B, 0);
        } else {
            digitalWrite(M_Y_A, 0);
            analogWrite(M_Y_B, -speedY);
        }
        digitalWrite(M_Y_EN, 1);
    }
#endif
}

#define ERR_LEN 64
volatile byte errX[ERR_LEN], errY[ERR_LEN];
volatile byte errXIdx = 0, errYIdx = 0;

ISR(PCINT1_vect) {
    // TODO : ajouter un default pour empiler les erreurs dans un tableau
    // et l'afficher avec le status 
    byte fX = ((digitalRead(FORK_X_A) & 1) << 1) | (digitalRead(FORK_X_B) & 1);
    if (fX != forkX) {
        switch ((fX << 2) | forkX) {
            case 0b0001:
            case 0b0111:
            case 0b1110:
            case 0b1000:
                posX++;
                break;
            case 0b0010:
            case 0b1011:
            case 0b1101:
            case 0b0100:
                posX--;
                break;
            default:
                errX[errXIdx % ERR_LEN] = (fX << 2) | forkX;
                errXIdx++;
                break;
       }
        forkX = fX;
    }

    byte fY = ((digitalRead(FORK_Y_A)  & 1)<< 1) | (digitalRead(FORK_Y_B) & 1);
    if (fY != forkY) {
        switch ((fY << 2) | forkY) {
            case 0b0001:
            case 0b0111:
            case 0b1110:
            case 0b1000:
                posY++;
                break;
            case 0b0010:
            case 0b1011:
            case 0b1101:
            case 0b0100:
                posY--;
                break;
            default:
                errY[errYIdx % ERR_LEN] = (fY << 2) | forkY;
                errYIdx++;
                break;
        }
        forkY = fY;
    }

    byte p = digitalRead(FORK_P);
    if (p != paperSensor) {
        paperSensor = p;
        digitalWrite(PAPER_LED, paperSensor);
    }

    byte h = digitalRead(FORK_H);
    if (h != headSensor) {
        headSensor = h;
    }
}

void status() {
	Serial.print("vX=");  Serial.print(speedX);
    Serial.print("\tfX=");Serial.print(forkX);
    Serial.print("\tpX=");Serial.println(posX);

	Serial.print("vY=");  Serial.print(speedY);
    Serial.print("\tfY=");Serial.print(forkY);
    Serial.print("\tpY=");Serial.println(posY);

    Serial.print("paperSensor=");Serial.print(paperSensor);
    Serial.print("\theadSensor=");Serial.println(headSensor);

    if (errXIdx != 0) {
        Serial.print(errXIdx);Serial.println(" X errors");
        for (byte i = 0; i<errXIdx && i<ERR_LEN; i++) {
            Serial.print(errX[errXIdx % ERR_LEN], BIN); Serial.print(' ');
        }
        Serial.println();
    }
    if (errYIdx != 0) {
        Serial.print(errYIdx);Serial.println(" Y errors");
        for (byte i = 0; i<errYIdx && i<ERR_LEN; i++) {
            Serial.print(errY[errYIdx % ERR_LEN], BIN); Serial.print(' ');
        }
        Serial.println();
    }
}

void initHead() {
    // if headSensor already 1, move to right until headSensor == 1
    if (headSensor == 1) {
        posX = 0;
        Serial.println("WAIT_FOR(headSensor == 0)");
        setSpeedX(M_X_SPEED);
        WAIT_FOR(headSensor == 0);
        setSpeedX(0);
        Serial.println("ok");
    }

    // move to left until headSensor == 1
    Serial.println("WAIT_FOR(headSensor == 1)");
    setSpeedX(-M_X_SPEED);
    WAIT_FOR(headSensor == 1);
    setSpeedX(0);
    Serial.println("ok");

    Serial.println("WAIT_FOR(posX >= X_MAX)");
    setSpeedX(M_X_SPEED);
    WAIT_FOR(posX >= X_MAX);
    setSpeedX(0);
    Serial.println("ok");

    Serial.println("WAIT_FOR(posX <= 0 || headSensor == 1)");
    setSpeedX(-M_X_SPEED);
    WAIT_FOR(posX <= 0 || headSensor == 1);
    setSpeedX(0);
    Serial.println("ok");

    // if headSensor == 1 while X far from 0, it's a problem
    status();
}

void feedPaper() {
    // if no paper present, abort
    if (paperSensor == 1) {
        Serial.println("No paper");
        return;
    }

    // feed paper until paperSensor == 1 (end of sheet)
    posY=0;
    Serial.println("WAIT_FOR(paperSensor == 1)");
    setSpeedY(M_Y_SPEED);
    WAIT_FOR(paperSensor == 1 || posY >= Y_MAX);
    Serial.println("ok");

    if (posY >= Y_MAX) {
        setSpeedY(0);
        Serial.println("can't find end of paper ?");
        return;
    }

    // move paper back until page bottom
    Serial.println("posY <= Y_MARGIN");
    setSpeedY(-M_Y_SPEED);
    WAIT_FOR(posY <= Y_MARGIN); // todo : or paperSensor==1 , but needs to rollback until 0 before
    setSpeedY(0);
    Serial.println("ok");

    status();

    // consider this as new 0
    posY=0;

}

InputItem inputs[] = {
	{ '?', 'f', (void *)status },
	{ 'x', 'I', (void *)setSpeedX },
	{ 'y', 'I', (void *)setSpeedY },
	{ 'h', 'f', (void *)initHead  },
	{ 'f', 'f', (void *)feedPaper },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(M_X_EN, OUTPUT);
	pinMode(M_X_A, OUTPUT);
	pinMode(M_X_B, OUTPUT);
	pinMode(FORK_X_A, INPUT);
	pinMode(FORK_X_B, INPUT);

	pinMode(M_Y_EN, OUTPUT);
	pinMode(M_Y_A, OUTPUT);
	pinMode(M_Y_B, OUTPUT);
	pinMode(FORK_Y_A, INPUT);
	pinMode(FORK_Y_B, INPUT);

	pinMode(FORK_P, INPUT_PULLUP);
	pinMode(FORK_H, INPUT_PULLUP);
	pinMode(PAPER_LED, OUTPUT);

    // Listen PCINT 8,9,10,11,12,13 == interrupts on PC0,1,2,3,4,5 == A0,1,2,3,4,5
    PCMSK0 = 0;
    PCMSK1 = _BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10) | _BV(PCINT11) | _BV(PCINT12) | _BV(PCINT13);
    PCMSK2 = 0;
    PCICR = _BV(PCIE1); // listen for PCINT[14:8]

    forkX = ((digitalRead(FORK_X_A) & 1) << 1) | (digitalRead(FORK_X_B) & 1);
    forkY = ((digitalRead(FORK_Y_A) & 1) << 1) | (digitalRead(FORK_Y_B) & 1);
    paperSensor = digitalRead(FORK_P);
    headSensor  = digitalRead(FORK_H);

// TODO : voir https://github.com/leomil72/analogComp/blob/master/analogComp.cpp pour implementer un analog comparator
// entre 7 et A5 sur lequel on mettra un pont diviseur pour fixer une tension de seuil

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();
}
