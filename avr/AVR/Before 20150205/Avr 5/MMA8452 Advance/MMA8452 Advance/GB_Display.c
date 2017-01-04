#include "GB_Display.h"
#include "GB_usart.h"


void Command_Display(uint8_t command)
{
	UWriteData(CMD);
	UWriteData(command);
}

void Change_Baudrate(uint8_t speed)
{	
	UWriteData(CMD);
	UWriteData(Bauderate);
	UWriteData(speed);
}

void Set_Cursor_Position(uint8_t column, uint8_t row)
{	
	UWriteData(CMD);
	UWriteData(Cursor);
	UWriteData(column);
	UWriteData(row);
}

void Set_Touch_Mode(unsigned char touch_mode)
{
	Command_Display(Touch_Mode);
	UWriteData(touch_mode);
}
void Set_Touch_Region(unsigned char RegNum, unsigned char x, unsigned char y, 
						unsigned char w, unsigned char h, unsigned char key_down, unsigned char key_up)
{
	Command_Display(Touch_Region);
	UWriteData(RegNum);
	UWriteData(x);
	UWriteData(y);
	UWriteData(w);
	UWriteData(h);
	UWriteData(key_down);
	UWriteData(key_up);
}

void Touch_Calibration()
{	
	Command_Display(Touch_Calibrat);

}

void Draw_Solid_Rect(unsigned char color, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(DRW_Solid_Rect);
	UWriteData(color);
	UWriteData(x1);
	UWriteData(y1);
	UWriteData(x2);
	UWriteData(y2);
}

void Draw_Rect_At(unsigned char color, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(Draw_Rectangle);
	UWriteData(color);
	UWriteData(x1);
	UWriteData(y1);
	UWriteData(x2);
	UWriteData(y2);
}

void Continue_Line_To(unsigned char x, unsigned char y)
{	
	Command_Display(Continue_Line);
	UWriteData(x);
	UWriteData(y);

}

void Draw_Line_At(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(Drawing_Line);
	UWriteData(x1);
	UWriteData(y1);
	UWriteData(x2);
	UWriteData(y2);
}

void Draw_Pixel_At(unsigned char x, unsigned char y)
{	
	Command_Display(Draw_Pixel);
	UWriteData(x);
	UWriteData(y);
}

void Choose_Color(unsigned char color)
{
	Command_Display(Drawing_Color);
	UWriteData(color);
}

void Choose_Font(unsigned char font_id)
{
	Command_Display(Set_Font);
	UWriteData(font_id);
} 

void Cursor_Pos(unsigned char col, unsigned char row)
{	
	Command_Display(Cursor);
	UWriteData(col);
	UWriteData(row);
}

void Cursor_Coordinate(unsigned char horiz, unsigned char vert)
{	
	Command_Display(Cursor_Cord);
	UWriteData(horiz);
	UWriteData(vert);
}