/*
 * toggle_led.c
 *
 * Created: 8/25/2014 3:03:01 PM
 *  Author: gregory.byrne
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
int main(void)
{
	 DDRB |= (1 << 0); // Set LED as output 
    while(1)
    {
        PORTB ^= (1 << 0); // Toggle the LED 
		_delay_ms(1000);
    }
}