#include <Arduino.h>
#include "LedControl/LedControl.h"
#include "led7/matrix.h"

LedControl lc = LedControl(2, 3, 4, 2);

void setup() {
	lc.shutdown(0, false);
	lc.setIntensity(0,8);
	lc.shutdown(1, false);
	lc.setIntensity(1,8);
}

byte mask = 1;
#define rotateLeft(m) do { (m) = ((m) == 0x80 ? 1 : ((m) << 1)); } while(0)

void pong() {
	bool dx = true, dy = true;
	byte x = 0, y = 0;
	for(int c = 0; c < 200; c++) {
		lc.setLed(x >= 8 ? 1 : 0, y, x & 0x07, 0);
		if (dx) {
			x++;
		} else {
			x--;
		}
		if (dy) {
			y++;
		} else {
			y--;
		}
		lc.setLed(x >= 8 ? 1 : 0, y, x & 0x07, 1);
		if (x == 0 || x == 15) {
			dx = !dx;
		}
		if (y == 0 || y == 7) {
			dy = !dy;
		}
		delay(50);
	}

}
void slashes() {
	for(int c = 0; c < 64; c++) {
		for (byte i = 0; i < 8; i++) {
			lc.setRow(0, i, mask);
			lc.setRow(1, i, mask);
			rotateLeft(mask);
		}
		rotateLeft(mask);
		delay(50);
	}
}

void message() {
	//	scrollFont(lc);
		scrollMessage(lc, "Bla bla . . .     ", 75);
	//	scrollMessage(lc, "Tout ca juste avec un peu de C et un Arduino ! . . .     ", 75);
}

void loop() {
	pong();

	slashes();

	lc.clearDisplay(0);
	lc.clearDisplay(1);

	message();
}
