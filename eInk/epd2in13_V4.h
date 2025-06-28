/*****************************************************************************
* | File      	:   epd2in13_V4.h
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

#ifndef epd2in13_V4
#define epd2in13_V4

// Display resolution
#define EPD_WIDTH       122
#define EPD_HEIGHT      250

#define INIT_MODE_NONE    0
#define INIT_MODE_FULL    1
#define INIT_MODE_FAST    2
#define INIT_MODE_PART    3

// commands
#define DRIVER_OUTPUT_CONTROL  0x01
#define GATE_DRIVING_VOLTAGE_CONTROL  0x03
#define SOURCE_DRIVING_VOLTAGE_CONTROL  0x04
#define INITIAL_CODE_SETTING_OTP_PROGRAM  0x08
#define WRITE_REGISTER_FOR_INITIAL_CODE_SETTING  0x09
#define BOOSTER_SOFT_START_CONTROL  0x0C
#define ENTER_DEEP_SLEEP  0x10
#define DATA_ENTRY_MODE  0x11
#define SOFT_RESET  0x12
#define HV_READY_DETECTION  0x14
#define VCI_DETECTION  0x15
#define TEMPERATURE_SENSOR_SELECTION  0x18
#define WRITE_TO_TEMPERATURE_REGISTER  0x1A
#define READ_FROM_TEMPERATURE_REGISTER  0x1B
#define MASTER_ACTIVATION  0x20
#define DISPLAY_UPDATE_CONTROL_1  0x21
#define DISPLAY_UPDATE_CONTROL_2  0x22
#define WRITE_RAM_BLACK  0x24
#define WRITE_RAM_RED  0x26
#define READ_RAM  0x27 // according to 0x41
#define VCOM_SENSE  0x28
#define VCOM_SENSE_DURATION  0x29
#define PROGRAM_VCOM_OTP  0x2A
#define WRITE_REGISTER_FOR_VCOM_CONTROL  0x2B
#define WRITE_VCOM_REGISTER  0x2C
#define OTP_REGISTER_READ_FOR_DISPLAY_OPTION  0x2D
#define USER_ID_READ  0x2E
#define STATUS_BIT_READ  0x2F
#define BORDER_WAVE_FORM  0x3C
#define READRAM_OPTION  0x41 // 0=black RAM, 1=red RAM
#define SET_RAM_X_ADDRESS_START_END_POSITION  0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION  0x45
#define SET_RAM_X_ADDRESS_COUNTER  0x4E
#define SET_RAM_Y_ADDRESS_COUNTER  0x4F
#define AUTO_WRITE_RED_RAM_FOR_REGULAR_PATTERN  0x46
#define AUTO_WRITE_BLACK_RAM_FOR_REGULAR_PATTERN  0x47
#define NOP  0x7F

// DISPLAY_UPDATE_CONTROL_1 bit mask
#define CTRL1_UPDATE_BLACK_NORMAL  0x00
#define CTRL1_UPDATE_BLACK_FORCE_0 0x04
#define CTRL1_UPDATE_BLACK_INVERT  0x08
#define CTRL1_UPDATE_RED_NORMAL    0x00
#define CTRL1_UPDATE_RED_FORCE_0   0x40
#define CTRL1_UPDATE_RED_INVERT    0x80

// DISPLAY_UPDATE_CONTROL_2 bit mask
#define CTRL2_ENABLE_CLOCK_SIGNAL     0x80
#define CTRL2_DISABLE_CLOCK_SIGNAL    0x01
#define CTRL2_ENABLE_ANALOG           0x40
#define CTRL2_DISABLE_ANALOG          0x02
#define CTRL2_LOAD_TEMPERATURE_VALUE  0x20
#define CTRL2_LOAD_LUT                0x10
#define CTRL2_DISPLAY_MODE_2          0x08
#define CTRL2_DISPLAY                 0x04

// DATA_ENTRY_MODE bit mask , "common reading sense" is X_THEN_Y | LEFT_TO_RIGHT | TOP_TO_BOTTOM
#define RIGHT_TO_LEFT 0x00
#define LEFT_TO_RIGHT 0x01
#define BOTTOM_TO_TOP 0x00
#define TOP_TO_BOTTOM 0x02
#define X_THEN_Y      0x00
#define Y_THEN_X      0x04

// REFRESH MODES
#define REFRESH_NONE 0
#define REFRESH_FAST ( \
    CTRL2_ENABLE_CLOCK_SIGNAL | CTRL2_ENABLE_ANALOG \
    | CTRL2_DISPLAY \
    | CTRL2_DISABLE_ANALOG | CTRL2_DISABLE_CLOCK_SIGNAL \
)
#define REFRESH_FULL ( \
    CTRL2_ENABLE_CLOCK_SIGNAL | CTRL2_ENABLE_ANALOG \
    | CTRL2_LOAD_TEMPERATURE_VALUE | CTRL2_LOAD_LUT | CTRL2_DISPLAY \
    | CTRL2_DISABLE_ANALOG | CTRL2_DISABLE_CLOCK_SIGNAL \
)
#define REFRESH_PART ( \
    CTRL2_ENABLE_CLOCK_SIGNAL | CTRL2_ENABLE_ANALOG \
    | CTRL2_LOAD_TEMPERATURE_VALUE | CTRL2_LOAD_LUT | CTRL2_DISPLAY \
    | CTRL2_DISPLAY_MODE_2 \
    | CTRL2_DISABLE_ANALOG | CTRL2_DISABLE_CLOCK_SIGNAL \
)

#define LAYER_BLACK WRITE_RAM_BLACK // 1 = white , 0 = black
#define LAYER_RED   WRITE_RAM_RED   // 1 = red , 0 = under color

class Epd {
public:
    unsigned long width;
    unsigned long height;
    int bufwidth;
    int bufheight;
    int count;

    Epd(
        unsigned int reset_pin,
        unsigned int dc_pin,
        unsigned int cs_pin,
        unsigned int busy_pin
    );
    ~Epd();

    int  Init(char Mode);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);

	void SetWindow(
        unsigned int Xstart,
        unsigned int Ystart,
        unsigned int Xend,
        unsigned int Yend
    );
	void SetCursor(unsigned int Xstart, unsigned int Ystart);
    void Reset(void);
    void Display(const unsigned char* frame_buffer, const unsigned int len, const unsigned char layer = LAYER_BLACK, const unsigned char refresh_mode = REFRESH_FULL);
    void DisplayPgm(const unsigned char* frame_buffer, const unsigned int len, const unsigned char layer = LAYER_BLACK, const unsigned char refresh_mode = REFRESH_FULL);
    void DisplayQuarter(const unsigned char* frame_buffer, const unsigned char layer = LAYER_BLACK, const unsigned char refresh_mode = REFRESH_FULL);
    void Clear(const unsigned char refresh_mode = REFRESH_FULL);
    void Refresh(const unsigned char refresh_mode = REFRESH_FULL);

    void Sleep(void);

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;

    void SpiTransfer(unsigned char data);
};

#endif /* EPD2IN13_V4_H */

/* END OF FILE */
