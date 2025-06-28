#ifndef IMAGE_4B_H
#define IMAGE_4B_H

#include "epd2in13_V4.h"

typedef struct _Image4b {
    unsigned int width;
    unsigned int height;
    unsigned int data_len;
    unsigned char *compressed_data;
} Image4b;

void displayImage4b(Epd *epd, Image4b *img, unsigned int x0, unsigned int y0);

#endif // IMAGE_4B_H