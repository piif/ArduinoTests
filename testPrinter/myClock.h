#include <Arduino.h>

#include "project.h"

// prepare timer0 to call "Compare Match A" interrupt handler every period ms 
void initMyClock(unsigned int period);

// activate interrupt
void stopMyClock();

// disable interrupt
void startMyClock();

// to be defined by caller
extern void myClockCallback(unsigned long clock);

#ifdef COMPUTE_ISR_DURATION
extern volatile unsigned long myClockNbIsrCall;
extern volatile unsigned long myClockAllIsrCall;
extern volatile unsigned long myClockMaxIsrCall;
#endif
