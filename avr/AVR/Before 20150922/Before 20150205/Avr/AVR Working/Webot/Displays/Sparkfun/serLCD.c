
/*
 * $Id: serLCD.c,v 1.3 2010/10/06 12:10:31 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: serLCD.c,v $
 * Revision 1.3  2010/10/06 12:10:31  clivewebster
 * Implement displaySetBrightness
 *
 * Revision 1.2  2010/06/14 18:45:06  clivewebster
 * Add copyright license info
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
 * serLCD.c
 *
 *  Created on: 10-Feb-2010
 *      Author: Clive Webster
 */
#include "serLCD.h"
#include <stdlib.h>
#include "../../core.h"

// Write a raw value to the screen
static void write(const DISPLAY* display, uint8_t c){
	SPARKFUN_SERLCD* device = (SPARKFUN_SERLCD*)display;
	device->_uart_->writer(c);
}


// 0x7c, 0x01 turn on  backlight
// 0x7c, 0x02 turn off backlight
static void backlight(DISPLAY* display,boolean on){
	write(display,0x7c);
	if(on){
		write(display, 1);
	}else{
		write(display, 2);
	}
}

// Set the backlight brightness
//  0x7c, n where n = 128 to 157
static void brightness(DISPLAY* display, uint8_t percent){
	uint8_t val = interpolateU(percent,0,100, 128,157);
	write(display,0x7c);
	write(display,val);
}


// Initialise the display
static void init(DISPLAY* display){
	SPARKFUN_SERLCD* device = (SPARKFUN_SERLCD*)display;
	// Initialise the uart
	_uartInit(device->_uart_ ,device->baudrate);

	if(device->buffer==null){
		device->buffer = malloc(device->_display_.columns * device->_display_.rows);
	}

}


// Clear the screen
static void	cls(DISPLAY* display){
	write(display, 0xfe);
	write(display, 1);

	SPARKFUN_SERLCD* device = (SPARKFUN_SERLCD*)display;
	if(device->buffer){
		uint8_t* pos = device->buffer;
		uint8_t count = display->columns * display->rows;
		while(count--){
			*pos++ = ' ';
		}
	}
	device->cursorX = device->cursorY = 0;
}

// Goto x,y cursor location
static void	gotoXY(DISPLAY* display,DISPLAY_COLUMN x, DISPLAY_LINE y){
	SPARKFUN_SERLCD* morb = (SPARKFUN_SERLCD*)display;
	uint8_t offset=0;
	if(y==1){
		offset = 64;
	}else if(y==2){
		offset = 0x14;
	}else if(y==3){
		offset = 0x54;
	}
	offset += x;

	write(display, 0xfe);
	write(display, offset + 128);
	morb->cursorX = x;
	morb->cursorY = y;
}
static uint8_t send(DISPLAY* display, uint8_t byte);

static void scrollUp(SPARKFUN_SERLCD* device){
	boolean old1 = device->_display_.autoscroll;
	boolean old2 = device->_display_.linewrap;

	device->_display_.autoscroll = FALSE;
	device->_display_.linewrap = TRUE;
	gotoXY(&device->_display_,0,0);

	// move lines up
	uint8_t count = (device->_display_.rows-1) * device->_display_.columns;
	uint8_t *pos = device->buffer + device->_display_.columns;
	while(count--){
		send(&device->_display_, *pos++);
	}
	// Fill last line with spaces
	count = device->_display_.columns;
	while(count--){
		send(&device->_display_, ' ');
	}

	// Restore original settings
	device->_display_.autoscroll = old1;
	device->_display_.linewrap = old2;
}

// Move cursor down a line
static void lcd_newline(SPARKFUN_SERLCD*device){
	// Move to the next line
	device->cursorY ++;

	if(device->cursorY == device->_display_.rows){
		// Gone past end of screen
		if(device->_display_.autoscroll){
			// Scroll the existing text up
			scrollUp(device);
			device->cursorY--;
		}else{
			// Just wrap around
			device->cursorY %= device->_display_.rows;
		}
	}
	gotoXY(&device->_display_,device->cursorX,device->cursorY);

}/* lcd_newline */

// Move cursor to start of line
static void lcd_return(SPARKFUN_SERLCD*device){
	device->cursorX = 0;
	// Move to start of line
	gotoXY(&device->_display_,device->cursorX,device->cursorY);

}/* lcd_return */


// Write a character to the screen
static uint8_t send(DISPLAY* display, uint8_t byte){
	SPARKFUN_SERLCD* device = (SPARKFUN_SERLCD*)display;
    if (byte=='\n'){
        lcd_newline(device);
    } else if( byte=='\r'){
    	lcd_return(device);
    } else  {
    	// If at end of line then move to next line down
    	if(device->cursorX == device->_display_.columns){
    		if(device->_display_.linewrap){
    			send(display, '\r');
    			send(display, '\n');
    	        send(display, byte);
    		}else{
    			// throw away the character
    		}
    	}else{
    		// In middle of line so just write it
    		device->_uart_->writer(byte);
	        if(device->buffer){
	        	device->buffer[device->cursorY * device->_display_.columns + device->cursorX] = byte;
	        }
	        device->cursorX++;
    	}
    }
	return byte;
}

DISPLAY_CLASS c_SERLCD = MAKE_DISPLAY_CLASS(&init,&cls,null,&gotoXY, null, null, &backlight, &brightness, null,&send,null,null);

