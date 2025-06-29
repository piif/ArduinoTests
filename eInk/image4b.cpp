
#include "image4b.h"

// stores 2x2 pixels maps for black and red layer
// 0bABCDEFGH means :
// black map (1=black, 0=white)
//  AB
//  CD
// red map (1=red, 0=off)
//  EF
//  GH
unsigned char colormap[16] = {
    0b00000000,
    0b10000000,
    0b10100000,
    0b11100000,
    0b11110000,

    0b00001000,
    0b10000001,
    0b10100100,
    0b11100001,

    0b00001010,
    0b10000101,
    0b10100101,

    0b00001110,
    0b10000111,

    0b00001111
};

short _displayLayer(Epd *epd, Image4b *img, unsigned char shift1, unsigned char shift2) {
    unsigned int line_len = ((img->width*2)+7)>>3;
    // 2 lines of data IN prepration
    unsigned char l1[line_len], l2[line_len];
    // x = current position in lines
    // y = current line (stop at height even if data is not completly read)
    // mask = current bit position in Line byte
    unsigned int x = 0, y = 0, mask = 0;
 
    unsigned int i;
    for (i = 0; i < img->data_len; i++) {
        unsigned char color = img->compressed_data[i] >> 4;
        unsigned char count = (img->compressed_data[i] & 0x0f) + 1;
        for (unsigned char c = 0; c < count; c++) {
            l1[x] = ( l1[x] & ~(0xc0 >> mask) ) | ( ( (colormap[color] << shift1) & 0xc0 ) >> mask );
            l2[x] = ( l2[x] & ~(0xc0 >> mask) ) | ( ( (colormap[color] << shift2) & 0xc0 ) >> mask );
            mask = (mask + 2) % 8;
            if (mask == 0) {
                x++;
                if (x == line_len) {
                    epd->SendData(l1, line_len);
                    epd->SendData(l2, line_len);
                    x = 0;
                    y++;
                    if (y == img->height) {
                        return 1;
                    }
                }
            }
        }
    }

    if (i == img->data_len) {
        return 0;
    } else {
        return -1;
    }
}

// return 0 if OK, -1 if not enough bytes, 1 if too many bytes
short displayImage4b(Epd *epd, Image4b *img, unsigned int x0, unsigned int y0) {
    // round x0 to lower multiple of 8
    x0 &= ~(0x07);
    epd->SetWindow(x0, y0, x0 + (2 * img->width) - 1, y0 + (2 * img->height) - 1);

    // we need to uncompress and draw all black layer, then red layer
    epd->SetCursor(x0, y0);
    epd->SendCommand(WRITE_RAM_BLACK);
    _displayLayer(epd, img, 0, 2);

    epd->SetCursor(x0, y0);
    epd->SendCommand(WRITE_RAM_RED);
    return _displayLayer(epd, img, 4, 6);
}
