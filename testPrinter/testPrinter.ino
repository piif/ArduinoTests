#include <Arduino.h>
#include <serialInput.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define M_X_EN  3
#define M_X_A  4
#define M_X_B  5

#define M_Y_EN 11
#define M_Y_A  9
#define M_Y_B 10

#define FORK_X_A A0
#define FORK_X_B A1

#define FORK_Y_A A2
#define FORK_Y_B A3

#define FORK_P 6

int speedX = 0, speedY = 0;
volatile byte forkX = 0, forkY = 0;
volatile long posX = 0, posY = 0;

void setSpeedX(int v) {
    speedX = v;
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
}

void setSpeedY(int v) {
    speedY = v;
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
}

ISR(PCINT1_vect) {
    // TODO : ajouter un default pour empiler les erreurs dans un tableau
    // et l'afficher avec le status 
    byte fX = ((digitalRead(FORK_X_A) & 1) << 1) | (digitalRead(FORK_X_B) & 1);
    if (fX != forkX) {
        switch (fX << 2 | forkX) {
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
        }
        forkX = fX;
    }

    byte fY = ((digitalRead(FORK_Y_A)  & 1)<< 1) | (digitalRead(FORK_Y_B) & 1);
    if (fY != forkY) {
        switch (fY << 2 | forkY) {
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
        }
        forkY = fY;
    }
}

void status() {
	Serial.print("vX=");  Serial.print(speedX);
    Serial.print("\tfX=");Serial.print(forkX);
    Serial.print("\tpX=");Serial.println(posX);

	Serial.print("vY=");  Serial.print(speedY);
    Serial.print("\tfY=");Serial.print(forkY);
    Serial.print("\tpY=");Serial.println(posY);
}

InputItem inputs[] = {
	{ '?', 'f', (void *)status },
	{ 'x', 'I', (void *)setSpeedX },
	{ 'y', 'I', (void *)setSpeedY },
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

    // Listen PCINT 8,9,10,11 == interrupts on PC0,1,2,3 == A0,1,2,3
    PCMSK0 = 0;
    PCMSK1 = _BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10) | _BV(PCINT11);
    PCMSK2 = 0;
    PCICR = _BV(PCIE1); // listen for PCINT[14:8]

    forkX = ((digitalRead(FORK_X_A) & 1) << 1) | (digitalRead(FORK_X_B) & 1);
    forkY = ((digitalRead(FORK_Y_A) & 1) << 1) | (digitalRead(FORK_Y_B) & 1);

// TODO : voir https://github.com/leomil72/analogComp/blob/master/analogComp.cpp pour implementer un analog comparator
// entre 7 et A5 sur lequel on mettra un pont diviseur pour fixer une tension de seuil

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();
}
