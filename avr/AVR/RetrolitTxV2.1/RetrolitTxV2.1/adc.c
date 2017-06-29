/*
 * adc.c
 *
 * Created: 6/27/2017 7:38:37 AM
 *  Author: TestDev
 */ 

//#include "prj.h"

#include "portHardware.h"

void adc_init()
{
	// divider = 2
	// fadc =  f = 0.50000 MHz
	//set_input(ADC7_DIR, ADC7_PIN_BIT);

	ADMUX |= (1<<REFS1)|(0<<REFS0)|(1<<ADLAR)|(0<<REFS2)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0); //ADC Multiplexer Selection Register... (ADC3(PB3) Selected ADC Input)
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(0<<ADPS1)|(1<<ADPS0); // ADC Control Register A
	ADCSRB |= (0<<BIN)|(0<<ACME)|(0<<IPR)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); // ADC Control Register B
	DIDR0 |= (0<<ADC0D)|(0<<ADC2D)|(0<<ADC3D)|(0<<ADC1D)|(0<<AIN1D)|(0<<AIN0D); // Digital input Disable Register
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

void Analog_Comparator_init() // Initialize Analog Comparator
{
	ADCSRA |= (0<<ADEN);
	ADCSRB |= (0<<BIN)|(1<<ACME)|(0<<IPR)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); //Analog Comparator Multiplexer Enable
	ADMUX |= (0<<REFS1)|(0<<REFS0)|(0<<ADLAR)|(0<<REFS2)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0); //ADC Multiplexer Selection Register... (ADC3(PB3) Selected ADC Input)
	ACSR |= (0<<ACD)|(1<<ACBG)|(0<<ACO)|(0<<ACI)|(1<<ACIE)|(1<<ACIS1)|(1<<ACIS0); // Analog Comparator Control Register
	DIDR0 |= (0<<ADC0D)|(0<<ADC2D)|(1<<ADC3D)|(0<<ADC1D)|(0<<AIN1D)|(0<<AIN0D); // Digital input Disable Register
}

ISR(ANA_COMP_vect)
{
	//toggleLED(2);
	Led2_off();
}

ISR(ADC_vect) // Interrupt for ADC Converter
{
	
}