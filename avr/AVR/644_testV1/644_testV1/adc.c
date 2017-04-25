// adc.c

#include "prj.h"

void adc_init() 
{ 
    // divider = 2  
	// fadc =  f = 4.0000 MHz  
	//set_input(ADC7_DIR, ADC7_PIN_BIT);
	
	ADMUX |= (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0); // 5V 
    //ADCSRA |= 0xE1; 
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	ADCSRB |= (0<<ACME)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);
	
	DIDR0 |= (0<<ADC7D)|(0<<ADC6D)|(0<<ADC5D)|(0<<ADC4D)|(0<<ADC3D)|(0<<ADC2D)|(0<<ADC1D)|(0<<ADC0D);
} 
 
unsigned int adc_get_value(unsigned char ch) 
{ 
    
	ADMUX = (ADMUX & ~0x07) | ch; //clear mux then or with new channel value
    ADCSRA |= (1<<ADSC); 
	_delay_ms(10); 
	return ( (unsigned int ) ( ADCL + (ADCH<<8) ) ); 
} 

unsigned int adc_get_value_ref(unsigned char ref, unsigned char ch)
{
	ref = ref << 6;
	ADMUX = (ADMUX & ~0xC7) | ref;
	ADMUX |= ch; //clear mux then or with new channel value
	ADCSRA |= (1<<ADSC);
	_delay_ms(10);
	return ( (unsigned int ) ( ADCL + (ADCH<<8) ) );
}

