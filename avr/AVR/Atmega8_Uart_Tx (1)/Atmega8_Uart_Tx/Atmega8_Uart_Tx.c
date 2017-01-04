/*
 * Atmega8_Uart_Tx.c
 *
 * Created: 12/11/2015 10:06:31 AM
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
#define Switch2 PB1
#define Switch3 PB2

#define PRINT_INT_BUFSIZE	16

const char DeviceAddress[] PROGMEM= "AZBYCXDW";
volatile uint16_t timer0Overflows = 0; // Stores the value for timer polling timer 0
volatile uint8_t switch_state = 1;	// Light Function state
volatile int8_t i = -128;
volatile int8_t d = 1;
volatile uint8_t BTN_Down = 0;

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
		//send_LED_Update();
	}
	if((timer0Overflows % 16) == 0) // push button polling on the release
	{
		send_LED_Update();
	}
	if((timer0Overflows % 32) == 0) // push button polling on the release
	{
		if (i == 127) d = -1;   // count down
		if (i == -128) d = 1;   // count up
		i += d;
		
		if(is_clear(PINB, Switch2))
		{
			switch_state--;
		}
		
	}
	
	timer0Overflows++;
}

void timer0_init(void) // Initialize timer 0
{
	TIMSK |= (1<<TOIE0);
	TCCR0 |= (1<<CS02)|(0<<CS01)|(1<<CS00); //

	timer0Overflows = 0; // Start timer overflows at 0
}
//void Send_Packet(uint8_t addr, uint8_t cmd)
void Send_Packet(uint8_t cmd)
{
	//uart_putc(SYNC);//send synchro byte
	//uart_putc(addr);//send receiver address
	//uart_putc(cmd);//send increment command
	//uart_putc((addr+cmd));//send checksum
	//uart_putc('J');  //J for junk

	//Send 'A'
	uart_putc('A');

	//Send Another 'A'
	uart_putc('A');
	
	uart_putc('A');
	
	uart_putc('A');
	
	uart_putc('A');
	
	uart_putc('A');
	
	uart_putc('A');
	
	uart_putc('A');
	
	uart_putc('A');
	
	uart_putc('A');

	//Send the data;
	//uart_putc(cmd);

	//Send inverse of data for error detection purpose

	//uart_putc(~cmd);

	//End the packet by writing 'Z'
	//uart_putc('Z');
}

void send_LED_Update(void)
{
	if(switch_state % 2 == 0 ) //
	{
		//Send_Packet(RADDR, LEDON);
		Send_Packet(LEDON);
	}
	else
	{
		//Send_Packet(RADDR, LEDOFF);
		Send_Packet(LEDOFF);
	}
}

void update_Display(void) //
{
	//commandDisplay(Clear_Screen);
	if(switch_state == 0 ) //
	{
		commandDisplay(Clear_Screen);
		uart_puts("State=0");
		commandDisplay(Line_feed);
		printInt(i);
	}
	if(switch_state == 1)	//Red
	{
		commandDisplay(Clear_Screen);
		uart_puts("State=1");
		commandDisplay(Line_feed);
		printInt(i);
	}
	else if(switch_state == 2)	//Orange
	{
		commandDisplay(Clear_Screen);
		uart_puts_p(DeviceAddress);
		commandDisplay(Line_feed);
		printInt(i);
	}
	else if(switch_state == 3)	//Yellow
	{
		commandDisplay(Clear_Screen);
		uart_puts("State=3");
		commandDisplay(Line_feed);
		printInt(i);
	}
	
	else if(switch_state == 4)	//Green
	{
		commandDisplay(Clear_Screen);
		uart_puts("State=4");
		commandDisplay(Line_feed);
		printInt(i);
	}
	else if(switch_state == 5)	//Blue
	{
		commandDisplay(Clear_Screen);
		uart_puts("State=5");
		commandDisplay(Line_feed);
		printInt(i);
	}
	else if(switch_state == 6)	//Blue
	{
		commandDisplay(Clear_Screen);
		uart_puts("State=6");
		commandDisplay(Line_feed);
		printInt(i);
	}
	else if(switch_state > 6)	//Blue
	{
		commandDisplay(Clear_Screen);
		uart_puts("State=");
		printInt(switch_state);
		commandDisplay(Line_feed);
		printInt(i);
	}
	else if(switch_state == 0)	//Light detected OFF
	{
		commandDisplay(Go_Home);
		uart_puts("State=0");
		commandDisplay(Line_feed);
		printInt(i);
	}
	else
	{
		
		switch_state = 0; // set switch state to 0 if the state is above 9
		
	}
}

int main(void)
{
	// uart_puts("String stored in SRAM\n");
	//uart_puts_P("String stored in FLASH\n");
	// itoa( num, buffer, 10);   // convert interger into string (decimal format)
	//uart_puts(buffer);        // and transmit string to UART
	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	
	timer0_init();

	sei();


	
	commandDisplay(Clear_Screen);
	
	
	set_input(DDRB, Switch1);
	set_input(DDRB, Switch2);
	set_input(DDRB, Switch3);
	
	
	while(1)
	{
		
		
	}
}
