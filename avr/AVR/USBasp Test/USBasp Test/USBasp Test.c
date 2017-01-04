/*
 * USBasp_Test.c
 *
 * Created: 11/13/2015 5:45:11 PM
 *  Author: Gregory Byrne
 */ 


#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);

#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);

#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)


#define Switch1 PB3
#define Switch2 PB4
#define GreenLED PB0
#define YellowLED PB1
#define RedLED PB2



int main(void)
{
	set_input(DDRB, Switch1);
	set_input(DDRB, Switch2);
	set_output(DDRB, GreenLED);
	set_output(DDRB, YellowLED);
	set_output(DDRB, RedLED);
	
	while(1)
	{
		if(is_set(PINB, Switch1) && is_set(PINB, Switch2))
		{
			output_low(PORTB, GreenLED);
			output_high(PORTB, RedLED);
			output_high(PORTB, YellowLED);
		}
		
		else if(is_clear(PINB, Switch1) && is_clear(PINB, Switch2))
		{
			output_low(PORTB, RedLED);
			output_high(PORTB, YellowLED);
			output_high(PORTB, GreenLED);
		}
		else
		{
			output_low(PORTB, YellowLED);
			output_high(PORTB, RedLED);
			output_high(PORTB, GreenLED);
		}
		
		_delay_ms(100);
	}
}