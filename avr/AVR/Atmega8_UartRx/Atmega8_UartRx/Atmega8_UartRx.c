/*
 * Atmega8_UartRx.c
 *
 * Created: 12/11/2015 2:50:17 PM
 *  Author: ByrneDawg
 */ 

#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/pgmspace.h>

#include "uart.h"

#define CMD 254
#define Backspace_Left 8
#define Move_Right 9
#define Line_feed 10
#define Cariage_Return 13
#define Clear_Screen 12
#define Go_Home 12

//define receive parameters
#define SYNC 65// synchro signal
#define RADDR 33
#define LEDON 69//switch led on command
#define LEDOFF 71//switch led off command

#define PRINT_NL uart_putc('\n')

#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);

#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);

#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)

#define Switch1 PB0
#define GreenLED PB1
#define Switch3 PB2

#define PRINT_INT_BUFSIZE	16

const char DeviceAddress[] PROGMEM= "AZBYCXDW";
volatile uint16_t timer0Overflows = 0; // Stores the value for timer polling timer 0
volatile uint8_t switch_state = 1;	// Light Function state
volatile int8_t i = -128;
volatile int8_t d = 1;
volatile uint8_t BTN_Down = 0;
volatile uint8_t lightOn = 0;
volatile uint8_t lightcounter = 0;
volatile uint8_t timeOff = 0;

void commandDisplay(uint8_t i)
{
	//uart_putc(CMD);
	uart_putc(i);
}


void printInt(int32_t i)
{
	char buf[PRINT_INT_BUFSIZE];
	int8_t sign = 1;
	int8_t len = 0;

	if (i < 0) { // look for the sign
		sign = -1;
		i = -i;
	}

	// fill buffer with digits (in reverse order)
	do {
		buf[len++] = 48 + i % 10;	// ASCII digits start at 48
	} while ((i /= 10) > 0 && len < PRINT_INT_BUFSIZE);

	if (sign < 0) {	// don't forget to add the sign
		buf[len] = '-';
		} else {
		len--;
	}

	// reverse output of the buffer
	do {
		uart_putc(buf[len--]);
	} while (len >= 0);
}

/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

/* 9600 baud */
#define UART_BAUD_RATE      300


ISR(TIMER0_OVF_vect) // timer 0 overflow interrupt
{
	TCNT0 += 6;
	if((timer0Overflows % 1) == 0) // push button polling 195 1 sec /975 5 sec/ 11700 1 min/
	{
		if(is_clear(PINB, Switch1)) // if the push button is held down connecting pin B2 to ground
		{
			BTN_Down = 1; // button down is true
		}
	}
	if((timer0Overflows % 2) == 0) // push button polling on the release
	{

		if(is_set(PINB, Switch1) && BTN_Down == 1) //
		{
			switch_state++; // increment mode states by 1 i.e. red -> yellow, green - > blue
			BTN_Down = 0; // push button is released so the button is no longer held down
			//update_Display();
		}
		//update_LED();
	}
	if((timer0Overflows % 16) == 0) // push button polling on the release
	{
		//update_LED();
		lightcounter++;
		timeOff++;
		if(lightOn == 1 )
		{
			output_low(PORTB, GreenLED);
		}
		else if(lightcounter < 2)
		{
			output_low(PORTB, GreenLED);
		}
		else
		{
			output_high(PORTB, GreenLED);
		}
	}
	if((timer0Overflows % 32) == 0) // push button polling on the release
	{
		if (i == 127) d = -1;   // count down
		if (i == -128) d = 1;   // count up
		i += d;
		
		//if(is_clear(PINB, Switch2))
		//{
		//	switch_state--;
		//}
		
	}
	
	timer0Overflows++;
}

void timer0_init(void) // Initialize timer 0
{
	TIMSK |= (1<<TOIE0);
	TCCR0 |= (1<<CS02)|(0<<CS01)|(1<<CS00); //

	timer0Overflows = 0; // Start timer overflows at 0
}

void update_LED(void) //
{
	uint8_t sync, raddress, data, chk;//transmitter address
	if(uart0_peek()!= UART_NO_DATA)
	{//define variables
		//receive destination address
		sync = uart0_getc();
		if(sync!=SYNC)
		{
			//uart0_flush();
			output_high(PORTB, GreenLED);
			
		}
		else
		{
			output_low(PORTB, GreenLED);
		}
		
	}
	else
	{
		//output_high(PORTB, GreenLED);
	}

}

int main(void)
{
	// uart_puts("String stored in SRAM\n");
	//uart_puts_P("String stored in FLASH\n");
	// itoa( num, buffer, 10);   // convert interger into string (decimal format)
	//uart_puts(buffer);        // and transmit string to UART
	
	uint8_t i; //Clasical loop varriable

	uint8_t packet[5];
	uint8_t data=0;
	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	
	timer0_init();

	sei();
	
	
	set_input(DDRB, Switch1);
	set_output(DDRB, GreenLED);
	set_input(DDRB, Switch3);
	output_high(PORTB, GreenLED);
	
	
	while(1)
	{	 
		 while(!uart0_available())
		 {
			 if(timeOff > 1)
			 {
				 lightOn = 0;
			 }
			 //_delay_ms(10);
		 }
		 
		 if(uart0_getc()=='A') 
		 {
			 lightOn = 1;
			 lightcounter = 0;
			 timeOff = 0;
			// output_low(PORTB, GreenLED);
		 }
		 else
		 {
			lightOn = 0;
		 }
		 /*
		 while(!uart0_available());
		 if(uart0_getc()!='A') 
		 {
			 continue;
		 }
		 while(uart0_available()!=3)
		 {
			//output_high(PORTB,GreenLED);
		 }

		 //Get the packet

		 for(i=2;i<5;i++)
		 {
			 packet[i]=uart0_getc();
		 }

		 //Is it ok?
		 if(packet[2]!=((uint8_t)~packet[3])) continue;

		 if(packet[4]!='Z') continue;

		 //The packet is ok

		 data=packet[2];

		 //Now we have data put it to PORTC
		 if(data == LEDON)
		 {
			 lightOn = 1;
			 timeOff = 0;
		 }
		 else
		 {
			 lightOn = 0;
			 timeOff = 0;
		 }
		 */
	}
}
