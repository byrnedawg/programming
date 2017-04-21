
#include "prj.h"


/******************************************************************
  Project     : 644_testV1
  Description : jett
  MCU         : MEGA644
  F_OSC       : 8.0000 MHz
  Library     : WinAVR-20100110
  Date        : 2017-03-29
  Version     : 
  History     : 

  Note
  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of  
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
******************************************************************/

// global variable



void init()
{
	initLEDs();
	toggleLED(2);
	initButtons();
	// Relay1 init
	//Relay1_DIR |= _BV(Relay1_BIT) ;
	//Relay1_off();
	//interrupt0_init();
	//interrupt1_init();
	Timer0_Init();
	Timer1_Init();
	Timer2_Init();
	//uart_init();
	//adc_init();

	// watchdog enable
	//wdt_enable(WDTO_15MS);
	

}

int main()
{
	// define variables

	init();
	sei();  // enable interrup
	while(1)
	{
		
		
		//wdt_reset();
	}
}