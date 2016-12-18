/**
 * début de bricolage pour faire un métronome
 * connecter les LED aux pins 8 à 12, via une résistance d'environ 150ohm
 */
// TODO : faire une base de temps à partir de millis et calculer le "temps" du prochain beat, pour être plus précis

#include <Arduino.h>

/***** PARTIE OUTPUT *****/

/* le nombre de leds "principale" */
#define NB_LEDS 4
/* et leur brochage */
const int beatLED[NB_LEDS]={ 8, 9, 10, 11 };
/* ainsi que celle pour la pulsation des croches */
const int subBeatLED=12;

#define subBeatDuration 50

/* les différentes mesures */
#define NB_TIMES 3
const int step4[]= { 0x08,  0x02,  0x04,  0x01 }; // 4 temps
const int step3[]= { 0x08,  0x04,  0x01 };        // 3 temps
const int step2[]= { 0x08,  0x01 };               // 2 temps
const int *steps[]={ step4, step3, step2 };

// le nombre de temps de chaque ligne du tableau précédent
// puisqu'on ne peut pas faire de int[][] et que sizeof ne marchera pas sur un *int[]
const int stepLengths[]={ 4, 3, 2 };

int tempo   =72; // 40 à 220 temps par minute */

int mode    =NB_TIMES-1;  // mesure courante (index dans steps)
int subMode =3;  // 2=binaire, 3=ternaire
// on initialise au dernier mode pour que le setup appelle "next" et fasse "tout ce qu'il faut"
// pour initialiser au mode suivant, c'est à dire le premier

long beatDelay; // temps d'attente entre deux "sous pulsations"

int  nbBeats;   // nombre de pulsation par mesure dans la mesure courante
int  beat;      // pulsation courante
int  subBeat;   // sous pulsation courante

/**
 * passer d'un mode à l'autre, en boucle, dans cet ordre :
 * 4 temps binaire, 4 temps ternaire, 3 temps binaire, 3 temps ternaire, 2 temps binaire, 2 temps ternaire
 */
void nextMode() {
  if(subMode==2) {
    subMode=3;
  } else {
    subMode=2;
    mode=(mode+1) % NB_TIMES;
    nbBeats= stepLengths[mode];
  }
  
  Serial.print("-> mode ");
  Serial.print(nbBeats);
  Serial.print(" / ");
  Serial.println(subMode);

  beatDelay= 60000/ (subMode*tempo);
  // reprendre la battue de zéro
  beat=0;
  subBeat=0;
}

/**
 * changer le tempo et en déduire de nouveau démai entre pulsations
 */
void changeTempo(int amount) {
  tempo+= amount;
  if(tempo<40) {
    tempo=40;
  } else if(tempo>220) {
    tempo=220;
  }
  Serial.print("-> tempo ");
  Serial.println(tempo);
  beatDelay= 60000/ (subMode*tempo);
}

void writeLeds(int value) {
  int mask, i;
  for(mask= 0x1 << (NB_LEDS-1), i=0; mask; mask>>=1, i++) {
    digitalWrite(beatLED[i], (value & mask) ? HIGH : LOW);
  }
}

/**
 * afficher le temps ou le "sous temps" suivant
 * puis préparer le suivant
 */
void doBeat() {
  // allumer la pulsation
  digitalWrite(subBeatLED, HIGH);
  if(subBeat==0) {
    writeLeds(steps[mode][beat]);
  }

  subBeat++;
  if(subBeat==subMode) {
    subBeat=0;
    beat++;
    if(beat==nbBeats) {
      beat=0;
    }
  }
}

// éteindre la pulsation
void stopSubBeat() {
  digitalWrite(subBeatLED, LOW);
}


/***** PARTIE INPUT *****/

// pin 2, (associé à l'interruption 0), est connectée à chaque bouton, via une diode
#define BUTTONS_IN 2
#define BUTTONS_INT 0

// l'autre patte de chacun de ces bouton est aux pin 4, 5 et 6
#define BUTTON_LESS  4
#define BUTTON_MORE  5
#define BUTTON_MODE  6

// si l'interruption est appelée plusieurs fois en mloins de MIN_DELAY millis,
// c'est du "bruit", on ne garde que le premier
#define MIN_DELAY 100

void interruptHandler()
{
  // repérer la dernière fois qu'no a appelé cette fonction
  static unsigned long lastCall= 0;
  unsigned long now= millis();

  // si l'état est HIGH, c'est qu'on relache le bouton
  // on ignore, mais on note l'heure pour ignorer les éventuels rebonds "LOW" qui vont suivre
  if(digitalRead(BUTTONS_IN) != LOW) {
    lastCall= now;
    return;
  }
  // si on est venu il y a moins de MIN_DELAY millis, on ignore
  if(lastCall != 0 && lastCall+MIN_DELAY>now) {
    return;
  }
  
  // sinon, on note l'heure
  lastCall= now;

  // maintenant, regardons l'état des boutons
  // dès qu'on en trouve un d'appuyé, on cherche pas plus loin
  // TODO : faire un truc plus tordu avec 2 touches à la fois pour switcher mode et submode séparément
  if(digitalRead(BUTTON_MODE) == LOW) {
    nextMode();
    return;
  }
  if(digitalRead(BUTTON_LESS) == LOW) {
    changeTempo(-4);
    return;
  }
  if(digitalRead(BUTTON_MORE) == LOW) {
    changeTempo(4);
    return;
  }

  // en théorie, on devrait jamais arriver là ...
  Serial.println("weird button state ...");
}

/***** INITIALISER TOUT CA *****/
void setup() {
  int i;

  Serial.begin(9600);

  for(i=0; i<NB_LEDS; i++) {
    pinMode(beatLED[i], OUTPUT);
  }
  pinMode(subBeatLED, OUTPUT);
  
  nextMode();

  pinMode(BUTTONS_IN, INPUT);
  digitalWrite(BUTTONS_IN, HIGH);

  // BUTTON_*_IN are also inputs, to detect current position of button
  pinMode(BUTTON_MORE, INPUT);
  digitalWrite(BUTTON_MORE, HIGH);
  pinMode(BUTTON_LESS, INPUT);
  digitalWrite(BUTTON_LESS, HIGH);
  pinMode(BUTTON_MODE, INPUT);
  digitalWrite(BUTTON_MODE, HIGH);

  // when button is pressed or release (because noise when button is released may make it appear as pressed again)
  attachInterrupt(BUTTONS_INT, interruptHandler, CHANGE);
}

/***** ET EN AVANT ... ****/
void loop() {
  doBeat();
  delay(subBeatDuration);
  stopSubBeat();
  delay(beatDelay-subBeatDuration);
}
