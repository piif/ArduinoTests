#include <Arduino.h>

// LCD Keypad Shield

#include <LiquidCrystal.h>

// Création de l'objet lcd (avec les différents ports numériques qu'il utilise)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Variables
int lcd_key     = 0;
int adc_key_in  = 0;

// Constantes
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

void setup()
{
 Serial.begin(115200);
 pinMode(10, OUTPUT);
 digitalWrite(10, HIGH);
 lcd.begin(16, 2);              // Démarrage de l'écran
 lcd.setCursor(0,0);            // Positionnement du curseur au début
 lcd.display();
 lcd.print("Arduino:essentiel"); // Message
 Serial.println("Arduino:essentiel"); // Message
}
 
void loop()
{
 lcd.setCursor(0,1);            // Positionnement du curseur début de ligne 1
 lcd_key = read_LCD_buttons();  // Lecture des touches

 switch (lcd_key)               // Action en cas de touche pressée
 {
   case btnRIGHT:
     {
     lcd.print("DROITE");
     Serial.println("DROITE");
     analogWrite(10, 255);
     break;
     }
   case btnLEFT:
     {
     lcd.print("GAUCHE");
     Serial.println("GAUCHE");
     analogWrite(10, 0);
     break;
     }
   case btnUP:
     {
     lcd.print("HAUT  ");
     Serial.println("HAUT");
     analogWrite(10, 100);
    break;
     }
   case btnDOWN:
     {
     lcd.print("BAS   ");
     Serial.println("BAS");
     analogWrite(10, 50);
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     Serial.println("SELECT");
     break;
     }
   case btnNONE:
     {
     lcd.print("      ");
     break;
     }
 }
}

// Fonction de lecture des touches
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);   // Lecture du port analogique

 // Les valeurs qui suivent doivent être adaptées au shield
 if (adc_key_in > 1000) return btnNONE;   // En principe 1023 quand aucune touche n'est pressée
 if (adc_key_in < 50)   return btnRIGHT;     // 0
 if (adc_key_in < 195)  return btnUP;        // 99
 if (adc_key_in < 380)  return btnDOWN;      // 255
 if (adc_key_in < 555)  return btnLEFT;      // 409
 if (adc_key_in < 790)  return btnSELECT;    // 640

 return btnNONE;
}
