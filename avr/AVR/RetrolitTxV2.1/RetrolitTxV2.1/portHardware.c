/*
 * portHardware.c
 *
 * Created: 6/27/2017 8:55:34 AM
 *  Author: TestDev
 */ 

//#include "prj.h"

#include "portHardware.h"

void initLEDs(void)
{
	set_output(Led1_DIR, Led1_PIN_BIT);
	//set_output(Led2_DIR, Led2_PIN_BIT);
}

void initDigitalOutput(void)
{
	set_output(Digital_Out1_DIR, Digital_Out1_PIN_BIT);
	set_output(Digital_Out2_DIR, Digital_Out2_PIN_BIT);
}

void initButtons(void)
{
	set_input(Button1_DIR, Button1_PIN_BIT);
	GIMSK |= (0<<INT0)|(1<<PCIE);
	PCMSK |= (0<<PCINT5)|(0<<PCINT4)|(0<<PCINT3)|(0<<PCINT2)|(0<<PCINT1)|(1<<PCINT0);
}

void toggleLED(int ledNum)
{
	if(ledNum == 1)
	{
		output_toggle(Led1_DIR, Led1_PIN_BIT);
	}
	else if(ledNum == 2)
	{
		//output_toggle(Led2_DIR, Led2_PIN_BIT);
	}
	else
	{
		
	}
}

unsigned int buttonIsPressed(int button)
{
	if(button == 1)
	{
		if(Button1_press())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

ISR (PCINT0_vect)
{
	if(buttonIsPressed(1))
	{
		Led1_on();
		Digital_Out1_Low();
		output_toggle(Digital_Out2_DIR, Digital_Out2_PIN_BIT);
	}
	else
	{
		Led1_off();
		Digital_Out1_High();
	}
}
