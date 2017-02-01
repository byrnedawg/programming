/*
 * atmega168Blink.c
 *
 * Created: 1/29/2017 10:24:06 PM
 * Author : Gregory Byrne
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "global.h"
#include "a2d.h"



#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);

#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);

#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)


#define Switch1 PB1
#define GreenLED PB2
unsigned int State = 0;

int main(void)
{
	DDRC = 0x00;
	// make sure pull-up resistors are turned off
	PORTC = 0x00;
	set_input(DDRB, Switch1);
	set_output(DDRB, GreenLED);
	a2dInit();
	a2dSetPrescaler(ADC_PRESCALE_DIV32);

	// set the a2d reference
	// - the reference is the voltage against which a2d measurements are made
	// - other allowed reference values can be found in a2d.h
	a2dSetReference(ADC_REFERENCE_AVCC);
	
    while(1)
    {
		if(is_clear(PINB, Switch1))
		{
			output_high(PORTB, GreenLED);	
		}
		else
		{
			output_low(PORTB, GreenLED);
			a2dConvert8bit(5);
		}
	
		
		_delay_ms(200);
	 }
   
}

