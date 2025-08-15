#include <Arduino.h>
#include "strop.h"

#include "PID_v1.h"

#include "axis.h"
#include "tools.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define PID_PERIOD 10

// X position PID parameters
volatile double xp_input = 0, xp_output = 0, xp_setpoint = -1;
#define XP_PID_P 1
#define XP_PID_I 0.0
#define XP_PID_D 0.0
double xpp = XP_PID_P, xpi = XP_PID_I, xpd = XP_PID_D;
PID xp_pid(&xp_input, &xp_output, &xp_setpoint, xpp, xpi, xpd, DIRECT);

// X speed PID parameters
volatile double xs_input = 0, xs_output = 0, xs_setpoint = 0;
// approximation of ratio between pwn duty cycle and speed in step/ms
#define XS_PID_P 1 // 20
#define XS_PID_I 0.0
#define XS_PID_D 0.0
double xsp = XS_PID_P, xsi = XS_PID_I, xsd = XS_PID_D;
PID xs_pid(&xs_input, &xs_output, &xs_setpoint, xsp, xsi, xsd, DIRECT);

// original position during a move
long xi, yi;
volatile long x_prev;
// desired target position , set thru serial input thus not "volatile"
long xf, yf;
// distance to move of , distance to 1st quarter (end of acceleration) and 3rd quarter (start of deceleration), current step
// long xd, xq1, xq2, xq3, xqb, xstep, xiter, xv;
// long yd, yq1, yq2, yq3, ystep, yiter, yv;

#define BUF_MAX 120
volatile int  bufp[BUF_MAX], bufas[BUF_MAX], bufts[BUF_MAX];
volatile int bufw[BUF_MAX];
volatile long i=0;
volatile short dir;

void move_x_to(int p) {
    if (p == X_pos) {
        return;
    }

    i = 0;
    xi = x_prev = X_pos; xf = p;
    if (xi < xf) {
        dir = 1;
        xp_setpoint = xf;
    } else {
        dir = -1;
        xp_setpoint = xf;
    }
    xs_setpoint = xs_output = M_X_SPEED_START-M_X_SPEED_MIN;
    axis_x_set_speed(xs_output+M_X_SPEED_MIN, dir);
    startMyClock();
}

void move_y_to(int p) {
}

volatile unsigned long my_clock = 0;

#ifdef COMPUTE_ISR_DURATION
volatile unsigned long nb_isr_call = 0;
volatile unsigned long all_isr_call = 0;
volatile unsigned long max_isr_call = 0;
#endif

// prepare timer0 to call "Compare Match A" interrupt handler every 1ms 
void initMyClock() {
    // timer 1 clock prescaler defaults to 011 = /64 , set to 001 = /1
    // /64 implies a frequency of 60Hz which is noisy , /8 = 4000Hz still audible, /1 = 32KHz inaubible.
    TCCR1B &= ~(1 << CS11);

    // timer 0 clock prescaler defaults to /64 and TOIE interrupt is used to update millis counter
    // we can set TIMSK0 bit OCIE0A and set OCR0A :
    // - to 125 to call OCIE0A every millisecond
    // - to  25 to call it  5 times per millisecond (every 200µs)
    OCR0A = 125;
}
// activate interrupt
void stopMyClock() {
    TIMSK0 &= ~(1 << OCIE0A);
}
// disable interrupt
void startMyClock() {
    TIMSK0 |= (1 << OCIE0A);
    my_clock = 0;
}

void my_head_callback() {
    xp_setpoint = -1;
}

ISR(TIMER0_COMPA_vect) {
    cli();
#ifdef COMPUTE_ISR_DURATION
    unsigned long tic = micros();
#endif

    if (my_clock % PID_PERIOD == 0) {
        if (xp_setpoint != -1) { // X is moving, compute its PID
            // compute position error : value is <0 if we went beyond the target
            xp_input = X_pos;
            // compute actual speed and related error
            double actual_speed = dir * (X_pos - x_prev);
            x_prev = X_pos;

            if (abs(xp_setpoint - xp_input) < 3 && abs(actual_speed) < 3) {
                xp_setpoint = -1;
                axis_x_set_speed(0);
            } else {
                // deduce target speed
                xp_pid.Compute();
                // xs_setpoint = xp_output;
                // xs_input = actual_speed;
                // xs_pid.Compute();
                xs_output=xp_output;
                if (xs_output < 0) {
                    // see in setup function : pwm is constrained in a limited range
                    // which has to be converted here
                    axis_x_set_speed(M_X_SPEED_MIN-xs_output, -dir);
                } else {
                    axis_x_set_speed(M_X_SPEED_MIN+xs_output, dir);
                }
            }

            bufp[i % BUF_MAX] = xp_input;
            bufas[i % BUF_MAX]= actual_speed;
            bufts[i % BUF_MAX]= xp_output;
            bufw[i % BUF_MAX] = xs_output;
            i++;
        }
    }
    my_clock++;

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

void status() {
    axis_status();

	Serial << F("xp_pid in=")
        << xp_input << F(" , out=") << xp_output << F(" , set=") << xp_setpoint << F(" / P=")
        << xp_pid.GetKp() << F(" , I=") << xp_pid.GetKi() << F(" , D=") << xp_pid.GetKd() << EOL;
	Serial << F("xs_pid in=")
        << xs_input << F(" , out=") << xs_output << F(" , set=") << xs_setpoint << F(" / P=")
        << xs_pid.GetKp() << F(" , I=") << xs_pid.GetKi() << F(" , D=") << xs_pid.GetKd() << EOL;

#ifdef COMPUTE_ISR_DURATION
        Serial << nb_isr_call << F(" Timer calls , avg ") << (all_isr_call/nb_isr_call) << F(" us , max = ") << max_isr_call << F(" us") << EOL;
#endif

    if (i > 0) {
        Serial << F("i=") << i << F("\npos\ta speed\tt speed\tpwm\n");
        int j = (i < BUF_MAX) ? 0 : (i % BUF_MAX);
        do {
            Serial << bufp[j] << '\t'<< bufas[j] << '\t'<< bufts[j] << '\t' << bufw[j] << EOL;
            j = (j+1) % BUF_MAX;
        } while (j != i % BUF_MAX);
    }
}

void setxpp(int i) {
    xpp = 0.001*i;
    xp_pid.SetTunings(xpp, xpi, xpd, P_ON_E);
}
void setxpi(int i) {
    xpi = 0.001*i;
    xp_pid.SetTunings(xpp, xpi, xpd, P_ON_E);
}
void setxpd(int i) {
    xpd = 0.001*i;
    xp_pid.SetTunings(xpp, xpi, xpd, P_ON_E);
}
void setxsp(int i) {
    xsp = 0.001*i;
    xs_pid.SetTunings(xsp, xsi, xsd, P_ON_E);
}
void setxsi(int i) {
    xsi = 0.001*i;
    xs_pid.SetTunings(xsp, xsi, xsd, P_ON_E);
}
void setxsd(int i) {
    xsd = 0.001*i;
    xs_pid.SetTunings(xsp, xsi, xsd, P_ON_E);
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
	{ 'p', 'I', (void *)setxpp },
	{ 'i', 'I', (void *)setxpi },
	{ 'd', 'I', (void *)setxpd },
	{ 'P', 'I', (void *)setxsp },
	{ 'I', 'I', (void *)setxsi },
	{ 'D', 'I', (void *)setxsd },
	// { 'X', 'I', (void *)axis_x_move_of },
	// { 'Y', 'I', (void *)axis_y_move_of },
	{ 'f', 'f', (void *)feed_paper },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

    axis_begin();

    set_sleep_mode(SLEEP_MODE_IDLE);

    xp_pid.SetSampleTime(PID_PERIOD);
    xp_pid.SetMode(AUTOMATIC);
    // xp_pid outputs a target speed in step/ms
    xp_pid.SetOutputLimits(-100, 100);

    xs_pid.SetSampleTime(PID_PERIOD);
    xs_pid.SetMode(AUTOMATIC);
    // xs_pid outputs a target duty cycle but we now nothing moves under M_X_SPEED_MIN
    // in the other hand, we want to go backward if needed
    // thus, we want to output something in [ -M_X_SPEED_MAX : -M_X_SPEED_MIN ] + [ M_X_SPEED_MIN : M_X_SPEED_MAX ]
    // thus, we "translate" this into [ -|M_X_SPEED_MAX-M_X_SPEED_MIN| : +|M_X_SPEED_MAX-M_X_SPEED_MIN| ] and will convert 
    // after each pid computation
    xs_pid.SetOutputLimits(M_X_SPEED_MIN-M_X_SPEED_MAX, M_X_SPEED_MAX-M_X_SPEED_MIN);
    initMyClock();

    head_callback = my_head_callback;

	registerInput(sizeof(inputs), inputs);

	Serial << F("setup ok") << EOL;

    status();
}

long px, py;
unsigned long now = millis();

void loop() {
//     if (X_speed != 0 || Y_speed != 0) {
//         long dx = X_pos - px; px = X_pos;
//         long dy = Y_pos - py; py = Y_pos;
//         unsigned long dt = millis() - now; now = millis();
//         float vx = X_dir * dx * 1000 / dt;
//         float vy = Y_dir * dy * 1000 / dt;
//         Serial << F("VX = ") << vx << F("\tVY = ") << vy << EOL;
// #ifdef COMPUTE_ISR_DURATION
//         Serial << nb_isr_call << F(" ISR calls , avg ") << (all_isr_call/nb_isr_call) << F(" us , max = ") << max_isr_call << F(" us") << EOL;
// #endif
//         delay(200);
//     } else {
        sleep_mode();
    // }
	handleInput();
}
