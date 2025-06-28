#include <SPI.h>
#include "epd2in13_V4.h"
#include <serialInput.h>

#define TEST_FAST_INIT 0
#define TEST_FULL_INIT 1

#define TEST_CLEAR 0
#define TEST_IMAGE 0
#define TEST_PAINT 1
#define TEST_PART  1
#define TEST_PATTERN 0

#include "epdpaint.h"
#include "image4b.h"
#include "T2_image.h"

#define COLORED     1 // black or red
#define UNCOLORED   0 // white or transparent

// Pin definition
#define RST_PIN         8
#define DC_PIN          9
#define CS_PIN          10
#define BUSY_PIN        7

/**
  * Due to Arduino UNO RAM size, a full frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
#if TEST_PAINT || TEST_PART
unsigned char image[1000]; // 128(round of 122)*250 / 8 / 4 = 1000
Paint paint(image, 0, 0);
#endif

Epd epd(RST_PIN, DC_PIN, CS_PIN, BUSY_PIN);

void dumpImage(unsigned char *image, const unsigned int width, const unsigned int height) {
  unsigned char *im = image;
  unsigned int w = (width+7)>>3;
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x<w; x++) {
      Serial.print(*im++, HEX);Serial.print(' ');
    }
    Serial.println();
  }
}

void test_init_none() {
  Serial.println("init none");
  epd.Init(INIT_MODE_NONE);
  Serial.println("end init none");
}

void test_init_fast() {
  Serial.println("init fast");
  epd.Init(INIT_MODE_FAST);
  Serial.println("end init fast");
}

void test_init_full() {
  Serial.println("init full");
  epd.Init(INIT_MODE_FULL);
  Serial.println("end init full");
}

void test_clear() {
  Serial.println("clear");
  epd.Clear();
  Serial.println("end clear");
}

void test_image() {
  Serial.println("image");
  displayImage4b(&epd, &T2_image, 16, 10);
  Serial.println("end image");
}

void test_draw() {
  Serial.print("draw ");Serial.print(epd.bufwidth*8);Serial.print('x');Serial.println(epd.bufheight);
  Paint paint(image, epd.bufwidth*8, epd.bufheight); // width should be a multiple of 8

  // epd.Clear(REFRESH_NONE);

  // BLACK PART

  Serial.println("text");
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(8, 2, "e-Paper Demo", &Font12, COLORED);
  paint.DrawStringAt(8, 20, "Hello world", &Font12, COLORED);

  epd.DisplayQuarter(image, LAYER_BLACK); // 1

  Serial.println("lines");
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(1,1,45,45,COLORED);
  paint.DrawLine(1,1,45,45,COLORED);
  paint.DrawLine(1,45,45,1,COLORED);
  paint.DrawFilledRectangle(47,1,96,49,COLORED);
  paint.DrawLine(47,1,96,49,UNCOLORED);
  paint.DrawLine(47,49,96,1,UNCOLORED);

  epd.DisplayQuarter(image); // 2

  Serial.println("circles");
  paint.Clear(UNCOLORED);
  paint.DrawCircle(25,25,20,COLORED);
  paint.DrawFilledCircle(75,25,20,COLORED);

  epd.DisplayQuarter(image); // 3

  Serial.println("align test");
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(2,2,120,59,COLORED);
  paint.DrawFilledRectangle(12,12,110,49,COLORED);

  // epd.DisplayQuarter(image, LAYER_BLACK, REFRESH_FAST); // 4, launches refresh
  epd.DisplayQuarter(image, LAYER_BLACK, REFRESH_NONE); // 4, without refresh

  // RED PART

  Serial.println("text");
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(9, 19, "Hello world", &Font12, COLORED);

  epd.DisplayQuarter(image, LAYER_RED); // 1

  Serial.println("lines");
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(4,4,42,42,COLORED);
  paint.DrawLine(4,4,42,42,COLORED);
  paint.DrawLine(4,42,42,4,COLORED);
  paint.DrawFilledRectangle(51,5,92,45,COLORED);
  paint.DrawLine(51,5,92,45,UNCOLORED);
  paint.DrawLine(51,45,92,5,UNCOLORED);

  epd.DisplayQuarter(image); // 2

  Serial.println("circles");
  paint.Clear(UNCOLORED);
  paint.DrawCircle(25,25,10,COLORED);
  paint.DrawFilledCircle(75,25,10,COLORED);
  
  epd.DisplayQuarter(image); // 3

  Serial.println("align test");
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(4,4,118,57,COLORED);
  paint.DrawFilledRectangle(15,15,107,46,COLORED);

  paint.DrawLine(111,60,121,60,UNCOLORED);
  paint.DrawLine(121,60,121,50,UNCOLORED);

  epd.DisplayQuarter(image); // 4, launches refresh
  // epd.Refresh(REFRESH_FULL);
  Serial.println("end draw");
}

void test_part() {
  Serial.println("part");
  Paint paint(image, 48, 48);
  paint.Clear(UNCOLORED);

  epd.SetWindow(48,65,95,112); // -> 48x48
  epd.SetCursor(48,65);
  epd.Display(image, paint.GetBufferSize(), LAYER_RED, REFRESH_NONE);

  paint.DrawHorizontalLine( 2, 2, 45, COLORED);
  paint.DrawVerticalLine(45, 2, 24, COLORED);
  paint.DrawStringAt(2, 10, "Part", &Font12, COLORED);
  // dumpImage(image, 48, 48);
  epd.SetCursor(48,65);
  epd.Display(image, paint.GetBufferSize(), LAYER_BLACK, REFRESH_FULL);
  Serial.println("end part");
}

void test_pattern() {
  epd.SendCommand(AUTO_WRITE_BLACK_RAM_FOR_REGULAR_PATTERN);
  epd.SendData(0x81);
  epd.SendCommand(AUTO_WRITE_RED_RAM_FOR_REGULAR_PATTERN);
  epd.SendData(0x08);
  epd.Refresh(REFRESH_PART);
}

// patterns = Black/Red , Even/Odd
void fill(
    unsigned char be, unsigned char bo, unsigned char re, unsigned char ro,
    int x0, int y0, int w, int h) {
  epd.SetWindow(x0, y0, x0+w-1, y0+h-1);
  Paint paint(image, w, h);
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x += 8) {
      paint.Draw8Pixels(x, y, y%2 ? bo : be);
    }
  }
  epd.SetCursor(x0, y0);
  epd.Display(image, paint.GetBufferSize(), LAYER_BLACK, REFRESH_NONE);
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x += 8) {
      paint.Draw8Pixels(x, y, y%2 ? ro : re);
    }
  }
  epd.SetCursor(x0, y0);
  epd.Display(image, paint.GetBufferSize(), LAYER_RED, REFRESH_NONE);
}

void test_fill() {
  epd.SetWindow(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
  epd.Clear(REFRESH_NONE);

  fill(0x00, 0x00, 0x00, 0x00,  0,   0, 24, 24); // .. .. / .. ..
  fill(0x00, 0x00, 0x00, 0x55, 24,   0, 24, 24); // .. .. / .. .R
  fill(0x00, 0x00, 0xAA, 0x55, 48,   0, 24, 24); // .. .. / R. .R
  fill(0x00, 0x00, 0xAA, 0xFF, 72,   0, 24, 24); // .. .. / R. RR
  fill(0x00, 0x00, 0xFF, 0xFF, 96,   0, 24, 24); // .. .. / RR RR

  fill(0xAA, 0x00, 0x00, 0x00,  0,  24, 24, 24); // B. .. / .. ..
  fill(0xAA, 0x00, 0x00, 0x55, 24,  24, 24, 24); // B. .. / .. .R
  fill(0xAA, 0x00, 0x55, 0xAA, 48,  24, 24, 24); // B. .. / .R R.
  fill(0xAA, 0x00, 0x55, 0xFF, 72,  24, 24, 24); // B. .. / .R RR

  fill(0xAA, 0x55, 0x00, 0x00,  0,  48, 24, 24); // B. .B / .. ..
  fill(0xAA, 0x55, 0x00, 0xAA, 24,  48, 24, 24); // B. .B / .. R.
  fill(0xAA, 0x55, 0x55, 0xAA, 48,  48, 24, 24); // B. .B / .R R.

  fill(0xFF, 0x55, 0x00, 0x00,  0,  72, 24, 24); // BB .B / .. ..
  fill(0xFF, 0x55, 0x00, 0xAA, 24,  72, 24, 24); // BB .B / .. R.

  fill(0xFF, 0xFF, 0x00, 0x00,  0,  96, 24, 24); // BB BB / .. ..


  fill(0x00, 0x00, 0x00, 0x00,  0, 130, 24, 24); // .. .. / .. ..
  fill(0x00, 0x00, 0x00, 0x55, 24, 130, 24, 24); // .. .. / .. .R
  fill(0x00, 0x00, 0xAA, 0x55, 48, 130, 24, 24); // .. .. / R. .R
  fill(0x00, 0x00, 0xAA, 0xFF, 72, 130, 24, 24); // .. .. / R. RR
  fill(0x00, 0x00, 0xFF, 0xFF, 96, 130, 24, 24); // .. .. / RR RR

  fill(0xAA, 0x00, 0x00, 0x00, 24, 154, 24, 24); // B. .. / .. ..
  fill(0xAA, 0x00, 0x00, 0x55, 48, 154, 24, 24); // B. .. / .. .R
  fill(0xAA, 0x00, 0x55, 0xAA, 72, 154, 24, 24); // B. .. / .R R.
  fill(0xAA, 0x00, 0x55, 0xFF, 96, 154, 24, 24); // B. .. / .R RR

  fill(0xAA, 0x55, 0x00, 0x00, 48, 178, 24, 24); // B. .B / .. ..
  fill(0xAA, 0x55, 0x00, 0xAA, 72, 178, 24, 24); // B. .B / .. R.
  fill(0xAA, 0x55, 0x55, 0xAA, 96, 178, 24, 24); // B. .B / .R R.

  fill(0xFF, 0x55, 0x00, 0x00, 72, 202, 24, 24); // BB .B / .. ..
  fill(0xFF, 0x55, 0x00, 0xAA, 96, 202, 24, 24); // BB .B / .. R.

  fill(0xFF, 0xFF, 0x00, 0x00, 96, 226, 24, 24); // BB BB / .. ..

  epd.Refresh();
}

void epd_sleep() {
  Serial.println("e-Paper sleep");
  epd.Sleep();
}

void menu() {
  Serial.println("? : this menu");
	Serial.println("i : test fast_init");
	Serial.println("I : test full_init");
	Serial.println("c : test Clear");
	Serial.println("m : test iMage");
	Serial.println("d : test Draw");
	Serial.println("p : test Part");
  Serial.println("t : test paTtern");
  Serial.println("f : test Fill");
  Serial.println("s : sleep");
}

InputItem inputs[] = {
	{ '?', 'f', (void *)menu },
	{ 'z', 'f', (void *)test_init_none },
	{ 'i', 'f', (void *)test_init_fast },
	{ 'I', 'f', (void *)test_init_full },
	{ 'c', 'f', (void *)test_clear },
	{ 'm', 'f', (void *)test_image },
	{ 'd', 'f', (void *)test_draw },
	{ 'p', 'f', (void *)test_part },
  { 't', 'f', (void *)test_pattern },
  { 'f', 'f', (void *)test_fill },
  { 's', 'f', (void *)epd_sleep }
};

void setup() {
  Serial.begin(115200);
	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");

  menu();
}

void loop() {
 	handleInput();
}
