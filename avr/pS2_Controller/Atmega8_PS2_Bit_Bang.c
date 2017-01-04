/*
 * Atmega8_PS2_Bit_Bang.c
 *
 * Created: 1/4/2016 3:24:04 PM
 *  Author: ByrneDawg
 */ 


/*

BIT BANG PS2  for atmega640 -connection according to SPI


-------------LOOKING AT THE PLUG-------------------
        -------------------------------
 PIN 1->| o  o  o | o  o  o | o  o  o |
        \_____________________________/
    
PIN # USAGE
 1    DATA
 2    COMMAND
 3    N/C (9 Volts unused)
 4    GND
 5    VCC
 6    ATT
 7    CLOCK
 8    N/C
 9    ACK


----NOTES--
*> 0x5A(by controller) to say "here comes the data".
*> bite 1. header. (should possibly put test on this byte to detect unplugging of controller.)
 ie if (temp==0x73)
*/

#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <compat/deprecated.h>
#include "uart.h"
#include "psx.h"


//#define PSclock     0               // PB0
//#define PSdata      1               // PB1
//#define PSacknolage 2               // PB2
//#define PScommand   3               // PB3
//#define PSattention 4               // PB4

/* 9600 baud */
#define UART_BAUD_RATE      19200
#define PRINT_INT_BUFSIZE	16

#define CMD 254
#define Backspace_Left 8
#define Move_Right 9
#define Line_feed 10
#define Cariage_Return 13
//#define Clear_Screen 12
#define Go_Home 12

//#define Switch1 PB0

#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);

#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);

#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)
//#define _BV(y)  (1<<y)

// PSx controller communication function.
// send a byte on the command line and receive one on the data line.
// needs Attention pin to have gone low before called to activate controller.

void commandDisplay(uint8_t i)
{
	//uart_putc(CMD);
	uart_putc(i);
}

void printInt(int32_t i)
{
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
	//uart_putc(buf[4]);
	//uart_putc(buf[3]);
	uart_putc(buf[2]);
	uart_putc(buf[1]);
	uart_putc(buf[0]);
}

void LCD_String (unsigned char *string)
{
	while (*string)
	uart_putc(*string++);
}

int main(void)
{
		
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	//psx_init(PINB0, PB0, PINB1, PB1, PINB2, PB2, PINB3, PB3);
	_delay_ms(10);
	//psx_init(PINB0, PB0, PINB1, PB1, PINB2, PB2, PINB3, PB3);
	//for(int i=0; i<5; i++)
	//{
		//psx_init(PORTB, PB0, PORTB, PB1, PORTB, PB2, PORTB, PB3);
		//psx_init(&PORTB, PORTB0, &PORTB, PORTB1, &PORTB, PORTB2, &PORTB, PORTB3); // works!!!
		psx_init(&PORTB, PB0, &PORTB, PB1, &PORTB, PB2, &PORTB, PB3);
		_delay_ms(10);
	//}
		
	//timer0_init();

	sei();

	commandDisplay(Go_Home);
	_delay_ms(10);
	//uart_putc('G');
	//uart_putc('R');
	//uart_putc('E');
	//uart_putc('G');
	LCD_String("Greg ");
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
		
		//set_input(DDRB, Switch1);
		//set_input(DDRB, Switch2);
		//set_input(DDRB, Switch3);
		
	
	//uart_putc('R');
	//uart_putc('E');
	//uart_putc('D');
	LCD_String("RED ");
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	
	psx_read_gamepad();
	psx_buttons();
	
	   
	
		//commandDisplay(Go_Home);
		//LCD_String("Failed \n");
		//printInt();
		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
	
	//wrdata(cnt);
	commandDisplay(Go_Home);
	_delay_ms(10);
	LCD_String("start");
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);

	// main program loop:
while (1){
	psx_read_gamepad();
	commandDisplay(Go_Home);
	_delay_ms(10);
	//printInt(psx_buttons());
	if(psx_button(PSB_CROSS))
	{
		LCD_String("X Button ");
	}
	if(psx_button(PSB_CIRCLE))
	{
		LCD_String("Circle ");
	}
	if(psx_button(PSB_SQUARE))
	{
		LCD_String("Square ");
	}
	if(psx_button(PSB_TRIANGLE))
	{
		LCD_String("Triangle ");
	}
	if(psx_button(PSB_R1))
	{
		LCD_String("R1 ");
	}
	if(psx_button(PSB_R2))
	{
		LCD_String("R2 ");
	}
	if(psx_button(PSB_L1))
	{
		LCD_String("L1 ");
	}
	if(psx_button(PSB_L2))
	{
		LCD_String("L2 ");
	}
	if(psx_button(PSB_START))
	{
		LCD_String("Start ");
	}
	if(psx_button(PSB_SELECT))
	{
		LCD_String("Select ");
	}
	if(psx_button(PSB_PAD_UP))
	{
		LCD_String("UP ");
	}
	if(psx_button(PSB_PAD_DOWN))
	{
		LCD_String("DOWN ");
	}
	if(psx_button(PSB_PAD_LEFT))
	{
		LCD_String("LEFT");
	}
	if(psx_button(PSB_PAD_RIGHT))
	{
		LCD_String("RIGHT");
	}
	if(psx_button(PSB_L3))
	{
		LCD_String("L3");
	}
	if(psx_button(PSB_R3))
	{
		LCD_String("R3");
	}
	if(psx_button(PSB_L2)&&psx_button(PSB_R2))
	{
		commandDisplay(Cariage_Return);
		uart_putc('R');
		uart_putc('X');
		uart_putc(':');
		printInt(psx_stick(PSS_RX));
		uart_putc(' ');
		uart_putc('R');
		uart_putc('Y');
		uart_putc(':');
		printInt(psx_stick(PSS_RY));
		uart_putc(' ');
	}
	else
	{
		//printInt(psx_buttons());
		commandDisplay(Cariage_Return);
		uart_putc('L');
		uart_putc('X');
		uart_putc(':');
		printInt(psx_stick(PSS_LX));
		uart_putc(' ');
		uart_putc('L');
		uart_putc('Y');
		uart_putc(':');
		printInt(psx_stick(PSS_LY));
		uart_putc(' ');	
	}
	_delay_ms(200);
	
	} // while(1)
} //main

	 
  
