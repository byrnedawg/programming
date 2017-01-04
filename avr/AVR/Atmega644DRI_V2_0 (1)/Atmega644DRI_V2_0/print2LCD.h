/*
 * print2LCD.h
 *
 * Created: 12/9/2015 2:49:04 PM
 *  Author: ByrneDawg
 */ 

#include "lcd_lib.h"

#ifndef PRINT2LCD_H_
#define PRINT2LCD_H_

#define PRINT_INT_BUFSIZE	16

uint8_t system_Second = 0;
uint8_t system_Minute = 0;
uint8_t system_Hour = 0;
uint8_t system_Day = 0;

void clearLCDScreen(uint8_t x, uint8_t y, uint8_t numchar  )
{
	LCDGotoXY(x, y);
	for(uint8_t j = 0; j<numchar; j++)
	{
		LCDsendChar(' ');
	}
}

void printTime(uint8_t x, uint8_t y, uint32_t system_clk)
{
	system_Second = system_clk % 60;
	system_Minute = system_clk / 60;
	system_Hour = system_clk / 3600;
	system_Day = system_clk / 86400;
	
	/*if((system_clk % 60) == 0) //
	{
		system_Minute++;
		//system_Second = 0;
		if((system_Minute % 60) == 0)
		{
			system_Hour++;
			system_Minute = 0;
			if((system_Hour % 24) == 0)
			{
				system_Day++;
				system_Hour = 0;
			}
		}
	}
	*/
	LCDGotoXY(x, y);
	char buf[PRINT_INT_BUFSIZE];
	
	buf[10] = system_Day / 10 +48 ;
	buf[9] =  system_Day % 10 +48 ;
	buf[8] =  ':' ;
	buf[7] = (system_Hour%24) / 10 +48 ;
	buf[6] = (system_Hour%24) % 10 +48 ;
	buf[5] =  ':' ;
	buf[4] = (system_Minute%60) / 10 +48 ;
	buf[3] =  (system_Minute%60) % 10 +48 ;
	buf[2] =  ':' ;
	buf[1] = system_Second / 10 +48 ;
	buf[0] =  system_Second % 10 +48 ;
	
	for(int k = 10; k >= 0; k--)
	{
		LCDsendChar(buf[k]);
	}
}
void printAccelMil(uint8_t x, uint8_t y, int accel)
{
	//accel = asin(accel)*1018.59163579;
	LCDGotoXY(x, y);
	char buf[PRINT_INT_BUFSIZE];
	int8_t sign = 1;
	if (accel < 0) { // look for the sign
		sign = -1;
		accel= -accel;
	}
	
	if (sign < 0)
	{	// don't forget to add the sign
		buf[4] = '-';
	}
	else
	{
		buf[4] = ' ';
	}
	buf[3] = accel / 1000 +48 ;
	buf[2] = (accel / 100) %10 +48 ;
	buf[1] = (accel / 10)%10 +48 ;
	buf[0] =  accel % 10 +48 ;
	
	for(int k = 4; k >= 0; k--)
	{
		LCDsendChar(buf[k]);
	}
}

void printAccelTilt(uint8_t x, uint8_t y, int accel)
{
	//accel = asin(accel)*1018.59163579;
	LCDGotoXY(x, y);
	char buf[PRINT_INT_BUFSIZE];
	int8_t sign = 1;
	if (accel < 0) { // look for the sign
		sign = -1;
		accel= -accel;
	}
	
	if (sign < 0)
	{	// don't forget to add the sign
		buf[4] = '-';
	}
	else
	{
		buf[4] = ' ';
	}
	buf[3] = accel / 1000 +48 ;
	buf[2] = (accel / 100) %10 +48 ;
	buf[1] = (accel / 10)%10 +48 ;
	buf[0] =  accel % 10 +48 ;
	
	for(int k = 4; k >= 0; k--)
	{
		LCDsendChar(buf[k]);
	}
}

void printAccelDeg(uint8_t x, uint8_t y, int accel)
{
	//accel = asin(accel)*57.2957795;
	LCDGotoXY(x, y);
	char buf[PRINT_INT_BUFSIZE];
	int8_t sign = 1;
	if (accel < 0) { // look for the sign
		sign = -1;
		accel= -accel;
	}
	
	if (sign < 0)
	{	// don't forget to add the sign
		buf[2] = '-';
	}
	else
	{
		buf[2] = ' ';
	}

	buf[1] = (accel / 10)%10 +48 ;
	buf[0] =  accel % 10 +48 ;
	
	for(int k = 2; k >= 0; k--)
	{
		LCDsendChar(buf[k]);
	}
}
void printInt(uint8_t x, uint8_t y, int32_t i)
{
	LCDGotoXY(x, y);
	char buf[PRINT_INT_BUFSIZE];
	int8_t sign = 1;
	//int8_t len = 0;
	if (i < 0) { // look for the sign
		sign = -1;
		i = -i;
	}
	
	buf[3] = i / 1000 + 48;
	buf[2] = (i / 100)%10 +48 ;
	buf[1] = (i / 10)%10 +48 ;
	buf[0] =  i % 10 +48 ;
	
	if (sign < 0)
	{	// don't forget to add the sign
		buf[4] = '-';
	}
	else
	{
		buf[4] = ' ';
	}
	
	if(i < 1000)
	{
		buf[3] = ' ';
	}
	if(i < 100)
	{
		buf[2] = ' ';
	}
	if(i < 10)
	{
		buf[1] = ' ';
	}
	LCDsendChar(buf[4]);
	LCDsendChar(buf[3]);
	LCDsendChar(buf[2]);
	LCDsendChar(buf[1]);
	LCDsendChar(buf[0]);
}

//demonstration of animation
void demoanimation(void)
{
	LCDclr();
	LCDdefinechar(backslash,0);
	CopyStringtoLCD(LCDanimation, 4, 1);
	for(uint8_t i=0;i<3;i++)
	{
		LCDGotoXY(8, 1);
		LCDsendChar(0);
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('-');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('/');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('|');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar(8);//backslash
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('-');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('/');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('|');
		delay1s();
	}
}


#endif /* PRINT2LCD_H_ */