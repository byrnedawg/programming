// adc.c

#include "prj.h"

void adc_init() 
{ 
    // divider = 2  
	// fadc =  f = 4.0000 MHz  
    ADCSRA |= 0xE1; 
} 
 
unsigned int adc_get_value(unsigned char ch) 
{ 
    ADMUX = ch; 
    ADCSRA |= 0xE1; 
	_delay_ms(10); 
	return ( (unsigned int ) ( ADCL + (ADCH<<8) ) ); 
} 

