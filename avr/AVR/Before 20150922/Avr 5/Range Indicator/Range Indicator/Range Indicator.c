/*
 * Range_Indicator.c
 *
 * Created: 1/10/2012 11:04:33 AM
 *  Author: gregory.byrne
 */ 

#define F_CPU 128000//1000000UL
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/sleep.h>
#include <util/atomic.h> 
#include <util/delay.h> 
#include <stdbool.h>

#define sbi(x,y) x |= _BV(y) //set bit (original sbi)
#define cbi(x,y) x &= ~(_BV(y)) //(original)cbi clear bit //reverse cause of 3904
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) (x & _BV(y)) == _BV(y) //(for input) checks if the 							  // input is high (Logic 1)
							  // if(is_high(PIND,PD0))

#define RESET &=~
#define SET   |=
#define TOGGLE ^=

#define PWM_200Hz 0x05
#define PWM_30Hz 0x20

volatile uint8_t switch_state;				// Temporary storage of switch port data
volatile uint8_t Brightness;
volatile uint16_t timer0Overflows;
volatile uint16_t timer1Overflows;
volatile uint16_t Battery_Voltage;
volatile bool Low_Battery;
volatile bool BTN_Down;

//enum months {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

ISR(ANA_COMP_vect)
{
      
}

ISR(ADC_vect)
{
	uint8_t temp;
	temp = ADCH ;
	Battery_Voltage = (93 * 255 / temp); // 56/ 1.1v = 255/5v ........ 5V = 255 1v = 51 1.1v = 56 2.6V = 132 // 255/3 ... 85/1v ... 93/1.1v 
	
	if(Battery_Voltage < 229) //132/2.6V Low Battery Voltage divider R1 324Ohms R2 200 229/2.7 @ 3v
 	{
		Low_Battery = true;
	//	PORTB = (0<<PB0);
	}
	else
	{
		Low_Battery = false;
	//	PORTB = (1<<PB0);
	}
}

ISR(TIMER0_OVF_vect)
{
    timer0Overflows++;

	if((timer0Overflows % 5) == 0) //15 4 seconds
	{
		if(OCR1C == PWM_30Hz)
		{
			OCR1C = PWM_200Hz; 
		}
		
		if(switch_state == 1)
			{
				sbi(PORTB,PB3);
				OCR1A = OCR1C*1/4;
			}
			else if(switch_state == 2)
			{
				OCR1C = PWM_200Hz;
				OCR1A = OCR1C*1/2;
		
			}
			else if(switch_state == 3)
			{
				OCR1A = OCR1C*3/4;
			
			}
			else if(switch_state == 4)
			{
				OCR1A = OCR1C;
		
			}
			else
			{
				switch_state = 0;
				cbi(PORTB,PB3);
				//PORTB = (0<<PB3);
				OCR1A = 0x00;
			
			}
			
	}
	
	if((timer0Overflows % 225) == 0)
	{
		ADCSRA|=(1<<ADSC);
		if(Low_Battery)
		{
			OCR1C = PWM_30Hz; 
		}
		else
		{
			OCR1C = PWM_200Hz;
		}
	}

	if(timer0Overflows > 2250) // 225 1 min // 1125 5 min // 2250 10min // 15 4seconds //
	{
		switch_state = 0;
		cbi(PORTB,PB3);
		//PORTB = (0<<PB3);
		OCR1A = 0x00;
		/*
		DIDR0 |= (1<<ADC0D)|(1<<ADC2D)|(1<<ADC3D)|(1<<ADC1D)|(1<<AIN1D)|(1<<AIN0D); // Digital input Disable Register
		PRR |= (1<<PRTIM1)|(1<<PRTIM0)|(0<<PRUSI)|(1<<PRADC);
		*/
		//OCR1B = 0x00;
	}
}

ISR(TIMER0_COMPA_vect)
{
    
}

ISR(TIMER0_COMPB_vect)
{
     
}

ISR(TIMER1_OVF_vect)
{
	timer1Overflows++;
	 
	if((timer1Overflows % 20) == 0) // 195 1 sec /975 5 sec/ 11700 1 min/
	{
		if(bit_is_clear(PINB, 4))
		{
			BTN_Down = true;
		}
    }
		
		
	if((timer1Overflows % 25) == 0)
	{
		if(bit_is_set(PINB, 4) && BTN_Down)
		{
			sbi(PORTB,PB3);
			switch_state++;
			BTN_Down = false;

			if(switch_state == 1)
			{
				
				ADCSRA|=(1<<ADSC);
				_delay_ms(2);
				sbi(PORTB,PB3);
				if(Low_Battery)
				{
					OCR1C = PWM_30Hz; 
				}
				else
				{
					OCR1C = PWM_200Hz;
				}
			
				OCR1A = OCR1C*1/4;
		
			}
			else if(switch_state == 2)
			{
				OCR1C = PWM_200Hz;
				OCR1A = OCR1C*1/2;
		
			}
			else if(switch_state == 3)
			{
				OCR1A = OCR1C*3/4;
			
			}
			else if(switch_state == 4)
			{
				OCR1A = OCR1C;
		
			}
			else
			{
				switch_state = 0;
				cbi(PORTB,PB3);
				//PORTB = (0<<PB3);
				OCR1A = 0x00;
			}
		}	
	}
	
}

ISR(TIMER1_COMPA_vect)
{
   // OCR1A = Brightness;
	//OCR1B = Brightness;	
}

ISR(TIMER1_COMPB_vect)
{
    //OCR1A = Brightness;
	//OCR1B = Brightness;	
}

ISR(PCINT0_vect)        // interrupt service routine 
{              // called when PCINT0 changes state 
  	// PORTB = (PORTB ^ 0x16);   // toggle red led, portb bit 5, pin 3 
	//switch_state = PINB;
	//if (switch_state != 0x00){					// Skip if button release
		//data=PINK;
		sleep_disable();
		/*
		if(switch_state == 0)
		{
			PRR |= (0<<PRTIM1)|(0<<PRTIM0)|(0<<PRUSI)|(0<<PRADC);
			timer0_init();
   			timer1_init();
   			ADC_init();
			SystemInit();
		}
		*/
		TCNT0  = 0x00;	
		TCNT1  = 0x00;
		timer0Overflows = 0;
	//	timer1Overflows = 0;
} 

void SystemInit(void) 
{ 
	PORTB = (1<<PB4)|(1<<PB3)|(1<<PB1)|(1<<PB0);
   	DDRB = (0<<DDB4)|(1<<DDB3)|(1<<DDB1)|(1<<DDB0);
	PORTB = (1<<PB3)|(0<<PB1)|(0<<PB0);
	Low_Battery = false;
	OCR1A = OCR1C*1/4;
	switch_state = 0;
	BTN_Down = false;
   	PCMSK |= (1<<PCINT4);   // pin change mask: listen to portb bit 2 
   	GIMSK |= (1<<PCIE);   // enable PCINT interrupt 
	MCUCR |= (1<<BODS)|(0<<PUD)|(1<<SE)|(0<<SM1)|(1<<SM0)|(0<<BODSE)|(1<<ISC01)|(0<<ISC00);
	PRR |= (0<<PRTIM1)|(0<<PRTIM0)|(1<<PRUSI)|(0<<PRADC); //Power Reduction Register
   	sei();         // enable all interrupts 
	timer0Overflows = 0;
	timer1Overflows = 0;
	_delay_ms(2);
	ADCSRA|=(1<<ADSC);
	_delay_ms(2);
	if(Low_Battery)
	{
		OCR1C = PWM_30Hz; 
	}
	else
	{
		OCR1C = PWM_200Hz;
	}

} 

void timer0_init(void)
{
// NOTE: Assumes DIV8 fuse set for 1 MHz operation
// 8-bit counter means overflow interrupt every 61 Hz to turn on the LED
// Variable Compare_A interrupt in between OVF to turn off the LED
  	TCCR0A |= (0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(0<<WGM01)|(0<<WGM00); // Fast PWM Mode
	TCCR0B |= (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(1<<CS02)|(0<<CS01)|(1<<CS00);  //0x03;	
	//OCR0A = Brightness;
	TCNT0  = 0x00;	
	timer0Overflows = 0;
}

void timer1_init(void)
{	
	TCCR1 |= (0<<CTC1)|(1<<PWM1A)|(1<<COM1A1)|(0<<COM1A0)|(1<<CS13)|(0<<CS12)|(0<<CS11)|(0<<CS10); // 1<<COM1A0 cause not oca1
	GTCCR |= (0<<TSM)|(0<<PWM1B)|(0<<COM1B1)|(0<<COM1B0)|(0<<FOC1B)|(0<<FOC1A)|(0<<PSR1)|(0<<PSR0); //1<<COM1B0 cause not ocab1
	OCR1A = 0x00;
	//OCR1B = 0x00;	
	OCR1C = PWM_200Hz;	//(1Mhz/prescaler)/(OCR1C+1)= PWM Frequency 	1M/128/39+1 = 200Hz	0x27 	1M/128/252+1 = 33Hz	0xFC @ 128000/128/5+1 = 200Hz 0x5 128K/128/+1 = 33Hz	0x20
	TIMSK |= (1<<OCIE1A)|(1<<OCIE1B)|(0<<OCIE0A)|(0<<OCIE0B)|(1<<TOIE1)|(1<<TOIE0);		// Compare A and OVF Interrupt enabled 
	TCNT1  = 0x00;
//	timer1Overflows = 0;

}

void Analog_Comparator_init(void)
{
	ADCSRB |= (0<<BIN)|(0<<ACME)|(0<<IPR)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); //Analog Comparator Multiplexer Enable
	ACSR |= (0<<ACD)|(0<<ACBG)|(0<<ACO)|(0<<ACI)|(0<<ACIE)|(0<<ACIS1)|(0<<ACIS0); // Analog Comparator Control Register
	DIDR0 |= (0<<ADC0D)|(0<<ADC2D)|(0<<ADC3D)|(0<<ADC1D)|(0<<AIN1D)|(0<<AIN0D); // Digital input Disable Register
}

void ADC_init(void)
{
	ADMUX |= (0<<REFS1)|(0<<REFS0)|(1<<ADLAR)|(0<<REFS2)|(1<<MUX3)|(1<<MUX2)|(0<<MUX1)|(0<<MUX0); //ADC Multiplexer Selection Register
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIF)|(1<<ADIE)|(0<<ADPS2)|(0<<ADPS1)|(0<<ADPS0); // ADC Control Register A
	ADCSRB |= (0<<BIN)|(0<<ACME)|(0<<IPR)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); // ADC Control Register B
	DIDR0 |= (0<<ADC0D)|(0<<ADC2D)|(0<<ADC3D)|(0<<ADC1D)|(0<<AIN1D)|(0<<AIN0D); // Digital input Disable Register
}

int main(void) 
{ 
     //DDRB = 0b00011001;     // portb bits 0,3,4 output; 1,2 in 
   	timer0_init();
   	timer1_init();
   	ADC_init();
	SystemInit();
   	while(1) 
   	{ 
      	sleep_enable();
   	} 
} 
