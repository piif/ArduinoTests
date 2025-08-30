#include <Arduino.h>
#include "strop.h"

#include "PID_v1.h"

#include "axis.h"
#include "tools.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define PID_PERIOD 10

#define COMPUTE_ISR_DURATION

// X position PID parameters
volatile double xp_input = 0, xp_output = 0, xp_setpoint = -1;
#define XP_PID_P 1.0
#define XP_PID_I 0.0
#define XP_PID_D 0.0
double xpp = XP_PID_P, xpi = XP_PID_I, xpd = XP_PID_D;
// PID xp_pid(&xp_input, &xp_output, &xp_setpoint, xpp, xpi, xpd, P_ON_E, DIRECT);

long x_prev;

// X speed PID parameters
volatile double xs_input = 0, xs_output = 0, xs_setpoint = 0;
volatile double xs_setpoint_prev = 0;
// approximation of ratio between pwn duty cycle and speed in step/ms
#define XS_PID_P 1.0 // 3.0
#define XS_PID_I 0.5 // 6.0
#define XS_PID_D 0.0
double xsp = XS_PID_P, xsi = XS_PID_I, xsd = XS_PID_D;
PID xs_pid(&xs_input, &xs_output, &xs_setpoint, xsp, xsi, xsd, P_ON_E, DIRECT);


// Y position PID parameters
volatile double yp_input = 0, yp_output = 0, yp_setpoint = -1;
#define YP_PID_P 1.0
#define YP_PID_I 0.0
#define YP_PID_D 0.0
double ypp = YP_PID_P, ypi = YP_PID_I, ypd = YP_PID_D;
// PID yp_pid(&yp_input, &yp_output, &yp_setpoint, xpp, xpi, xpd, P_ON_E, DIRECT);

// Y speed PID parameters
volatile double ys_input = 0, ys_output = 0, ys_setpoint = 0;
volatile double ys_setpoint_prev = 0;
// approximation of ratio between pwn duty cycle and speed in step/ms
#define YS_PID_P 0.1
#define YS_PID_I 1.0
#define YS_PID_D 0.0
double ysp = YS_PID_P, ysi = YS_PID_I, ysd = YS_PID_D;
PID ys_pid(&ys_input, &ys_output, &ys_setpoint, ysp, ysi, ysd, P_ON_E, DIRECT);

long y_prev;

#define SPEED_RING_LEN 5
static long xring[SPEED_RING_LEN];
static int  xring_idx=0;
static long xring_sum=0;

static long yring[SPEED_RING_LEN];
static int  yring_idx=0;
static long yring_sum=0;

#define BUF_MAX 120
volatile int    bufp[BUF_MAX];
volatile double bufe[BUF_MAX];
volatile short  bufs[BUF_MAX], bufw[BUF_MAX];
volatile long bufi=0;
volatile short dir;
volatile byte moving;

void move_x_to(int p) {
    if (p == X_pos) {
        return;
    }

    bufi = 0;
    x_prev = X_pos;
    xring_sum = 0;
    for(byte i=0; i<SPEED_RING_LEN; i++) {
        xring[xring_idx] = 0;
    }
    xp_setpoint = p;
    xs_setpoint = xs_setpoint_prev = xs_output = 0;
    axis_x_set_speed(0);
    moving = 1;
    startMyClock();
}

void move_y_to(int p) {
    if (p == Y_pos) {
        return;
    }

    bufi = 0;
    y_prev = Y_pos;
    yring_sum = 0;
    for(byte i=0; i<SPEED_RING_LEN; i++) {
        yring[yring_idx] = 0;
    }
    yp_setpoint = p;
    ys_setpoint = ys_setpoint_prev = ys_output = 0;
    axis_y_set_speed(0);
    moving = 1;
    startMyClock();
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
    xs_setpoint = -1;
    stopMyClock();
}

ISR(TIMER0_COMPA_vect) {
    my_clock++;
    if (my_clock % PID_PERIOD != 0) {
        return;
    }

    cli();

#ifdef COMPUTE_ISR_DURATION
    unsigned long tic = micros();
#endif

    if (xp_setpoint != -1) {
        double err = xp_setpoint - X_pos;

        // get current speed (average on 5 last measures)
        long actual_speed = X_pos - x_prev;
        x_prev = X_pos;
        xring_sum -= xring[xring_idx];
        xring_sum += actual_speed;
        xring[xring_idx] = actual_speed;
        xring_idx = (xring_idx+1) % SPEED_RING_LEN;

        // "end of move" logic
        if (abs(err) < 5 && abs(xs_setpoint) < 5) {
            stopMyClock();
            xp_setpoint = -1;
            xs_setpoint = -1;
            axis_x_set_speed(0);
            // TODO : set moving to 0 only if other axis is not moving (or use 2 flags)
            moving = 0;

            bufp[bufi % BUF_MAX] = err;
            bufe[bufi % BUF_MAX] = actual_speed;
            bufs[bufi % BUF_MAX] = 0;
            bufw[bufi % BUF_MAX] = 0;
            bufi++;

            return;
        }

        xs_input = xring_sum / SPEED_RING_LEN;

        // external loop : position -> target speed

        // position PD (D parameter = -Kd*xs_input)
        xs_setpoint = xpp * err - xpd * xs_input;

        // Limitation par distance (anticipation freinage)
        double speed_limit = sqrt(2.0 * M_X_ACCEL_MAX * abs(err));
        speed_limit = min(speed_limit, M_X_SPEED_MAX);
        xs_setpoint = constrain(xs_setpoint, -speed_limit, +speed_limit);

        // (option) Slew-rate on speed setpoint
        xs_setpoint = constrain(xs_setpoint, xs_setpoint_prev - M_X_ACCEL_MAX, xs_setpoint_prev + M_X_ACCEL_MAX);
        xs_setpoint_prev = xs_setpoint;

        // internal loop : speed -> PWM
        xs_pid.Compute();
        // int pwm = constrain(ff_kv * v_sp + ff_ka * a_sp + xs_output, 0, 255);
        // // a_sp ≈ (v_sp - v_sp_prev)/dt if acceleration feedforward required
        if (xs_output < 0) {
            // see in setup function : pwm is constrained in a limited range
            // which has to be converted here
            axis_x_set_speed(M_X_PWM_MIN-xs_output, -1);
        } else  if (ys_output > 0) {
            axis_x_set_speed(M_X_PWM_MIN+xs_output, 1);
        } else {
            axis_x_set_speed(0);
        }

        bufp[bufi % BUF_MAX] = err;
        bufe[bufi % BUF_MAX] = actual_speed;
        bufs[bufi % BUF_MAX] = xs_setpoint;
        bufw[bufi % BUF_MAX] = xs_output;
        bufi++;
    }

    if (yp_setpoint != -1) {
        double err = yp_setpoint - Y_pos;

        // get current speed (average on 5 last measures)
        long actual_speed = Y_pos - y_prev;
        y_prev = Y_pos;
        yring_sum -= yring[yring_idx];
        yring_sum += actual_speed;
        yring[yring_idx] = actual_speed;
        yring_idx = (yring_idx+1) % SPEED_RING_LEN;

        // "end of move" logic
        if (abs(err) < 5 && abs(ys_setpoint) < 5) {
            stopMyClock();
            yp_setpoint = -1;
            ys_setpoint = -1;
            axis_y_set_speed(0);
            // TODO : set moving to 0 only if other axis is not moving (or use 2 flags)
            moving = 0;

            bufp[bufi % BUF_MAX] = err;
            bufe[bufi % BUF_MAX] = actual_speed;
            bufs[bufi % BUF_MAX] = 0;
            bufw[bufi % BUF_MAX] = 0;
            bufi++;

            return;
        }

        ys_input = yring_sum / SPEED_RING_LEN;

        // external loop : position -> target speed

        // position PD (D parameter = -Kd*ys_input)
        ys_setpoint = ypp * err - ypd * ys_input;

        // Limitation par distance (anticipation freinage)
        double speed_limit = sqrt(2.0 * M_Y_ACCEL_MAX * abs(err));
        speed_limit = min(speed_limit, M_Y_PWM_MAX);
        ys_setpoint = constrain(ys_setpoint, -speed_limit, +speed_limit);

        // (option) Slew-rate on speed setpoint
        ys_setpoint = constrain(ys_setpoint, ys_setpoint_prev - M_Y_ACCEL_MAX, ys_setpoint_prev + M_Y_ACCEL_MAX);
        ys_setpoint_prev = ys_setpoint;

        // internal loop : speed -> PWM
        ys_pid.Compute();
        // int pwm = constrain(ff_kv * v_sp + ff_ka * a_sp + ys_output, 0, 255);
        // // a_sp ≈ (v_sp - v_sp_prev)/dt if acceleration feedforward required
        if (ys_output < 0) {
            // see in setup function : pwm is constrained in a limited range
            // which has to be converted here
            axis_y_set_speed(M_Y_PWM_MIN-ys_output, -1);
        } else if (ys_output > 0) {
            axis_y_set_speed(M_Y_PWM_MIN+ys_output, 1);
        } else {
            axis_y_set_speed(0);
        }

        bufp[bufi % BUF_MAX] = err;
        bufe[bufi % BUF_MAX] = actual_speed;
        bufs[bufi % BUF_MAX] = ys_setpoint;
        bufw[bufi % BUF_MAX] = ys_output;
        bufi++;
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

void feed_paper() {
    Serial << F("feed_paper") << EOL; axis_status();

    if (!head_max) {
        axis_x_set_speed(M_X_PWM_AVG, 1);
        WAIT_FOR(head_max);
        Serial << F("after 1st move right") << EOL; axis_status();
    }
    axis_x_set_speed(M_X_PWM_AVG, -1);
    WAIT_FOR(X_pos <= X_MAX - 100);
    axis_stop();

    axis_x_set_speed(M_X_PWM_AVG, 1);
    WAIT_FOR(head_max);

    // if paper detected, eject it until bottom border
    if (paper_present) {
        axis_y_set_speed(M_Y_PWM_AVG, -1);
        WAIT_FOR(!paper_present);
        axis_stop();
    }
    // then, feed until it's detected
    axis_y_set_speed(M_Y_PWM_AVG, 1);
    WAIT_FOR(paper_present);
    axis_stop();

    // this is position for bottom of page
    Y_pos = 0;

    Serial << F("bottom of page") << EOL; axis_status();

    // find top border
    axis_y_set_speed(M_Y_PWM_AVG, 1);
    WAIT_FOR(!paper_present);
    axis_stop();

    Serial << F("top of page") << EOL; axis_status();

    // go back a few (~ 1cm)
    long bottom_margin = Y_pos - 1000;
    axis_y_set_speed(M_Y_PWM_AVG, -1);
    WAIT_FOR(Y_pos <= bottom_margin);
    axis_stop();
    if (!paper_present) {
        Serial << F("end of paper before bottom margin of page ?!?") << EOL;
        return;
    }

#ifdef CAN_DETECT_LEFT_BORDER
    // look for left border
    axis_x_set_speed(M_X_PWM_AVG, -1);
    WAIT_FOR(!paper_present);
    axis_stop();

    Serial << F("left of page") << EOL; axis_status();
#endif // CAN_DETECT_LEFT_BORDER

    // go back to bottom
    axis_y_set_speed(M_Y_PWM_AVG, -1);
    WAIT_FOR(!paper_present);
    axis_stop();
    // if (!paper_present) {
    //     Serial << F("end of paper before top of page") << EOL;
    //     return;
    // }
}

void status() {
    axis_status();

    if (!moving) {
    	Serial << F("NOT ");
    }
    Serial << F("MOVING\n");
	Serial << F("xp_pid in=")
        << xp_input << F(" , out=") << xp_output << F(" , set=") << xp_setpoint 
        << F(" / P=") << xpp << F(" , D=") << xpd
        << EOL;
	Serial << F("xs_pid in=")
        << xs_input << F(" , out=") << xs_output << F(" , set=") << xs_setpoint
        << F(" / P=") << xs_pid.GetKp() << F(" , I=") << xs_pid.GetKi() << F(" , D=") << xs_pid.GetKd() << EOL;

	Serial << F("yp_pid in=")
        << yp_input << F(" , out=") << yp_output << F(" , set=") << yp_setpoint 
        << F(" / P=") << ypp << F(" , D=") << ypd
        << EOL;
	Serial << F("ys_pid in=")
        << ys_input << F(" , out=") << ys_output << F(" , set=") << ys_setpoint
        << F(" / P=") << ys_pid.GetKp() << F(" , I=") << ys_pid.GetKi() << F(" , D=") << ys_pid.GetKd() << EOL;

#ifdef COMPUTE_ISR_DURATION
        Serial << nb_isr_call << F(" Timer calls , avg ") << (all_isr_call/nb_isr_call) << F(" us , max = ") << max_isr_call << F(" us") << EOL;
#endif

    if (bufi > 0) {
        Serial << F("bufi=") << bufi << F("\ni\tpos\ta speed\tt speed\tpwm\n"); // 
        int j = (bufi < BUF_MAX) ? 0 : (bufi % BUF_MAX);
        do {
            Serial << j << '\t' << bufp[j] << '\t' << bufe[j] << '\t' << bufs[j] << '\t' << bufw[j] << EOL;
            j = (j+1) % BUF_MAX;
        } while (j != bufi % BUF_MAX);
    }
}

void setxpp(int i) {
    xpp = 0.001*i;
    // xp_pid.SetTunings(xpp, xpi, xpd, P_ON_E);
}
void setxpi(int i) {
    xpi = 0.001*i;
    // xp_pid.SetTunings(xpp, xpi, xpd, P_ON_E);
}
void setxpd(int i) {
    xpd = 0.001*i;
    // xp_pid.SetTunings(xpp, xpi, xpd, P_ON_E);
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


void setypp(int i) {
    ypp = 0.001*i;
    // yp_pid.SetTunings(ypp, ypi, ypd, P_ON_E);
}
void setypi(int i) {
    ypi = 0.001*i;
    // yp_pid.SetTunings(ypp, ypi, ypd, P_ON_E);
}
void setypd(int i) {
    ypd = 0.001*i;
    // yp_pid.SetTunings(ypp, ypi, ypd, P_ON_E);
}
void setysp(int i) {
    ysp = 0.001*i;
    ys_pid.SetTunings(ysp, ysi, ysd, P_ON_E);
}
void setysi(int i) {
    ysi = 0.001*i;
    ys_pid.SetTunings(ysp, ysi, ysd, P_ON_E);
}
void setysd(int i) {
    ysd = 0.001*i;
    ys_pid.SetTunings(ysp, ysi, ysd, P_ON_E);
}

InputItem inputs[] = {
	{ '0', 'f', (void *)axis_stop },
	{ 's', 'f', (void *)axis_stop },
	{ '?', 'f', (void *)status },
	{ 'x', 'I', (void *)set_x_speed },
	{ 'y', 'I', (void *)set_y_speed },
	{ 'X', 'I', (void *)move_x_to },
	{ 'Y', 'I', (void *)move_y_to },
	{ 'p', 'I', (void *)setypp },
	{ 'i', 'I', (void *)setypi },
	{ 'd', 'I', (void *)setypd },
	{ 'P', 'I', (void *)setysp },
	{ 'I', 'I', (void *)setysi },
	{ 'D', 'I', (void *)setysd },
	// { 'X', 'I', (void *)axis_x_move_of },
	// { 'Y', 'I', (void *)axis_y_move_of },
	{ 'f', 'f', (void *)feed_paper },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

    axis_begin();

    set_sleep_mode(SLEEP_MODE_IDLE);

    // xp_pid.SetSampleTime(PID_PERIOD);
    // // xp_pid outputs a target speed in step/ms
    // xp_pid.SetOutputLimits(-50, 50);
    // xp_pid.SetMode(AUTOMATIC);

    xs_pid.SetSampleTime(PID_PERIOD);
    // xs_pid outputs a target duty cycle but we now nothing moves under M_X_PWM_MIN
    // in the other hand, we want to go backward if needed
    // thus, we want to output something in [ -M_X_PWM_MAX : -M_X_PWM_MIN ] + [ M_X_PWM_MIN : M_X_PWM_MAX ]
    // thus, we "translate" this into [ -|M_X_PWM_MAX-M_X_PWM_MIN| : +|M_X_PWM_MAX-M_X_PWM_MIN| ] and will convert 
    // after each pid computation
    // xs_pid.SetOutputLimits(0, 255);
    xs_pid.SetOutputLimits(M_X_PWM_MIN-M_X_PWM_MAX, M_X_PWM_MAX-M_X_PWM_MIN);
    xs_pid.SetMode(AUTOMATIC);

    ys_pid.SetSampleTime(PID_PERIOD);
    ys_pid.SetOutputLimits(M_Y_PWM_MIN-M_Y_PWM_MAX, M_Y_PWM_MAX-M_Y_PWM_MIN);
    ys_pid.SetMode(AUTOMATIC);

    initMyClock();

    head_callback = my_head_callback;

	registerInput(sizeof(inputs), inputs);

	Serial << F("setup ok") << EOL;

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
        Serial << F("VX = ") << X_speed << F(" / ") << vx << F("\tVY = ") << Y_speed << F(" / ") << vy << EOL;
#ifdef COMPUTE_ISR_DURATION
        Serial << nb_isr_call << F(" Timer calls , avg ") << (all_isr_call/nb_isr_call) << F(" us , max = ") << max_isr_call << F(" us") << EOL;
#endif
        delay(200);
    } else {
        sleep_mode();
    }
	handleInput();
}
