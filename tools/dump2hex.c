/**
 * simple converter from "raw hexdump" format to intel Hex one
 */

#include <stdio.h>

typedef unsigned short word;
typedef unsigned char byte;

int fromHex(char src, byte *dst) {
	if (src >= '0' && src <= '9') {
		*dst = src - '0';
	} else if (src >= 'A' && src <= 'F') {
		*dst = src - 'A' + 10;
	} else if (src >= 'a' && src <= 'f') {
		*dst = src - 'a' + 10;
	} else {
		return 0;
	}
	return 1;
}

char toHex(byte digit) {
	if (digit < 10) {
		return digit + '0';
	} else {
		return digit - 10 + 'A';
	}
}

char *readHex16(char *ic, word *value) {
	*value = 0;

	byte digit = 0;
	while (fromHex(*ic, &digit)) {
		*value = (*value << 4) + digit;
		ic++;
	}
	return ic;
}

char *readHex8(char *ic, byte *value) {
	*value = 0;
	byte b = 0;

	if (fromHex(*ic, &b)) {
		*value = b;
		ic++;
		if (fromHex(*ic, &b)) {
			*value = (*value << 4) + b;
			ic++;
		}
	}
	return ic;
}

char *writeHex16(word value, char *ic) {
	*ic++ = toHex((value >> 12) & 0x0F);
	*ic++ = toHex((value >> 8 ) & 0x0F);
	*ic++ = toHex((value >> 4 ) & 0x0F);
	*ic++ = toHex(value & 0x0F);
	return ic;
}

char *writeHex8(byte value, char *ic) {
	*ic++ = toHex((value >> 4 ) & 0x0F);
	*ic++ = toHex(value & 0x0F);
	return ic;
}

char *convertToHex(char *inputLine, char *outputLine) {
	char *ic = inputLine;
//	printf("convert '%s'\n", inputLine);
	word addr;
	byte line[16];
	int count = 0;
	ic = readHex16(ic, &addr);
	while (count < 16 && *ic != '\0') {
		while (*ic == ' ' || *ic == ':') {
			ic++;
		}
		if (*ic == '\0') {
			break;
		}
		ic = readHex8(ic, &(line[count]));
		count++;
	}
//	printf(" => %04x %02x %02x ...\n", addr, line[0], line[1]);
	byte sum = count + (addr >> 8) + addr;

	char *oc = outputLine;
	*oc++ = ':';
	oc = writeHex8(count, oc);
	oc = writeHex16(addr, oc);
	oc = writeHex8(0, oc);

	for (int i = 0; i < count; i++) {
		sum += line[i];
		oc = writeHex8(line[i], oc);
	}
	oc = writeHex8(~sum + 1, oc);
	*oc = '\0';
	return outputLine;
}

byte convertToBin(char *inputLine, byte *outputLine) {
	char *ic = inputLine;
	byte *oc = outputLine;
	word addr;
	byte b, count = 0;
	ic = readHex16(ic, &addr);
	while (*ic != '\0') {
		while (*ic == ' ' || *ic == ':') {
			ic++;
		}
		if (*ic == '\0') {
			break;
		}
		ic = readHex8(ic, oc);
		oc++;
		count++;
	}
	*oc = '\0';
	return count;
}

#ifdef TESTS
void tests() {
	char s[10];
	char *c = s;
	c = writeHex16(0x1234, c);
	*c='\0';
	printf("writeHex16(0x1234) : %s\n", s);

	c = s;
	c = writeHex8(0x5A, c);
	*c='\0';
	printf("writeHex8(0x5A) : %s\n", s);

	word h16 = 0;
	c = readHex16("FE98:", &h16);
	printf("readHex16(\"FE98:\") : %04x\n", h16);

	byte h8 = 0;
	c = readHex8("42:", &h8);
	printf("readHex8(\"42:\") : %02x\n", h8);
}
#endif

#define MODE_HEX 1
#define MODE_BIN 2

int main(int argc, char **argv) {
#ifdef TESTS
	tests();
#else
	byte mode = MODE_HEX;
	if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'b' && argv[1][2] == '\0') {
		mode = MODE_BIN;
	}

	char inputLine[100];
	char *ic = inputLine;

	int c;
	while((c = getchar()) != -1) {
		if (c == '\r') {
			continue;
		} else if (c == '\n') {
			*ic = '\0';
			if (mode == MODE_HEX) {
				char outputLine[100];
				convertToHex(inputLine, outputLine);
				puts(outputLine);
			} else {
				byte outputLine[20];
				byte count = convertToBin(inputLine, outputLine);
				for (byte i = 0; i < count; i++) {
					putchar(outputLine[i]);
				}
			}
			ic = inputLine;
		} else {
			*ic = c;
			ic++;
		}
	}
	if (mode == MODE_HEX) {
		puts(":00000001FF");
	}
#endif
	return 0;
}

