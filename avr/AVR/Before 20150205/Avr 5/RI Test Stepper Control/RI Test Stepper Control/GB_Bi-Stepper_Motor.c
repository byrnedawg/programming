

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

uint8_t stepper_state = 0;

void rotate_RI_Counter_CW(int16_t steps)
{
	
	while(steps != 0)
	{
		if(steps > 0)
		{	
			switch (stepper_state)
			{
				case 0:
				PORTB = (1<<PB2) | (1<<PB1) | (1<<PB0); 
				PORTD = (0<<PD7) | (0<<PD6) | (0<<PD5);
				_delay_ms(15);       // wait
				steps--;
				stepper_state++;
				break;
				
				case 1:
				PORTB = (1<<PB2) | (1<<PB1) | (0<<PB0);// 6
				PORTD = (0<<PD7) | (1<<PD6) | (0<<PD5); //64
				_delay_ms(15);       //Wait
				steps--;
				stepper_state++;
				break;
		
				case 2:
				PORTB = (1<<PB2) | (1<<PB1) | (0<<PB0); //6
				PORTD = (1<<PD7) | (0<<PD6) | (0<<PD5); //128
				_delay_ms(15);       //Wait
				steps--;
				stepper_state++;
				break;
				
				case 3:
		
				PORTB = (1<<PB2) | (1<<PB1) | (0<<PB0); 
				PORTD = (0<<PD7) | (0<<PD6) | (1<<PD5);
				_delay_ms(15);       //Wait
				steps--;
				stepper_state = 0;
				break;
		}			
		
	}
	
}		

void rotate_RI_CW(uint16_t steps)
{
	/*
	while(steps > 0)
	{
		if(steps > 0 && stepper_state == 0)
		{	
			PORTB = (1<<PB2) | (1<<PB1) | (0<<PB0); 
			PORTD = (0<<PD7) | (0<<PD6) | (1<<PD5);
			_delay_ms(15);       //Wait
			steps--;
			stepper_state++;
		}			
		
		if(steps > 0 && stepper_state == 1)
		{	
			PORTB = (1<<PB2) | (1<<PB1) | (0<<PB0); 
			PORTD = (1<<PD7) | (0<<PD6) | (0<<PD5);
			_delay_ms(15);       //Wait
			steps--;
			stepper_state++;
		}			
		
		if(steps > 0 && stepper_state == 2)
		{	
			PORTB = (1<<PB2) | (1<<PB1) | (0<<PB0); 
			PORTD = (0<<PD7) | (1<<PD6) | (0<<PD5);
			_delay_ms(15);       //Wait
			steps--;
			stepper_state++;
		}			
		
		if(steps > 0 && stepper_state == 3)
		{	
			PORTB = (1<<PB2) | (1<<PB1) | (1<<PB0); 
			PORTD = (0<<PD7) | (0<<PD6) | (0<<PD5);
			_delay_ms(15);       // wait
			steps--;
			stepper_state = 0;
		}	
				
	}
	*/
}			