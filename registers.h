#ifndef REGISTERS_H
#define REGISTERS_H

#include <Arduino.h>
#include <stddef.h>

// do what we want with ##
#define PRIMITIVE_CAT(x, y) x##y
#define CAT(x, y) PRIMITIVE_CAT(x, y)

// https://groups.google.com/forum/#!topic/comp.std.c/d-6Mj5Lko_s
#define PP_NARG(...) PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(_1,_2,_3,_4,_5,_6,_7,_8, N, ...) N
#define PP_RSEQ_N() 8,7,6,5,4,3,2,1,0

#define DECLARE_1(a1) \
	a1
#define DECLARE_2(a1, a2) \
	DECLARE_1(a2) \
	DECLARE_1(a1)
#define DECLARE_3(a1, a2, a3) \
	DECLARE_1(a3) \
	DECLARE_2(a1, a2)
#define DECLARE_4(a1, a2, a3, a4) \
	DECLARE_1(a4) \
	DECLARE_3(a1, a2, a3)
#define DECLARE_5(a1, a2, a3, a4, a5) \
	DECLARE_1(a5) \
	DECLARE_4(a1, a2, a3, a4)
#define DECLARE_6(a1, a2, a3, a4, a5, a6) \
	DECLARE_1(a6) \
	DECLARE_5(a1, a2, a3, a4, a5)
#define DECLARE_7(a1, a2, a3, a4, a5, a6, a7) \
	DECLARE_1(a7) \
	DECLARE_6(a1, a2, a3, a4, a5, a6)
#define DECLARE_8(a1, a2, a3, a4, a5, a6, a7, a8) \
	DECLARE_1(a8) \
	DECLARE_7(a1, a2, a3, a4, a5, a6, a7)

#define DECLARE(...) CAT(DECLARE_ , PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define BIT_(x) byte x:1;
#define BIT_1(x) byte x:1;
#define BIT_2(x) byte x:2;
#define BIT_3(x) byte x:3;
#define BIT_4(x) byte x:4;
#define BIT_5(x) byte x:5;
#define BIT_6(x) byte x:6;
#define BIT_7(x) byte x:7;
#define BIT_8(x) byte x:8;

#define BIT(x, ...) BIT_ ## __VA_ARGS__ (x)
#define RESERVED_BITS(n) BIT(CAT(reserved_, __COUNTER__), n)

#define RESERVED_BYTES(from, to) byte reserved_ ## from ## _ ## to[to - from + 1]

#define REGISTER(reg, ...) \
	union {			\
		byte reg;	\
		struct {	\
			DECLARE(__VA_ARGS__)	\
		};			\
	}

#define BITS_RANGE(reg) \
	byte reg ## 0:1;	\
	byte reg ## 1:1;	\
	byte reg ## 2:1;	\
	byte reg ## 3:1;	\
	byte reg ## 4:1;	\
	byte reg ## 5:1;	\
	byte reg ## 6:1;	\
	byte reg ## 7:1;

#if defined(__AVR_ATtinyX5__)
#include "registers.ATTinyX5.h"
#elif defined(__AVR_ATmega328P__)
struct _registers {
	#include "registers.ATMega328p.h"
} *registers_map;
byte *registers = (byte *)0x20;
typedef union _register_accessors {
	#include "registers.ATMega328p.h"
} registers_accessors;
#else
#error Architecture not yet implemented
#endif

#define R(reg) (((registers_accessors *)(registers + offsetof(struct _registers, reg)))->reg)
#define RF(reg, field) (((registers_accessors *)(registers + offsetof(struct _registers, reg)))->field)
#endif
