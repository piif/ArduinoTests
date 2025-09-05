
#include "myClock.h"

static volatile unsigned long my_clock = 0;
static volatile unsigned int  my_clock_period = 1000;

#ifdef COMPUTE_ISR_DURATION
volatile unsigned long myClockNbIsrCall = 0;
volatile unsigned long myClockAllIsrCall = 0;
volatile unsigned long myClockMaxIsrCall = 0;
#endif

// prepare timer0 to call "Compare Match A" interrupt handler every period ms 
void initMyClock(unsigned int period) {
    my_clock_period = period;

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
void startMyClock(unsigned int period = 0) {
    if (period != 0) {
        my_clock_period = period;
    }
    TIMSK0 |= (1 << OCIE0A);
    my_clock = 0;
}


ISR(TIMER0_COMPA_vect) {
    my_clock++;
    if (my_clock % my_clock_period != 0) {
        return;
    }

    cli();

#ifdef COMPUTE_ISR_DURATION
    unsigned long tic = micros();
#endif

    myClockCallback(my_clock);

#ifdef COMPUTE_ISR_DURATION
    unsigned long duration = micros() - tic;
    myClockNbIsrCall++;
    myClockAllIsrCall += duration;
    if (duration > myClockMaxIsrCall) {
        myClockMaxIsrCall = duration;
    }
#endif

    sei();
}
