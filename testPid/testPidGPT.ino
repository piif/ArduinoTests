#ifdef VERSION_GPT

#include <Arduino.h>
#include <PID_v1.h>

#define SENSOR A2

// ---------------------
// Paramètres encodeur
// ---------------------
volatile long encoderTicks = 0;
long prevTicks = 0;

volatile byte sensor, new_sensor;

ISR(PCINT1_vect) {
    cli();

    byte new_sensor = digitalRead(SENSOR);
    byte changes = new_sensor ^ sensor;
    sensor = new_sensor;

    if (changes && sensor) {
        encoderTicks++;
    }

    sei();
}

// ---------------------
// Paramètres PID
// ---------------------
double input = 0;      // vitesse mesurée (ticks / 10ms)
double output = 160;   // PWM calculé par le PID
double setpoint = 100; // consigne : 100 ticks / 10ms (soit 10k ticks/s)


// 2.0, 5.0, 0.0

// Kp, Ki, Kd (à ajuster après test)
PID myPID(&input, &output, &setpoint, 2, 3, 0.05, P_ON_E, DIRECT);

// ---------------------
// Broches
// ---------------------
#define MOTOR_ENABLE  3
#define MOTOR_PIN_A  10
#define MOTOR_PIN_B   5

// ---------------------
void setup() {
  Serial.begin(115200);

  // Encoder sur interruption (adapter pin et mode)
  PCMSK0 = 0;
  PCMSK1 = _BV(PCINT10);
  PCMSK2 = 0;
  PCICR  = _BV(PCIE1); // listen for PCINT[14:8]

  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(MOTOR_PIN_A, OUTPUT);
  pinMode(MOTOR_PIN_B, OUTPUT);
  digitalWrite(MOTOR_ENABLE, 1);
  digitalWrite(MOTOR_PIN_A, 0);
  digitalWrite(MOTOR_PIN_B, 0);

  pinMode(SENSOR, INPUT);
  sensor = digitalRead(SENSOR);

  // Config PID
  myPID.SetSampleTime(10);     // période 10 ms
  myPID.SetOutputLimits(0, 255);
  myPID.SetMode(AUTOMATIC);
}

// ---------------------
void loop() {
  static unsigned long iter = 0;
  static unsigned long lastTime = 0;
  unsigned long now = millis();

  static long ring[5]; static int idx=0; static long sum=0;

  if (now - lastTime >= 10) { // toutes les 10 ms
    lastTime = now;

    // lecture atomique du compteur
    noInterrupts();
    long ticks = encoderTicks;
    interrupts();

    long dticks = ticks - prevTicks;
    prevTicks = ticks;

    sum -= ring[idx];
    sum += dticks;
    ring[idx] = dticks;
    idx = (idx+1) % 5;

    input = sum / 5.0;   // toujours en "ticks / 10ms", mais lissé sur 50ms

    // Calcul PID
    myPID.Compute();

    // Application PWM
    analogWrite(MOTOR_PIN_A, (int)output);

    // Debug série
    Serial.print(iter);
    Serial.print("  in=");
    Serial.print(input);
    Serial.print("  out=");
    Serial.print(output);
    Serial.print("  sp=");
    Serial.println(setpoint);
    iter++;
  }

  if (Serial.available()) {
    digitalWrite(MOTOR_ENABLE, 0);
    myPID.SetMode(MANUAL);
    for(;;) {}
  }
}

#endif