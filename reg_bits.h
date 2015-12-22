#define RF_com1a TCCR1A,6,2
#define RF_com1b TCCR1A,4,2
#define RF_cs1 TCCR1A,0,3

#define RF_com01a	TCCR0A, 6, 2
#define RF_com01b	TCCR0A, 4, 2
#define RF_wgm0l	TCCR0A, 0, 2
#define RF_foc0a	TCCR0B, 7, 1
#define RF_foc0b	TCCR0B, 6, 1

#define MASK_1 0x01
#define MASK_2 0x03
#define MASK_3 0x07
#define MASK_4 0x0f
#define MASK_5 0x1f
#define MASK_6 0x3f
#define MASK_7 0x7f
#define MASK_8 0xff

#define SET___(r, m, v) r = ((r & ^(m)) | (v))
#define SET__(r, o, s, v) SET___(r, MASK_ ## s << o, (v & MASK_ ## s) << o)
#define SET_(field, value) SET__(field, value)
#define SET(field, value) SET_(RF_ ## field, value)

#define CLEAR___(r, m) r &= ^(m)
#define CLEAR__(r, o, s) CLEAR___(r, MASK_ ## s << o)
#define CLEAR_(field) CLEAR__(field)
#define CLEAR(field) CLEAR_(RF_ ## field)

SET(com1a, 1)
SET(com1b, 2)
SET(cs1, 10)

CLEAR(cs1)
CLEAR(com1a)
CLEAR(com1b)
