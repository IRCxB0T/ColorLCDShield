/**
 *  PhilipsLCDShield.h - Arduino Library to control a Nokia 6100 LCD,
 *  specifically that found on SparkFun's Color LCD Shield.
 *  This code should work for Philips display drivers
 *  normally found on the Color LCD Shield.
 *
 *  License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 *  to use and abuse this code however you'd like. If you find it useful
 *  please attribute, and SHARE-ALIKE!
 *
 *  This is based on code by Jim Lindblom.
 **/

#ifndef PhilipsLCDShield_H
#define PhilipsLCDShield_H

#include "CommonLCDShield.h"

//********************************************************************
//
//			PHILIPS Controller Definitions
//
//********************************************************************
//LCD Commands
#define	NOPP		0x00	// No operation
#define	BSTRON		0x03	// Booster voltage on
#define SLEEPIN     0x10	// Sleep in
#define	SLEEPOUT	0x11	// Sleep out
#define	NORON		0x13	// Normal display mode on
#define	INVOFF		0x20	// Display inversion off
#define INVON      	0x21	// Display inversion on
#define	SETCON		0x25	// Set contrast
#define DISPOFF     0x28	// Display off
#define DISPON      0x29	// Display on
#define CASETP      0x2A	// Column address set
#define PASETP      0x2B	// Page address set
#define RAMWRP      0x2C	// Memory write
#define RGBSET	    0x2D	// Color set
#define	MADCTL		0x36	// Memory data access control
#define	COLMOD		0x3A	// Interface pixel format
#define DISCTR      0xB9	// Super frame inversion
#define	EC			0xC0	// Internal or external oscillator

//*******************************************************
//				12-Bit Color Definitions
//*******************************************************
#define BLACK		0x000
#define NAVY 		0x800
#define BLUE		0xF00
#define TEAL 		0x880
#define EMERALD		0x5C0
#define	GREEN		0x0F0
#define CYAN		0xFF0
#define SLATE 		0x442
#define INDIGO  	0x804
#define TURQUOISE	0xDE4
#define OLIVE 		0x286
#define MAROON 		0x008
#define PURPLE 		0x808
#define GRAY 		0x888
#define SKYBLUE		0xEC8
#define BROWN		0x22B
#define CRIMSON 	0x31D
#define ORCHID 		0xD7D
#define RED			0x00F
#define MAGENTA		0xF0F
#define ORANGE 		0x04F
#define PINK		0xA6F
#define CORAL 		0x57F
#define SALMON 		0x78F
#define ORANGE		0x0AF
#define GOLD 		0x0DF
#define YELLOW		0x0FF
#define WHITE		0xFFF

class PhilipsLCDShield : public LCDShield {
	public:
		virtual void init();
		virtual void clear(int color);
		virtual void setChar(char c, int x, int y, int fColor, int bColor);
		virtual void setPixel(int color, unsigned char x, unsigned char y);
		virtual void on(void);
		virtual void off(void);
		virtual void contrast(char setting);
};

#endif	// PhilipsLCDShield_H