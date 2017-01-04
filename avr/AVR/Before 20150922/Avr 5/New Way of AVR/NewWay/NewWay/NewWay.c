/*
 * NewWay.c
 *
 * Created: 9/2/2014 3:10:23 PM
 *  Author: gregory.byrne
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define LED PB1
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)

#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

/*
void setPinHigh(volatile int *port, int pin)
{
	*port |= (1<<pin);
}

void setPinLow(volatile int *port, int pin)
{
	*port &= ~(1<<pin);
}
*/
int main(void)
{
	//DDRB = 0xff;
	UCSRC=(1<<URSEL)|(3<<UCSZ0);
	set_output(DDRB, LED);
    while(1)
    {
        //PORTB |= 0x02;
		output_high(PORTB, LED);
		_delay_ms(2000);
		
		//PORTB &= ~0x02;
		output_low(PORTB, LED);
		//setPinLow(&PORTB, 1);
		_delay_ms(2000);
    }
}