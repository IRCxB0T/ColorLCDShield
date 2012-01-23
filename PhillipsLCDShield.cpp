/**
 *  PhillipsLCDShield.cpp - Arduino Library to control a Nokia 6100 LCD,
 *  specifically that found on SparkFun's Color LCD Shield.
 *  This code should work for Phillips display drivers
 *  normally found on the Color LCD Shield.
 *
 *  License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 *  to use and abuse this code however you'd like. If you find it useful
 *  please attribute, and SHARE-ALIKE!
 *
 *  This is based on code by Jim Bloom.
 **/


#include "PhillipsLCDShield.h"

extern "C" {
	#if defined(ARDUINO) && ARDUINO >= 100
		#include "wiring_private.h"
	#else
		#include "wiring.h"
	#endif
}

void PhillipsLCDShield::init(void)
{
	delay(200);
	
	cbi(LCD_PORT_SCK, SCK);     //CLK = LOW
	cbi(LCD_PORT_DIO, DIO);     //DIO = LOW
	delayMicroseconds(10);
	sbi(LCD_PORT_CS, CS);       //CS = HIGH
	delayMicroseconds(10);
	cbi(LCD_PORT_RES, LCD_RES); //RESET = LOW
	delay(200);
	sbi(LCD_PORT_RES, LCD_RES); //RESET = HIGH
	delay(200);
	sbi(LCD_PORT_SCK, SCK);     // SCK = HIGH
	sbi(LCD_PORT_DIO, DIO);     // DIO = HIGH
	delayMicroseconds(10);
	
	LCDCommand(SLEEPOUT); //sleep out(PHILLIPS)
	
	LCDCommand(BSTRON);   //Booset On(PHILLIPS)
	
	LCDCommand(COLMOD);   // Set Color Mode(PHILLIPS)
	LCDData(0x03);
	
	LCDCommand(MADCTL);   // Memory Access Control(PHILLIPS)
	LCDData(0xC8);
	
	LCDCommand(SETCON);   // Set Contrast(PHILLIPS)
	LCDData(0x30);
	
	LCDCommand(NOPP);     // nop(PHILLIPS)
	
	delayMicroseconds(200);
	
	LCDCommand(DISPON);   // display on(PHILLIPS)
}

void PhillipsLCDShield::clear(int color)
{
	LCDCommand(PASETP);
	LCDData(0);
	LCDData(131);
	
	LCDCommand(CASETP);
	LCDData(0);
	LCDData(131);
	
	LCDCommand(RAMWRP);
	
	for(unsigned int i=0; i < (131*131)/2; i++)
	{
		LCDData((color>>4)&0x00FF);
		LCDData(((color&0x0F)<<4)|(color>>8));
		LCDData(color&0x0FF);
	}
	
	x_offset = 0;
	y_offset = 0;
}

void PhillipsLCDShield::setChar(char c, int x, int y, int fColor, int bColor)
{
	y	=	(COL_HEIGHT - 1) - y; // make display "right" side up
	x	=	(ROW_LENGTH - 2) - x;
	
	int             i,j;
	unsigned int    nCols;
	unsigned int    nRows;
	unsigned int    nBytes;
	unsigned char   PixelRow;
	unsigned char   Mask;
	unsigned int    Word0;
	unsigned int    Word1;
	unsigned char   *pFont;
	unsigned char   *pChar;
	
	// get pointer to the beginning of the selected font table
	pFont = (unsigned char *)FONT8x16;
	// get the nColumns, nRows and nBytes
	nCols = *pFont;
	nRows = *(pFont + 1);
	nBytes = *(pFont + 2);
	// get pointer to the last byte of the desired character
	pChar = pFont + (nBytes * (c - 0x1F)) + nBytes - 1;
	
	// Row address set (command 0x2B)
	LCDCommand(PASETP);
	LCDData(x);
	LCDData(x + nRows - 1);
	// Column address set (command 0x2A)
	LCDCommand(CASETP);
	LCDData(y);
	LCDData(y + nCols - 1);
	
	// WRITE MEMORY
	LCDCommand(RAMWRP);
	// loop on each row, working backwards from the bottom to the top
	pChar+=nBytes-1;  // stick pChar at the end of the row - gonna reverse print on phillips
	for (i = nRows - 1; i >= 0; i--) {
		// copy pixel row from font table and then decrement row
		PixelRow = *pChar--;
		// loop on each pixel in the row (left to right)
		// Note: we do two pixels each loop
		Mask = 0x01;  // <- opposite of epson
		for (j = 0; j < nCols; j += 2) 
		{
			// if pixel bit set, use foreground color; else use the background color
			// now get the pixel color for two successive pixels
			if ((PixelRow & Mask) == 0)
				Word0 = bColor;
			else
				Word0 = fColor;
			Mask = Mask << 1; // <- opposite of epson
			if ((PixelRow & Mask) == 0)
				Word1 = bColor;
			else
				Word1 = fColor;
			Mask = Mask << 1; // <- opposite of epson
			// use this information to output three data bytes
			LCDData((Word0 >> 4) & 0xFF);
			LCDData(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
			LCDData(Word1 & 0xFF);
		}
	}
}

void PhillipsLCDShield::setPixel(int color, unsigned char x, unsigned char y)
{
	y	=	(COL_HEIGHT - 1) - y;
	x = (ROW_LENGTH - 1) - x;
	
	LCDCommand(PASETP); // page start/end ram
	LCDData(x);
	LCDData(x);
	
	LCDCommand(CASETP); // column start/end ram
	LCDData(y);
	LCDData(y);
	
	LCDCommand(RAMWRP); // write
	
	LCDData((unsigned char)((color>>4)&0x00FF));
	LCDData((unsigned char)(((color&0x0F)<<4)|0x00));
}

void PhillipsLCDShield::off(void)
{
	LCDCommand(DISPOFF);
}

void PhillipsLCDShield::on(void)
{
	LCDCommand(DISPON);
}