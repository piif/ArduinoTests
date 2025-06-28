
#include "image4b.h"

// 0bABCDEFGH means :
// black map (1=black, 0=white)
// AB
// CD
// red map (1=red, 0=off)
// EF
// GH
unsigned char colormap[16] = {
  0b00000000,
  0b00001000,
  0b00001010,
  0b00001110,
  0b00001111,

  0b10000000,
  0b00011000,
  0b01001010,
  0b00011110,

  0b10100000,
  0b01011000,
  0b01011010,

  0b11100000,
  0b01111000,

  0b11110000,
};

void displayImage4b(Epd *epd, Image4b *img, unsigned int x0, unsigned int y0) {
    unsigned int line_len = ((img->width*2)+7)>>3;
    unsigned char l1[line_len], l2[line_len];

    epd->SetWindow(x0, y0, x0+img->width-1, y0+img->height-1);
    epd->SetCursor(x0, y0);

    unsigned int x = 0;
    // TODO
    // iterer sur les data
    // pour chaque char / count , avancer dans les lignes
    // à chaque fois qu'une paire de lignes est prete, les envoyer précédée de la commande de buffer
    // + revoir convImg pour sortir la structure en 2 fois
}
