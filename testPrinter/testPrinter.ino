#include <Arduino.h>

#include "axis.h"
#include "strop.h"
#include "tools.h"
#include "strop.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

void feed_paper() {
    Serial << F("feed_paper") << EOL; axis_status();

    if (!head_max) {
        axis_x_set_speed(M_X_SPEED_START, 1);
        WAIT_FOR(head_max);
        Serial << F("after 1st move right") << EOL; axis_status();
    }
    axis_x_set_speed(M_X_SPEED_START, -1);
    WAIT_FOR(X_pos <= X_MAX - 100);
    axis_stop();

    axis_x_set_speed(M_X_SPEED_START, 1);
    WAIT_FOR(head_max);

    // if paper detected, eject it until bottom border
    if (paper_present) {
        axis_y_set_speed(M_Y_SPEED, -1);
        WAIT_FOR(!paper_present);
        axis_stop();
    }
    // then, feed until it's detected
    axis_y_set_speed(M_Y_SPEED, 1);
    WAIT_FOR(paper_present);
    axis_stop();

    // this is position for bottom of page
    Y_pos = 0;

    Serial << F("bottom of page") << EOL; axis_status();

    // find top border
    axis_y_set_speed(M_Y_SPEED, 1);
    WAIT_FOR(!paper_present);
    axis_stop();

    Serial << F("top of page") << EOL; axis_status();

    // go back a few (~ 1cm)
    long bottom_margin = Y_pos - 1000;
    axis_y_set_speed(M_Y_SPEED, -1);
    WAIT_FOR(Y_pos <= bottom_margin);
    axis_stop();
    if (!paper_present) {
        Serial << F("end of paper before bottom margin of page ?!?") << EOL;
        return;
    }

#ifdef CAN_DETECT_LEFT_BORDER
    // look for left border
    axis_x_set_speed(M_X_SPEED_START, -1);
    WAIT_FOR(!paper_present);
    axis_stop();

    Serial << F("left of page") << EOL; axis_status();
#endif // CAN_DETECT_LEFT_BORDER

    // go back to bottom
    axis_y_set_speed(M_Y_SPEED, -1);
    WAIT_FOR(!paper_present);
    axis_stop();
    // if (!paper_present) {
    //     Serial << F("end of paper before top of page") << EOL;
    //     return;
    // }
}

// original position during a move
long xi, yi;
// desired target position , set thru serial input thus not "volatile"
long xf, yf;
// distance to move of , distance to 1st quarter (end of acceleration) and 3rd quarter (start of deceleration), current step
long xd, xq1, xq2, xq3, xqb, xstep, xiter, xv;
long yd, yq1, yq2, yq3, ystep, yiter, yv;

#define BUF_MAX 100
long bufs[BUF_MAX], bufp[BUF_MAX];
byte bufv[BUF_MAX];
byte i=0;

void move_x_to_callback() {
    xiter++;
    xstep = (xi < xf) ? (X_pos - xi) : (xi - X_pos);
    if (xstep >= xd) {
        xv = -1;
        axis_x_set_speed(0);
        x_callback = NULL;
    } else if (xstep >= xqb) {
        xv = -2;
        axis_x_set_speed(M_X_SPEED_MAX, (xi < xf) ? -1 : 1);
    } else if (xstep >= xq3) {
        xv = M_X_SPEED_END;
        axis_x_set_speed(xv);        
    } else if (xstep >= xq2) {
        xv = M_X_SPEED_MAX - (xstep-xq2) * (M_X_SPEED_MAX-M_X_SPEED_END) / xq1;
        axis_x_set_speed(xv);        
    } else if (xstep >= xq1) {
        xv = M_X_SPEED_MAX;
        axis_x_set_speed(xv);
    } else {
        xv = M_X_SPEED_START + xstep * (M_X_SPEED_MAX-M_X_SPEED_START) / xq1;
        axis_x_set_speed(xv);
    }
    if (i < BUF_MAX) {
        bufs[i] = xstep;
        bufp[i] = X_pos;
        bufv[i] = xv;
        i++;
    }
}

void move_x_to(int p) {
    if (p == X_pos) {
        return;
    }

    xiter = 0;
    i = 0;
    xi = X_pos; xf = p;
    short dir;
    if (xi < xf) {
        dir = 1;
        xd = xf-xi;
    } else {
        dir = -1;
        xd = xi-xf;
    }
    xq1 = xd / 4;
    xq2 = xd / 2;
    xq3 = xd - xq1;
    xqb = xd - 5;
    x_callback = move_x_to_callback;
    Serial << F("move_x_to ") << p << F(" => ")
        << F(" , xi=") << xi
        << F(" , xf=") << xf
        << F(" , xd=") << xd
        << F(" , dir=") << dir
        << F(" , xq1=") << xq1
        << F(" , xq3=") << xq3
        << EOL;
    axis_x_set_speed(M_X_SPEED_START, dir);
}

void move_y_to(int p) {
    yi = Y_pos; yf = p;
}

void set_x_speed(int v) {
    if (v == 0) {
        axis_x_set_speed(0);
    } else if ( v < 0) {
        axis_x_set_speed(-v, -1);
    } else {
        axis_x_set_speed(v, 1);
    }
}

void set_y_speed(int v) {
    if (v == 0) {
        axis_y_set_speed(0);
    } else if ( v < 0) {
        axis_y_set_speed(-v, -1);
    } else {
        axis_y_set_speed(v, 1);
    }
}

void test_break(int v) {
    // // check Y inertia
    // Y_pos = 0;
    // // axis_y_set_speed(250, 1);
    // // WAIT_FOR(Y_pos >= 3000);
    // // axis_y_set_speed(180, 1);
    // // WAIT_FOR(Y_pos >= 5000);
    // axis_y_set_speed(v, 1);
    // WAIT_FOR(Y_pos >= 6000);
    // int before = Y_pos;
    // axis_stop();
    // int after = Y_pos;
    // Serial << F("pos before break") << before << EOL;
    // Serial << F("pos after break") << after << EOL;
    // delay(1000);
    // Serial << F("pos after 1s") << Y_pos << EOL;

    // check X inertia
    unsigned long tac = micros();
    X_pos = 0;
    // axis_x_set_speed(180, 1);
    // WAIT_FOR(X_pos >= 1500);
    // unsigned long tac = micros();
    // int before = X_pos;
    axis_x_set_speed(v, 1);
    WAIT_FOR(X_pos >= 50);
    unsigned long toc = micros();
    axis_stop();
    int after = X_pos;
    // Serial << F("pos before break ") << before << F(" in ") << tac-tic << EOL;
    Serial << F("pos after break ") << after << F(" in ") << toc-tac << EOL;
    delay(500);
    Serial << F("pos after .5s ") << X_pos << EOL;

    // // check micro move X
    // X_pos = 0;
    // axis_x_set_speed(M_X_SPEED_START, 1);
    // delay(v);
    // int before = X_pos;
    // axis_x_set_speed(0);
    // delay(1000);
    // int after = X_pos;
    // Serial << F("pos before move ") << before << EOL;
    // Serial << F("pos after move ") << after << EOL;

    // // check micro move Y
    // Y_pos = 0;
    // axis_y_set_speed(M_Y_SPEED, 1);
    // delay(v);
    // int before = Y_pos;
    // axis_y_set_speed(0);
    // delay(1000);
    // int after = Y_pos;
    // Serial << F("pos before move ") << before << EOL;
    // Serial << F("pos after move ") << after << EOL;

    // // check move X with break => b8 or b9 OK ?
    // X_pos = 0;
    // axis_x_set_speed(M_X_SPEED_START, -1);
    // WAIT_FOR(X_pos <= -100);
    // int before = X_pos;
    // X_pos = 0;
    // axis_x_set_speed(255, 1);
    // delay(v);
    // axis_x_set_speed(0);
    // int after = X_pos;
    // X_pos = 0;
    // Serial << F("pos before break ") << before << EOL;
    // Serial << F("pos after break ") << after << EOL;
    // delay(1000);
    // Serial << F("pos after 1s ") << X_pos << EOL;

    // // check move Y with break
    // Y_pos = 0;
    // axis_y_set_speed(M_Y_SPEED, 1);
    // WAIT_FOR(Y_pos >= 3000);
    // int before = Y_pos;
    // Y_pos = 0;
    // axis_y_set_speed(255, -1);
    // delay(v);
    // axis_y_set_speed(0);
    // int after = Y_pos;
    // Y_pos = 0;
    // Serial << F("pos before break ") << before << EOL;
    // Serial << F("pos after break ") << after << EOL;
    // delay(1000);
    // Serial << F("pos after 1s ") << Y_pos << EOL;

}

void status() {
    axis_status();
    Serial << F("xstep = ") << xstep << F(" , iter = ") << xiter << F(" , v = ") << xv;
    if (x_callback) { Serial << F(" , X callback set"); }
    Serial << EOL;
    Serial << F("ystep = ") << ystep << F(" , iter = ") << yiter << F(" , v = ") << yv;
    if (y_callback) { Serial << F(" , Y callback set"); }
    Serial << EOL;
    Serial << F("bufs");
    for (int j = 0; j<i; j++) {
        Serial << ' ' << bufs[j];
    }
    Serial << F("\nbufp");
    for (int j = 0; j<i; j++) {
        Serial << ' ' << bufp[j];
    }
    Serial << F("\nbufv");
    for (int j = 0; j<i; j++) {
        Serial << ' ' << bufv[j];
    }
    Serial << EOL;
}

InputItem inputs[] = {
	{ '0', 'f', (void *)axis_stop },
	{ 's', 'f', (void *)axis_stop },
	{ '?', 'f', (void *)status },
	{ 'b', 'I', (void *)test_break },
	{ 'x', 'I', (void *)set_x_speed },
	{ 'y', 'I', (void *)set_y_speed },
	{ 'X', 'I', (void *)move_x_to },
	{ 'Y', 'I', (void *)move_y_to },
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
	Serial << F("setup ok") << EOL;

    // timer 1 clock prescaler defaults to 011 = /64 , set to 001 = /1
    // /64 implies a frequency of 60Hz which is noisy , /8 = 4000Hz still audible, /1 = 32KHz inaubible.
    TCCR1B &= ~(1 << CS11);

    // timer 0 clock prescaler defaults to /64 and TOIE interrupt is used to update millis counter
    // we can set TIMSK0 bit OCIE0A and set OCR0A :
    // - to 125 to call OCIE0A every millisecond
    // - to  25 to call it  5 times per millisecond (every 200µs)
    OCR0A = 125;
    TIMSK0 |= (1 << OCIE0A);

    // Serial << F("TCCR0A ") << EOL; Serial << TCCR0A, HEX << EOL;
    // Serial << F("TCCR0B ") << EOL; Serial << TCCR0B, HEX << EOL;
    // Serial << F("TIMSK0 ") << EOL; Serial << TIMSK0, HEX << EOL;
    status();
}

long px, py;
unsigned long now = millis();

void loop() {
    if (X_speed != 0 || Y_speed != 0) {
        long dx = X_pos - px; px = X_pos;
        long dy = Y_pos - py; py = Y_pos;
        unsigned long dt = millis() - now; now = millis();
        float vx = X_dir * dx * 1000 / dt;
        float vy = Y_dir * dy * 1000 / dt;
        Serial << F("VX = ") << vx << F("\tVY = ") << vy << EOL;
#ifdef COMPUTE_ISR_DURATION
        Serial << nb_isr_call << F(" ISR calls , avg ") << (all_isr_call/nb_isr_call) << F(" us , max = ") << max_isr_call << F(" us") << EOL;
#endif
        delay(200);
    } else {
        sleep_mode();
    }
	handleInput();
}
