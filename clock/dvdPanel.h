#define BUTTON_1 0x0002
#define BUTTON_2 0x0004
#define BUTTON_3 0x2000
#define BUTTON_4 0x0010
#define BUTTON_5 0x0020

class DVDPanel {
public:
    byte *lcdDisplay;
    DVDPanel(byte display[]) {
        lcdDisplay = display;
        clearPanel();
    }

    // for each map, we consider a list of segment references
    // each reference is a byte with b7-b4 = lcdDisplay entry (0 to 0x0C) and b3-b0 = bit number
    // thus 0x23 is bit 3 of lcdDisplay[2]

    static byte diskMap[8];
    static byte symbolsMap[9];
    static byte colonMap[2];

    static byte digitMap[7][7];

    static byte digitSegments[10];

    void setSegment(byte ref) {
        lcdDisplay[ref>>4] |= 1 << (ref&0x0f);
    }
    void clearSegment(byte ref) {
        lcdDisplay[ref>>4] &= ~(1 << (ref&0x0f));
    }

    void clearPanel() {
        for (byte i=0; i<14; i++) {
            lcdDisplay[i] = 0;
        }
    }
    void clearDigits() {
        for (byte pos=0; pos <= 0xC; pos+=2) {
            lcdDisplay[pos] &= 0x40;
        }
    }

    void setDigit(byte position, byte value) {
        byte segments = digitSegments[value];
        for(byte bit = 0; bit < 7; bit++) {
            if (segments & (1 << bit)) {
                setSegment(digitMap[position][bit]);
            }
        }
    }

    void displayValue(long value) {
        byte rank = 0;
        clearDigits();
        for (long mask = 1000000; mask; mask /= 10, rank++) {
            byte digit = value / mask;
            // Serial.print(digit);
            setDigit(rank, digit);
            value = value % mask;
        }
        // Serial.println();
    }
};