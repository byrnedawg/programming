/*
 * Uart2560.c
 *
 * Created: 11/19/2013 1:40:40 PM
 *  Author: gregory.byrne
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "uart.c"

#define 	CMD				254 
#define 	Backspace_Left 	8
#define 	Move_Right 		9
#define		Line_Feed 		11 
#define 	New_Page		12
#define		Cariage_Return	13 
#define 	Clear_Screen	88
#define		Go_Home			72
#define 	New_Char		78
#define		Down_Arrow		0
#define		Up_Arrow		1
#define		Display_On		66
#define		Display_OFF		70
#define		Set_Font		49
#define		Small_Filled	1
#define		Futura_16		2
#define 	Cursor			71
#define		Cursor_Cord		121
#define		Auto_Scroll_On	81
#define		Auto_Scroll_Off	82
#define		Drawing_Color	99
#define		Draw_Pixel		112
#define		Drawing_Line	108
#define		Continue_Line	101
#define		Draw_Rectangle	114
#define		DRW_Solid_Rect	120
#define		Touch_Calibrat 	139
#define		Touch_Region	132
#define		Touch_Mode		135
#define		Co_ordinate		1
#define		Region			0

unsigned char Empty[]= " ";
unsigned char Wulf[]= "Wulf V1.0";

void Command_Display(unsigned char command)
{	
	UART_putc(CMD);
	UART_putc(command);
}

void Wait(uint8_t time)
{
   uint8_t i;
   for(i=0;i<time;i++)
      _delay_loop_2(0); 
}

void Write_Blank(uint8_t spaces)
{
   uint8_t i;
   for(i=0;i<spaces;i++)
      UART_puts(Empty);  
}


void Set_Touch_Mode(unsigned char touch_mode)
{
	Command_Display(Touch_Mode);
	UART_putc(touch_mode);
}
void Set_Touch_Region(unsigned char RegNum, unsigned char x, unsigned char y, 
						unsigned char w, unsigned char h, unsigned char key_down, unsigned char key_up)
{
	Command_Display(Touch_Region);
	UART_putc(RegNum);
	UART_putc(x);
	UART_putc(y);
	UART_putc(w);
	UART_putc(h);
	UART_putc(key_down);
	UART_putc(key_up);
}

void Touch_Calibration()
{	
	Command_Display(Touch_Calibrat);

}

void Draw_Solid_Rect(unsigned char color, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(DRW_Solid_Rect);
	UART_putc(color);
	UART_putc(x1);
	UART_putc(y1);
	UART_putc(x2);
	UART_putc(y2);
}

void Draw_Rect_At(unsigned char color, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(Draw_Rectangle);
	UART_putc(color);
	UART_putc(x1);
	UART_putc(y1);
	UART_putc(x2);
	UART_putc(y2);
}

void Continue_Line_To(unsigned char x, unsigned char y)
{	
	Command_Display(Continue_Line);
	UART_putc(x);
	UART_putc(y);

}

void Draw_Line_At(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(Drawing_Line);
	UART_putc(x1);
	UART_putc(y1);
	UART_putc(x2);
	UART_putc(y2);
}

void Draw_Pixel_At(unsigned char x, unsigned char y)
{	
	Command_Display(Draw_Pixel);
	UART_putc(x);
	UART_putc(y);
}

void Choose_Color(unsigned char color)
{
	Command_Display(Drawing_Color);
	UART_putc(color);
}

void Choose_Font(unsigned char font_id)
{
	Command_Display(Set_Font);
	UART_putc(font_id);
} 

void Cursor_Pos(unsigned char col, unsigned char row)
{	
	Command_Display(Cursor);
	UART_putc(col);
	UART_putc(row);
}

void Cursor_Coordinate(unsigned char horiz, unsigned char vert)
{	
	Command_Display(Cursor_Cord);
	UART_putc(horiz);
	UART_putc(vert);
}

int main(void)
{
	PORTC=0xFF;
	UART_init();
	Command_Display(Go_Home);
	UART_puts(Wulf);
    while(1)
    {
        //TODO:: Please write your application code 
    }
}