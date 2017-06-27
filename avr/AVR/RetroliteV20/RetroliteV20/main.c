/*
 * RetroliteV20.c
 *
 * Created: 5/21/2015 8:01:03 AM
 *  Author: gregory.byrne
 */ 

#include "prj.h"
//												   ______
//	(PCINT5/~RESET/ADC0/dW)			Reset	--PB5 |		 |VCC--	5V
//	(PCINT3/XTAL1/CLKI/~OC1B/ADC3)	LDR		--PB3 |		 |PB2--	RelayOFF (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
//	(PCINT4/XTAL2/CLKO/OC1B/ADC2)	BLU LED	--PB4 |		 |PB1--	RelayON	(MISO/DO/AIN1/OC0B/OC1A/PCINT1)
//											--GND |______|PB0--	Switch	(MOSI/DI/SDA/AIN0/OC0A/~OC1A/AREF/PCINT0)
//

#define Switch1 PB0
#define RelayStatus PB1
#define TransmissionEn PB2
#define ReadADC2 PB4
																									
void init()
{
	Timer0_Init();
//	adc_init();
}

void PortInit(void) // initialize port B inputs and outputs
{
	set_input(DDRB, Switch1);
	set_input(DDRB, ReadADC2);
	set_output(DDRB, RelayStatus);
	set_output(DDRB, TransmissionEn);
}

int main(void)
{
	init();
	PortInit(); // inputs and outputs initialize
	output_low(PORTB, TransmissionEn);
	output_high(PORTB, RelayStatus);
	_delay_ms(2000);
	output_high(PORTB, TransmissionEn);
    while(1)
    {
		if(is_clear(PINB, Switch1))
		{
			_delay_ms(250);
			if(is_set(PINB, Switch1))
			{
				output_low(PORTB, TransmissionEn);
				output_toggle(PORTB, RelayStatus);
				_delay_ms(1000);
			}
			else
			{
				output_high(PORTB, TransmissionEn);
			}
		
		}
		else
		{
			output_high(PORTB, TransmissionEn);
		}
		
		_delay_ms(5);
	 }
   
   return 0;
}