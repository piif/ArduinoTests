#ifndef HAVE_PINS_H
#define HAVE_PINS_H

// FROM arduino pins.h , variant "standard" and not "sanguino" (D24 to D31 inverted)
// D0  = PB0 , PCINT8
// D1  = PB1 , PCINT9
// D2  = PB2 , PCINT10
// D3  = PB3 , PCINT11 , OC0A
// D4  = PB4 , PCINT12 , OC0B
// D5  = PB5 , PCINT13 , MOSI
// D6  = PB6 , PCINT14 , MISO
// D7  = PB7 , PCINT15 , SCK

// D8  = PD0 , PCINT24
// D9  = PD1 , PCINT25
// D10 = PD2 , PCINT26
// D11 = PD3 , PCINT27
// D12 = PD4 , PCINT28 , OC1B
// D13 = PD5 , PCINT29 , 0C1A
// D14 = PD6 , PCINT30 , OC2B
// D15 = PD7 , PCINT31 , OC2A

// D16 = PC0 , PCINT16
// D17 = PC1 , PCINT17
// D18 = PC2 , PCINT18
// D19 = PC3 , PCINT19
// D20 = PC4 , PCINT20
// D21 = PC5 , PCINT21
// D22 = PC6 , PCINT22
// D23 = PC7 , PCINT23

// D24 = PA0 , PCINT0
// D25 = PA1 , PCINT1
// D26 = PA2 , PCINT2
// D27 = PA3 , PCINT3
// D28 = PA4 , PCINT4
// D29 = PA5 , PCINT5
// D30 = PA6 , PCINT6
// D31 = PA7 , PCINT7

// FROM https://github.com/kliment/Sprinter/blob/master/Sprinter/pins.h#L301
// + adaptations because this code is based on "sanguino" variant

#define CAT2(a, b) a ## b
#define CAT3(a, b, c) a ## b ## c
#define PIN_PCMSK(intr_num) CAT2(PCMSK, intr_num)
#define PIN_PCICR(intr_num) CAT2(PCIE, intr_num)
#define PIN_PCINT_vect(intr_num) CAT3(PCINT, intr_num, _vect)

// x axis
#define X_STEP_PIN 15   // PD7 , OC2A
#define X_DIR_PIN 18    // PC2
#define X_ENABLE_PIN 31 // PA7
#define X_MIN_PIN 20    // PC4 = PCINT20
#define X_MIN_INTR  PCINT20
#define X_MIN_INTR_NUM 2
#define X_MIN_PCMSK PCMSK2
#define X_MIN_PCICR PCIE2
#define X_MIN_PCINT_vect PCINT2_vect

// y axes
#define Y_STEP_PIN 23   // PC7
#define Y_DIR_PIN 22    // PC6
#define Y_ENABLE_PIN 31 // PA7
#define Y_MIN_PIN 30    // PA6 = PCINT6
#define Y_MIN_INTR  PCINT6
#define Y_MIN_INTR_NUM 0
#define Y_MIN_PCMSK PCMSK0
#define Y_MIN_PCICR PCIE0
#define Y_MIN_PCINT_vect PCINT0_vect

// z axes
#define Z_STEP_PIN 28   // PA4
#define Z_DIR_PIN 27    // PA3
#define Z_ENABLE_PIN 26 // PA2
#define Z_MIN_PIN 25    // PA1 = PCINT1
#define Z_MIN_INTR  PCINT1
#define Z_MIN_INTR_NUM  0
#define Z_MIN_PCMSK PCMSK0
#define Z_MIN_PCICR PCIE0
#define Z_MIN_PCINT_vect PCINT0_vect

//extruder pins
#define E_STEP_PIN 12   // PD4 , OC1B
#define E_DIR_PIN 17    // PC1
#define E_ENABLE_PIN 3  // PB3 , OC0A

#define DEBUG_PIN 0

// LOW should open mofset linked to FAN connector
#define FAN_PIN 2 // PB2

// 5 lines of 3 pins (2 for bottom one) at bottom left corner
// see http://reprap.org/wiki/Monotronics#Pinout_Maps
// pinout :
// GND +5V D26/A5
// GND +5V D1
// GND +5V ARef
// GND +5V D4
// GND D31/A0
#define USER_0 29  // PA5
#define USER_0_INTR  PCINT5
#define USER_0_INTR_NUM 0
#define USER_0_PCMSK PCMSK0
#define USER_0_PCICR PCIE0
#define USER_0_PCINT_vect PCINT0_vect

#define USER_1 1   // PB1
#define USER_1_INTR  PCINT9
#define USER_1_INTR_NUM 1
#define USER_1_PCMSK PCMSK1
#define USER_1_PCICR PCIE1
#define USER_1_PCINT_vect PCINT1_vect

// no USER_2 pin

#define USER_3 4   // PB4 , OC0B
#define USER_3_INTR  PCINT12
#define USER_3_INTR_NUM 1
#define USER_3_PCMSK PCMSK1
#define USER_3_PCICR PCIE1
#define USER_3_PCINT_vect PCINT1_vect

// pin 24 is always +5V ???
#define USER_4 24  // PA0
#define USER_4_INTR  PCINT0
#define USER_4_INTR_NUM 0
#define USER_4_PCMSK PCMSK0
#define USER_4_PCICR PCIE0
#define USER_4_PCINT_vect PCINT0_vect

#define RJ45_A_0 13 // PD4
#define RJ45_A_7 14 // PD5
#define RJ45_B_0 10 // PD2
#define RJ45_B_7 11 // PD3

// remaining pins :
// PB5,6,7 = MISO, MOSI, SCK used to upload program
// PD0,1 = RX, TX for USB port
// 19 = PC3 and 21 = PC5 seems connected nowhere

#endif // HAVE_PINS_H
