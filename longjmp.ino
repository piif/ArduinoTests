#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
#else
#endif

#include <setjmp.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

jmp_buf mainScheduler;

typedef void schedulableFunc(struct _schedulable *sched);
typedef struct _schedulable {
	jmp_buf env;
	schedulableFunc *func;
} schedulable;

void newSchedulable(schedulable *s, schedulableFunc *f) {
	s->func = f;
	if (setjmp(mainScheduler) == 0) {
		f(s);
	}
}

void schedule(schedulable *s) {
	if (setjmp(mainScheduler) == 0) {
		longjmp(s->env, 1);
	}
}

#define yield() \
	if (setjmp(sched->env) == 0) longjmp(mainScheduler, 1)

#define SCHEDULABLE(func) \
	void func(schedulable *sched)

SCHEDULABLE(f1) {
	static byte b1 = 0;
	yield();
	for(;;) {
		b1 += 2;
		Serial.print("f1 : "); Serial.println(b1);
		Serial.flush();
		yield();
	}
}

SCHEDULABLE(f2) {
	yield();
	static byte b2 = 1;
	for(;;) {
		b2 += 2;
		Serial.print("f2 : "); Serial.println(b2);
		Serial.flush();
		yield();
	}
}

schedulable s1, s2a, s2b;

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);
	Serial.println(F("setup"));
	Serial.flush();
	newSchedulable(&s1, f1);
	newSchedulable(&s2a, f2);
	newSchedulable(&s2b, f2);
//	START_SCHEDULABLE(f1);
//	START_SCHEDULABLE(f2);
	Serial.println(F("setup ok"));
	Serial.flush();
}

void loop() {
	Serial.println(F("loop"));
	Serial.flush();
	schedule(&s1);
	schedule(&s2a);
	schedule(&s2b);
//	SCHEDULE(f1);
//	SCHEDULE(f2);
	delay(1000);
}
