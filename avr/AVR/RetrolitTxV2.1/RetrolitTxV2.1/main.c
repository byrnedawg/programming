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
	initButtons();
	Led2_on();
	//interrupt0_init();
	//interrupt1_init();
	Timer0_Init();
	//Timer1_Init();
	//adc_init();
	Analog_Comparator_init();
	
	for(int i = 0; i < 4; i++)
	{
		toggleLED(1);
		_delay_ms(250);
	}
	
	
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
