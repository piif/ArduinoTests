#include "registers.h"

struct _registers {
	/* 0x20 */	RESERVED_BYTES(0x20, 0x22);
	/* 0x23 */	REGISTER(pinb, BITS_RANGE(pinb));
	/* 0x24 */	REGISTER(ddrb, BITS_RANGE(ddrb));
	/* 0x25 */	REGISTER(portb, BITS_RANGE(portb));
	/* 0x26 */	REGISTER(pinc, BITS_RANGE(pinc));
	/* 0x27 */	REGISTER(ddrc, BITS_RANGE(ddrc));
	/* 0x28 */	REGISTER(portc, BITS_RANGE(portc));
	/* 0x29 */	REGISTER(pind, BITS_RANGE(pind));
	/* 0x2A */	REGISTER(ddrd, BITS_RANGE(ddrd));
	/* 0x2B */	REGISTER(portd, BITS_RANGE(portd));
	/* 0x2C */	RESERVED_BYTES(0x2C, 0x34);
	/* 0x35 */	REGISTER(tifr0, RESERVED_BITS(5), BIT(ocf0b), BIT(ocf0a), BIT(tov0));
	/* 0x36 */	REGISTER(tifr1, RESERVED_BITS(2), BIT(icf1), RESERVED_BITS(2), BIT(ocf1b), BIT(ocf1a), BIT(tov1));
	/* 0x37 */	REGISTER(tifr2, RESERVED_BITS(5), BIT(ocf2b), BIT(ocf2a), BIT(tov2));
	/* 0x38 - 0x3A */	RESERVED_BYTES(0x38, 0x3A);
	/* 0x3B */	REGISTER(pcifr, RESERVED_BITS(5), BIT(pcif2), BIT(pcif1), BIT(pcif0));
	/* 0x3C */	REGISTER(eifr, RESERVED_BITS(6), BIT(intf1), BIT(intf0));
	/* 0x3D */	REGISTER(eimsk, RESERVED_BITS(6), BIT(int1), BIT(int0));
	/* 0x3E */	byte gpior0;	// General Purpose I/O Register 0
	/* 0x3F */	REGISTER(eecr, RESERVED_BITS(2), BIT(eepm1), BIT(eepm0), BIT(eerie), BIT(eempe), BIT(eepe), BIT(eere));
	/* 0x40 */	byte eedr;	// EEPROM Data Register
	/* 0x42 */	word eear;	// EEPROM Address Register
	/* 0x43 */	REGISTER(gtccr, BIT(tsm), RESERVED_BITS(5), BIT(psrasy), BIT(psrsync));
	/* 0x44 */	REGISTER(tccr0a, BIT(com0a, 2), BIT(com0b, 2), RESERVED_BITS(2), BIT(wgm0l, 2));
	/* 0x45 */	REGISTER(tccr0b, BIT(foc0a), BIT(foc0b), RESERVED_BITS(2), BIT(wgm0h), BIT(cs0, 3));
	/* 0x46 */	byte tcnt0;	// Timer/Counter0
	/* 0x47 */	byte ocr0a;	// Timer/Counter0 Output Compare Register A
	/* 0x48 */	byte ocr0b;	// Timer/Counter0 Output Compare Register B
	/* 0x49 */	RESERVED_BYTES(0x49, 0x49);
	/* 0x4A */	byte gpior1;	// General Purpose I/O Register 1
	/* 0x4B */	byte gpior2;	// General Purpose I/O Register 2
	/* 0x4C */	REGISTER(spcr, BIT(spie), BIT(spe), BIT(dord), BIT(mstr), BIT(cpol), BIT(cpha), BIT(spr1), BIT(spr0));
	/* 0x4D */	REGISTER(spsr, BIT(spif), BIT(wcol), RESERVED_BITS(5), BIT(spi2x));
	/* 0x4E */	byte spdr;
	/* 0x4F */	RESERVED_BYTES(0x4F, 0x4F);
	/* 0x50 */	REGISTER(acsr, BIT(acd), BIT(acbg), BIT(aco), BIT(aci), BIT(acie), BIT(acic), BIT(acis1), BIT(acis0));
	/* 0x51 */	RESERVED_BYTES(0x51, 0x51);
	/* 0x52 */	RESERVED_BYTES(0x52, 0x52);
	/* 0x53 */	REGISTER(smcr, RESERVED_BITS(4), BIT(sm2), BIT(sm1), BIT(sm0), BIT(se));
	/* 0x54 */	REGISTER(mcusr, RESERVED_BITS(4), BIT(wdrf), BIT(borf), BIT(extrf), BIT(porf));
	/* 0x55 */	REGISTER(mcucr, RESERVED_BITS(1), BIT(bods), BIT(bodse), BIT(pud), RESERVED_BITS(2), BIT(ivsel), BIT(ivce));
	/* 0x56 */	RESERVED_BYTES(0x56, 0x56);
	/* 0x57 */	REGISTER(spmcsr, BIT(spmie), BIT(rwwsb), RESERVED_BITS(1), BIT(rwwsre), BIT(blbset), BIT(pgwrt), BIT(pgers), BIT(selfprgen));
	/* 0x58 - 0x5C */	RESERVED_BYTES(0x58, 0x5C);
	/* 0x5E */	word sp;
	/* 0x5F */	REGISTER(sreg, BIT(i), BIT(t), BIT(h), BIT(s), BIT(v), BIT(n), BIT(z), BIT(c));
	/* 0x60 */	REGISTER(wdtcsr, BIT(wdif), BIT(wdie), BIT(wdp3), BIT(wdce), BIT(wde), BIT(wdp2), BIT(wdp1), BIT(wdp0));
	/* 0x61 */	REGISTER(clkpr, BIT(clkpce), RESERVED_BITS(3), BIT(clkps, 4));
	/* 0x62 - 0x63 */	RESERVED_BYTES(0x62, 0x63);
	/* 0x64 */	REGISTER(prr, BIT(prtwi), BIT(prtim2), BIT(prtim0), RESERVED_BITS(1), BIT(prtim1), BIT(prspi), BIT(prusart0), BIT(pradc));
	/* 0x65 */	RESERVED_BYTES(0x65, 0x65);
	/* 0x66 */	byte osccal;	// Oscillator Calibration Register
	/* 0x67 */	RESERVED_BYTES(0x67, 0x67);
	/* 0x68 */	REGISTER(pcicr, RESERVED_BITS(5), BIT(pcie2), BIT(pcie1), BIT(pcie0));
	/* 0x69 */	REGISTER(eicra, RESERVED_BITS(4), BIT(isc1, 2), BIT(isc0, 2));
	/* 0x6A */	RESERVED_BYTES(0x6A, 0x6A);
	/* 0x6B */	REGISTER(pcmsk0, BIT(pcint7), BIT(pcint6), BIT(pcint5), BIT(pcint4), BIT(pcint3), BIT(pcint2), BIT(pcint1), BIT(pcint0));
	/* 0x6C */	REGISTER(pcmsk1, RESERVED_BITS(1), BIT(pcint14), BIT(pcint13), BIT(pcint12), BIT(pcint11), BIT(pcint10), BIT(pcint9), BIT(pcint8));
	/* 0x6D */	REGISTER(pcmsk2, BIT(pcint23), BIT(pcint22), BIT(pcint21), BIT(pcint20), BIT(pcint19), BIT(pcint18), BIT(pcint17), BIT(pcint16));
	/* 0x6E */	REGISTER(timsk0, RESERVED_BITS(5), BIT(ocie0b), BIT(ocie0a), BIT(toie0));
	/* 0x6F */	REGISTER(timsk1, RESERVED_BITS(2), BIT(icie1), RESERVED_BITS(2), BIT(ocie1b), BIT(ocie1a), BIT(toie1));
	/* 0x70 */	REGISTER(timsk2, RESERVED_BITS(5), BIT(ocie2b), BIT(ocie2a), BIT(toie2));
	/* 0x71 - 0x77 */	RESERVED_BYTES(0x71, 0x77);
	/* 0x79 */	word adc;
	/* 0x7A */	REGISTER(adcsra, BIT(aden), BIT(adsc), BIT(adate), BIT(adif), BIT(adie), BIT(adps, 3));
	/* 0x7B */	REGISTER(adcsrb, RESERVED_BITS(1), BIT(acme), RESERVED_BITS(3), BIT(adts, 3));
	/* 0x7C */	REGISTER(admux, BIT(refs, 2), BIT(adlar), RESERVED_BITS(1), BIT(mux, 4));
	/* 0x7D */	RESERVED_BYTES(0x7D, 0x7D);
	/* 0x7E */	REGISTER(didr0, RESERVED_BITS(2), BIT(adcxd, 6));
	/* 0x7F */	REGISTER(didr1, RESERVED_BITS(6), BIT(ain1d), BIT(ain0d));
	/* 0x80 */	REGISTER(tccr1a, BIT(com1a, 2), BIT(com1b, 2), RESERVED_BITS(2), BIT(wgm1l, 2));
	/* 0x81 */	REGISTER(tccr1b, BIT(icnc1), BIT(ices1), RESERVED_BITS(1), BIT(wgm1h, 2), BIT(cs1, 3));
	/* 0x82 */	REGISTER(tccr1c, BIT(foc1a), BIT(foc1b), RESERVED_BITS(6));
	/* 0x83 */	RESERVED_BYTES(0x83, 0x83);
	/* 0x85 */	word tcnt1;
	/* 0x87 */	word icr1;
	/* 0x89 */	word ocr1a;
	/* 0x8B */	word ocr1b;
	/* 0x8C - 0xAF */	RESERVED_BYTES(0x8C, 0xAF);
	/* 0xB0 */	REGISTER(tccr2a, BIT(com2a, 2), BIT(com2b, 2), RESERVED_BITS(2), BIT(wgm2l, 2));
	/* 0xB1 */	REGISTER(tccr2b, BIT(foc2a), BIT(foc2b), RESERVED_BITS(2), BIT(wgm2h), BIT(cs2, 3));
	/* 0xB2 */	byte tcnt2;	// Timer/Counter2);
	/* 0xB3 */	byte ocr2a;	// Timer/Counter2 Output Compare Register A
	/* 0xB4 */	byte ocr2b;	// Timer/Counter2 Output Compare Register B
	/* 0xB5 */	RESERVED_BYTES(0xB5, 0xB5);
	/* 0xB6 */	REGISTER(assr, RESERVED_BITS(1), BIT(exclk), BIT(as2), BIT(tcn2ub), BIT(ocr2aub), BIT(ocr2bub), BIT(tcr2aub), BIT(tcr2bub));
	/* 0xB7 */	RESERVED_BYTES(0xB7, 0xB7);
	/* 0xB8 */	byte twbr;	// 2-wire Serial Interface Bit Rate Register
	/* 0xB9 */	REGISTER(twsr, BIT(tws7), BIT(tws6), BIT(tws5), BIT(tws4), BIT(tws3), RESERVED_BITS(1), BIT(twps1), BIT(twps0));
	/* 0xBA */	REGISTER(twar, BIT(twa, 7), BIT(twgce));
	/* 0xBB */	byte twdr;	// 2-wire Serial Interface Data Register
	/* 0xBC */	REGISTER(twcr, BIT(twint), BIT(twea), BIT(twsta), BIT(twsto), BIT(twwc), BIT(twen), RESERVED_BITS(1), BIT(twie));
	/* 0xBD */	REGISTER(twamr, BIT(twam, 7), RESERVED_BITS(1));
	/* 0xBE - 0xBF */	RESERVED_BYTES(0xBE, 0xBF);
	/* 0xC0 */	REGISTER(ucsr0a, BIT(rxc0), BIT(txc0), BIT(udre0), BIT(fe0), BIT(dor0), BIT(upe0), BIT(u2x0), BIT(mpcm0));
	/* 0xC1 */	REGISTER(ucsr0b, BIT(rxcie0), BIT(txcie0), BIT(udrie0), BIT(rxen0), BIT(txen0), BIT(ucsz02), BIT(rxb80), BIT(txb80));
	/* 0xC2 */	REGISTER(ucsr0c, BIT(umsel01), BIT(umsel00), BIT(upm01), BIT(upm00), BIT(usbs0), BIT(ucsz01_udord0), BIT(ucsz00_ucpha0), BIT(ucpol0));
	/* 0xC3 */	RESERVED_BYTES(0xC3, 0xC3);
	/* 0xC4 */	word ubrr0;	// USART Baud Rate Register
	/* 0xC6 */	byte udr0;	// USART I/O
	/* 0xC7 - 0xFF */	RESERVED_BYTES(0xC7, 0xFF);
} *registers = (struct _registers*)0x20;

#define SET_WGM(timer, value) do { \
	registers->CAT(CAT(wgm, timer), h) = (value) >> 2; \
	registers->CAT(CAT(wgm, timer), l) = (value); \
} while(0)

typedef void (*interrupt_callback)();

typedef enum {
	V_RESET,			// External Pin, Power-on Reset, Brown-out Reset and Watchdog System Reset
	V_INT0,			// External Interrupt Request 0
	V_INT1,			// External Interrupt Request 1
	V_PCINT0,		// Pin Change Interrupt Request 0
	V_PCINT1,		// Pin Change Interrupt Request 1
	V_PCINT2,		// Pin Change Interrupt Request 2
	V_WDT,			// Watchdog Time-out Interrupt
	V_TIMER2_COMPA,	// Timer/Counter2 Compare Match A
	V_TIMER2_COMPB,	// Timer/Counter2 Compare Match B
	V_TIMER2_OVF,	// Timer/Counter2 Overflow
	V_TIMER1_CAPT,	// Timer/Counter1 Capture Event
	V_TIMER1_COMPA,	// Timer/Counter1 Compare Match A
	V_TIMER1_COMPB,	// Timer/Coutner1 Compare Match B
	V_TIMER1_OVF,	// Timer/Counter1 Overflow
	V_TIMER0_COMPA,	// Timer/Counter0 Compare Match A
	V_TIMER0_COMPB,	// Timer/Counter0 Compare Match B
	V_TIMER0_OVF,	// Timer/Counter0 Overflow
	V_SPI_STC,		// SPI Serial Transfer Complete
	V_USART_RX,		// USART Rx Complete
	V_USART_UDRE,	// USART, Data Register Empty
	V_USART_TX		// USART, Tx Complete
} interrupt_names;

interrupt_callback *interrupt_vectors = 0x0000;
