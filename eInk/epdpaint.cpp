/**
 *  @filename   :   epdpaint.cpp
 *  @brief      :   Paint tools
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     September 9 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "epdpaint.h"

#if defined (INCLUDE_FONT_24) || defined (INCLUDE_FONT_20) || defined (INCLUDE_FONT_16) || defined (INCLUDE_FONT_12) || defined (INCLUDE_FONT_8)
#include <stdint.h>
#endif

Paint::Paint(unsigned char* image, int width, int height) {
    this->rotate = ROTATE_0;
    this->image = image;
    /* 1 byte = 8 pixels, so the width should be the upper multiple of 8 */
    this->width = width % 8 ? width + 8 - (width % 8) : width;
    this->height = height;
    this->buffer_width = width >> 3;
    this->buffer_size = buffer_width * height;
}

Paint::~Paint() {
}

/**
 *  @brief: clear the image
 */
void Paint::Clear(int colored) {
    int len = this->buffer_size;
    unsigned char pattern = (IF_INVERT_COLOR == colored) ? 0xFF : 0x00;
    for (int i = 0; i < len; i++) {
        image[i] = pattern;
    }
}

/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 */
void Paint::DrawAbsolutePixel(int x, int y, int colored) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    // 1 & 1 or 0 & 0 => must set bit
    // 1 & 0 or 0 & 1 => must clear bit
    if (IF_INVERT_COLOR == colored) {
        image[(x + y * width) / 8] |= 0x80 >> (x % 8);
    } else {
        image[(x + y * width) / 8] &= ~(0x80 >> (x % 8));
    }
}

/**
 *  @brief: Getters and Setters
 */
unsigned char* Paint::GetImage(void) {
    return image;
}

int Paint::GetBufferSize(void) {
    return buffer_size;
}

int Paint::GetWidth(void) {
    return width;
}

void Paint::SetWidth(int width) {
    this->width = width % 8 ? width + 8 - (width % 8) : width;
    buffer_width= this->width >> 3;
    buffer_size = buffer_width * height;
}

int Paint::GetHeight(void) {
    return height;
}

void Paint::SetHeight(int height) {
    this->height = height;
    buffer_size = buffer_width * height;
}

void Paint::SetSize(int width, int height) {
    this->width = width % 8 ? width + 8 - (width % 8) : width;
    this->height= height;
    buffer_width= this->width >> 3;
    buffer_size = buffer_width * height;
}

int Paint::GetRotate(void) {
    return rotate;
}

void Paint::SetRotate(int rotate){
    this->rotate = rotate;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void Paint::DrawPixel(int x, int y, int colored) {
    int point_temp;
    if (rotate == ROTATE_0) {
        if(x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        DrawAbsolutePixel(x, y, colored);
    } else if (rotate == ROTATE_90) {
        if(x < 0 || x >= height || y < 0 || y >= width) {
          return;
        }
        point_temp = x;
        x = width - y;
        y = point_temp;
        DrawAbsolutePixel(x, y, colored);
    } else if (rotate == ROTATE_180) {
        if(x < 0 || x >= width || y < 0 || y >= height) {
          return;
        }
        x = width - x;
        y = height - y;
        DrawAbsolutePixel(x, y, colored);
    } else if (rotate == ROTATE_270) {
        if(x < 0 || x >= height || y < 0 || y >= width) {
          return;
        }
        point_temp = x;
        x = y;
        y = height - point_temp;
        DrawAbsolutePixel(x, y, colored);
    }
}

void Paint::Draw8Pixels(int x, int y, unsigned char pattern) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    if (IF_INVERT_COLOR) {
        image[(x + y * width) / 8] = pattern;
    } else {
        image[(x + y * width) / 8] = ~pattern;
    }

}

#if defined (INCLUDE_FONT_24) || defined (INCLUDE_FONT_20) || defined (INCLUDE_FONT_16) || defined (INCLUDE_FONT_12) || defined (INCLUDE_FONT_8)
/**
 *  @brief: this draws a charactor on the frame buffer but not refresh
 */
void Paint::DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored) {
    uint16_t char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const uint8_t* ptr = &font->table[char_offset];

    for (uint16_t j = 0; j < font->Height; j++) {
        for (uint16_t i = 0; i < font->Width; i++) {
            if (pgm_read_byte(ptr) & (0x80 >> (i % 8))) {
                DrawPixel(x + i, y + j, colored);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (font->Width % 8 != 0) {
            ptr++;
        }
    }
}

/**
*  @brief: this displays a string on the frame buffer but not refresh
*/
void Paint::DrawStringAt(int x, int y, const char* text, sFONT* font, int colored) {
    const char* p_text = text;
    unsigned int counter = 0;
    int refcolumn = x;
    
    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        /* Display one character on EPD */
        DrawCharAt(refcolumn, y, *p_text, font, colored);
        /* Decrement the column position by 16 */
        refcolumn += font->Width;
        /* Point on the next character */
        p_text++;
        counter++;
    }
}
#endif // INCLUDE_FONT_*


/**
*  @brief: this draws a line on the frame buffer
*/
void Paint::DrawLine(int x0, int y0, int x1, int y1, int colored) {
    /* Bresenham algorithm */
    int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while((x0 != x1) && (y0 != y1)) {
        DrawPixel(x0, y0 , colored);
        if (2 * err >= dy) {     
            err += dy;
            x0 += sx;
        }
        if (2 * err <= dx) {
            err += dx; 
            y0 += sy;
        }
    }
    DrawPixel(x1, y1 , colored);
}

/**
*  @brief: this draws a horizontal line on the frame buffer
*/
void Paint::DrawHorizontalLine(int x, int y, int line_width, int colored) {
    int i;
    for (i = x; i < x + line_width; i++) {
        DrawPixel(i, y, colored);
    }
}

/**
*  @brief: this draws a vertical line on the frame buffer
*/
void Paint::DrawVerticalLine(int x, int y, int line_height, int colored) {
    int i;
    for (i = y; i < y + line_height; i++) {
        DrawPixel(x, i, colored);
    }
}

/**
*  @brief: this draws a rectangle
*/
void Paint::DrawRectangle(int x0, int y0, int x1, int y1, int colored) {
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    
    DrawHorizontalLine(min_x, min_y, max_x - min_x + 1, colored);
    DrawHorizontalLine(min_x, max_y, max_x - min_x + 1, colored);
    DrawVerticalLine(min_x, min_y, max_y - min_y + 1, colored);
    DrawVerticalLine(max_x, min_y, max_y - min_y + 1, colored);
}

/**
*  @brief: this draws a filled rectangle
*/
void Paint::DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored) {
    int min_x, min_y, max_x, max_y;
    int i;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    
    for (i = min_x; i <= max_x; i++) {
      DrawVerticalLine(i, min_y, max_y - min_y + 1, colored);
    }
}

/**
*  @brief: this draws a circle
*/
void Paint::DrawCircle(int x, int y, int radius, int colored) {
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do {
        DrawPixel(x - x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y - y_pos, colored);
        DrawPixel(x - x_pos, y - y_pos, colored);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos) {
              e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}

/**
*  @brief: this draws a filled circle
*/
void Paint::DrawFilledCircle(int x, int y, int radius, int colored) {
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do {
        DrawPixel(x - x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y - y_pos, colored);
        DrawPixel(x - x_pos, y - y_pos, colored);
        DrawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, colored);
        DrawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, colored);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }
        if(e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while(x_pos <= 0);
}


void Paint::DumpImage() {
  unsigned char *im = image;
  char pattern[9] = { 0, };
  Serial.print(buffer_width); Serial.print('x'); Serial.println(height);
  for (unsigned int y = 0; y < height; y++) {
    Serial.print('|');
    for (unsigned int x = 0; x < buffer_width; x++) {
      unsigned char i = 0;
      for (unsigned char m = 0x80; m; m >>= 1) {
        pattern[i++] = (*im & m) ? 'X' : ' ';
      }
      im++;
      Serial.print(pattern);
    }
    Serial.println('|');
  }
}

/* END OF FILE */























