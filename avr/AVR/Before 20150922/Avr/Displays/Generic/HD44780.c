
/*
 * $Id: HD44780.c,v 1.4 2010/07/01 23:57:19 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: HD44780.c,v $
 * Revision 1.4  2010/07/01 23:57:19  clivewebster
 * pin_make_output now specifies the initial output value
 *
 * Revision 1.3  2010/06/14 18:45:05  clivewebster
 * Add copyright license info
 *
 * Revision 1.2  2010/02/21 19:48:10  clivewebster
 * Fix issue when changing between Vertical and Horizontal graphs
 *
 * Revision 1.1  2010/02/18 01:45:09  clivewebster
 * Added
 *
 * ===========
 *
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
 * HD44780.c
 *
 *  Created on: 09-Feb-2010
 *      Author: Clive Webster
 *
 *  Implement a generic parallel display based on the HD44780 or SED1278 controller chips
 */

#include "HD44780.h"
#include "../../timer.h"
#include <stdlib.h>

#define LCD_CLR   			0b00000001		// Clear screen

#define LCD_HOME  			0b00000010		// Home

#define LCD_ENTRY 			0b00000100		// Set the Entry mode to:
#define LCD_ENTRY_INC   	0b00000010		// Set=increment, Clear=decrement
#define LCD_ENTRY_SHIFT 	0b00000001		// Set=display shift on, Clear=display shift off

#define LCD_ON 				0b00001000		// Turn LCD on using:
#define LCD_ON_DISPLAY		0b00000100		// Turn LCD display on
#define LCD_ON_CURSOR		0b00000010		// Turn cursor on
#define LCD_ON_BLINK		0b00000001		// Turn cursor blink on

#define LCD_MOVE			0b00010000		// What moves?
#define LCD_MOVE_DISP		0b00001000		//  Scroll the display
#define LCD_MOVE_RIGHT		0b00000100		//  Move the cursor right

#define LCD_FUNCTION		0b00100000		// Function set
#define LCD_FUNCTION_8BIT	0b00010000		// - Set for 8 bit mode, Clear for 4 bit mode
#define LCD_FUNCTION_2LINES	0b00001000		// - Set for 2 lines, Clear for 1 line
#define LCD_FUNCTION_10DOTS	0b00000100		// - Set for 5x10 font, Clear for 5x7 font

#define LCD_CGRAM			0b01000000		// Set CG ram address

#define LCD_DDRAM			0b10000000		// Set DD ram address

#define LCD_BUSY			0b10000000		// Is the LCD busy?

// #define LCD_CONTROLLER_KS0073 0  /**< Use 0 for HD44780 controller, 1 for KS0073 controller */

//#define LCD_FUNCTION_4BIT_1LINE  0x20   /* 4-bit interface, single line, 5x7 dots */
//#define LCD_FUNCTION_4BIT_2LINES 0x28   /* 4-bit interface, dual line,   5x7 dots */
//#define LCD_FUNCTION_8BIT_1LINE  0x30   /* 8-bit interface, single line, 5x7 dots */
//#define LCD_FUNCTION_8BIT_2LINES 0x38   /* 8-bit interface, dual line,   5x7 dots */

//#define LCD_START_LINE1  0x00     /**< DDRAM address of first char of line 1 */
//#define LCD_START_LINE2  0x40     /**< DDRAM address of first char of line 2 */
//#define LCD_START_LINE3  0x14     /**< DDRAM address of first char of line 3 */
//#define LCD_START_LINE4  0x54     /**< DDRAM address of first char of line 4 */


// The DDRAM address for the start of each line
static uint8_t PROGMEM lineStartAddr[] = {0x00, 0x40, 0x14, 0x54 };

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



#define lcd_e_delay()   __asm__ __volatile__( "rjmp 1f\n 1:" );
#define lcd_e_high(display) pin_high(display->ctrlE)
#define lcd_e_low(display)  pin_low(display->ctrlE)


static uint8_t send(DISPLAY *display,uint8_t c);

static void toggle_e(const HD44780* device){
	lcd_e_high(device);
    delay_us(10);
	lcd_e_low(device);
}

// Make all databus pins into outputs
static void databus_output(const HD44780* device){
	for(uint8_t i=0; i<8; i++){
		pin_make_output(device->data[i],TRUE);
	}
}

// Make all databus pins into inputs with pullups
static void databus_input(const HD44780* device){
	for(uint8_t i=0; i<8; i++){
		pin_make_input(device->data[i],TRUE);
	}
}

static uint8_t readNibble(const HD44780* device, uint8_t pos){
	uint8_t rtn = 0;
	uint8_t mask = 1;
	for(uint8_t i=0; i<4; i++){
		if(pin_is_high(device->data[pos++])){
			// Pin is high
			rtn |= mask;
		}
		mask <<= 1;
	}
	return rtn;
}

static void writeNibble(const HD44780* device, const uint8_t data, uint8_t pos){
	uint8_t mask = 1;
	for(uint8_t i=0; i<4; i++){

		boolean val = (data & mask) ? TRUE : FALSE;
		pin_set(device->data[pos++],val);
		mask <<= 1;
	}
}

// rs = TRUE to write data, FALSE to write to command
static void lcd_write(const HD44780* device,uint8_t data,boolean rs){
    pin_set(device->ctrlRS, rs);
    pin_low(device->ctrlRW);		// write mode

	/* configure data pins as output */
    databus_output(device);

    if(device->fourBit){
    	// High nibble first
    	writeNibble(device,data>>4,4);
    	toggle_e(device);
    	// Low nibble last
    	writeNibble(device,data,4);
    	toggle_e(device);

    	/* all data pins high (inactive) */
    	for(uint8_t pin=4; pin<8; pin++){
    		pin_high(device->data[pin]);
    	}
    }else{
    	// Write high nibble first
    	writeNibble(device,data>>4,4);
    	// Low nibble last
    	writeNibble(device,data   ,0);
    	toggle_e(device	);
    	/* all data pins high (inactive) */
    	for(uint8_t pin=0; pin<8; pin++){
    		pin_high(device->data[pin]);
    	}
    }
}


/*************************************************************************
Low-level function to read byte from LCD controller
Input:    rs     TRUE: read data
                 FALSE: read busy flag / address counter
Returns:  byte read from LCD controller
*************************************************************************/
static uint8_t lcd_read(const HD44780* device, boolean rs){
    uint8_t data=0;

    // Set rs high/low
    pin_set(device->ctrlRS, rs);	// set RS high/low
    pin_high(device->ctrlRW);		// read mode

	/* configure data pins as input */
    databus_input(device);

	lcd_e_high(device);
	lcd_e_delay();

	if(device->fourBit){
		// read 2 lots of four bits
		/* read high nibble first */
		data |= readNibble(device,4) << 4;
		lcd_e_low(device);
		lcd_e_delay();                       /* Enable 500ns low       */

		/* read low nibble */
		lcd_e_high(device);
		lcd_e_delay();
		data |= readNibble(device,4);
	}else{
		// read all 8 bits
		data |= readNibble(device,4) << 4;
		data |= readNibble(device,0);
	}

	lcd_e_low(device);

	return data;
}



static uint8_t lcd_waitbusy(const HD44780* device){
    register uint8_t c;

    /* wait until busy flag is cleared */
   while ( (c=lcd_read(device,FALSE)) & (LCD_BUSY)) {    }

    /* the address counter is updated 4us after the busy flag is cleared */
    delay_us(4);

    /* now read the address counter */
    return (lcd_read(device,FALSE));  // return address counter

}/* lcd_waitbusy */


// Cinvert an addr into an X,Y position
static void addrToXY(const HD44780* device, uint8_t addr, uint8_t* x, uint8_t* y){
	uint8_t best=255;
	*x = *y = 0;
	for(uint8_t line = 0; line<device->_display_.rows; line++){
		uint8_t start = pgm_read_byte(&lineStartAddr[line]);
		if(addr >= start){
			uint8_t thisDiff = addr - start;
			if(line==0 || thisDiff<best){
				// best so far
				best = thisDiff;
				*x = best;
				*y = line;
			}
		}
	}
}

// Send a command to the LCD
static void lcd_command(const HD44780* device, uint8_t cmd){
    lcd_waitbusy(device);
    lcd_write(device,cmd,FALSE);
}


// Send a data byte to the LCD
static void lcd_data(const HD44780*device,uint8_t data){
    lcd_waitbusy(device);
    lcd_write(device, data,TRUE);
}

// x,y are zero relative and are within range
static void gotoXY(DISPLAY* display,uint8_t x, uint8_t y){
	HD44780* device = (HD44780*) display;
	uint8_t addr = pgm_read_byte(&lineStartAddr[y]);
	addr += (LCD_DDRAM) + x;
	lcd_command(device,addr);
}

// Clear screen and home cursor
static void cls(DISPLAY* display){
	HD44780* device = (HD44780*) display;
	lcd_command(device,LCD_CLR);
	if(device->buffer){
		uint8_t* pos = device->buffer;
		uint8_t count = display->columns * display->rows;
		while(count--){
			*pos++ = ' ';
		}
	}
}

// Move cursor to home position
static void home(DISPLAY*display){
	HD44780* device = (HD44780*) display;
    lcd_command(device,LCD_HOME);
}

static void scrollUp(HD44780* device){
	boolean old1 = device->_display_.autoscroll;
	boolean old2 = device->_display_.linewrap;

	device->_display_.autoscroll = FALSE;
	device->_display_.linewrap = TRUE;
	home(&device->_display_);

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
static void lcd_newline(HD44780*device,uint8_t pos){
	uint8_t x,y;

	// Get the current x,y
	addrToXY(device,pos,&x,&y);

	// Move to the next line
	y++;

	if(y == device->_display_.rows){
		// Gone past end of screen
		if(device->_display_.autoscroll){
			// Scroll the existing text up
			scrollUp(device);
			y--;
		}else{
			// Just wrap around
			y %= device->_display_.rows;
		}
	}
	gotoXY(&device->_display_,x,y);

}/* lcd_newline */

// Move cursor to start of line
static void lcd_return(HD44780*device,uint8_t pos){
	uint8_t x,y;

	// Get the current x,y
	addrToXY(device,pos,&x,&y);

	// Move to start of line
	gotoXY(&device->_display_,0,y);

}/* lcd_return */



/*************************************************************************
Display character at current cursor position
Input:    character to be displayed
Returns:  none
*************************************************************************/
static uint8_t send(DISPLAY *display,uint8_t c){

    uint8_t pos;
    HD44780* device = (HD44780*)display;

    pos = lcd_waitbusy(device);   // read busy-flag and address counter
    if (c=='\n'){
        lcd_newline(device,pos);
    } else if( c=='\r'){
    	lcd_return(device,pos);
    } else  {
    	uint8_t x,y;
    	// Get the current position
    	addrToXY(device,pos,&x,&y);

    	// If at end of line then move to next line down
    	if(x == device->_display_.columns){
    		if(device->_display_.linewrap){
    			send(display, '\r');
    			send(display, '\n');
    	        send(display, c);
    		}else{
    			// throw away the character
    		}
    	}else{
    		// In middle of line so just write it
	        lcd_data(device, c);
	        if(device->buffer){
	        	device->buffer[y * device->_display_.columns + x] = c;
	        }
    	}
    }
    return c;
}

static void customChar(HD44780* device, uint8_t* def, char charNum){
	uint8_t pos = lcd_waitbusy(device);
	uint8_t x,y;
	addrToXY(device,pos,&x,&y);

	uint8_t addr = charNum * 8;

	for(uint8_t i=0; i<8; i++,addr++){
		lcd_command(device, LCD_CGRAM | addr);  		/* set CG RAM start address 0 */
		lcd_data(device, pgm_read_byte(&def[i]));
	}

	gotoXY(&device->_display_,x,y);
}

static void setHGraph(HD44780* device){
	// Define the horiz graph characters
	uint8_t c;
	for(c = 0; c<5; c++){
		customChar(device, &HorizCustomChar[c<<3], c);
	}
	device->_display_.hgraph = TRUE;
}

static void setVGraph(HD44780* device){
    // Set up vertical graph chars
	uint8_t c;
    for(c=0; c<8; c++){
    	customChar(device, &VertCustomChar[c], c);
    }
	device->_display_.hgraph = FALSE;
}

// Intialise the display
static void init(DISPLAY* display){
	HD44780* device = (HD44780*) display;

	if(device->buffer==null){
		device->buffer = malloc(device->_display_.columns * device->_display_.rows);
	}

	// Set all control pins as outputs
	pin_make_output(device->ctrlRS,FALSE);
	pin_make_output(device->ctrlRW,FALSE);
	pin_make_output(device->ctrlE,FALSE);

	// Make all data pins as outputs
	databus_output(device);

	// Wait for 16 ms
	delay_ms(16);

	// Use 8 bit mode. Make D4 and D5 high
	pin_high(device->data[4]);
	pin_high(device->data[5]);
	toggle_e(device);
	delay_ms(5);

	// repeat last command a few times
	for(uint8_t i=0; i<2; i++){
		toggle_e(device);
		delay_us(64);
	}

	// Configure for 4 bit mode if required
	if(device->fourBit){
		pin_low(device->data[4]);
		toggle_e(device);
		delay_us(64);
	}

	// Configure the number of lines
	uint8_t cmd = LCD_FUNCTION;						// Assume 1 line display
	if(device->_display_.rows >= 2){
		cmd |= LCD_FUNCTION_2LINES;					// Set as 2 line display
	}
    lcd_command(device,cmd);

    lcd_command(device,LCD_ON);      	        	/* display off                  */
//    cls(&device->_display_);                      	/* display clear                */
    lcd_command(device, LCD_ENTRY | LCD_ENTRY_INC); /* set entry mode = increment cursor */
    lcd_command(device, LCD_ON + LCD_ON_DISPLAY);   /* display on                   */

    // Set up horizontal graph chars
    setHGraph(device);
}

static void hgraph(DISPLAY* display,DISPLAY_COLUMN x,DISPLAY_COLUMN y,uint16_t pixels,uint8_t width){
	HD44780* device = (HD44780*) display;

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

}

static void vgraph(DISPLAY* display,DISPLAY_COLUMN x,DISPLAY_COLUMN y, uint16_t pixels,uint8_t height){
	HD44780* device = (HD44780*) display;

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

}


DISPLAY_CLASS c_HD44780 = MAKE_DISPLAY_CLASS(&init,&cls,&home,&gotoXY, null, null, null, null, null,&send,&hgraph,&vgraph);
