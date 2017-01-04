#include "rprintf.h"

void Wait(uint8_t time);

void Write_Blank(uint8_t spaces);

void Button_Feedback(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void Delete_All_Regions();

void Set_Touch_Mode(unsigned char touch_mode);

void Set_Touch_Region(unsigned char RegNum, unsigned char x, unsigned char y, 
			unsigned char w, unsigned char h, unsigned char key_down, unsigned char key_up);

void Touch_Calibration();

void Draw_Solid_Rect(unsigned char color, unsigned char x1, unsigned char y1,
			unsigned char x2, unsigned char y2);


void Draw_Rect_At(unsigned char color, unsigned char x1, unsigned char y1, 
		unsigned char x2, unsigned char y2);


void Continue_Line_To(unsigned char x, unsigned char y);


void Draw_Line_At(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);


void Draw_Pixel_At(unsigned char x, unsigned char y);


void Choose_Color(unsigned char color);


void Choose_Font(unsigned char font_id);


void Command_Display(unsigned char command);


void Cursor_Pos(unsigned char col, unsigned char row);


void Cursor_Coordinate(unsigned char horiz, unsigned char vert);


