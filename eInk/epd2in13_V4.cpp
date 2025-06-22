/*****************************************************************************
* | File      	:   epd2in13_V4.cpp
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper V4
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-6-25
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include "epd2in13_V4.h"

Epd::~Epd() {
}

/******************************************************************************
function :	Pin definition
parameter:
******************************************************************************/
Epd::Epd(
        unsigned int reset_pin,
        unsigned int dc_pin,
        unsigned int cs_pin,
        unsigned int busy_pin) {
    this->reset_pin = reset_pin;
    this->dc_pin = dc_pin;
    this->cs_pin = cs_pin;
    this->busy_pin = busy_pin;

    width = EPD_WIDTH;
    height = EPD_HEIGHT;
    bufwidth = 128/8;  //16
    bufheight = 63;
}

/******************************************************************************
function :	send command
parameter:
     command : Command register
******************************************************************************/
void Epd::SendCommand(unsigned char command) {
    digitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
void Epd::SendData(unsigned char data) {
    digitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

void Epd::SpiTransfer(unsigned char data) {
    digitalWrite(cs_pin, LOW);
    SPI.transfer(data);
    digitalWrite(cs_pin, HIGH);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void Epd::WaitUntilIdle(void) {
    while(digitalRead(busy_pin)) {      //LOW: idle, HIGH: busy
        delay(10);
    }
}

/******************************************************************************
function :	Setting the display window
parameter:
	Xstart : X-axis starting position
	Ystart : Y-axis starting position
	Xend : End position of X-axis
	Yend : End position of Y-axis
******************************************************************************/
void Epd::SetWindows(
        unsigned char Xstart,
        unsigned char Ystart,
        unsigned char Xend,
        unsigned char Yend) {
    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    SendData((Xstart>>3) & 0xFF);
    SendData((Xend>>3) & 0xFF);
	
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
    SendData(Yend & 0xFF);
    SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
	Xstart : X-axis starting position
	Ystart : Y-axis starting position
******************************************************************************/
void Epd::SetCursor(unsigned char Xstart, unsigned char Ystart) {
    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    SendData(Xstart & 0xFF);

    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
}



/******************************************************************************
function :	Initialize the e-Paper register
parameter:
	Mode : Mode selection
******************************************************************************/
int Epd::Init(char Mode) {
    pinMode(reset_pin, OUTPUT);
    pinMode(dc_pin, OUTPUT);
    pinMode(cs_pin, OUTPUT);
    pinMode(busy_pin, INPUT); 
    
    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    
    Reset();
    
    if(Mode == FULL) {
        WaitUntilIdle();
        SendCommand(SOFT_RESET);
        WaitUntilIdle();

        SendCommand(DRIVER_OUTPUT_CONTROL);
        SendData(0xF9);
        SendData(0x00);
        SendData(0x00);

        SendCommand(DATA_ENTRY_MODE);
        SendData(0x03);

		SetWindows(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
		SetCursor(0, 0);
		
		SendCommand(BORDER_WAVE_FORM);
		SendData(0x05);	

		SendCommand(DISPLAY_UPDATE_CONTROL_1);
		SendData(0x00);
		SendData(0x80);	

		SendCommand(TEMPERATURE_SENSOR_SELECTION);
		SendData(0x80);	
		WaitUntilIdle();
    } else if(Mode == FAST) {
        WaitUntilIdle();
        SendCommand(SOFT_RESET);
        WaitUntilIdle();

        SendCommand(TEMPERATURE_SENSOR_SELECTION);
		SendData(0x80);	

        SendCommand(DATA_ENTRY_MODE);
        SendData(0x03);

		SetWindows(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
		SetCursor(0, 0);
		
		SendCommand(DISPLAY_UPDATE_CONTROL_2); // Load temperature value
		SendData(ENABLE_CLOCK_SIGNAL | LOAD_TEMPERATURE_VALUE | LOAD_LUT_WITH_DISPLAY_MODE_1 | DISABLE_CLOCK_SIGNAL);	
        SendCommand(MASTER_ACTIVATION);
        WaitUntilIdle();

		SendCommand(WRITE_TO_TEMPERATURE_REGISTER);
		SendData(0x64);
		SendData(0x00);	

        SendCommand(DISPLAY_UPDATE_CONTROL_2); // stop load temperature value
		SendData(ENABLE_CLOCK_SIGNAL | LOAD_LUT_WITH_DISPLAY_MODE_1 | DISABLE_CLOCK_SIGNAL);
		SendCommand(MASTER_ACTIVATION);	
		WaitUntilIdle();
    } else if(Mode == PART) {	
		digitalWrite(reset_pin, LOW); // module reset
		delay(1);
		digitalWrite(reset_pin, HIGH);
		
		SendCommand(BORDER_WAVE_FORM);
		SendData(0x80);	

        SendCommand(DRIVER_OUTPUT_CONTROL);
		SendData(0xF9);	
        SendData(0x00);	
        SendData(0x00);	
	
		SendCommand(DATA_ENTRY_MODE);
        SendData(0x03); 
		
		SetWindows(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
		SetCursor(0, 0);
    } else {
        return -1;
    }

    return 0;
}

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
void Epd::Reset(void) {
    digitalWrite(reset_pin, HIGH);
    delay(20);
    digitalWrite(reset_pin, LOW);                //module reset
    delay(2);
    digitalWrite(reset_pin, HIGH);
    delay(20);
    this->count = 0; 
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
	frame_buffer : Image data
******************************************************************************/
void Epd::Display(const unsigned char* frame_buffer) {
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        SendCommand(WRITE_RAM_BLACK);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }
    }

    //DISPLAY REFRESH
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(
        ENABLE_CLOCK_SIGNAL | ENABLE_ANALOG
      | LOAD_TEMPERATURE_VALUE | LOAD_LUT_WITH_DISPLAY_MODE_1 | DISPLAY_WITH_DISPLAY_MODE_1
      | DISABLE_ANALOG | DISABLE_CLOCK_SIGNAL);
    SendCommand(MASTER_ACTIVATION);
    WaitUntilIdle();
}

void Epd::DisplayQuarter(const unsigned char* frame_buffer) {
    if(this->count == 0) {
        SendCommand(WRITE_RAM_BLACK);
        this->count++;
    } else if(this->count > 0 && this->count < 4 ) {
        this->count++;
    }
    for(int i = 0; i < this->bufwidth * this->bufheight; i++) {
            SendData(frame_buffer[i]);
    }
    if(this->count == 4) {
        SendCommand(DISPLAY_UPDATE_CONTROL_2);
        SendData(
            ENABLE_CLOCK_SIGNAL | ENABLE_ANALOG
            | LOAD_TEMPERATURE_VALUE | LOAD_LUT_WITH_DISPLAY_MODE_1 | DISPLAY_WITH_DISPLAY_MODE_1
            | DISABLE_ANALOG | DISABLE_CLOCK_SIGNAL);
        SendCommand(MASTER_ACTIVATION);
        WaitUntilIdle();
        this->count = 0;
    }
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and fast displays
parameter:
	frame_buffer : Image data
******************************************************************************/
void Epd::Display_Fast(const unsigned char* frame_buffer) {
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        SendCommand(WRITE_RAM_BLACK);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }
    }

    //DISPLAY REFRESH
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(
        ENABLE_CLOCK_SIGNAL | ENABLE_ANALOG
      | DISPLAY_WITH_DISPLAY_MODE_1
      | DISABLE_ANALOG | DISABLE_CLOCK_SIGNAL);
    SendData(0xC7);
    SendCommand(MASTER_ACTIVATION);
    WaitUntilIdle();
}




/******************************************************************************
function :	Refresh a base image
parameter:
	frame_buffer : Image data	
******************************************************************************/
void Epd::DisplayPartBaseImage(const unsigned char* frame_buffer) {
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        SendCommand(WRITE_RAM_BLACK);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }

        SendCommand(WRITE_RAM_RED);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }
    }

    //DISPLAY REFRESH
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(
        ENABLE_CLOCK_SIGNAL | ENABLE_ANALOG
        | LOAD_TEMPERATURE_VALUE | LOAD_LUT_WITH_DISPLAY_MODE_1 | DISPLAY_WITH_DISPLAY_MODE_1
        | DISABLE_ANALOG | DISABLE_CLOCK_SIGNAL);
    SendCommand(MASTER_ACTIVATION);
    WaitUntilIdle();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and partial refresh
parameter:
	frame_buffer : Image data
******************************************************************************/
void Epd::DisplayPart(const unsigned char* frame_buffer) {
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        SendCommand(WRITE_RAM_BLACK);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }
    }

    //DISPLAY REFRESH
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(
        ENABLE_CLOCK_SIGNAL | ENABLE_ANALOG
        | LOAD_TEMPERATURE_VALUE | LOAD_LUT_WITH_DISPLAY_MODE_2 | DISPLAY_WITH_DISPLAY_MODE_1
        | DISABLE_ANALOG | DISABLE_CLOCK_SIGNAL);
    SendCommand(MASTER_ACTIVATION);
    WaitUntilIdle();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void Epd::Clear(void) {
    // SendCommand(DISPLAY_UPDATE_CONTROL_1);
    // // SendData(UPDATE_RED_FORCE_0 | UPDATE_BLACK_FORCE_0);
    // SendData(UPDATE_RED_FORCE_0 | UPDATE_BLACK_INVERT);
    // SendData(0x80);
    int w, h;
    w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    h = EPD_HEIGHT;
    SendCommand(WRITE_RAM_BLACK);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            SendData(0xff);
        }
    }
    SendCommand(WRITE_RAM_RED);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            SendData(0x00);
        }
    }

    //DISPLAY REFRESH
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(
        ENABLE_CLOCK_SIGNAL | ENABLE_ANALOG
      | LOAD_TEMPERATURE_VALUE | LOAD_LUT_WITH_DISPLAY_MODE_1 | DISPLAY_WITH_DISPLAY_MODE_1
      | DISABLE_ANALOG | DISABLE_CLOCK_SIGNAL);
    SendCommand(MASTER_ACTIVATION);
    WaitUntilIdle();

    // SendCommand(DISPLAY_UPDATE_CONTROL_1);
    // SendData(UPDATE_RED_NORMAL | UPDATE_BLACK_NORMAL);
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void Epd::Sleep() {
    SendCommand(ENTER_DEEP_SLEEP); //enter deep sleep
    SendData(0x01);
    delay(200);

    digitalWrite(reset_pin, LOW);
}

/* END OF FILE */
