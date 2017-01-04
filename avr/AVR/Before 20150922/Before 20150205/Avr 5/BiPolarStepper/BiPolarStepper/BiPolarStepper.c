/*
 * BiPolarStepper.c
 *
 * Created: 3/21/2014 1:18:21 PM
 *  Author: gregory.byrne
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/eeprom.h> 

#define sbi(x,y) x |= _BV(y) //set bit (original sbi)
#define cbi(x,y) x &= ~(_BV(y)) //(original)cbi clear bit //reverse cause of 3904
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) (x & _BV(y)) == _BV(y) //(for input) checks if the // input is high (Logic 1) i.e. if(is_high(PIND,PD0))						
#define RESET &=~
#define SET |=
#define TOGGLE ^=

uint8_t stepper_state = 0;

void rotate_RI_Counter_CW(uint16_t steps)
{
	while(steps > 0)
	{
		if(steps > 0 && stepper_state == 0)
		{	
			PORTB = (1<<PB2) | (1<<PB1) | (1<<PB0); 
			PORTD = (0<<PD7) | (0<<PD6) | (0<<PD5);
			_delay_ms(15);       // wait
			steps--;
			stepper_state++;
		}

		if(steps > 0 && stepper_state == 1)
		{
			PORTB = (1<<PB2) | (1<<PB1) | (0<<PB0);// 6
			PORTD = (0<<PD7) | (1<<PD6) | (0<<PD5); //64
			_delay_ms(15);       //Wait
			steps--;
			stepper_state++;
		}
				
		if(steps > 0 && stepper_state == 2)
		{
			PORTB = (1<<PB2) | (1<<PB1) | (0<<PB0); //6
			PORTD = (1<<PD7) | (0<<PD6) | (0<<PD5); //128
			_delay_ms(15);       //Wait
			steps--;
			stepper_state++;
		}	
			
		if(steps > 0 && stepper_state == 3)
		{
			PORTB = (1<<PB2) | (1<<PB1) | (0<<PB0); 
			PORTD = (0<<PD7) | (0<<PD6) | (1<<PD5);
			_delay_ms(15);       //Wait
			steps--;
			stepper_state = 0;
		}			
		
	}
}		



void rotate_RI_CW(uint16_t steps)
{
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
}			

int main(void)
{
	//int i;
	//DDRC =0xFF; //PA5=ENB ,PA4=ENA ,PA3=IN4,PA2=IN3,PA1=IN2,PA0=IN1
	//DDRB = 0xFF;
	DDRB = (0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);// set PB1  PB2 Enable pins on L298n PB0 motor drive  
	PORTB = (0<<PB7) | (0<<PB6) | (0<<PB5) | (0<<PB4) | (0<<PB3) | (0<<PB2) | (0<<PB1) | (0<<PB0); // set all pins we are using high
	
	DDRD = 0xFF;
	DDRD = (1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);// set PD7 - PD5 motor drive  
	PORTD = (0<<PD7) | (0<<PD6) | (0<<PD5) | (0<<PD4) | (0<<PD3) | (0<<PD2) | (0<<PD1) | (0<<PD0); // set all pins we are using high
	
	while (1) //loop forever
	{
		for(int i=0; i<5; i++)
		{
			rotate_RI_Counter_CW(50);
		
			_delay_ms(1000);
		}
		for(int i=0; i<5; i++)
		{
			rotate_RI_CW(50);
			
			_delay_ms(1000);
		}
		
		rotate_RI_Counter_CW(150);
		
		rotate_RI_CW(200);
	}
}