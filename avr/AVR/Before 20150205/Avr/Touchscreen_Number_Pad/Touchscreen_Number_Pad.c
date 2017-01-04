#include <avr/io.h>
#include <util/delay.h>
#include<avr\interrupt.h>
#include <avr/wdt.h> 
#include "usart.h"

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
uint8_t current_cursor_x;
uint8_t current_cursor_y;

//*********************************************************

void InitTimer()
{
   // Prescaler = FCPU/256/256 = 61.0352 hz 
   TCCR0B|=(1<<CS02)|(0<<CS00)|(0<<CS00);

   //Enable Overflow Interrupt Enable
   TIMSK0|=(1<<TOIE0);
   TIMSK1|=(1<<TOIE1);

   //Initialize Counter
   TCNT0=0;


   TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
   TCCR1B|=(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10); //PRESCALER=64 MODE 14(FAST PWM)

   ICR1=9999;  //fPWM=50Hz (Period = 20ms Standard).
   //OCR1A=316;

   DDRD|=(1<<PD4)|(1<<PD5);   //PWM Pins as Out

   //Initialize our varriable
   count=0;
   //sei();
}

void InitDisplay()
{
	Command_Display(Display_On);
	Choose_Font(Small_Filled);
	Choose_Color(color);
	Command_Display(Clear_Screen);
}

void Draw_Pad()
{
	Draw_Rect_At(255, 65, 5, 174, 30);

	Cursor_Coordinate(70,10);
	UWriteData('A');
	UWriteData('Z');
	UWriteData(':');
	Write_Blank(6);
	UWriteData('R');
	UWriteData('D');
	UWriteData(':');

	Cursor_Coordinate(70,20);
	UWriteData('E');
	UWriteData('L');
	UWriteData(':');
	Write_Blank(5);
	UWriteData('C');
	UWriteData('H');
	UWriteData('G');
	UWriteData(':');

	Draw_Rect_At(255, 65, 35, 90, 60);  	//1
	Draw_Rect_At(255, 65, 63, 90, 88);		//4
	Draw_Rect_At(255, 65, 91, 90, 116);		//7

	Draw_Rect_At(255, 93, 35, 118, 60);		//2
	Draw_Rect_At(255, 93, 63, 118, 88);		//5
	Draw_Rect_At(255, 93, 91, 118, 116);	//8	

	Draw_Rect_At(255, 121, 35, 146, 60);	//3
	Draw_Rect_At(255, 121, 63, 146, 88);	//6
	Draw_Rect_At(255, 121, 91, 146, 116);	//9

	Draw_Rect_At(255, 149, 35, 174, 60);	//C
	Draw_Rect_At(255, 149, 63, 174, 88);	//<
	Draw_Rect_At(255, 149, 91, 174, 116);	//0

	Cursor_Coordinate(75,45);
	UWriteData('1');
	Cursor_Coordinate(75,73);
	UWriteData('4');
	Cursor_Coordinate(75,101);
	UWriteData('7');

	Cursor_Coordinate(103,45);
	UWriteData('2');
	Cursor_Coordinate(103,73);
	UWriteData('5');
	Cursor_Coordinate(103,101);
	UWriteData('8');

	Cursor_Coordinate(131,45);
	UWriteData('3');
	Cursor_Coordinate(131,73);
	UWriteData('6');
	Cursor_Coordinate(131,101);
	UWriteData('9');

	Cursor_Coordinate(161,45);
	UWriteData('C');
	Cursor_Coordinate(161,73);
	UWriteData('<');
	Cursor_Coordinate(161,101);
	UWriteData('0');
}

void Draw_Touch_Region()
{
	Set_Touch_Mode(Region); // Region or Co_ordinate
	Delete_All_Regions();

	Set_Touch_Region(1, 65, 38, 19, 19, 1, 10);		// 1, 10)
	Set_Touch_Region(4, 65, 66, 19, 19, 4, 40); 	// 4, 40)
	Set_Touch_Region(7, 65, 94, 19, 19, 7, 70);		// 7, 70)

	Set_Touch_Region(2, 93, 38, 19, 19, 2, 20);		// 2, 20)
	Set_Touch_Region(5, 93, 66, 19, 19, 5, 50);		// 5, 50)
	Set_Touch_Region(8, 93, 94, 19, 19, 8, 80);		// 8, 80)

	Set_Touch_Region(3, 121, 38, 19, 19, 3, 30);	// 3, 30)	
	Set_Touch_Region(6, 121, 66, 19, 19, 6, 60);	// 6, 60)
	Set_Touch_Region(9, 121, 94, 19, 19, 9, 90);	// 9, 90)	

	Set_Touch_Region(12, 152, 38, 19, 19, 13, 14);	// 13, 14)
	Set_Touch_Region(11, 152, 66, 19, 19, 11, 12);	// 11, 12)	
	Set_Touch_Region(10, 152, 94, 19, 19, 101, 100);	//101, 100)
}
void InitADC()
{
ADMUX=(1<<REFS0);                         // For Aref=AVcc;
ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADIE); //Rrescalar div factor =128
sei();  //start interrupt
wdt_reset();
}


//***********************************************************************
void main()
{
    //WDTCR |= (1<<WDE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);
   	InitTimer();
   //Initialize ADC
   	InitADC();
	
	//Initialize the USART with Baud rate = 2400bps 
	USARTInit(12);// 19200 4mhz 12 2400 4mhz 103 8mhz 207 9600 4mhz 25

	//Enable Internal Pullups on PORTC
	PORTC=0xFF;
	
	InitDisplay();
	
	Command_Display(Go_Home);
	Write_Blank(5);
	Command_Display(Go_Home);
	//for(i=1;i<2;i++)
	//{
	//	packet[1]=UReadData();
	//	p_data=packet[1];
		//Write_Blank(2);
	//	itoa(p_data, message3, 10); 
	//	WriteString(message3);
//	}

	Draw_Pad();
	Draw_Touch_Region();
	current_cursor_x = Az_Start_X;
	current_cursor_y = Az_Start_Y;

	ADCSRA|=(1<<ADSC);

   while(1)
   {
		
	//	while(!UDataAvailable());
	//	if(UReadData()!=0xFE) continue;
	//	while(UDataAvailable()!=2);
	//	{
		//	if(packet[1]!=255)
			//{
			//	wdt_reset();
		//	//}
		//}

		//		p_data=255;
		//		Command_Display(Go_Home);
		//		Write_Blank(5);
		//		Command_Display(Go_Home);
			//	for(i=1;i<2;i++)
				//{
					packet[1]=UReadData();
			//	}
					p_data=packet[1];
				//	Write_Blank(2);
				//	itoa(p_data, message3, 10); 
				//	WriteString(message3);
					if(p_data == 13)
					{
						Cursor_Coordinate(Az_Start_X,Az_Start_Y);
						Write_Blank(6);
						current_cursor_x= Az_Start_X;
						current_cursor_y= Az_Start_Y;
						Button_Feedback(149, 35, 174, 60);
					}
					else if(p_data == 11 && (current_cursor_x > Az_Start_X))
					{
						current_cursor_x -=7;
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						Write_Blank(1);
						Button_Feedback(149, 63, 174, 88);
					}
					else if(p_data == 1 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('1');
						current_cursor_x +=7;
						Button_Feedback(65, 35, 90, 60);
					}
					else if(p_data == 2 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('2');
						current_cursor_x +=7;
						Button_Feedback(93, 35, 118, 60);
					}
					else if(p_data == 3 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('3');
						current_cursor_x +=7;
						Button_Feedback(121, 35, 146, 60);
					}
					else if(p_data == 4 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('4');
						current_cursor_x +=7;
						Button_Feedback(65, 63, 90, 88);
					}
					else if(p_data == 5 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('5');
						current_cursor_x +=7;
						Button_Feedback(93, 63, 118, 88);
					}
					else if(p_data == 6 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('6');
						current_cursor_x +=7;
						Button_Feedback(121, 63, 146, 88);
					}
					else if(p_data == 7 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('7');
						current_cursor_x +=7;
						Button_Feedback(65, 91, 90, 116);
					}
					else if(p_data == 8 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('8');
						current_cursor_x +=7;
						Button_Feedback(93, 91, 118, 116);
					}
					else if(p_data == 9 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('9');
						current_cursor_x +=7;
						Button_Feedback(121, 91, 146, 116);
					}
					else if(p_data == 101 && (current_cursor_x < Az_Finish_X))
					{
						Cursor_Coordinate(current_cursor_x, current_cursor_y);
						UWriteData('0');
						current_cursor_x +=7;
						Button_Feedback(149, 91, 174, 116);
					}
					else
					{
						
					}
	}			
}

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
      WriteString(Empty);  
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
	Wait(1);
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
	Wait(1);
}

void Draw_Rect_At(unsigned char color, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{	
	Command_Display(Draw_Rectangle);
	UWriteData(color);
	UWriteData(x1);
	UWriteData(y1);
	UWriteData(x2);
	UWriteData(y2);
	Wait(1);
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

void Command_Display(unsigned char command)
{	
	UWriteData(CMD);
	UWriteData(command);
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

uint16_t ReadADC(uint8_t ch)
{
   //Select ADC Channel ch must be 0-7
   ch=ch&0b00000111;
   ADMUX|=ch;

   //Start Single conversion
   ADCSRA|=(1<<ADSC);

   //Wait for conversion to complete
   while(!(ADCSRA & (1<<ADIF)));

   //Clear ADIF by writing one to it
   //Note you may be wondering why we have write one to clear it
   //This is standard way of clearing bits in io as said in datasheets.
   //The code writes '1' but it result in setting bit to '0' !!!

   ADCSRA|=(1<<ADIF);

   return(ADC); 
}

void WriteString(unsigned char *strPointer)
{   
   while (*strPointer)
   {
      UWriteData(*strPointer);
      strPointer++;   
   }
   
   return;
} 


//******************************************************************************
ISR(RESET_vect)
{

}


ISR(INT0_vect)
{
//	switch_temp = ~PINK;
	//if (switch_temp != 0x00){					// Skip if button release
	//	pulse_incr = 0x10;					// Maximum step = 16
	//	for (i=0x80;i > 0x00; i = i >> 1){
	//		if (switch_temp & i) break;
		//	pulse_incr -= 2;
	//	}
	//	brightness = INCREASING;
		//slider_value = 0x00;				// Start new cycle with LED off
//	}
}

ISR(ADC_vect)
{
	adc_result=ADC; 
	OCR1A= ADC + 150;
	if(abs(temp-adc_result) > 2)
	{
		if(temp > adc_result)
		{
			temp = adc_result;
			//Command_Display(Go_Home);
			//Cursor_Pos(4,5);
			Cursor_Coordinate(5,120);
			Write_Blank(10);
			//Command_Display(Go_Home);
			//Cursor_Pos(4,5);
			Cursor_Coordinate(5,120);
			whole = adc_result;///204;
			//WriteString(Value);
			itoa(whole, message1, 10); 
			WriteString(message1);
		}
		else if(temp < adc_result)
		{
			temp = adc_result;
			//Command_Display(Go_Home);
			//Cursor_Pos(4,5);
			Cursor_Coordinate(5,120);
			Write_Blank(10);
			//Command_Display(Go_Home);
			//Cursor_Pos(4,5);
			Cursor_Coordinate(5,120);
			whole = adc_result;///204;
			//WriteString(Value);
			itoa(whole, message1, 10); 
			WriteString(message1);
		}
		else
		{
			temp = adc_result;
		}
	}
	else
	{
		temp = adc_result;
	}

}

ISR(TIMER0_OVF_vect)
{
   //This is the interrupt service routine for TIMER0 OVERFLOW Interrupt.
   //CPU automatically call this when TIMER0 overflows.

   //Increment our variable
   count++;
   if(count==20)  //61 = 1 sec : 20 = 1/3 sec
   {
      	
	  ADCSRA|=(1<<ADSC);
	  data=PINC;
		if(data == 0xFE) //FE FD FB F7 EF DF BF 7F
		{
			
		//	Command_Display(Go_Home);
		//	Write_Blank(36);
		//	Command_Display(Go_Home);
		//	WriteString(Wulf);
		OCR1A=194;
		}
		else if(data == 0xFD)
		{
		//	Command_Display(Go_Home);
		//	Write_Blank(36);
		//	Command_Display(Go_Home);
		//	WriteString(Status);
		OCR1A=1070;
		}
		else if(data == 0xFB)
		{
			//for(i=1;i<3;i++)
			//	{
					packet[0]=UReadData();
				//}
			
				p_data=packet[0];
		    	Command_Display(Go_Home);
				Write_Blank(36);
				Command_Display(Go_Home);
				WriteString(Packet);
				itoa(p_data, message3, 10); 
				WriteString(message3);
		}
		else if(data == 0xF7)
		{
			draw_y++;
			Draw_Pixel_At(draw_x, draw_y);
		}
		else if(data == 0xEF)
		{
			Command_Display(Display_On);
			InitDisplay();
		}
		else if(data == 0xDF)
		{
			Command_Display(Display_OFF);
		}
		else if(data == 0xBF)
		{
			Command_Display(Clear_Screen);
		}
		else if(data == 0x7F)
		{
			Touch_Calibration();
			//while(!UDataAvailable());
			
		}
		else
		{
		//	Command_Display(Go_Home);
		//	Write_Blank(36);
		//	Command_Display(Go_Home);	
		}
		count=0;
		
   }


}



