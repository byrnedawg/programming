
#include<stdio.h>
#include<wiringPi.h>
#include<wiringSerial.h>
void lcd_init(void);	//init the lcd
void enable();
void lcd_cmd(unsigned char cmd);
void lcd_char(unsigned char data);
void data_write(unsigned char val);
void lcd_string(const char*);
void gotoxy1(unsigned char num);
void gotoxy2(unsigned char num);
/*

LCD interfacing to pi

------------LCD------------------------pi------------
PIN Name	Pin no.	|	PIN Name	Pin no.		
------------------------------------------------------
DB4			11		|	 GPIO2		13
DB5			12		|	 GPIO3		15
DB6			13		|	 GPIO4		16
DB7			14		|	 GPIO5		18
					|
RS			04		|	 GPIO0		11
RW			05		|	 GND		GROUND
EN			06		|	 GPIO7		07
					|
VSS			01		| 	 GND		GROUND
VO			03		|	 GND		GROUND		
VCC			02		|	 VCC		2(5V)
------------------------------------------------------
*/
/******************************
FOR ROOT ACCESS
*******************************/
/*
#define RS   0
#define EN   7
#define DB4  2
#define DB5  3
#define DB6  4
#define DB7  5

*/
/*******************************
FOR NON ROOT ACCESS
********************************/
#define RS   17
#define EN   4
#define DB4  27
#define DB5  22
#define DB6  23
#define DB7  24


void lcd_init(void)	//init the lcd
{	
	pinMode (DB4,OUTPUT);pinMode (DB5,OUTPUT);pinMode (DB6,OUTPUT);		//All pis as output
	pinMode (DB7,OUTPUT);pinMode (RS,OUTPUT);pinMode (EN,OUTPUT);
	delay(15);
	
	digitalWrite (RS,LOW) ;
	digitalWrite (EN,LOW) ;

//	PORTUSED=0x20;
	lcd_cmd(0x01);
    delay(100);
	lcd_cmd(0x0F);
	lcd_cmd(0x02);
	lcd_cmd(0x80);
    delay(150);
		
}
//============================================================================================
void enable()
{
	digitalWrite (EN,HIGH) ;
	delay(15);                      //......
	digitalWrite (EN,LOW) ;
}
//============================================================================================
void lcd_cmd(unsigned char cmd)
{

	digitalWrite (RS,LOW) ;
	data_write(cmd);

}
//----------------------------------------------------------------------------------------
void lcd_char(unsigned char data)
{
	digitalWrite (RS,HIGH) ;
	data_write(data);
	
}
//=========================================================================================
void data_write(unsigned char val)
{	

		digitalWrite (EN,HIGH) ;
	digitalWrite (DB7,((val&0x80)>>7) );
	digitalWrite (DB6,((val&0x40)>>6) );
	digitalWrite (DB5,((val&0x20)>>5) );
	digitalWrite (DB4,((val&0x10)>>4) );
	
	delay(1);                      //......
	digitalWrite (EN,LOW) ;
	
digitalWrite (EN,HIGH) ;
	digitalWrite (DB7,((val&0x08)>>3) );
	digitalWrite (DB6,((val&0x04)>>2) );
	digitalWrite (DB5,((val&0x02)>>1) );
	digitalWrite (DB4,((val&0x01)>>0) );
	
	delay(1);                      //......
	digitalWrite (EN,LOW) ;
	

}
//==========================================================
void lcd_string(const char* str)
{
unsigned char i=0;
	while(str[i]!='\0')
	{
		lcd_char(str[i]);
		
		if(i>15)
		{
		lcd_cmd(0xC1);
		lcd_char(str[i]);
		
		}
		i++;
	
	}

}
//=================================================
void gotoxy1(unsigned char num)
{
	lcd_cmd(0x80+num);	
}
//================================================
void gotoxy2(unsigned char num)
{
	lcd_cmd(0xc0+num);	
}
