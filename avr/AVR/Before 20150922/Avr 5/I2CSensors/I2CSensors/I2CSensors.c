/*
 * I2CSensors.c
 *
 * Created: 8/25/2014 8:59:03 AM
 *  Author: gregory.byrne
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "i2cmaster.h"
#include "LIS302DL.h"
#include "GB_usart.h"

#define MAG_ADDR  0x0E //7-bit address for the MAG3110, doesn't change

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

uint32_t sensor_Data = 0; // 0
uint32_t aNumberAsString[10];
uint8_t No_Data_String[]= "No Data";
uint8_t pause_String[]= "Paused";
uint8_t uTeslas_String[]= "uTeslas";
uint8_t start_Screen1[]= "Sensor";
uint8_t start_Screen2[]= "Display";
uint8_t first_Pause = 0;




void placeEmptyCharacters (char charsNumber[8], unsigned char emptySpacesNumber);

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
	DDRB = (0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);// set PB1  PB2 Enable pins on L298n PB0 motor drive  
	PORTB = (0<<PB7) | (0<<PB6) | (0<<PB5) | (0<<PB4) | (0<<PB3) | (0<<PB2) | (0<<PB1) | (0<<PB0); // set all pins we are using high
	
	DDRC =  (0<<DDC6) | (1<<DDC5) | (1<<DDC4) | (0<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);// set PB1  PB2 Enable pins on L298n PB0 motor drive  
	PORTC = (0<<PC6) | (0<<PC5) | (0<<PC4) | (0<<PC3) | (0<<PC2) | (0<<PC1) | (0<<PC0); // set all pins we are using high
	
	DDRD = 0xFF;
	DDRD = (1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);// set PD7 - PD5 motor drive  
	PORTD = (0<<PD7) | (0<<PD6) | (0<<PD5) | (0<<PD4) | (0<<PD3) | (0<<PD2) | (0<<PD1) | (0<<PD0); // set all pins we are using high 
	 
}

void SystemInit(void) // Power on variable initialization 
{
	USARTInit(UBRRVAL);
	sensor_Data = 0; // 0 
	Change_Baudrate(51); // 19200
	Command_Display(Clear_Screen);
	Command_Display(Go_Home);
	//Change_Baudrate(83); // 1200
	
}


//--------------------------------------------------------------------

//--------------------------------------------------------------------
int main(void)
{
	char data[16];
	int axe_value=5;
	
	SystemInit(); // system variables and state initialize
	PortInit(); // inputs and outputs initialize
	LIS302DL_init();				// initialize I2C LIS302DL XYZ accelerometer.
	//sei();						// Enable interrupts.

	Command_Display(Clear_Screen);	
	Command_Display(Go_Home);
	itoa(axe_value, data, 10);		//Convert the X value integer into string.
	UWriteString(data);					//Print X value number on LCD.
	Set_Cursor_Position(1,2);
	UWriteString(uTeslas_String);	
	_delay_ms(5000);
    while(1)
    {	
	
		//-------------------------------------------------------------------
		// Read accelerometer and print X,Y,Z axis value on LCD.	
		//-------------------------------------------------------------------
		
		axe_value = LIS302DL_read(outX); //Read X axe value.
		Command_Display(Clear_Screen);	
		Command_Display(Go_Home);
		itoa(axe_value, data, 10);		//Convert the X value integer into string.
		UWriteString(data);					//Print X value number on LCD.  
		Set_Cursor_Position(1,2);
		UWriteString(uTeslas_String);
		
		/*	
		axe_value = LIS302DL_read(outY); //Read Y axe value.
		Command_Display(Clear_Screen);	
		Command_Display(Go_Home);
		itoa(axe_value, data, 10);
		UWriteString(data);
		placeEmptyCharacters(data,4);
		
		Set_Cursor_Position(1,2);
		UWriteString(uTeslas_String);
			
		axe_value = LIS302DL_read(outZ); //Read Z axe value.
		Command_Display(Clear_Screen);	
		Command_Display(Go_Home);
		itoa(axe_value , data, 10);
		UWriteString(data);
		placeEmptyCharacters(data,4);
		
		Set_Cursor_Position(1,2);
		UWriteString(uTeslas_String);
			*/
		_delay_ms(1000);	
    }
}

//==================================================================================
// Prints space characters on LCD starting from the last used LCD address.
// This function is used to erase previous printed data on LCD from specific location.
//==================================================================================
void placeEmptyCharacters (char charsNumber[8], unsigned char emptySpacesNumber)
{
	unsigned char length, i;
	length = strlen(charsNumber);
	for(i=0;i<(emptySpacesNumber - length);i++)
		UWriteString(' ');	
}	