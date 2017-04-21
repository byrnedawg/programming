// led.c

#include "prj.h"

void initLEDs(void)
{
	set_output(Led1_DIR, Led1_PIN_BIT);
	set_output(Led2_DIR, Led2_PIN_BIT);
}

void toggleLED(int ledNum)
{
	if(ledNum == 1)
	{
		output_toggle(Led1_DIR, Led1_PIN_BIT);
	}
	else if(ledNum == 2)
	{
		output_toggle(Led2_DIR, Led2_PIN_BIT);
	}
	else
	{
		
	}
}
	