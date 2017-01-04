/*
 * RetroliteV20.c
 *
 * Created: 5/21/2015 8:01:03 AM
 *  Author: gregory.byrne
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
#define is_clear(portpin,pin) !(portpin & (1<<pin)) == (1<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)																									


#define Switch1 PB0
#define RelayON PB1
#define RelayOFF PB2


int main(void)
{
	set_input(DDRB, Switch1);
	set_output(DDRB, RelayOFF);
	set_output(DDRB, RelayON);
	
    while(1)
    {
		if(is_set(PINB, Switch1))
		{
			_delay_ms(50);
			if(is_set(PINB, Switch1))
			{
				output_high(PORTB, RelayON);
				output_low(PORTB, RelayOFF);
			}
			else
			{
				output_high(PORTB, RelayOFF);
				output_low(PORTB, RelayON);
			}
		
		}
		else
		{
			output_high(PORTB, RelayOFF);
			output_low(PORTB, RelayON);
		}
		
		_delay_ms(5);
	 }
   
}