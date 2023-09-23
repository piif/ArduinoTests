#include <Arduino.h>

// LCD Keypad Shield

#include <LiquidCrystal.h>

// Création de l'objet lcd (avec les différents ports numériques qu'il utilise)
// = RS, Enable, D0, D1, D2, D3
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Variables
byte lcd_key = 0;

// Constantes
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

byte char_up[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00000,
};
byte char_down[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000,
};
byte char_left[8] = {
  0b00100,
  0b01000,
  0b11110,
  0b01001,
  0b00101,
  0b00001,
  0b11110,
  0b00000,
};
byte char_right[8] = {
  0b00100,
  0b00010,
  0b01111,
  0b10010,
  0b10100,
  0b10000,
  0b01111,
  0b00000,
};

void setup() {
    Serial.begin(115200);
    pinMode(10, OUTPUT);
    analogWrite(10, 50);
    lcd.begin(16, 2);              // Démarrage de l'écran
    lcd.display();
    lcd.createChar(1, char_up);
    lcd.createChar(2, char_down);
    lcd.createChar(3, char_left);
    lcd.createChar(4, char_right);
    lcd.setCursor(0,0);            // Positionnement du curseur au début
    lcd.print("Setup OK"); // Message
    Serial.println("Setup OK"); // Message
}

byte charList = 0x20;

void updateCharList() {
    char list[17] = { 0, };
    for (byte b= 0, i=charList; b<16; b++, i++) {
        list[b] = i;
    }
    if (charList == 0) {
        list[0] = ' '; // Eviter le caractère 0 (marqueur de fin de chaine ...)
    }
    lcd.setCursor(0, 0);
    lcd.print(list);
    char digit = charList>>4;
    if (digit > 9) {
        digit += 'A'-10;
    } else {
        digit += '0';
    }
    lcd.setCursor(0, 1);
    lcd.print(digit);
    lcd.print('0');
    lcd.setCursor(14, 1);
    lcd.print(digit);
    lcd.print('F');
}

void loop() {
    byte new_lcd_key = read_LCD_buttons();  // Lecture des touches
    if (new_lcd_key != lcd_key) {
        lcd_key = new_lcd_key;
        lcd.setCursor(4,1);            // Positionnement du curseur début de ligne 1

        switch (lcd_key) {             // Action en cas de touche pressée
            case btnRIGHT:
                lcd.print("DROITE");
                Serial.println("DROITE");
                // analogWrite(10, 255);
                if (charList<0xF0) {
                    charList+=0x10;
                    updateCharList();
                }
            break;
            case btnLEFT:
                lcd.print("GAUCHE");
                Serial.println("GAUCHE");
                // analogWrite(10, 0);
                if (charList>0x00) {
                    charList-=0x10;
                    updateCharList();
                }
            break;
            case btnUP:
                lcd.print("HAUT  ");
                Serial.println("HAUT");
                // analogWrite(10, 100);
            break;
            case btnDOWN:
                lcd.print("BAS   ");
                Serial.println("BAS");
                // analogWrite(10, 50);
            break;
            case btnSELECT:
                lcd.print("SELECT");
                Serial.println("SELECT");
                updateCharList();
            break;
            case btnNONE:
                lcd.print("      ");
            break;
        }
    }
}

// Fonction de lecture des touches
int read_LCD_buttons() {
    int adc_key_in = analogRead(0);   // Lecture du port analogique

    // Les valeurs qui suivent doivent être adaptées au shield
    if (adc_key_in > 1000) return btnNONE;   // En principe 1023 quand aucune touche n'est pressée
    if (adc_key_in < 50)   return btnRIGHT;     // 0
    if (adc_key_in < 195)  return btnUP;        // 99
    if (adc_key_in < 380)  return btnDOWN;      // 255
    if (adc_key_in < 555)  return btnLEFT;      // 409
    if (adc_key_in < 790)  return btnSELECT;    // 640

    return btnNONE;
}
