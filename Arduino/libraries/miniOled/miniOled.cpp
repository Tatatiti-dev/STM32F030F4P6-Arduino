/*
V 0.4.0  miniOled.cpp

A VERY BASIC oled SSD1306 (0.96" I2C) library, compatible with SoftWire I2C library.
Designed initially to fit on STM32F030F4P6 board (16K flash, 48 MHz):
       for software I2C able to have custom pin selection
       for small flash memory footprint

1. Text. (Very) small characters.  Optional double width. Variable chr-spacing.
      page mode: always in one of 8 fixed horizontal lines ("pages" of SSD1306)
      horizontal mode: streaming over many lines (no interline spacing control).
2. Also big font 18x24 (inside 24x32) numbers 0-9 only, plus " "   "-"   "."   "/"  and  ":"
      still aligned to start on one of the 8 pages/lines.
3. <hr> horizontal line.
4. Load bitmap image.

I2C pins (software) configurable.

I2C output is unbuffered, one byte transfer at a time. Slow.
Derived by substantial hacking (down) of following:
   https://github.com/mistertwo/ozoled-oled-display-096-i2c-driver-library     2014/2015 

You need to download & install two other-party libraries
https://www.arduinolibraries.info/libraries/soft-wire   The 2.0.0 version of SoftWire
https://github.com/stevemarple/AsyncDelay   The AsyncDelay library, used by SoftWire
*/

#include "miniOled.h"
#define SOFTWIRE
// define SOFTWIRE, and software library SoftWire will be used, with choice of pins
// undefine SOFTWIRE, and native Wire on sda PA10  scl PA9   will apply


#ifdef SOFTWIRE
	#include <SoftWire.h>
	SoftWire Wire(PA6, PA5);  // default gpio (SoftWire needs a default)
#else
	#include <Wire.h>
#endif

OLED::OLED(byte w, byte h) : Adafruit_GFX(128,64){
	;
}

// ====================== LOW LEVEL =========================

void OLED::sendCommand(byte command){
	Wire.beginTransmission(OLED_ADDRESS); // begin transmitting
	Wire.write(OLED_COMMAND_MODE);//data mode
	Wire.write(command);
	Wire.endTransmission();    // stop transmitting
}


void OLED::sendData(byte data){
	
	Wire.beginTransmission(OLED_ADDRESS); // begin transmitting
	Wire.write(OLED_DATA_MODE);//data mode
	Wire.write(data);
	Wire.endTransmission();    // stop transmitting
}

void OLED::setCursorXY(byte X, byte Y){
	// Y - 1 unit = 1 page (8 pixel rows)   0 - 15
	// X - 1 unit = 8 pixel columns         0 - 7

    sendCommand(0x00 + (8*X & 0x0F)); 		//set column lower address
    sendCommand(0x10 + ((8*X>>4)&0x0F)); 	//set column higher address
	sendCommand(0xB0 + Y); 					//set page address
}


// =================== High Level ===========================

void OLED::init(int sda, int scl){  // customise sda.scl ignored for regular Wire
#ifdef SOFTWIRE
	Wire.enablePullups();
    Wire.setRxBuffer(ibuffer, 60);  // common buffer is OK, but rx buffer not used anyway
    Wire.setTxBuffer(ibuffer, 60);
    if(sda>=0) // SDA SCL optional parameters. Otherwise defaults apply 
    {
		Wire.setSda(sda);   // non-default pins
		Wire.setScl(scl);
    }
#endif
	Wire.begin() ;   

	static const byte ini[] = {0xae,0xa6,0xAE,0xD5,0x80,0xA8,0x3F,0xD3,
		0x0,0x40 | 0x0,0x8D,0x14,0x20,0x00,      
        0xA0 | 0x1,0xC8,0xDA,0x12,0x81,0xCF,
        0xd9,0xF1,0xDB,0x40,0xA4,0xA6,0x2e,0x20,0x00};

    for (byte i=0; i<29;i++)
        sendCommand(ini[i]);
    setPowerOn();
}


void OLED::clearDisplay()	{
	memset(buf,0,1024);
}


void OLED::setPowerOff(){
	sendCommand(OLED_CMD_DISPLAY_OFF);
}

void OLED::setPowerOn(){
	sendCommand(OLED_CMD_DISPLAY_ON);
}

void OLED::setPageMode(){
	addressingMode = PAGE_ADDRESSING;
	sendCommand(0x20); 				//set addressing mode
	sendCommand(PAGE_ADDRESSING); 	//set page addressing mode
}

void OLED::setHorizontalMode(){
	addressingMode = HORIZONTAL_ADDRESSING;
	sendCommand(0x20); 				//set addressing mode
	sendCommand(HORIZONTAL_ADDRESSING); 	//set page addressing mode
}
void OLED::drawPixel(int x, int y, uint16_t color){
	int byteid = x;
	byteid += ((y/8)*128);
	int bitid = y%8;
	bitWrite(buf[byteid],bitid,color);
}
void OLED::display(){
	setCursorXY(0, 0);
	byte column = 0; 
	byte Y = 0;
	for(int i=0; i<1024; i++) {  
		if(column%8 == 0)setCursorXY(column/8,Y);
		sendData(buf[i]);
		if(++column == 128) {
			column = 0;
			setCursorXY( 0, ++Y);
		} 
	}
}

