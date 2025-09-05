#include <Arduino.h>

#include "project.h"

// prepare timer0 to call "Compare Match A" interrupt handler every period ms 
void initMyClock(unsigned int period);

// activate interrupt any 'period' ms (if 0, period stays as defined by init or last start)
void startMyClock(unsigned int period = 0);

// disable interrupt
void stopMyClock();

// to be defined by caller
extern void myClockCallback(unsigned long clock);

#ifdef COMPUTE_ISR_DURATION
extern volatile unsigned long myClockNbIsrCall;
extern volatile unsigned long myClockAllIsrCall;
extern volatile unsigned long myClockMaxIsrCall;
#endif
