/************************************************************

A Program to demonstrate the use of I2C RTC (Real Time Clock)
(DS1307). Here the DS1307 RTC Module is connected with an
AVR ATmega8 MCU by I2C Serial Bus.

This program reads time from the DS1307 and displays it in 
16x2 LCD Module. The system also has two buttons for user 
interaction. The 3 buttons are

1)MENU/Enter/Move selection.(Connected to PB2)
2)LEFT/Increase(Connected to PB1)
3)RIGHT/Decrease(Connected to PB0)

When powered up the display is like this.

|----------------|
|DS1307 RTC Exmpl|
|  06:07:48: PM  |
|----------------|

Then you can press 1)Menu Key to bring up the main menu.

|----------------|
|    Main Menu   |
|< Set Time     >|
|----------------|

The Main Menu Has following options.

1)Set Time
2)Set On Time
3)Set Off Time
4)Quit

By Using the "Set Time" option the user can set the time.

Hardware
********
|ATmega8 Running at 1MHz Internal Clock. Fuse Byte set as
|follows.
|
|LOW=0x21 HIGH=0xD9
|
|->LCD Connection
|  **************
|  A standard 16x2 LCD Module is connected as follows.
|  LCD PIN | ATmega8 PIN
|  ---------------------
|  D4        PD0
|  D5        PD1
|  D6        PD2
|  D7        PD3
|  E         PD4
|  RS        PD6
|  RW        PD5
|
|->Push Button Connection
|  **********************
|  1)MENU/Enter/Move selection.(Connected to PB2)
|  2)LEFT/Increase(Connected to PB1)
|  3)RIGHT/Decrease(Connected to PB0)
| 
|  All Buttons Connected Like This
|  
|  i/o pin of mcu ----[Button]--->GND
|
|->DS1307 RTC
|  **********
|  
|  DS1307 PIN | ATmega8 PIN
|  ------------------------
|  SDA          SDA
|  SCL          SCL
|
|  Both PINs must be pulled up to 5v by 4.7K resistors.


PLEASE SEE WWW.EXTREMEELECTRONICS.CO.IN FOR DETAILED 
SCHEMATICS,USER GUIDE AND VIDOES.

COPYRIGHT (C) 2008-2009 EXTREME ELECTRONICS INDIA
************************************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "I2C.h"
#include "lcd.h"
#include "ds1307.h"

void ShowMainMenu();
void SetTime();

void Wait()
{
	uint8_t i;
	for(i=0;i<20;i++)
		_delay_loop_2(0);
}

uint8_t PREV_PINB=0xFF;
/*

Function to test the current status of keys(0 to 2)

returns
0 if NOT pressed
1 if Pressed

*/
uint8_t GetKeyStatus(uint8_t key)
{
	return (!(PINB & (1<<key)));
}

/*

Function to test the previous status of keys(0 to 2)

returns
0 if NOT pressed
1 if Pressed

*/
uint8_t GetPrevKeyStatus(uint8_t key)
{
	return (!(PREV_PINB & (1<<key)));
}
void main()
{
	//Wait Util Other device startup
	_delay_loop_2(0);
	_delay_loop_2(0);

	//Initialize the LCD Module
	LCDInit(LS_BLINK);

	//Initialize I2C Bus
	I2CInit();

	//Enable Pull ups on keys
	PORTB|=((1<<PB2)|(1<<PB1)|(1<<PB0));

	//Clear CH bit of RTC
	#define CH 7

	uint8_t temp;
	DS1307Read(0x00,&temp);

	//Clear CH Bit
	temp&=(~(1<<CH));

	DS1307Write(0x00,temp);

	//Set 12 Hour Mode
	DS1307Read(0x02,&temp);

	//Set 12Hour BIT
	temp|=(0b01000000);

	//Write Back to DS1307
	DS1307Write(0x02,temp);

	
	LCDClear();
	
	LCDWriteString("DS1307 RTC Exmple");
	
	char Time[12];	//hh:mm:ss AM/PM
	
	//Now Read and format time
	uint8_t data;
	
	while(1)
	{
		
		DS1307Read(0x00,&data);
		Time[8]='\0';
		Time[7]=48+(data & 0b00001111);
		Time[6]=48+((data & 0b01110000)>>4);
		Time[5]=':';
	
		DS1307Read(0x01,&data);
	
		Time[4]=48+(data & 0b00001111);
		Time[3]=48+((data & 0b01110000)>>4);
		Time[2]=':';
	
		DS1307Read(0x02,&data);
	
		Time[1]=48+(data & 0b00001111);
		Time[0]=48+((data & 0b00010000)>>4);

		LCDClear();
	
		LCDWriteString("DS1307 RTC Exmple");
	
		LCDWriteStringXY(2,1,Time);

		//AM/PM
		if(data & 0b00100000)
		{
			LCDWriteStringXY(11,1,"PM");
		}
		else
		{
			LCDWriteStringXY(11,1,"AM");
		}

		//Wait Some time and keep testing key input
		uint8_t i;
		for(i=0;i<20;i++)
		{
		
			if(GetKeyStatus(2))
			{
				//Go To Main Menu
				ShowMainMenu();

				_delay_loop_2(0);
				_delay_loop_2(0);
				_delay_loop_2(0);

			}
			_delay_loop_2(5000);
		}
	}
	

	
}

void ShowMainMenu()
{
	//The Main Menu
	char *menu_items[]={ 	"Set Time",
							"Set On Time",
							"Set Off Time",
							"Quit"
						};
	uint8_t menu_count=4;
	uint8_t selected=0;

	_delay_loop_2(0);
	_delay_loop_2(0);

	while(1)
	{
		LCDClear();
		LCDWriteString("    Main Menu  ");

		LCDWriteStringXY(2,1,menu_items[selected]);

		LCDWriteStringXY(0,1,"<");
		LCDWriteStringXY(15,1,">");

		if(GetKeyStatus(1))
		{
			//Left Key(No 1) is pressed
			//Check that it was not pressed previously
			if(!GetPrevKeyStatus(1))
			{
				if(selected !=0)
					selected--;
			}
		}

		if(GetKeyStatus(0))
		{
			//Right Key(No 0) is pressed
			//Check that it was not pressed previously
			if(!GetPrevKeyStatus(0))
			{
				if(selected !=(menu_count-1))
					selected++;
			}
		}

		if(GetKeyStatus(2))
		{
			//Enter Key Pressed
			//Check that it was not pressed previously
			if(!GetPrevKeyStatus(2))
			{
				//Call Appropriate Function
				switch (selected)
				{
					case 0:
						SetTime();
						break;
					case 1:
						//SetOnTime();
						break;

					case 2:
						//SetOffTime();
						break;

					case 3:
						return;//Quit
				}
				
			}
		}

		PREV_PINB=PINB;

		_delay_loop_2(5000);
	}
}

void SetTime()
{
	

	uint8_t hr,min,sec,am_pm,temp;

	//Read the Second Register
	DS1307Read(0x00,&temp);
	sec=(((temp & 0b01110000)>>4)*10)+(temp & 0b00001111);

	//Read the Minute Register
	DS1307Read(0x01,&temp);
	min=(((temp & 0b01110000)>>4)*10)+(temp & 0b00001111);

	//Read the Hour Register
	DS1307Read(0x02,&temp);
	hr=(((temp & 0b00010000)>>4)*10)+(temp & 0b00001111);

	am_pm=(temp & 0b00100000)>>4;

	//If Hour Register is 0 make it 12, as 00:00:00 invalid time
	if(hr==0) hr=12;

	uint8_t sel=0;

	while(1)
	{
		LCDClear();

		LCDWriteString("00:00:00    <OK>");
		
		LCDWriteIntXY(0,0,hr,2);
		LCDWriteIntXY(3,0,min,2);
		LCDWriteIntXY(6,0,sec,2);

		if(am_pm)
		{
			LCDWriteStringXY(9,0,"PM");
		}
		else
		{
			LCDWriteStringXY(9,0,"AM");
		}

		//Draw Pointer
		LCDWriteStringXY(sel*3,1,"^^");

		//Input Up key
		if(GetKeyStatus(1))
		{
			if(!GetPrevKeyStatus(1))
			{
				if(sel==0)
				{	
					//Hour
					if(hr==12)
					{
						hr=1;
					}
					else
					{
						hr++;
					}
				}

				if(sel==1)
				{	
					//Min
					if(min==59)
					{
						min=0;
					}
					else
					{
						min++;
					}
				}

				if(sel==2)
				{	
					//Sec
					if(sec==59)
					{
						sec=0;
					}
					else
					{
						sec++;
					}
				}

				if(sel==3)
				{	
					//AM-PM
					if(am_pm==0)
					{
						am_pm=1;
					}
					else
					{
						am_pm=0;
					}
				}
				if(sel == 4)
				{
					//OK
					break;
				}
			}
		}

		//Input Down
		if(GetKeyStatus(0))
		{
			if(!GetPrevKeyStatus(0))
			{
				if(sel==0)
				{	
					//Hour
					if(hr==1)
					{
						hr=12;
					}
					else
					{
						hr--;
					}
				}

				if(sel==1)
				{	
					//Min
					if(min==0)
					{
						min=59;
					}
					else
					{
						min--;
					}
				}

				if(sel==2)
				{	
					//Sec
					if(sec==0)
					{
						sec=59;
					}
					else
					{
						sec--;
					}
				}

				if(sel==3)
				{	
					//AM-PM
					if(am_pm==0)
					{
						am_pm=1;
					}
					else
					{
						am_pm=0;
					}
				}
				if(sel == 4)
				{
					//OK
					break;
				}
			}
		}

		if(GetKeyStatus(2))
		{
			if(!GetPrevKeyStatus(2))
			{
				//Change Selection
				if(sel==4)
					sel=0;
				else
					sel++;
			}
		}

		PREV_PINB=PINB;

		_delay_loop_2(30000);

	}

	//Now write time back to RTC Module
	temp=((sec/10)<<4)|(sec%10);
	DS1307Write(0x00,temp);

	temp=((min/10)<<4)|(min%10);
	DS1307Write(0x01,temp);

	temp=((hr/10)<<4)|(hr%10);
	temp|=0b01000000; //12 Hr Mode
	if(am_pm)
	{
		temp|=0b00100000;
	}
	DS1307Write(0x02,temp);

	LCDClear();
	LCDWriteString("Message !");
	LCDWriteStringXY(0,1,"Main Time Set");

	uint8_t i;
	for(i=0;i<10;i++)
		_delay_loop_2(0);




}
