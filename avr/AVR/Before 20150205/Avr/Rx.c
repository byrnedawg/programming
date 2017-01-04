#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

#define BAUDRATE 1200
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)
#define LEDON 0x11//switch led on command
#define LEDOFF 0x22//switch led off command

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

	uint8_t i; //Clasical loop varriable
	
	uint8_t packet[5],data=0;

	//Initialize the USART with Baud rate = 2400bps
	USARTInit(UBRRVAL);   //USARTInit(416);

	//Enable Internal Pullups on PORTC
	DDRD = (1<<PIND7);
	cbi(PORTD,PD7); //LED OFF;
	DDRB = (1<<PINB0);
	
	//DDRC|=0xFF;	//All Output

	//Initialize the USART with Baud rate = 2400bps
//	USARTInit(416);

	/*
	Get data from the remote Tx Station
	The data is the value of PORTC on Remote Tx Board
	So we will copy it to the PORTC of this board.
	*/

	while(1)
	{
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
		//PORTC=data;
        if(data == LEDON)
		{
			//Send_Packet(RADDR, LEDON);
			sbi(PORTD,PD7); //LED ON;
			cbi(PORTB,PB0); //LED OFF;
			delayms(100);
		}
		else
		{
			//send command to switch led OFF
			//Send_Packet(RADDR, LEDOFF);
			cbi(PORTD,PD7); //LED OFF;
			sbi(PORTB,PB0); //LED ON;
			delayms(100);
		}

	}
		

}
