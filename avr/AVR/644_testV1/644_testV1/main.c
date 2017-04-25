
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

//demonstration of progress bar
void progress(void)
{
	Lcm1_SetCursor(4,0);
	Lcm1_ShowString("DRI V1.1");
	_delay_ms(1000);
	for(uint8_t i=0;i<255;i++)
	{
		_delay_ms(100);
		Lcm1_SetCursor(4,1);
		LCDprogressBar(i, 255, 8);
	}
	Lcm1_Clearscreen();
}

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
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	adc_init();
	Lcm1_Init();
	uart0_puts("Hardware Initialized\r\n");
	Lcm1_Clearscreen();
	progress();
	_delay_ms(500);
	Lcm1_SetCursor(0,0);
	Lcm1_ShowString("Hardware");
	Lcm1_SetCursor(0,1);
	Lcm1_ShowString("Initialized");
	
	

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