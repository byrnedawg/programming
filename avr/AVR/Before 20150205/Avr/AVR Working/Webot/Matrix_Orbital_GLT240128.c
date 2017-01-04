#include "Matrix_Orbital_GLT240128.h"

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
#define		Delete_Region	134
#define		Az_Start_X		90
#define		Az_Start_Y		10
#define		Az_Finish_X		132
#define		button_delay	4

/*
volatile uint8_t count;
unsigned char message1[]; 
unsigned char message2[]; 
unsigned char message3[]; 
unsigned char Wulf[]= "Wulf V1.0";
unsigned char Status[]= "Greg Byrne --->12345678901234567890 ";
unsigned char Value[]= "DCV = ";
unsigned char Packet[]= "Packet = ";
unsigned char Button_Down[]= "Button Down = ";
unsigned char Button_Up[]= "Button Up = ";
unsigned char Screen_Touched[]= "Screen Touched = ";
unsigned char Empty[]= " ";
unsigned char EL[]= "EL:";
uint16_t adc_result;
uint16_t whole;
uint16_t temp = 0;
uint8_t i; //Clasical loop varriable
uint8_t packet[4];
uint8_t p_data;
uint8_t data;
uint8_t	draw_x = 0;
uint8_t	draw_y = 0;
uint8_t	color = 50;
*/
//*********************************************************



/*
void InitDisplay()
{
	Command_Display(Display_On);
	Choose_Font(Small_Filled);
	Choose_Color(color);
	Set_Touch_Mode(Co_ordinate); // Region or Co_ordinate
	//Set_Touch_Region(1, 90, 90, 35, 35, 170, 195);
	Command_Display(Clear_Screen);
}
 */


//***********************************************************************

//********************************************************************************
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
      rprintfChar(" ");  
}

void Button_Feedback(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	Draw_Rect_At(255, x1+1, y1+1, x2-1, y2-1);
	Draw_Rect_At(255, x1+2, y1+2, x2-2, y2-2);
	Wait(button_delay);
	Draw_Rect_At(0, x1+1, y1+1, x2-1, y2-1);
	Draw_Rect_At(0, x1+2, y1+2, x2-2, y2-2);
}

void Delete_All_Regions()
{
	Command_Display(Delete_Region);
}

void Set_Touch_Mode(unsigned char touch_mode)
{
	Command_Display(Touch_Mode);
	rprintfChar(touch_mode);
}
void Set_Touch_Region(unsigned char RegNum, unsigned char x, unsigned char y, 
						unsigned char w, unsigned char h, unsigned char key_down, unsigned char key_up)
{
	Command_Display(Touch_Region);
	rprintfChar(RegNum);
	rprintfChar(x);
	rprintfChar(y);
	rprintfChar(w);
	rprintfChar(h);
	rprintfChar(key_down);
	rprintfChar(key_up);
}

void Touch_Calibration()
{	
	Command_Display(Touch_Calibrat);

}

void Draw_Solid_Rect(unsigned char color, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(DRW_Solid_Rect);
	rprintfChar(color);
	rprintfChar(x1);
	rprintfChar(y1);
	rprintfChar(x2);
	rprintfChar(y2);
}

void Draw_Rect_At(unsigned char color, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(Draw_Rectangle);
	rprintfChar(color);
	rprintfChar(x1);
	rprintfChar(y1);
	rprintfChar(x2);
	rprintfChar(y2);
}

void Continue_Line_To(unsigned char x, unsigned char y)
{	
	Command_Display(Continue_Line);
	rprintfChar(x);
	rprintfChar(y);

}

void Draw_Line_At(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(Drawing_Line);
	rprintfChar(x1);
	rprintfChar(y1);
	rprintfChar(x2);
	rprintfChar(y2);
}

void Draw_Pixel_At(unsigned char x, unsigned char y)
{	
	Command_Display(Draw_Pixel);
	rprintfChar(x);
	rprintfChar(y);
}

void Choose_Color(unsigned char color)
{
	Command_Display(Drawing_Color);
	rprintfChar(color);
}

void Choose_Font(unsigned char font_id)
{
	Command_Display(Set_Font);
	rprintfChar(font_id);
} 

void Command_Display(unsigned char command)
{	
	rprintfChar(CMD);
	rprintfChar(command);
}

void Cursor_Pos(unsigned char col, unsigned char row)
{	
	Command_Display(Cursor);
	rprintfChar(col);
	rprintfChar(row);
}

void Cursor_Coordinate(unsigned char horiz, unsigned char vert)
{	
	Command_Display(Cursor_Cord);
	rprintfChar(horiz);
	rprintfChar(vert);
}




