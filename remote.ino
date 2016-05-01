// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs


/**
 * some tests with https://github.com/cyborg5/IRLib/
 *
 * Télécommande combiné Samsung, en mode DVD : NECx, A0A0....
 *  12ED		4CB3
 *  20DF	A05F	609F
 *  10EF	906F	50AF
 *  30CF	B04F	708F
 *  8C73	8877	807F
 *
 * Télécommande MakeBlock : NEC, FF....
 *  A25D	629D	E21D
 *  22DD	02FD	C23D
 *  E01F	A857	906F
 *  6897	9867	B04F
 *  30CF	18E7	7A85
 *  10EF	38C7	5AA5
 *  42BD	4AB5	52AD
 */

#ifdef PIF_TOOL_CHAIN
	#include <Arduino.h>
	#include <IRLib/IRLib.h>
#else
	#include <IRLib.h>
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define IR_PIN 3
#define IR_PIN_INTERRUPT 1
IRdecode My_Decoder;
IRrecv My_Receiver(IR_PIN);

typedef struct _key { uint32_t code; char key; } Key;
Key MakeBlockRemote[] = {
	{ 0xA25D, 'A' }, { 0x629D, 'B' }, { 0xE21D, 'C' },
	{ 0x22DD, 'D' }, { 0x02FD, 'u' }, { 0xC23D, 'E' },
	{ 0xE01F, 'l' }, { 0xA857, 'x' }, { 0x906F, 'r' },
	{ 0x6897, '0' }, { 0x9867, 'd' }, { 0xB04F, 'F' },
	{ 0x30CF, '1' }, { 0x18E7, '2' }, { 0x7A85, '3' },
	{ 0x10EF, '4' }, { 0x38C7, '5' }, { 0x5AA5, '6' },
	{ 0x42BD, '7' }, { 0x4AB5, '8' }, { 0x52AD, '9' },
	{ 0, 0 }
};
Key PhilipsRemote[] = {
	{ 0xC, 'X' },
	{ 0x31, 'S' }, { 0x30, 'p' },
	{ 0x2B, '<' }, { 0x2C, 'P' }, { 0x28, '>' },
	{ 0x6D, 'R' }, { 0x6E, 'G' }, { 0x6F, 'Y' }, { 0x70, 'B' },
	{ 0xCC, '?' }, { 0x54, 'H' }, { 0x90, 'A' },
	{ 0x58, 'u' },
	{ 0x4D, '-' }, { 0x5A, 'l' }, { 0x5C, 'k' }, { 0x5B, 'r' }, { 0x4C, '+' },
	{ 0x59, 'b' },
	{ 0x38, 's' }, { 0x0A, '^' }, { 0x40, 'O' },
	{ 0x11, 'v' }, { 0x0D, 'M' }, { 0x10, 'V' },
	{ 0x01, '1' }, { 0x02, '2' }, { 0x03, '3' },
	{ 0x04, '4' }, { 0x05, '5' }, { 0x06, '6' },
	{ 0x07, '7' }, { 0x08, '8' }, { 0x09, '9' },
	{ 0x4B, '_' }, { 0x00, '0' }, { 0x3C, 'T' },
	{ 0, 0 }
};

char lookUp(unsigned long code, Key *map) {
	while(map->key != 0) {
		if (code == map->code) {
			return map->key;
		}
		map++;
	}
	return '?';
}

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	digitalWrite(4, LOW);
	digitalWrite(5, HIGH);
	My_Receiver.enableIRIn();
	My_Receiver.blink13(true);
	Serial.println("setup ok");
}

void loop() {
	if (My_Receiver.GetResults(&My_Decoder)) {//wait till it returns true
		My_Decoder.decode();
		My_Decoder.DumpResults();
		Serial.println(lookUp(My_Decoder.value & 0xffff, MakeBlockRemote));
		Serial.println(lookUp(My_Decoder.value & 0xffff, PhilipsRemote));
		My_Receiver.resume();
	}
}

