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
    bufwidth = (EPD_WIDTH + 7) >> 3; // ceil(EPD_WIDTH / 8)
    bufheight = 63; // quarter buffer height = ceil(EPD_HEIGHT/4)
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
	Xstart : X-axis starting position : must be multiple of 8
	Ystart : Y-axis starting position
	Xend : End position of X-axis : must be multiple of 8
	Yend : End position of Y-axis
******************************************************************************/
void Epd::SetWindow(
        unsigned int Xstart,
        unsigned int Ystart,
        unsigned int Xend,
        unsigned int Yend) {
    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    SendData(Xstart >> 3);
    SendData(Xend >> 3);
	
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    SendData(Ystart);
    SendData(Ystart >> 8);
    SendData(Yend);
    SendData(Yend >> 8);
}

/******************************************************************************
function :	Set Cursor
parameter:
	Xstart : X-axis starting position
	Ystart : Y-axis starting position
******************************************************************************/
void Epd::SetCursor(unsigned int Xstart, unsigned int Ystart) {
    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    SendData(Xstart >> 3);

    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(Ystart);
    SendData(Ystart >> 8);
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
    
    if(Mode == INIT_MODE_FULL) {
        WaitUntilIdle();
        SendCommand(SOFT_RESET);
        WaitUntilIdle();

        SendCommand(DRIVER_OUTPUT_CONTROL);
        SendData(0xF9);
        SendData(0x00);
        SendData(0x00);

        SendCommand(DATA_ENTRY_MODE);
        SendData(X_THEN_Y | LEFT_TO_RIGHT | TOP_TO_BOTTOM);

		SetWindow(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
		SetCursor(0, 0);
		
		SendCommand(BORDER_WAVE_FORM);
		SendData(0x05);	

		SendCommand(DISPLAY_UPDATE_CONTROL_1);
		SendData(CTRL1_UPDATE_BLACK_INVERT | CTRL1_UPDATE_RED_NORMAL);
		SendData(0x80);	// Available Source from S8 to S167 instead of S0 to S175 ??

		SendCommand(TEMPERATURE_SENSOR_SELECTION);
		SendData(0x80);	
		WaitUntilIdle();
    } else if(Mode == INIT_MODE_FAST) {
        WaitUntilIdle();
        SendCommand(SOFT_RESET);
        WaitUntilIdle();

        SendCommand(TEMPERATURE_SENSOR_SELECTION);
		SendData(0x80);	// internal censor

        SendCommand(DATA_ENTRY_MODE);
        SendData(X_THEN_Y | LEFT_TO_RIGHT | TOP_TO_BOTTOM);

		SetWindow(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
		SetCursor(0, 0);
		
		SendCommand(DISPLAY_UPDATE_CONTROL_2); // Load temperature value
		SendData(CTRL2_ENABLE_CLOCK_SIGNAL | CTRL2_LOAD_TEMPERATURE_VALUE | CTRL2_LOAD_LUT | CTRL2_DISABLE_CLOCK_SIGNAL);	
        SendCommand(MASTER_ACTIVATION);
        WaitUntilIdle();

		SendCommand(WRITE_TO_TEMPERATURE_REGISTER);
		SendData(0x64);
		SendData(0x00);	

        SendCommand(DISPLAY_UPDATE_CONTROL_2); // stop load temperature value
		SendData(CTRL2_ENABLE_CLOCK_SIGNAL | CTRL2_LOAD_LUT | CTRL2_DISABLE_CLOCK_SIGNAL);
		SendCommand(MASTER_ACTIVATION);	
		WaitUntilIdle();
    } else if(Mode == INIT_MODE_PART) {	
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
        SendData(X_THEN_Y | LEFT_TO_RIGHT | TOP_TO_BOTTOM); 
		
		SetWindow(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
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
    refresh_mode
******************************************************************************/
void Epd::Display(const unsigned char* frame_buffer, const unsigned int len, const unsigned char layer, const unsigned char refresh_mode) {
    SendCommand(layer);
    int l = len;
    while (l--) {
        SendData(*frame_buffer++);
    }

    Refresh(refresh_mode);
}
void Epd::DisplayPgm(const unsigned char* frame_buffer, const unsigned int len, const unsigned char layer, const unsigned char refresh_mode) {
    SendCommand(layer);
    int l = len;
    while (l--) {
        SendData(pgm_read_byte(frame_buffer++));
    }

    Refresh(refresh_mode);
}

void Epd::DisplayQuarter(const unsigned char* frame_buffer, const unsigned char layer, const unsigned char refresh_mode) {
    if(this->count == 0) {
        SendCommand(layer);
        this->count++;
    } else if(this->count > 0 && this->count < 4 ) {
        this->count++;
    }
    for(int i = 0; i < this->bufwidth * this->bufheight; i++) {
        SendData(frame_buffer[i]);
    }
    if(this->count == 4) {
        Refresh(refresh_mode);
        this->count = 0;
    }
}

/******************************************************************************
function :	Clear screen
parameter:
   refresh_mode: REFRESH_NONE (to prepare buffer only) , REFRESH_FAST or REFRESH_FULL
******************************************************************************/
void Epd::Clear(const unsigned char refresh_mode) {
    int len = ( (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1) ) * EPD_HEIGHT;
    SendCommand(WRITE_RAM_BLACK);
    for (int i = 0; i < len; i++) {
        SendData(0x00);
    }
    SendCommand(WRITE_RAM_RED);
    for (int i = 0; i < len; i++) {
        SendData(0x00);
    }

    Refresh(refresh_mode);
}

/******************************************************************************
function :	Sends refresh sequence
parameter:
	mode : fast / full refresh
******************************************************************************/
void Epd::Refresh(const unsigned char refresh_mode) {
    if (refresh_mode) {
        SendCommand(DISPLAY_UPDATE_CONTROL_2);
        SendData(refresh_mode);
        SendCommand(MASTER_ACTIVATION);
        WaitUntilIdle();
    }
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
