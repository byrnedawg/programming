
/*
 * $Id: _display_common.c,v 1.4 2010/10/06 12:09:54 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _display_common.c,v $
 * Revision 1.4  2010/10/06 12:09:54  clivewebster
 * Set backlight brightness to 50% at startup
 *
 * Revision 1.3  2010/06/14 18:45:07  clivewebster
 * Add copyright license info
 *
 * Revision 1.2  2010/02/21 19:47:11  clivewebster
 * *** empty log message ***
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
 * _display_common.c
 *
 *  Created on: 07-Feb-2010
 *      Author: Clive Webster
 *
 *  Implement the abstract methods for all displays
 */
#include "_display_common.h"
#include "../core.h"

// Initialise a display
void _displayInit(DISPLAY* display){
	// Get the method implementation for this class of camera
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*) = (void (*)(DISPLAY*))pgm_read_word(&class->init);
	if(fn){
		fn(display);
	}
	_displayBrightness(display,50);		// 50% backlight
	_displayBacklight(display,FALSE);
	_displayClear(display);
	_displayAutoScroll(display,FALSE);
	_displayLineWrap(display,FALSE);
}

// Clear the screen
void _displayClear(DISPLAY* display){
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*) = (void (*)(DISPLAY*))pgm_read_word(&class->cls);
	if(fn){
		fn(display);
	}
}

// Move cursor to home position
void _displayHome(DISPLAY* display){
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*) = (void (*)(DISPLAY*))pgm_read_word(&class->home);
	if(fn){
		fn(display);
	}else{
		_displayGoto(display,0,0);
	}
}

// Move to a given x,y
void _displayGoto(DISPLAY* display, DISPLAY_COLUMN x, DISPLAY_LINE y){
	const DISPLAY_CLASS* class = display->class;

	// Put into valid range
	x %= display->columns;
	y %= display->rows;

	void (*fn)(DISPLAY*, DISPLAY_COLUMN, DISPLAY_LINE) =
		(void (*)(DISPLAY*, DISPLAY_COLUMN, DISPLAY_LINE))pgm_read_word(&class->gotoXY);
	if(fn){
		fn(display,x,y);
	}
}

void _displayLineWrap(DISPLAY* display,boolean on){
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*, boolean) = (void (*)(DISPLAY*,boolean))pgm_read_word(&class->lineWrap);
	if(fn){
		fn(display,on);
	}
	display->linewrap = on;
}
void _displayAutoScroll(DISPLAY* display,boolean on){
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*, boolean) = (void (*)(DISPLAY*,boolean))pgm_read_word(&class->autoScroll);
	if(fn){
		fn(display,on);
	}
	display->autoscroll = on;
}
void _displayBacklight(DISPLAY* display,boolean on){
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*, boolean) = (void (*)(DISPLAY*,boolean))pgm_read_word(&class->backlight);
	if(fn){
		fn(display,on);
	}
}
void _displayBrightness(DISPLAY* display,uint8_t percent){
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*, uint8_t) = (void (*)(DISPLAY*,uint8_t))pgm_read_word(&class->setBrightness);
	if(fn){
		fn(display,CLAMP(percent,0,100));
	}
}
void _displayContrast(DISPLAY* display,uint8_t percent){
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*, uint8_t) = (void (*)(DISPLAY*,uint8_t))pgm_read_word(&class->setContrast);
	if(fn){
		fn(display,CLAMP(percent,0,100));
	}
}
uint8_t _displaySendByte(DISPLAY* display,uint8_t byte){
	const DISPLAY_CLASS* class = display->class;
	uint8_t (*fn)(DISPLAY*, uint8_t) = (uint8_t (*)(DISPLAY*,uint8_t))pgm_read_word(&class->sendByte);
	uint8_t rtn = byte;
	if(fn){
		rtn = fn(display,byte);
	}
	return rtn;
}

// Display a horizontal graph with its left point at x,y
// val is the value to be shown - where max is the largest possible value
// count is the number of character cells across to show
void _displayHorizGraph(DISPLAY* display, DISPLAY_COLUMN x, DISPLAY_LINE y, uint16_t val, uint16_t max, uint8_t count){
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*, uint16_t,uint8_t) = (void (*)(DISPLAY*,uint16_t, uint8_t))pgm_read_word(&class->hgraph);
	if(fn){
		_displayGoto(display, x, y);				// Move to start of graph
		val = CLAMP(val,0,max);						// make sure value is in range
		val = interpolateU(val,0,max,0,5*count);	// convert to number of pixels
		fn(display,val,count);						// Draw the graph
	}
}

// Display a vertical graph with its top point at x,y
// val is the value to be shown - where max is the largest possible value
// count is the number of character cells across to show
void _displayVertGraph(DISPLAY* display, DISPLAY_COLUMN x, DISPLAY_LINE y, uint16_t val, uint16_t max, uint8_t count){
	const DISPLAY_CLASS* class = display->class;
	void (*fn)(DISPLAY*, DISPLAY_COLUMN, DISPLAY_LINE,uint16_t,uint8_t) = (void (*)(DISPLAY*,DISPLAY_COLUMN,DISPLAY_LINE,uint16_t, uint8_t))pgm_read_word(&class->vgraph);
	if(fn){
		val = CLAMP(val,0,max);						// make sure value is in range
		val = interpolateU(val,0,max,0,8*count);	// convert to number of pixels
		fn(display,x,y,val,count);						// Draw the graph
	}
}
