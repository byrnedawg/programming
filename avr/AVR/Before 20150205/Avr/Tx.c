#define F_CPU 8000000UL
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
	//Initialize the USART with Baud rate = 2400bps
	USARTInit(UBRRVAL);   //USARTInit(416);

	//Enable Internal Pullups on PORTC
	DDRD = (1<<PIND7);
	cbi(PORTD,PD7); //LED OFF;
	DDRB = (0<<PINB0);

	/* 
	Keep transmitting the Value of Local PORTC
	to the Remote Station.

	On Remote RX station the Value of PORTC
	sent on AIR will be latched on its local PORTC
	*/
	
	uint8_t data;
	while(1)
	{
		//data=PINC;

		if(bit_is_clear(PINB,PB0))
		{
			//Send_Packet(RADDR, LEDON);
			sbi(PORTD,PD7); //LED ON;
			data = LEDON;
			delayms(100);
		}
		else
		{
			//send command to switch led OFF
			//Send_Packet(RADDR, LEDOFF);
			cbi(PORTD,PD7); //LED OFF;
			data = LEDOFF;
			delayms(100);
		}

		/* 
		Now send a Packet
		Packet Format is AA<data><data inverse>Z
		total Packet size if 5 bytes.
		*/

		//Stabilize the Tx Module By Sending JUNK data
		UWriteData('J');	//J for junk

		//Send 'A'
		UWriteData('A');

		//Send Another 'A'
		UWriteData('A');

		//Send the data;
		UWriteData(data);

		//Send inverse of data for error detection purpose
		UWriteData(~data);

		//End the packet by writing 'Z'
		UWriteData('Z');

		//Wait for some time
		delayms(100);
	//	_delay_loop_2(0);
	//	_delay_loop_2(0);
	//	_delay_loop_2(0);
	//	_delay_loop_2(0);
	}
}
