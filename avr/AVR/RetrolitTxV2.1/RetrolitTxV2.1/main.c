/*
 * RetrolitTxV2.1.c
 *
 * Created: 6/27/2017 7:14:24 AM
 * Author : TestDev
 */ 

//#include "prj.h"
#include "portHardware.h"

//												   ______
//	(PCINT5/~RESET/ADC0/dW)			Reset	--PB5 |		 |VCC--	5V
//	(PCINT3/XTAL1/CLKI/~OC1B/ADC3)	ADC		--PB3 |		 |PB2--	YLW LED	(SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
//	(PCINT4/XTAL2/CLKO/OC1B/ADC2)	N/C 	--PB4 |		 |PB1--	GRN LED	(MISO/DO/AIN1/OC0B/OC1A/PCINT1)
//											--GND |______|PB0--	Switch	(MOSI/DI/SDA/AIN0/OC0A/~OC1A/AREF/PCINT0)
//

void init()
{
	initLEDs();
	initDigitalOutput();
	initButtons();
	Led1_on();
	Digital_Out1_Low();
	Digital_Out2_Low();
	 _delay_ms(1500);
	Led1_off();
	Digital_Out1_High();
	Digital_Out2_Low();
	Analog_Comparator_init();
	 
	//Timer0_Init();
	//Analog_Comparator_init();
	//_delay_ms((unsigned char)500);
	//Timer1_Init();
	//adc_init();
	// watchdog enable
	//wdt_enable(WDTO_15MS);
}
int main()
{
	init();
	sei();  // enable interrupt
	while(1)
	{
		
		//wdt_reset();
	}
	return 0;
}
