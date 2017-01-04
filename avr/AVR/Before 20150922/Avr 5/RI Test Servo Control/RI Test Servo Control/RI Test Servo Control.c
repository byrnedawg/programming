/*
 * RI_Test_Servo_Control.c
 *
 * Created: 2/18/2014 1:56:29 PM
 *  Author: gregory.byrne
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/eeprom.h> 
#include "GB_usart.c"

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

#define sbi(x,y) x |= _BV(y) //set bit (original sbi)
#define cbi(x,y) x &= ~(_BV(y)) //(original)cbi clear bit //reverse cause of 3904
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) (x & _BV(y)) == _BV(y) //(for input) checks if the // input is high (Logic 1) i.e. if(is_high(PIND,PD0))						
#define RESET &=~
#define SET |=
#define TOGGLE ^=

#define BAUDRATE 19200
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

volatile uint16_t rotate_Servo_Clockwise; // 
volatile uint16_t rotate_Servo_Counter_Clockwise; //
volatile uint16_t stop_Servo; // 
volatile uint16_t stop_Time; //
volatile uint16_t CCW_rotate_Time; //
volatile uint16_t CW_rotate_Time; //

uint32_t rotation_Count = 0; //
uint32_t aNumberAsString[10];
uint8_t Label[]= "1/4 Turn";
uint8_t pause_String[]= "Paused";
uint8_t finished_String[]= "Finished";
uint8_t start_Screen1[]= "Fatigue";
uint8_t start_Screen2[]= "RI Test";
uint8_t first_Pause = 0;
     

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
	UWriteData(71);
	UWriteData(column);
	UWriteData(row);
}

void PortInit(void) // initialize port B inputs and outputs 
{
	PORTB |= (1<<PB7) | (1<<PB6) | (1<<PB2) | (1<<PB1) | (1<<PB0); // set all pins we are using high
	DDRB |= (0<<DDB7) | (0<<DDB6) | (1<<DDB2) | (1<<DDB1) | (0<<DDB0);// set PB1  output 
	 
}

void SystemInit(void) // Power on variable initialization 
{
	USARTInit(UBRRVAL);
	rotate_Servo_Clockwise = 650; // 650 
	rotate_Servo_Counter_Clockwise = 850; // 850
	stop_Servo = 768; // 770 = stop point 
	stop_Time = 500; // Time servo rest between rotations
	CCW_rotate_Time = 420; // how far each rotation is 1/4 knob
	CW_rotate_Time = 421; // how far each rotation is 1/4 knob
	rotation_Count = 0; // 65500 
	Change_Baudrate(51); // 19200
	Command_Display(Clear_Screen);
	Command_Display(Go_Home);
	//Change_Baudrate(83); // 1200
	
}
/*
void timer0_init(void) // Initialize timer 0 
{
	// NOTE: Assumes DIV8 fuse set for 1 MHz operation
	// 8-bit counter means overflow interrupt every 61 Hz to turn on the LED
	// Variable Compare_A interrupt in between OVF to turn off the LED
	TCCR0A |= (1<<COM0A1)|(1<<COM0A0)|(1<<COM0B1)|(1<<COM0B0)|(1<<WGM01)|(1<<WGM00); // Set OC0A/OC0B on Compare Match, clear OC0A/OC0B at BOTTOM (inverting mode) ,Fast PWM Mode
	TCCR0B |= (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(0<<CS02)|(1<<CS01)|(0<<CS00); //0x02;	pre-scale= CK/8
	OCR0A = 0x00; // Governs Duty Cycle (192 ~75% of 256) for inverting mode OCR0A = (255-duty-cycle)
	OCR0B = 0x00; // Governs Duty Cycle (192 ~75% of 256) for inverting mode OCR0B = (255-duty-cycle)
	TCNT0 = 0x00; // Start Timer 
}
*/

void timer1_init(void) // Initialize timer 1
{
	TCCR1A |=  (1<<COM1A1) | (0<<COM1A0) | 
			(1<<COM1B1) | (0<<COM1B0) | 
			(0<<FOC1A) | (0<<FOC1B) | 
			(0<<WGM11) | (0<<WGM10);     
			      
	TCCR1B |= (0<<ICNC1) | (0<<ICES1) | 
			(1<<WGM13)  | (0<<WGM12) | 
			(0<<CS12) | (1<<CS11) | (0<<CS10); 
			
	ICR1 = 10000; //10000;    period = 20 ms   ICR1 = (time[us] * F_CPU[MHz] / prescaler) - 1 
	TCNT1 = 0x00; // Start Timer
}

void rotate_RI_CW(uint16_t cycles)
{
	while(cycles > 0)
	{
		OCR1A = rotate_Servo_Clockwise; 
		OCR1B = rotate_Servo_Clockwise;   
		_delay_ms(CW_rotate_Time);  
		OCR1A = stop_Servo;  
		OCR1B = stop_Servo; 
		rotation_Count++;   
		update_Display_Count(rotation_Count);
		_delay_ms(stop_Time); 
		cycles--;
	}		
}

void rotate_RI_Counter_CW(uint16_t cycles)
{
	while(cycles > 0)
	{
		OCR1A = rotate_Servo_Counter_Clockwise;  
		OCR1B = rotate_Servo_Counter_Clockwise;  
		_delay_ms(CCW_rotate_Time); 
		OCR1A = stop_Servo;
		OCR1B = stop_Servo;
		rotation_Count++; 
		update_Display_Count(rotation_Count);    
		_delay_ms(stop_Time); 
		cycles--;
	}		
}

void update_Display_Count (uint32_t count)
{
	Command_Display(Clear_Screen);	
	Command_Display(Go_Home);
	ltoa(count, aNumberAsString, 10);
	UWriteString(aNumberAsString);	
	Set_Cursor_Position(1,2);
	UWriteString(Label);
}

void start_Test (void)
{
	Command_Display(Clear_Screen);
	Command_Display(Go_Home);
	UWriteString(start_Screen1);
	Set_Cursor_Position(1,2);
	UWriteString(start_Screen2);
}

void pause_Test (void)
{
	Command_Display(Clear_Screen);	
	Command_Display(Go_Home);
	UWriteString(aNumberAsString);	
	Set_Cursor_Position(1,2);
	UWriteString(pause_String);
}

void RI_Test_1(void)
{
	if(rotation_Count < 100000)
	{
		rotate_RI_CW(5);
		rotate_RI_Counter_CW(5);
		_delay_ms(500);
	}
	else
	{
		Command_Display(Clear_Screen);	
		Command_Display(Go_Home);
		ltoa(rotation_Count, aNumberAsString, 10);
		UWriteString(aNumberAsString);	
		Set_Cursor_Position(1,2);
		UWriteString(finished_String);
		while(1)
		{
			
		}		
	}		
}

int main(void) 
{
	//timer0_init(); // timer 0 initialize 
	SystemInit(); // system variables and state initialize
	timer1_init(); // timer 1 initialize
	PortInit(); // inputs and outputs initialize
	_delay_ms(500);
	start_Test();
	_delay_ms(5000);
	while(1)
	{
		if(is_high(PINB,PB0))
		{
			if(first_Pause == 0)
			{
				pause_Test();
				first_Pause++;
			}
		}
		else
		{
			first_Pause= 0;
			RI_Test_1();
		}
	}
}

   