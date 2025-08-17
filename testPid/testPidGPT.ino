#ifdef VERSION_GPT

#include <Arduino.h>
#include <PID_v1.h>

// ---------------------
// Paramètres encodeur
// ---------------------
volatile long encoderTicks = 0;
long prevTicks = 0;

// ISR : incrémente le compteur à chaque front (adapter à ton encodeur)
void encoderISR() {
  encoderTicks++;
}

// ---------------------
// Paramètres PID
// ---------------------
double input = 0;      // vitesse mesurée (ticks / 10ms)
double output = 0;     // PWM calculé par le PID
double setpoint = 100; // consigne : 100 ticks / 10ms (soit 10k ticks/s)

// Kp, Ki, Kd (à ajuster après test)
PID myPID(&input, &output, &setpoint, 2.0, 5.0, 0.0, DIRECT);

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
  attachInterrupt(digitalPinToInterrupt(2), encoderISR, RISING);

  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(MOTOR_PIN_A, OUTPUT);
  pinMode(MOTOR_PIN_B, OUTPUT);
  digitalWrite(MOTOR_ENABLE, 1);
  digitalWrite(MOTOR_PIN_A, 0);
  digitalWrite(MOTOR_PIN_B, 0);

  // Config PID
  myPID.SetSampleTime(100);     // période 10 ms
  myPID.SetOutputLimits(0, 255);
  myPID.SetMode(AUTOMATIC);
}

// ---------------------
void loop() {
  static unsigned long iter = 0;
  static unsigned long lastTime = 0;
  unsigned long now = millis();

  if (now - lastTime >= 100) { // toutes les 10 ms
    lastTime = now;

    // lecture atomique du compteur
    noInterrupts();
    long ticks = encoderTicks;
    interrupts();

    long dticks = ticks - prevTicks;
    prevTicks = ticks;

    // vitesse en ticks / 10 ms
    input = dticks/10;

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