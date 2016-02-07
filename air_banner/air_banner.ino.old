/**
 * quelques tambouilles pour tester des jeux de leds
 * version amelioree
 */

#include <MsTimer2.h>
#include <avr/sleep.h>

#define POLICE "police_5x8_rev.h"
#define CHAR_WIDTH 5
#include "banner.h"

// WIRES :

// pin for display are 4 to 7 and 8 to 11 which are associated with PORTD (4 high bits) and PORTB (4 low bits) register.
#define DISPLAY_FIRST_PIN 4

// pin to listen initial button press, and associated interruption
#define BUTTON_PIN 2
#define BUTTON_INTERRUPT 1

// pin to launch motor
#define MOTOR_PIN 12

// SCENARIO :

// delay between button press and motor launch (ms)
#define BEFORE_MOTOR 2000

// delay between motor launch and animation start (ms)
#define BEFORE_ANIMATION 2000

// animation duration (ms)
#define ANIMATION_DURATION 10000

// animation speed (ms between each step)
#define TIMER_DELAY 3

// number of steps that can be "displayed" during 1 rotation
#define STEPS_BY_ROTATION 63

// => after STEPS_BY_ROTATION steps, banner restart with following offset
#define STEPS_OFFSET 1

// current state = waiting for button press (true) or playing animation (false)
volatile int nbSteps = 0;
volatile int stepOffset = 0;
volatile int stepsBeforeRestart = 0;

char *message;

/***** INITIALIZE ALL THOSE STUFFS *****/
void setup() {
  byte i;

  // display is output
  for (i = 0; i < 8; i++) {
    pinMode(DISPLAY_FIRST_PIN + i, OUTPUT);
    digitalWrite(DISPLAY_FIRST_PIN + i, HIGH);
  }

  // motor pin too
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);

  // button pin is input, with pullup resistor active
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);

  // TODO : read message from serial
  // => malloc ?
  message = "Bonjour !!! ";
  bannerSetMessage(message);
}

void launchAnimation() {
  // signal step "waiting button"
  digitalWrite(DISPLAY_FIRST_PIN, HIGH);

  // wait for button press

/** This methods doesn't works **
  attachInterrupt(BUTTON_INTERRUPT, buttonHandler, RISING);
  sleepNow(); // will wake up after button press
  // even with 2 steps
  attachInterrupt(BUTTON_INTERRUPT, buttonHandler, FALLING);
  sleepNow(); // will wake up after button press
  detachInterrupt(BUTTON_INTERRUPT);
**/
/** Neither this methods **
    set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here
    sleep_enable();
    attachInterrupt(BUTTON_INTERRUPT, buttonHandler, HI);
    sleep_mode();
    sleep_disable();
    detachInterrupt(BUTTON_INTERRUPT);
**/

  // have to use this ugly one ...
  while (digitalRead(BUTTON_PIN) != LOW) {
    delay(100);
  }

  // signal step "will launch motor"
  digitalWrite(DISPLAY_FIRST_PIN, LOW);
  digitalWrite(DISPLAY_FIRST_PIN + 1, HIGH);
  // wait
  delay(BEFORE_MOTOR);

  // launch motor
  digitalWrite(MOTOR_PIN, HIGH);

  // signal step "will launch animation"
  digitalWrite(DISPLAY_FIRST_PIN + 1, LOW);
  digitalWrite(DISPLAY_FIRST_PIN + 2, HIGH);

  // wait
  delay(BEFORE_ANIMATION);

  nbSteps = ANIMATION_DURATION / TIMER_DELAY;
  stepsBeforeRestart = STEPS_BY_ROTATION;
  bannerSeek(0, 0);

  // launch main timer
  MsTimer2::set(TIMER_DELAY, doStep);
  MsTimer2::start();
}

/***
void buttonHandler() {
  // nothing to do, we just want to be called to wake up
  digitalWrite(4, HIGH);
}
***/

void doStep() {
  byte out, port;

  // one step beyond ...
  nbSteps--;
  if (!nbSteps) {
    // stop motor
    digitalWrite(MOTOR_PIN, LOW);
    // and timer
    MsTimer2::stop();
    // will have to restart "launch" procedure
    return;
  }

  // display new animation step
  out = bannerNext();
  // low bits send to portB = out 8 to 11
  PORTB = (PORTB & 0xF0) | (out & 0x0F);
  // high bits send to portD = out 4 to 7
  PORTD = (PORTD & 0x0F) | (out & 0xF0);
  
  stepsBeforeRestart--;
  if (!stepsBeforeRestart) {
    stepsBeforeRestart = STEPS_BY_ROTATION;
    stepOffset += STEPS_OFFSET;
    bannerSeek(stepOffset / (CHAR_WIDTH+1), stepOffset % (CHAR_WIDTH+1));
  }
}

/** cut/paste from http://www.arduino.cc/playground/Learning/ArduinoSleepCode */
void sleepNow() {
    /* 
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     */  
    set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here
    sleep_enable();
 
//    attachInterrupt(0,wakeUpNow, LOW);
    sleep_mode();
    sleep_disable();
//    detachInterrupt(0); 
}

/***** ET EN AVANT ... ****/
void loop() {
  if (!nbSteps) {
    launchAnimation();
  }

  // just stay idle between timer calls
  sleepNow();
}

