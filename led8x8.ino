#include <Arduino.h>
#include "LedControl/LedControl.h"
#include "led7/matrix.h"

LedControl lc = LedControl(2, 3, 4);

void setup() {
	lc.shutdown(0, false);
	lc.setIntensity(0,8);
}

byte mask = 1;
#define rotateLeft(m) do { (m) = ((m) == 0x80 ? 1 : ((m) << 1)); } while(0)

void loop() {
	for(int c= 0; c<64; c++) {
		for (byte i = 0; i < 8; i++) {
			lc.setRow(0, i, mask);
			rotateLeft(mask);
		}
		rotateLeft(mask);
		delay(50);
	}

//	scrollFont(lc);
	scrollMessage(lc, "Tout ca juste avec un peu de C et un Arduino ! . . .     ", 75);
}
