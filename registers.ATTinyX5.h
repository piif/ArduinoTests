#include "registers.h"

struct _registers {
	/* 0x00 - 0x02 */	RESERVED_BYTES(0x00, 0x02);
	/* 0x03 */	REGISTER(adcsrb, BIT(bin), BIT(acme), BIT(ipr), RESERVED_BITS(2), BIT(adts2), BIT(adts1), BIT(adts0));
	/* 0x05 */	word adc;
	/* 0x06 */	REGISTER(adcsra, BIT(aden), BIT(adsc), BIT(adate), BIT(adif), BIT(adie), BIT(adps2), BIT(adps1), BIT(adps0));
	/* 0x07 */	REGISTER(admux, BIT(refs1), BIT(refs0), BIT(adlar), BIT(refs2), BIT(mux3), BIT(mux2), BIT(mux1), BIT(mux0));
	/* 0x08 */	REGISTER(acsr, BIT(acd), BIT(acbg), BIT(aco), BIT(aci), BIT(acie), RESERVED_BITS(1), BIT(acis1), BIT(acis0));
	/* 0x09 - 0x0B */	RESERVED_BYTES(0x09, 0x0B);
	/* 0x0C */	RESERVED_BYTES(0x0C, 0x0C);
	/* 0x0D */	REGISTER(usicr, BIT(usisie), BIT(usioie), BIT(usiwm1), BIT(usiwm0), BIT(usics1), BIT(usics0), BIT(usiclk), BIT(usitc));
	/* 0x0E */	REGISTER(usisr, 	BIT(usisif), BIT(usioif), BIT(usipf), BIT(usidc), BIT(usicnt3), BIT(usicnt2), BIT(usicnt1), BIT(usicnt0));
	/* 0x0F */	byte usidr;	// USI Data Register
	/* 0x10 */	byte usibr;	// USI Buffer Register
	/* 0x11 */	byte gpior0;	// General Purpose I/O Register 0
	/* 0x12 */	byte gpior1;	// General Purpose I/O Register 1
	/* 0x13 */	byte gpior2;	// General Purpose I/O Register 2
	/* 0x14 */	REGISTER(didr0, RESERVED_BITS(2), BIT(adc0d), BIT(adc2d), BIT(adc3d), BIT(adc1d), BIT(ain1d), BIT(ain0d));
	/* 0x15 */	REGISTER(pcmsk, RESERVED_BITS(2), BIT(pcint5), BIT(pcint4), BIT(pcint3), BIT(pcint2), BIT(pcint1), BIT(pcint0));
	/* 0x16 */	REGISTER(pinb, 	RESERVED_BITS(2), BIT(pinb5), BIT(pinb4), BIT(pinb3), BIT(pinb2), BIT(pinb1), BIT(pinb0));
	/* 0x17 */	REGISTER(ddrb, 	RESERVED_BITS(2), BIT(ddb5), BIT(ddb4), BIT(ddb3), BIT(ddb2), BIT(ddb1), BIT(ddb0));
	/* 0x18 */	REGISTER(portb, RESERVED_BITS(2), BIT(portb5), BIT(portb4), BIT(portb3), BIT(portb2), BIT(portb1), BIT(portb0));
	/* 0x19 - 0x1B */	RESERVED_BYTES(0x19, 0x1B);
	/* 0x1C */	REGISTER(eecr, RESERVED_BITS(2), BIT(eepm1), BIT(eepm0), BIT(eerie), BIT(eempe), BIT(eepe), BIT(eere));
	/* 0x1D */	byte eedr;	// EEPROM Data Register
	/* 0x1F */	word eear;
	/* 0x20 */	REGISTER(prr, RESERVED_BITS(2), BIT(prtim1), BIT(prtim0), BIT(prusi), BIT(pradc));
	/* 0x21 */	REGISTER(wdtcr, BIT(wdif), BIT(wdie), BIT(wdp3), BIT(wdce), BIT(wde), BIT(wdp2), BIT(wdp1), BIT(wdp0));
	/* 0x22 */	REGISTER(dwdr, BITS_RANGE(dwdr));
	/* 0x23 */	REGISTER(dtps1, RESERVED_BITS(6), BIT(dtps11), BIT(dtps10));
	/* 0x24 */	REGISTER(dt1b, BIT(dt1bh, 4), BIT(dt1bl, 4));
	/* 0x25 */	REGISTER(dt1a, BIT(dt1ah, 4), BIT(dt1al, 4));
	/* 0x26 */	REGISTER(clkpr, BIT(clkpce), RESERVED_BITS(3), BIT(clkps, 4));
	/* 0x27 */	REGISTER(pllcsr, BIT(lsm)	RESERVED_BITS(4), BIT(pcke), BIT(plle), BIT(plock));
	/* 0x28 */	byte ocr0b;	// Timer/Counter0 Output Compare Register B
	/* 0x29 */	byte àcr0a;	// Timer/Counter0 Output Compare Register A
	/* 0x2A */	REGISTER(tccr0a, BIT(com0a, 2), BIT(com0b, 2), RESERVED_BITS(2), BIT(wgm0, 2));
	/* 0x2B */	byte ocr1b;	// Timer/Counter1 Output Compare Register B
	/* 0x2C */	REGISTER(gtccr, BIT(tsm), BIT(pwm1b), BIT(com1b, 2), BIT(foc1b), BIT(foc1a), BIT(psr1), BIT(psr0));
	/* 0x2D */	byte ocr1c;	// Timer/Counter1 Output Compare Register C
	/* 0x2E */	byte ocr1a;	// Timer/Counter1 Output Compare Register A
	/* 0x2F */	byte tcnt1;	// Timer/Counter1
	/* 0x30 */	REGISTER(tccr1, BIT(ctc1), BIT(pwm1a), BIT(com1a, 2), BIT(cs1, 4));
	/* 0x31 */	byte osccal;	// Oscillator Calibration Register
	/* 0x32 */	byte tcnt0;	// Timer/Counter0
	/* 0x33 */	REGISTER(tccr0b, BIT(foc0a), BIT(foc0b), RESERVED_BITS(2), BIT(wgm02), BIT(cs0, 3));
	/* 0x34 */	REGISTER(mcusr, RESERVED_BITS(4), BIT(wdrf), BIT(borf), BIT(extrf), BIT(porf));
	/* 0x35 */	REGISTER(mcucr,  BIT(bods), BIT(pud), BIT(se), BIT(sm1), RESERVED_BITS(1), BIT(sm0), BIT(bodse), BIT(isc0, 2));
	/* 0x36 */	RESERVED_BYTES(0x36, 0x36);
	/* 0x37 */	REGISTER(spmcsr, RESERVED_BITS(2), BIT(rsig), BIT(ctpb), BIT(rflb), BIT(pgwrt), BIT(pgers), BIT(spmen));
	/* 0x38 */	REGISTER(tifr, RESERVED_BITS(1), BIT(ocf1a), BIT(ocf1b), BIT(ocf0a), BIT(ocf0b), BIT(tov1), BIT(tov0), RESERVED_BITS(1));
	/* 0x39 */	REGISTER(timsk, RESERVED_BITS(1), BIT(ocie1a), BIT(ocie1b), BIT(ocie0a), BIT(ocie0b), BIT(toie1), BIT(toie0), RESERVED_BITS(1));
	/* 0x3A */	REGISTER(gifr, RESERVED_BITS(1), BIT(intf0), BIT(pcif), RESERVED_BITS(5));
	/* 0x3B */	REGISTER(gimsk, RESERVED_BITS(1), BIT(int0), BIT(pcie), RESERVED_BITS(5));
	/* 0x3C */	RESERVED_BYTES(0x3C, 0x3C);
	/* 0x3D */	word sp;
	/* 0x3F */	REGISTER(sreg, BIT(i), BIT(t), BIT(h), BIT(s), BIT(v), BIT(n), BIT(z), BIT(c));
} *registers = (struct _registers*)0x20;

typedef void (*interrupt_callback)();

typedef enum {
	V_RESET, // External Pin, Power-on Reset, Brown-out Reset, Watchdog Reset
	V_INT0, // External Interrupt Request 0
	V_PCINT0, // Pin Change Interrupt Request 0
	V_TIMER1_COMPA, // Timer/Counter1 Compare Match A
	V_TIMER1_OVF, // Timer/Counter1 Overflow
	V_TIMER0_OVF, // Timer/Counter0 Overflow
	V_EE_RDY, // EEPROM Ready
	V_ANA_COMP, // Analog Comparator
	V_ADC, // ADC Conversion Complete
	V_TIMER1_COMPB, // Timer/Counter1 Compare Match B
	V_TIMER0_COMPA, // Timer/Counter0 Compare Match A
	V_TIMER0_COMPB, // Timer/Counter0 Compare Match B
	V_WDT, // Watchdog Time-out
	V_USI_START, // USI START
	V_USI_OVF // USI Overflow
} interrupt_names;

interrupt_callback *interrupt_vectors = 0x0000;
