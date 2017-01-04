
/*
 * $Id: MatrixOrbital.c,v 1.3 2010/06/14 18:45:04 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: MatrixOrbital.c,v $
 * Revision 1.3  2010/06/14 18:45:04  clivewebster
 * Add copyright license info
 *
 * Revision 1.2  2010/02/21 19:48:57  clivewebster
 * Make graphing compatible with other LCDs
 *
 * Revision 1.1  2010/02/18 01:45:09  clivewebster
 * Added
 *
 * ===========
 *
 * Copyright (C) 2010 Clive Webster (webbot@webbot.org.uk)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 * MatrixOrbital.c
 *
 *  Created on: 10-Feb-2010
 *      Author: Clive Webster

 *
 */
#include "MatrixOrbital.h"

// Custom chars are 5 pixels across and 8 pixels down
static uint8_t PROGMEM HorizCustomChar[] = {
		// Horizontal bar graph
		0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, // 0. 1/5 full progress block
		0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, // 1. 2/5 full progress block
		0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, // 2. 3/5 full progress block
		0x00, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x00, // 3. 4/5 full progress block
		0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 4. 5/5 full progress block
};

// Vertical bar graph
static uint8_t PROGMEM VertCustomChar[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E
};

// Write a raw character to the screen
static void write(const DISPLAY* display, uint8_t c){
	MATRIX_ORBITAL* morb = (MATRIX_ORBITAL*)display;
	morb->_uart_->writer(c);
}


//
// y = 255 * percent / 100
// set brightness n (0..255) = 254,153,n
static void setBrightness(DISPLAY* display, uint8_t percent){
	uint16_t temp = 255 * percent;
	temp /= 100;
	write(display, 254);
	write(display, 153);
	write(display, (uint8_t)temp);
}
//
// y = 255 * percent / 100
// set contrast n (0..255) = 254,80,n
static void setContrast(DISPLAY* display, uint8_t percent){
	uint16_t temp = 255 * percent;
	temp /= 100;
	write(display, 254);
	write(display,  80);
	write(display, (uint8_t)temp);
}

// line wrap on = 254,67
// line wrap off = 254,68
static void lineWrap(DISPLAY* display,boolean on){
	write(display,254);
	write(display, (on) ? 67 : 68);
}

// auto scroll on = 254,81
// auto scroll off = 254,82
static void autoScroll(DISPLAY* display,boolean on){
	write(display,254);
	write(display, (on) ? 81 : 82);
}

//backlight on  = 254,66,0
//backlight off = 254,70
static void backlight(DISPLAY* display,boolean on){
	write(display,254);
	if(on){
		write(display, 66);
		write(display, 0);	// keep 'on' for unlimited time
	}else{
		write(display, 70);
	}
}

static void customChar(DISPLAY* display, uint8_t* def, char charNum){
	write(display, 254);write(display,78);				// set custom char
	write(display, charNum);
	for(uint8_t i=0; i<8; i++){
		write(display, pgm_read_byte(&def[i]));
	}
}



static void setHGraph(MATRIX_ORBITAL* device){
	// Define the horiz graph characters
//	write(&device->_display_,254);
//	write(&device->_display_,'h');

	uint8_t c;
	for(c = 0; c<5; c++){
		customChar(&device->_display_, &HorizCustomChar[c<<3], c);
	}

	device->_display_.hgraph = TRUE;
}

static void setVGraph(MATRIX_ORBITAL* device){

    // Initialise vertical graph chars
//	write(&device->_display_,254);
//	write(&device->_display_,'s');

    // Set up vertical graph chars
	uint8_t c;
    for(c=0; c<8; c++){
    	customChar(&device->_display_, &VertCustomChar[c], c);
    }


	device->_display_.hgraph = FALSE;
}

// Initialise the display
static void init(DISPLAY* display){
	MATRIX_ORBITAL* device = (MATRIX_ORBITAL*)display;
	_uartInit(device->_uart_ ,device->baudrate);


	write(display, 254);write(display,75);					// Turn off underline cursor
	write(display, 254);write(display,84);					// Turn off blinking block cursor


    // Set up horizontal graph chars
    setHGraph(device);

}

// Clear the screen
static void	cls(DISPLAY* display){
	write(display, 254);
	write(display, 88);
}

// Goto x,y cursor location
static void	gotoXY(DISPLAY* display,DISPLAY_COLUMN x, DISPLAY_LINE y){
	write(display, 254);
	write(display, 71);
	write(display, x+1);
	write(display, y+1);
}

static uint8_t send(DISPLAY* display, uint8_t byte){
	MATRIX_ORBITAL* morb = (MATRIX_ORBITAL*)display;
	return 	morb->_uart_->writer(byte);
}

static void hgraph(DISPLAY* display,DISPLAY_COLUMN x,DISPLAY_COLUMN y,uint16_t pixels,uint8_t width){
	MATRIX_ORBITAL* device = (MATRIX_ORBITAL*) display;

	// Make sure horizontal graph mode is enabled
	if(!display->hgraph){
		setHGraph(device);
	}

	while(width--){
		uint8_t c;
		c = (pixels>=5) ? c=5 : pixels;
		pixels -= c;
		c = (c==0) ? ' ' : c-1;
		send(display,c);
	}


	/*
	write(display,254);write(display,124); // Draw horizontal graph
	write(display,x+1);		// 1s rel column
	write(display,y+1);		// 1s rel line
	write(display,0);		// From left to right
	write(display,pixels);	// The number of pixels
	*/
}

static void vgraph(DISPLAY* display,DISPLAY_COLUMN x,DISPLAY_COLUMN y, uint16_t pixels,uint8_t height){
	MATRIX_ORBITAL* device = (MATRIX_ORBITAL*) display;

	// Make sure vertical graph mode is enabled
	if(display->hgraph){
		setVGraph(device);
	}

	while(height--){
		_displayGoto(display,x,y+height);
		uint8_t c;
		c = (pixels>=8) ? c=8 : pixels;
		pixels -= c;
		c = (c==0) ? ' ' : c-1;
		send(display,c);
	}
/*
 * Inbuilt cmd uses both lines of the display
	write(display,254);write(display,61);	// Draw vertical graph
	write(display,x+1);						// 1s rel column
	write(display,pixels);					// The number of pixels
*/
}



DISPLAY_CLASS c_MatrixOrbital = MAKE_DISPLAY_CLASS(&init,&cls,null,&gotoXY, &lineWrap, &autoScroll, &backlight, &setBrightness, &setContrast,&send, &hgraph, &vgraph);
