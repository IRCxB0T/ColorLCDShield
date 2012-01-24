/**
 *  CommonLCDShield.cpp - Arduino Library to control a Nokia 6100 LCD,
 *  specifically that found on SparkFun's Color LCD Shield.
 *  This code should work for Phillips display drivers
 *  normally found on the Color LCD Shield.
 *
 *  This class handles all the funtionality shared between the two
 *  display drivers found on the Color LCD Shield.
 *
 *  License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 *  to use and abuse this code however you'd like. If you find it useful
 *  please attribute, and SHARE-ALIKE!
 *
 *  This is based on code by Jim Lindblom.
 **/

#include "CommonLCDShield.h"

extern "C" {
	#if defined(ARDUINO) && ARDUINO >= 100
		#include "wiring_private.h"
	#else
		#include "wiring.h"
	#endif
}

LCDShield::LCDShield()
{
	
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
	DDRB = ((1<<DIO)|(1<<SCK));     //Set DIO and SCK pins on PORTB as outputs
	DDRH = ((1<<CS)|(1<<LCD_RES));  //Set CS and RES pins PORTH as outputs
#else
	DDRB = ((1<<CS)|(1<<DIO)|(1<<SCK)|(1<<LCD_RES));  //Set the control pins as outputs
#endif
	
	DDRD	=	0x00;
	PORTD	=	0xFF;
}

void LCDShield::LCDCommand(unsigned char data)
{
	char jj;
	
	cbi(LCD_PORT_CS, CS);     // enable chip
	cbi(LCD_PORT_DIO, DIO);   // output low on data out (9th bit low = command)
	
	cbi(LCD_PORT_SCK, SCK);   // send clock pulse
	delayMicroseconds(1);
	sbi(LCD_PORT_SCK, SCK);
	
	for (jj = 0; jj < 8; jj++)
	{
		if ((data & 0x80) == 0x80)
			sbi(LCD_PORT_DIO, DIO);
		else
			cbi(LCD_PORT_DIO, DIO);
		
		cbi(LCD_PORT_SCK, SCK); // send clock pulse
		delayMicroseconds(1);
		sbi(LCD_PORT_SCK, SCK);
		
		data <<= 1;
	}
	
	sbi(LCD_PORT_CS, CS);     // disable
}

void LCDShield::LCDData(unsigned char data)
{
	char j;
	
	cbi(LCD_PORT_CS, CS);     // enable chip
	sbi(LCD_PORT_DIO, DIO);   // output high on data out (9th bit high = data)
	
	cbi(LCD_PORT_SCK, SCK);   // send clock pulse
	delayMicroseconds(1);
	sbi(LCD_PORT_SCK, SCK);   // send clock pulse
	
	for (j = 0; j < 8; j++)
	{
		if ((data & 0x80) == 0x80)
			sbi(LCD_PORT_DIO, DIO);
		else
			cbi(LCD_PORT_DIO, DIO);
		
		cbi(LCD_PORT_SCK, SCK); // send clock pulse
		delayMicroseconds(1);
		sbi(LCD_PORT_SCK, SCK);
		
		data <<= 1;
	}
	
	LCD_PORT_CS	|=	(1<<CS);  // disable
}

void LCDShield::drawCircle (uint8_t x0, uint8_t y0, int radius, int color)
{
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;
	
	setPixel(color, x0, y0 + radius);
	setPixel(color, x0, y0 - radius);
	setPixel(color, x0 + radius, y0);
	setPixel(color, x0 - radius, y0);
	
	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		
		setPixel(color, x0 + x, y0 + y);
		setPixel(color, x0 - x, y0 + y);
		setPixel(color, x0 + x, y0 - y);
		setPixel(color, x0 - x, y0 - y);
		setPixel(color, x0 + y, y0 + x);
		setPixel(color, x0 - y, y0 + x);
		setPixel(color, x0 + y, y0 - x);
		setPixel(color, x0 - y, y0 - x);
	}
}

void LCDShield::setCircle (uint8_t x0, uint8_t y0, int radius, int color, uint8_t fill)
{
	if (fill == 1) {
		uint8_t i = 0;
		for (i; i<=radius; i++) {
			drawCircle(x0, y0, i, color);
		}
	}
	else {
		drawCircle(x0, y0, radius, color);
	}

}

void LCDShield::setStr(char *pString, uint8_t x, uint8_t y, int fColor, int bColor)
{
	x = x + 16;
	y = y + 8;
	// loop until null-terminator is seen
	while (*pString != 0x00) {
		// draw the character
		setChar(*pString++, x, y, fColor, bColor);
		// advance the y position
		y = y + 8;
		// bail out if y exceeds 131
		if (y > 131) break;
	}
}

void LCDShield::setLine(int x0, int y0, int x1, int y1, int color)
{
	int dy = y1 - y0; // Difference between y0 and y1
	int dx = x1 - x0; // Difference between x0 and x1
	int stepx, stepy;
	
	if (dy < 0)
	{
		dy = -dy;
		stepy = -1;
	}
	else
		stepy = 1;
	
	if (dx < 0)
	{
		dx = -dx;
		stepx = -1;
	}
	else
		stepx = 1;
	
	dy <<= 1; // dy is now 2*dy
	dx <<= 1; // dx is now 2*dx
	setPixel(color, x0, y0);
	
	if (dx > dy) 
	{
		int fraction = dy - (dx >> 1);
		while (x0 != x1)
		{
			if (fraction >= 0)
			{
				y0 += stepy;
				fraction -= dx;
			}
			x0 += stepx;
			fraction += dy;
			setPixel(color, x0, y0);
		}
	}
	else
	{
		int fraction = dx - (dy >> 1);
		while (y0 != y1)
		{
			if (fraction >= 0)
			{
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;
			setPixel(color, x0, y0);
		}
	}
}

void LCDShield::setRect(int x0, int y0, int x1, int y1, unsigned char fill, int color)
{
	// check if the rectangle is to be filled
	if (fill == 1)
	{
		int xDiff;
		
		if(x0 > x1)
			xDiff = x0 - x1; //Find the difference between the x vars
		else
			xDiff = x1 - x0;
		
		while(xDiff > 0)
		{
			setLine(x0, y0, x0, y1, color);
			
			if(x0 > x1)
				x0--;
			else
				x0++;
			
			xDiff--;
		}
		
	}
	else 
	{
		// best way to draw an unfilled rectangle is to draw four lines
		setLine(x0, y0, x1, y0, color);
		setLine(x0, y1, x1, y1, color);
		setLine(x0, y0, x0, y1, color);
		setLine(x1, y0, x1, y1, color);
	}
}