#ifndef IMAGE_4B_H
#define IMAGE_4B_H

#include "epd2in13_V4.h"

// from an image of W x H pixel, coded with 4 bits per pixel, 
// we compress it as a list of bytes, each bytes containing :
// 4 higher bits = pixel value
// 4 lower bits = number of consecutives pixel - 1
// thus 0x42 means 3 pixels with color 4
// if there are more than 16 pixels with same color N, we repeat it by sets of 16, with 0xNf
// thus 35 pixels with color 9 is stored as 0x9f 0x9f 0x92
typedef struct _Image4b {
    unsigned int width;     // uncompressed image witdh
    unsigned int height;    // uncompressed image height
    unsigned int data_len;  // compressed data length (= size of array below)
    unsigned char *compressed_data; // compressed data (data_len bytes)
} Image4b;

short displayImage4b(Epd *epd, Image4b *img, unsigned int x0, unsigned int y0);

#endif // IMAGE_4B_H