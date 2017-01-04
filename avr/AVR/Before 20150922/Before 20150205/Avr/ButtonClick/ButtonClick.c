#define F_CPU 1000000UL
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

#define RESET &=~
#define SET   |=
#define TOGGLE ^=

#define LEFT 0x00
#define RIGHT 0x01
#define INCREASING 0x00
#define DECREASING 0x01

volatile uint8_t LED_mask = 0x01;			// Right-most LED
volatile uint8_t direction = LEFT;
volatile uint8_t brightness = INCREASING;
volatile uint8_t slider_value = 0x00;		// Off
volatile uint16_t slider_temp;				// Non-linear brightness algorithm
volatile uint8_t pulse_incr = 0x04;			// Slow pulse, range of 0x02 to 0x10
volatile uint8_t switch_temp;				// Temporary storage of switch port data
volatile uint8_t data;

volatile unsigned long i;


ISR(TIMER0_OVF_vect)
{
      PORTC RESET LED_mask;  				// Turn on

}

ISR(TIMER0_COMPA_vect)
{
      PORTC SET LED_mask;   				// Turn off
}

ISR(PCINT2_vect)
{
	switch_temp = PINK;
	if (switch_temp != 0x00){					// Skip if button release
		PORTC = switch_temp;
		//data=PINK;
		if(is_high(PINK,PK0))
		{
			data=0x05 ;
		}
		else
		{
			data=0xFE;
		}
		
	}
	
}

void delayms(uint8_t t)//delay in ms
{
uint8_t i;
for(i=0;i<t;i++)
	_delay_ms(1);
}


void timer0_init(void)
{
// NOTE: Assumes DIV8 fuse set for 1 MHz operation
// 8-bit counter means overflow interrupt every 61 Hz to turn on the LED
// Variable Compare_A interrupt in between OVF to turn off the LED
//	OCR0A = slider_value;
//  	TCCR0A = 0x03; 							// Fast PWM Mode
//	TCCR0B = 0x03;							// Set prescaler to 64
//	TIMSK0 |= (1<<OCIE0A) | (1<<TOIE0);		// Compare A and OVF Interupt enabled 
//	TCNT0  = 0x00;
}

void LED_init(void)
{
	PORTC = 0xFF;							// All PORTC set (LED off)
	DDRC = 0xFF;							// All PORTC output
}

void SW_init(void)
{
	PORTK = 0xFF;							// All PORTK set
	DDRK = 0x00;							// All PORTK input
	PCICR = 1 << PCIE2;						// Enable pin change interrupt 16..23
	PCMSK2 = 0xFF;							// Enable mask for PCINT
}

void power_init(void)
{
//	PRR0 = 0xCF;							// Only Timer0 enabled
	//PRR1 = 0x3F;							// All disabled
//	SMCR = 0x01;							// Enable Idle mode
}

int main(void)
{
	//timer0_init();

	LED_init();
	SW_init();
	power_init();
	//Initialize the USART with Baud rate = 2400bps
	USARTInit(416);
	data = 0xFE;//0b01010101;

	sei(); 

	while(1)
	{


	//	_SLEEP();
		
		
	//data = 'B';
		/* 
		Now send a Packet
		Packet Format is AA<data><data inverse>Z
		total Packet size if 5 bytes.
		*/

		//Stabilize the Tx Module By Sending JUNK data
	//	UWriteData('J');	//J for junk

		//Send 'A'
	//	UWriteData('A');

		//Send Another 'A'
	//	UWriteData('A');

		//Send the data;
		UWriteData(data);

		//Send inverse of data for error detection purpose
	//	UWriteData(~data);

		//End the packet by writing 'Z'
	//	UWriteData('Z');

		//Wait for some time
		delayms(20);
	//	_delay_loop_2(0);
	//	_delay_loop_2(0);
	//	_delay_loop_2(0);
	}
    
	return 0;
}
