#include <avr/io.h>

/****************************************************************************************************/

//Macros
#define 	CMD				254 
#define		Bauderate       57
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


void Command_Display(uint8_t command);

void Change_Baudrate(uint8_t speed);

void Set_Cursor_Position(uint8_t column, uint8_t row);

void Set_Touch_Mode(unsigned char touch_mode);

void Set_Touch_Region(unsigned char RegNum, unsigned char x, unsigned char y, 
						unsigned char w, unsigned char h, unsigned char key_down, unsigned char key_up);
						
void Touch_Calibration();

void Draw_Solid_Rect(unsigned char color, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

void Draw_Rect_At(unsigned char color, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

void Continue_Line_To(unsigned char x, unsigned char y);

void Draw_Line_At(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

void Draw_Pixel_At(unsigned char x, unsigned char y);

void Choose_Color(unsigned char color);

void Choose_Font(unsigned char font_id);

void Cursor_Pos(unsigned char col, unsigned char row);

void Cursor_Coordinate(unsigned char horiz, unsigned char vert);



/****************************************************************************************************/







