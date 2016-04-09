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
	{ 0xFFA25D, 'A' }, { 0xFF629D, 'B' }, { 0xFFE21D, 'C' },
	{ 0xFF22DD, 'D' }, { 0xFF02FD, 'u' }, { 0xFFC23D, 'E' },
	{ 0xFFE01F, 'l' }, { 0xFFA857, 'x' }, { 0xFF906F, 'r' },
	{ 0xFF6897, '0' }, { 0xFF9867, 'd' }, { 0xFFB04F, 'F' },
	{ 0xFF30CF, '1' }, { 0xFF18E7, '2' }, { 0xFF7A85, '3' },
	{ 0xFF10EF, '4' }, { 0xFF38C7, '5' }, { 0xFF5AA5, '6' },
	{ 0xFF42BD, '7' }, { 0xFF4AB5, '8' }, { 0xFF52AD, '9' },
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

	My_Receiver.enableIRIn();
	My_Receiver.blink13(true);
	Serial.println("setup ok");
}

void loop() {
	if (My_Receiver.GetResults(&My_Decoder)) {//wait till it returns true
		My_Decoder.decode();
//		My_Decoder.DumpResults();
		Serial.println(lookUp(My_Decoder.value, MakeBlockRemote));
		My_Receiver.resume();
	}
}
