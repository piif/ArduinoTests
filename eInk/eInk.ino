#include <SPI.h>
#include "epd2in13_V4.h"
#include <serialInput.h>

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
unsigned char image[1008]; // 128(round of 122) * 63(round of 250/4) / 8 = 1008
Paint paint(image, 0, 0);

Epd epd(RST_PIN, DC_PIN, CS_PIN, BUSY_PIN);

int test_init_none() {
  Serial.println(F("init none"));
  epd.Init(INIT_MODE_NONE);
  Serial.println(F("end init none"));
  return 0;
}

int test_init_fast() {
  Serial.println(F("init fast"));
  epd.Init(INIT_MODE_FAST);
  Serial.println(F("end init fast"));
  return 0;
}

int test_init_full() {
  Serial.println(F("init full"));
  epd.Init(INIT_MODE_FULL);
  Serial.println(F("end init full"));
  return 0;
}

int test_clear() {
  Serial.println(F("clear"));
  epd.Clear();
  Serial.println(F("end clear"));
  return 0;
}

int test_image() {
  Serial.println(F("image"));
  displayImage4b(&epd, &T2_image, 16, 10);
  epd.Refresh();
  Serial.println(F("end image"));
  return 0;
}

int test_draw() {
  Serial.print(F("draw "));Serial.print(epd.bufwidth*8);Serial.print('x');Serial.println(epd.bufheight);
  paint.SetSize(epd.bufwidth*8, epd.bufheight);

  // epd.Clear(REFRESH_NONE);

  // BLACK PART

  Serial.println(F("text"));
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(8, 2, "e-Paper Demo", &Font12, COLORED);
  paint.DrawStringAt(8, 20, "Hello world", &Font12, COLORED);

  // paint.DumpImage();
  epd.DisplayQuarter(image, LAYER_BLACK); // 1

  Serial.println(F("lines"));
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(1,1,45,45,COLORED);
  paint.DrawLine(1,1,45,45,COLORED);
  paint.DrawLine(1,45,45,1,COLORED);
  paint.DrawFilledRectangle(47,1,96,50,COLORED);
  paint.DrawLine(47,1,96,50,UNCOLORED);
  paint.DrawLine(47,50,96,1,UNCOLORED);

  // paint.DumpImage();
  epd.DisplayQuarter(image); // 2

  Serial.println(F("circles"));
  paint.Clear(UNCOLORED);
  paint.DrawCircle(25,25,20,COLORED);
  paint.DrawFilledCircle(75,25,20,COLORED);

  epd.DisplayQuarter(image); // 3

  Serial.println(F("align test"));
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(12,2,120,59,COLORED);
  paint.DrawFilledRectangle(22,12,110,49,COLORED);

  paint.DrawHorizontalLine(3,3,3,COLORED);
  paint.DrawVerticalLine(6,4,3,COLORED);

  // epd.DisplayQuarter(image, LAYER_BLACK, REFRESH_FAST); // 4, launches refresh
  epd.DisplayQuarter(image, LAYER_BLACK, REFRESH_NONE); // 4, without refresh

  // RED PART

  Serial.println(F("text"));
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(9, 19, "Hello world", &Font12, COLORED);

  epd.DisplayQuarter(image, LAYER_RED); // 1

  Serial.println(F("lines"));
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(4,4,42,42,COLORED);
  paint.DrawLine(4,4,42,42,COLORED);
  paint.DrawLine(4,42,42,4,COLORED);
  paint.DrawFilledRectangle(51,5,92,46,COLORED);
  paint.DrawLine(51,5,92,46,UNCOLORED);
  paint.DrawLine(51,46,92,5,UNCOLORED);

  epd.DisplayQuarter(image); // 2

  Serial.println(F("circles"));
  paint.Clear(UNCOLORED);
  paint.DrawCircle(25,25,10,COLORED);
  paint.DrawFilledCircle(75,25,10,COLORED);
  
  epd.DisplayQuarter(image); // 3

  Serial.println(F("align test"));
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(14,4,118,57,COLORED);
  paint.DrawFilledRectangle(25,15,107,46,COLORED);

  paint.DrawLine(111,60,121,60,UNCOLORED);
  paint.DrawLine(121,60,121,50,UNCOLORED);

  paint.DrawRectangle(2,2,7,7,COLORED);
  paint.DrawLine(3,6,5,4,COLORED);

  epd.DisplayQuarter(image); // 4, launches refresh
  // epd.Refresh(REFRESH_FULL);
  Serial.println(F("end draw"));
  return 0;
}

int test_part() {
  Serial.println(F("part"));
  paint.SetSize(48, 48);
  paint.Clear(UNCOLORED);

  epd.SetWindow(48,65,95,112); // -> 48x48
  epd.SetCursor(48,65);
  epd.Display(image, paint.GetBufferSize(), LAYER_RED, REFRESH_NONE);

  paint.DrawHorizontalLine( 2, 2, 44, COLORED);
  paint.DrawVerticalLine(45, 2, 24, COLORED);
  paint.DrawLine(44, 0, 47, 3, COLORED);
  paint.DrawStringAt(2, 10, "Part", &Font12, COLORED);
  // paint.DumpImage();
  epd.SetCursor(48,65);
  epd.Display(image, paint.GetBufferSize(), LAYER_BLACK, REFRESH_FULL);
  Serial.println(F("end part"));
  return 0;
}

int test_pattern() {
  epd.SendCommand(AUTO_WRITE_BLACK_RAM_FOR_REGULAR_PATTERN);
  epd.SendData(0x81);
  epd.SendCommand(AUTO_WRITE_RED_RAM_FOR_REGULAR_PATTERN);
  epd.SendData(0x08);
  epd.Refresh(REFRESH_PART);
  return 0;
}

// patterns = Black/Red , Even/Odd
void fill(
    unsigned char be, unsigned char bo, unsigned char re, unsigned char ro,
    int x0, int y0, int w, int h) {
  epd.SetWindow(x0, y0, x0+w-1, y0+h-1);
  paint.SetSize(w, h);
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

int test_fill() {
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
  return 0;
}

int epd_sleep() {
  Serial.println(F("e-Paper sleep"));
  epd.Sleep();
  return 0;
}

void menu() {
  Serial.println(F(
    "? : this menu\n"
    "i : test fast_init\n"
    "I : test full_init\n"
    "c : test Clear\n"
    "m : test iMage\n"
    "d : test Draw\n"
    "p : test Part\n"
    "t : test paTtern\n"
    "f : test Fill\n"
    "s : sleep\n"
  ));
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
	Serial.println(F("setup ok"));

  menu();
}

void loop() {
  Serial.println(F("loop"));
  for(;;) {
   	if (handleInput() != -1) {
      Serial.println(F("handled input"));
    }
  }
}
