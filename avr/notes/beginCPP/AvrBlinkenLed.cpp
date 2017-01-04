/*
 * AvrBlinkenLed.cpp
 *
 * Created: 1/15/2012 4:21:24 PM
 *  Author: Peter.Brown
 */ 
#define F_CPU 8000000

#include "ATmega1284P.h"
#include "IOPort.h"
#include "Led.h"

int main(void)
{
	//DDRA = 0b11111111;
	
	IOPort portA(&PORTA, &PINA, &DDRA);
	
	portA.SetPortDirection(0xFF);
	//portA.SetPortValues(0b10101111);
	
	Led ledA0(&portA, (uint8_t)0, LedCurrentSource);
	Led ledA1(&portA, (uint8_t)1, LedCurrentSource);
	Led ledA2(&portA, (uint8_t)2, LedCurrentSource);
	Led ledA3(&portA, (uint8_t)3, LedCurrentSource);
	Led ledA4(&portA, (uint8_t)4, LedCurrentSource);
	Led ledA5(&portA, (uint8_t)5, LedCurrentSource);
	Led ledA6(&portA, (uint8_t)6, LedCurrentSource);
	Led ledA7(&portA, (uint8_t)7, LedCurrentSource);
	
	ledA0.TurnOn();
	ledA1.TurnOn();
	ledA2.TurnOn();
	ledA3.TurnOn();
	ledA4.TurnOn();
	ledA5.TurnOn();
	ledA6.TurnOn();
	ledA7.TurnOn();

    while(1)
    {
        //TODO:: Please write your application code 
		
    }
}