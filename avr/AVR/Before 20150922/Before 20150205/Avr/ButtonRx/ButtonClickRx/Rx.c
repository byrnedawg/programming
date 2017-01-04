#define F_CPU 4000000UL
#include <avr/io.h>
#include <stdint.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <compat/ina90.h>

#include "usart.h"

#define sbi(x,y) x |= _BV(y) //set bit
#define cbi(x,y) x &= ~(_BV(y)) //clear bit
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) (x & _BV(y)) == _BV(y) //(for input) checks if the 							  // input is high (Logic 1)
							  // if(is_high(PIND,PD0))

void delayms(uint8_t t)//delay in ms 
{
uint8_t i;
for(i=0;i<t;i++)
	_delay_ms(1);
}

void main()
{

//	uint8_t i; //Clasical loop varriable
	
//	uint8_t packet[5];
	
	uint8_t data=0;
	
	DDRC|=0xFF;	//All Output

	cbi(PORTC,PC0);
	cbi(PORTC,PC1);
	cbi(PORTC,PC2);

	//Initialize the USART with Baud rate = 2400bps
	USARTInit(416);

	/*
	Get data from the remote Tx Station
	The data is the value of PORTC on Remote Tx Board
	So we will copy it to the PORTC of this board.
	*/

	while(1)
	{

		if(UDataAvailable())
		{			 
			data= UReadData();
			if(data == 0x05)
			{
				sbi(PORTC,PC0); //LED OFF
				cbi(PORTC,PC1);
				tbi(PORTC,PC2);
			}
			if(data == 0xFE)
			{
				sbi(PORTC,PC1);
				cbi(PORTC,PC0);
				tbi(PORTC,PC2);
			}
			else
			{
				tbi(PORTC,PC0);
				tbi(PORTC,PC1);
				cbi(PORTC,PC2);
			}
		}
		
		
		
		/*
		//Wait for a packet
		while(!UDataAvailable());
		if(UReadData()!='A') continue;
		while(!UDataAvailable());
		if(UReadData()!='A') continue;
		
		while(UDataAvailable()!=3);

		//Get the packet
		for(i=2;i<5;i++)
		{
			packet[i]=UReadData();
		}

		//Is it ok?
		if(packet[2]!=((uint8_t)~packet[3])) continue;

		if(packet[4]!='Z') continue;

		//The packet is ok
		data=packet[2];

		//Now we have data put it to PORTC
		PORTC=data;
		*/
	}
		

}
