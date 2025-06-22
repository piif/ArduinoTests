#include <SPI.h>
#include "epd2in13_V4.h"

#define TEST_IMAGE 1
#define TEST_PAINT 0
#define TEST_PART  0

#if TEST_PAINT
#include "epdpaint.h"
#endif
#if TEST_IMAGE
#include "imagedata.h"
#endif

#define COLORED     0
#define UNCOLORED   1

// Pin definition
#define RST_PIN         8
#define DC_PIN          9
#define CS_PIN          10
#define BUSY_PIN        7

/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
#if TEST_PAINT
unsigned char image[1050];
Paint paint(image, 0, 0);
#endif

Epd epd(RST_PIN, DC_PIN, CS_PIN, BUSY_PIN);

void setup() {
  Serial.begin(115200);

#if TEST_IMAGE
  Serial.println("init fast");
  epd.Init(FAST);
  Serial.println("Display_Fast");
  epd.Display_Fast(IMAGE_DATA);
  Serial.println("delay");
  delay(2000);
#endif

#if TEST_PAINT
  Serial.println("epd FULL");
  epd.Init(FULL);
  Paint paint(image, epd.bufwidth*8, epd.bufheight);    //width should be the multiple of 8

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(8, 2, "e-Paper Demo", &Font12, COLORED);
  paint.DrawStringAt(8, 20, "Hello world", &Font12, COLORED);
  epd.Display1(image);//1

  paint.Clear(UNCOLORED);
  paint.DrawRectangle(2,2,50,50,COLORED);
  paint.DrawLine(2,2,50,50,COLORED);
  paint.DrawLine(2,50,50,2,COLORED);
  paint.DrawFilledRectangle(52,2,100,50,COLORED);
  paint.DrawLine(52,2,100,50,UNCOLORED);
  paint.DrawLine(100,2,52,50,UNCOLORED);
  epd.Display1(image);//2
  
  paint.Clear(UNCOLORED);
  paint.DrawCircle(25,25,20,COLORED);
  paint.DrawFilledCircle(75,25,20,COLORED);
  epd.Display1(image);//3
  
  paint.Clear(UNCOLORED);
  epd.Display1(image);//4

  delay(2000);

#endif

#if TEST_PART

  Serial.println("epd PART");
  epd.DisplayPartBaseImage(IMAGE_DATA);
  char i = 0;
  for (i = 0; i < 10; i++) {
    Serial.println("e-Paper PART IMAGE_DATA");
    epd.Init(PART);
    epd.DisplayPart(IMAGE_DATA);
    Serial.println("e-Paper PART Clear");
	  epd.Init(PART);
    epd.ClearPart();
    delay(2000);
  }

#endif

  // epd.Init(FULL);
  // Serial.println("e-Paper clear");
  // epd.Clear();
  Serial.println("e-Paper sleep");
  epd.Sleep();

}

void loop() {

}
