/*
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle, slightly modified and adapted to run on a Raspberry Pi by Anorak
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LedControl_h
#define LedControl_h

#include <wiringPi.h>

typedef char byte;

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

#define MAX_DEVICES 24

constexpr byte digits[][8] = {{0x3e, 0x51, 0x49, 0x45, 0x3e, 0x00, 0x00, 0x00},  // '0' //TODO: Rename, add other characters
                              {0x00, 0x42, 0x7f, 0x40, 0x00, 0x00, 0x00, 0x00},  // '1'
                              {0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x00, 0x00},  // '2'
                              {0x21, 0x41, 0x45, 0x4b, 0x31, 0x00, 0x00, 0x00},  // '3'
                              {0x18, 0x14, 0x12, 0x7f, 0x10, 0x00, 0x00, 0x00},  // '4'
                              {0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00, 0x00},  // '5'
                              {0x3c, 0x4a, 0x49, 0x49, 0x30, 0x00, 0x00, 0x00},  // '6'
                              {0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00},  // '7'
                              {0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00},  // '8'
                              {0x06, 0x49, 0x49, 0x29, 0x1e, 0x00, 0x00, 0x00}};  // '9'

class LedControl {
    private :
        /* The array for shifting the data to the devices */
        byte spidata[MAX_DEVICES * 2];
        /* Send out a single command to the device */
        void spiTransfer(int addr, byte opcode, byte data);
        /* Replacement for the Arduino-function */
        void shiftOut(byte data);

        /* We keep track of the led-status for all 8 devices in this array */
        byte status[MAX_DEVICES * 8];
        /* Data is shifted out of this pin*/
        int SPI_MOSI;
        /* The clock is signaled on this pin */
        int SPI_CLK;
        /* This one is driven LOW for chip selection */
        int SPI_CS;
        /* The maximum number of devices we use */
        int maxDevices;

    public:
        /* 
         * Create a new controler 
         * Params :
         * dataPin		pin on the Arduino where data gets shifted out
         * clockPin		pin for the clock
         * csPin		pin for selecting the device 
         * numDevices	maximum number of devices that can be controled
         */
        LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);

        /*
         * Gets the number of devices attached to this LedControl.
         * Returns :
         * int	the number of devices on this LedControl
         */
        int getDeviceCount();

        /* 
         * Set the shutdown (power saving) mode for the device
         * Params :
         * addr	The address of the display to control
         * status	If true the device goes into power-down mode. Set to false
         *		for normal operation.
         */
        void shutdown(int addr, bool status);

        /* 
         * Set the number of digits (or rows) to be displayed.
         * See datasheet for sideeffects of the scanlimit on the brightness
         * of the display.
         * Params :
         * addr	address of the display to control
         * limit	number of digits to be displayed (1..8)
         */
        void setScanLimit(int addr, int limit);

        /* 
         * Set the brightness of the display.
         * Params:
         * addr		the address of the display to control
         * intensity	the brightness of the display. (0..15)
         */
        void setIntensity(int addr, int intensity);

        /* 
         * Switch all Leds on the display off. 
         * Params:
         * addr	address of the display to control
         */
        void clearDisplay(int addr);

        /* 
         * Set the status of a single Led.
         * Params :
         * addr	address of the display 
         * row	the row of the Led (0..7)
         * col	the column of the Led (0..7)
         * state	If true the led is switched on, 
         *		if false it is switched off
         */
        void setLed(int addr, int row, int col, bool state);

        /* 
         * Set all 8 Led's in a row to a new state
         * Params:
         * addr	address of the display
         * row	row which is to be set (0..7)
         * value	each bit set to 1 will light up the
         *		corresponding Led.
         */
        void setRow(int addr, int row, byte value);

        /* 
         * Set all 8 Led's in a column to a new state
         * Params:
         * addr	address of the display
         * col	column which is to be set (0..7)
         * value	each bit set to 1 will light up the
         *		corresponding Led.
         */
        void setColumn(int addr, int col, byte value);

        /*
         * Write a character to a display
         * Params:
         * addr address of the display
         * character the caracter to be displayed
         */
        void setCharacter(int addr, char character);

        /*Clean up on Ctrl+C or SIGTERM*/
        void cleanUp();
};


#endif	//LedControl.h
