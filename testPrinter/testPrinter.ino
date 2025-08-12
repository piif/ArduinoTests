#include <Arduino.h>

#include "axis.h"
#include "strop.h"
#include "tools.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

void feed_paper() {
    Serial.println("feed_paper"); axis_status();

    if (!head_max) {
        axis_x_set_speed(M_X_SPEED);
        WAIT_FOR(head_max);
        Serial.println("after 1st move right"); axis_status();
    }
    axis_x_set_speed(-M_X_SPEED);
    WAIT_FOR(X_pos <= X_MAX - 100);
    axis_stop();

    axis_x_set_speed(M_X_SPEED);
    WAIT_FOR(head_max);

    // if paper detected, eject it until bottom border
    if (paper_present) {
        axis_y_set_speed(-M_Y_SPEED);
        WAIT_FOR(!paper_present);
        axis_stop();
    }
    // then, feed until it's detected
    axis_y_set_speed(M_Y_SPEED);
    WAIT_FOR(paper_present);
    axis_stop();

    // this is position for bottom of page
    Y_pos = 0;

    Serial.println("bottom of page"); axis_status();

    // find top border
    axis_y_set_speed(M_Y_SPEED);
    WAIT_FOR(!paper_present);
    axis_stop();

    Serial.println("top of page"); axis_status();

    // go back a few (~ 1cm)
    long bottom_margin = Y_pos - 1000;
    axis_y_set_speed(-M_Y_SPEED);
    WAIT_FOR(Y_pos <= bottom_margin);
    axis_stop();
    if (!paper_present) {
        Serial.println("end of paper before bottom margin of page ?!?");
        return;
    }

#ifdef CAN_DETECT_LEFT_BORDER
    // look for left border
    axis_x_set_speed(-M_X_SPEED);
    WAIT_FOR(!paper_present);
    axis_stop();

    Serial.println("left of page"); axis_status();
#endif // CAN_DETECT_LEFT_BORDER

    // go back to bottom
    axis_y_set_speed(-M_Y_SPEED);
    WAIT_FOR(!paper_present);
    axis_stop();
    // if (!paper_present) {
    //     Serial.println("end of paper before top of page");
    //     return;
    // }
}

// desired speed , set thru serial input thus not "volatile"
long vx = 0, vy = 0;

void set_speed_x(int v) {

}

void set_speed_y(int t) {

}

void test_break(int v) {
    // // check Y inertia
    // Y_pos = 0;
    // axis_y_set_speed(M_Y_SPEED);
    // WAIT_FOR(Y_pos >= 3000);
    // int before = Y_pos;
    // axis_stop();
    // int after = Y_pos;
    // Serial.print("pos before break"); Serial.println(before);
    // Serial.print("pos after break"); Serial.println(after);
    // delay(1000);
    // Serial.print("pos after 1s"); Serial.println(Y_pos);

    // // check X inertia
    // X_pos = 0;
    // axis_x_set_speed(M_X_SPEED);
    // WAIT_FOR(X_pos >= 1500);
    // int before = X_pos;
    // axis_stop();
    // int after = X_pos;
    // Serial.print("pos before break "); Serial.println(before);
    // Serial.print("pos after break "); Serial.println(after);
    // delay(1000);
    // Serial.print("pos after 1s "); Serial.println(X_pos);

    // // check micro move X
    // X_pos = 0;
    // axis_x_set_speed(M_X_SPEED);
    // delay(v);
    // int before = X_pos;
    // axis_x_set_speed(0);
    // delay(1000);
    // int after = X_pos;
    // Serial.print("pos before move "); Serial.println(before);
    // Serial.print("pos after move "); Serial.println(after);

    // // check micro move Y
    // Y_pos = 0;
    // axis_y_set_speed(M_Y_SPEED);
    // delay(v);
    // int before = Y_pos;
    // axis_y_set_speed(0);
    // delay(1000);
    // int after = Y_pos;
    // Serial.print("pos before move "); Serial.println(before);
    // Serial.print("pos after move "); Serial.println(after);

    // check move X with break => b8 or b9 OK ?
    X_pos = 0;
    axis_x_set_speed(-M_X_SPEED);
    WAIT_FOR(X_pos <= -100);
    int before = X_pos;
    X_pos = 0;
    axis_x_set_speed(255);
    delay(v);
    axis_x_set_speed(0);
    int after = X_pos;
    X_pos = 0;
    Serial.print("pos before break "); Serial.println(before);
    Serial.print("pos after break "); Serial.println(after);
    delay(1000);
    Serial.print("pos after 1s "); Serial.println(X_pos);

    // // check move Y with break
    // Y_pos = 0;
    // axis_y_set_speed(M_Y_SPEED);
    // WAIT_FOR(Y_pos >= 3000);
    // int before = Y_pos;
    // Y_pos = 0;
    // axis_y_set_speed(-255);
    // delay(v);
    // axis_y_set_speed(0);
    // int after = Y_pos;
    // Y_pos = 0;
    // Serial.print("pos before break "); Serial.println(before);
    // Serial.print("pos after break "); Serial.println(after);
    // delay(1000);
    // Serial.print("pos after 1s "); Serial.println(Y_pos);

}

InputItem inputs[] = {
	{ '0', 'f', (void *)axis_stop },
	{ 's', 'f', (void *)axis_stop },
	{ '?', 'f', (void *)status },
	{ 'b', 'I', (void *)test_break },
	{ 'x', 'I', (void *)axis_x_set_speed },
	{ 'y', 'I', (void *)axis_y_set_speed },
	{ 'X', 'I', (void *)set_speed_x },
	{ 'Y', 'I', (void *)set_speed_y },
	// { 'X', 'I', (void *)axis_x_move_of },
	// { 'Y', 'I', (void *)axis_y_move_of },
	{ 'f', 'f', (void *)feed_paper },
};

volatile unsigned long my_clock = 0;

#ifdef COMPUTE_ISR_DURATION
volatile unsigned long nb_isr_call = 0;
volatile unsigned long all_isr_call = 0;
volatile unsigned long max_isr_call = 0;
#endif

ISR(TIMER0_COMPA_vect) {
    cli();
#ifdef COMPUTE_ISR_DURATION
    unsigned long tic = micros();
#endif

    my_clock++;
    if (my_clock == 200) { // adjust motors 5 times per second

    }

#ifdef COMPUTE_ISR_DURATION
    unsigned long duration = micros() - tic;
    nb_isr_call++;
    all_isr_call += duration;
    if (duration > max_isr_call) {
        max_isr_call = duration;
    }
#endif
    sei();
}

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

    axis_begin();

    set_sleep_mode(SLEEP_MODE_IDLE);

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");

    // timer 1 clock prescaler defaults to 011 = /64 , set to 001 = /1
    // /64 implies a frequency of 60Hz which is noisy , /8 = 4000Hz still audible, /1 = 32KHz inaubible.
    TCCR1B &= ~(1 << CS11);

    // timer 0 clock prescaler defaults to /64 and TOIE interrupt is used to update millis counter
    // we can set TIMSK0 bit OCIE0A and set OCR0A :
    // - to 125 to call OCIE0A every millisecond
    // - to  25 to call it  5 times per millisecond (every 200µs)
    OCR0A = 125;
    TIMSK0 |= (1 << OCIE0A);

    // Serial.println("TCCR0A "); Serial.println(TCCR0A, HEX);
    // Serial.println("TCCR0B "); Serial.println(TCCR0B, HEX);
    // Serial.println("TIMSK0 "); Serial.println(TIMSK0, HEX);
    status();
}

void status() {
    axis_status();
}

long px, py;
unsigned long now = millis();

void loop() {
//     long dx = X_pos - px; px = X_pos;
//     long dy = Y_pos - py; py = Y_pos;
//     unsigned long dt = millis() - now; now = millis();
//     float vx = X_dir * dx * 1000 / dt;
//     float vy = Y_dir * dy * 1000 / dt;
//     Serial.print("VX = "); Serial.print(vx); Serial.print("\tVY = "); Serial.println(vy);
// #ifdef COMPUTE_ISR_DURATION
// 	Serial << nb_isr_call << " ISR calls , avg " << (all_isr_call/nb_isr_call) << " µs , max = " << max_isr_call << " µs" << EOL;
// #endif
//     delay(500);
    sleep_mode();
	handleInput();
}
