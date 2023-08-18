/*
  miniOled.h - 0.96' I2C 128x64 OLED Driver Library
*/

#ifndef OLED_H
#define OLED_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

#define OLED_ADDRESS            0x3C

// registers
#define OLED_COMMAND_MODE       0x80
#define OLED_DATA_MODE          0x40

// commands
#define OLED_CMD_DISPLAY_OFF    0xAE
#define OLED_CMD_DISPLAY_ON     0xAF

#define HORIZONTAL_ADDRESSING   0x00
#define PAGE_ADDRESSING         0x02

#define BLACK 0
#define WHITE 1

class OLED : public Adafruit_GFX{

public:
	OLED(byte width = 128, byte height = 64);
    byte addressingMode;
    void init(int sda= -1, int scl = -1);
    void setCursorXY(byte Column, byte Row);
    void clearDisplay();
    void setPowerOff();
    void setPowerOn();
    void setPageMode();
    void setHorizontalMode();
	void drawPixel(int x, int y, uint16_t color);
	void display();

private:
	uint8_t buf[1024] {
		B10000000, B10000000, B11000000, B11000000, B11100000, B11100000, B11110000, B11110000, 
		B11111000, B11111000, B11111100, B11111100, B11111110, B11111110, B11111111, B11111111,
	};
    void sendCommand(byte command);
    void sendData(byte Data);
    char ibuffer[60];  // for i2c
    char buffer[20];  // for integer calc
};


#endif


