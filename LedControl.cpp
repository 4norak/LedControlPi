/*
 *    LedControl.cpp - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle, modified and adapted to run on a Raspberry Pi by Anorak
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


#include "LedControl.h"

LedControl::LedControl(const int dataPin, const int clkPin, const int csPin, int numDevices) {
    //initialize the GPIOs
    wiringPiSetupGpio();
    
    //set the pins
    SPI_MOSI=dataPin;
    SPI_CLK=clkPin;
    SPI_CS=csPin;
    
    //set the number of devices used
    if(numDevices<=0 || numDevices>MAX_DEVICES)
        numDevices=MAX_DEVICES;
    maxDevices=numDevices;

    //initialize the pins
    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,HIGH);
    
    //initialize all LED-states to off
    for(int i=0;i<MAX_DEVICES*8;i++) 
        status[i]=0x00;

    //initialize all displays
    for(int i=0;i<maxDevices;i++) {
        spiTransfer(i,OP_DISPLAYTEST,0);
        //scanlimit is set to max on startup
        setScanLimit(i,7);
        //decode is done in source
        spiTransfer(i,OP_DECODEMODE,0);
        //we go into shutdown-mode on startup
        shutdown(i,true);
        //no, we don't
        shutdown(i, false);
        //prepare display
        setIntensity(i, 8);
        clearDisplay(i);
    }
}

int LedControl::getDeviceCount() {
    return maxDevices;
}

void LedControl::shutdown(const int addr, const bool status) {
    if(addr<0 || addr>=maxDevices)
        return;
    spiTransfer(addr, OP_SHUTDOWN,!status);
}

void LedControl::setScanLimit(const int addr, const int limit) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(limit>=0 && limit<8)
        spiTransfer(addr, OP_SCANLIMIT, limit);
}

void LedControl::setIntensity(const int addr, const int intensity) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(intensity>=0 && intensity<16)	
        spiTransfer(addr, OP_INTENSITY, intensity);
}

void LedControl::clearDisplay(const int addr) {
    int offset;
    if(addr<0 || addr>=maxDevices)
        return;
    offset=addr*8;
    for(int i=0;i<8;i++) {
        status[offset+i]=0;
        spiTransfer(addr, i+1, status[offset+i]);
    }
}

void LedControl::setLed(const int addr, const int row, const int column, const bool state) {
    int offset;
    byte val=0x00;
    if(addr<0 || addr>=maxDevices)
        return;
    if(row<0 || row>7 || column<0 || column>7)
        return;
    offset=addr*8;
    val=0b10000000 >> column;
    if(state)
        status[offset+row]=status[offset+row]|val;
    else {
        val=~val;
        status[offset+row]=status[offset+row]&val;
    }
    spiTransfer(addr, row+1, status[offset+row]);
}

void LedControl::setRow(const int addr, const int row, const byte value) {
    int offset;
    if(addr<0 || addr>=maxDevices)
        return;
    if(row<0 || row>7)
        return;
    offset=addr*8;
    status[offset+row]=value;
    spiTransfer(addr, row+1, status[offset+row]);
}

void LedControl::setColumn(const int addr, const int col, const byte value) {
    byte val;
    if(addr<0 || addr>=maxDevices)
        return;
    if(col<0 || col>7) 
        return;
    for(int row=0;row<8;row++) {
        val=value >> (7-row);
        val=val & 0x01;
        setLed(addr, row, col, val);
    }
}

void LedControl::setCharacter(const int addr, char character) {
    if(character < ' ' || character > '~')
        character = ' ';
    const byte *charMap = digits[character-' '];
    for(int row=0; row<8; row++)
        LedControl::setRow(addr, row, charMap[row]);
}

void LedControl::spiTransfer(const int addr, volatile const byte opcode, volatile const byte data) {
    //Create an array with the data to shift out
    int offset=addr*2;
    int maxbytes=maxDevices*2;
    for(int i=0;i<maxbytes;i++)
        spidata[i]=(byte)0;
    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;
    //Now shift out the data 
    digitalWrite(SPI_CS, LOW);
    for(int i=maxbytes;i>0;i--)
        shiftOut(spidata[i-1]);
    digitalWrite(SPI_CS, HIGH);
}

void LedControl::shiftOut(byte data) {
    for(int i = 0; i < 8; i++) {
        digitalWrite(SPI_MOSI, data & 0x80);
        digitalWrite(SPI_CLK, HIGH);
        data <<= 1;
        digitalWrite(SPI_CLK, LOW);
    }
}

void LedControl::cleanUp() {
    digitalWrite(SPI_CS, HIGH);
    digitalWrite(SPI_CLK, LOW);
    digitalWrite(SPI_MOSI, 0);
    digitalWrite(SPI_CLK, HIGH);
    digitalWrite(SPI_CLK, LOW);
}
