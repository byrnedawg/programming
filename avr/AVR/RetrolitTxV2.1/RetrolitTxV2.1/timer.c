/*
 * timer.c
 *
 * Created: 6/27/2017 7:30:23 AM
 *  Author: TestDev
 */ 

// timer.c

//#include "prj.h"

#include "portHardware.h"

volatile unsigned int repeat_cnt0 = 0;
volatile unsigned int repeat_cnt1 = 0;
volatile unsigned int msCountedTimer0 = 0;
volatile unsigned int msCountedTimer1 = 0;
volatile unsigned int toggleMsTime = 500;
volatile unsigned int adcValue = 0;


// Timer 0  timeout= 1.000 ms, fosc = 8.0000 MHz 
//=========================================
void Timer0_Init()
{
   // TCCR0 = 0x03; // divider 64
/*
	TCCR0A |= (0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(0<<WGM01)|(0<<WGM00);
	TCCR0B |= (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(0<<CS02)|(1<<CS01)|(1<<CS00); //64
	TIMSK0 |= (0<<OCIE0B)|(0<<OCIE0A)|(1<<TOIE0);
	TIFR0 |=  (0<<OCF0B)|(0<<OCF0A)|(0<<TOV0);
	TCNT0 = 256 - 125; 	 
	*/
// NOTE: Assumes DIV8 fuse set for 1 MHz operation
// 8-bit counter means overflow interrupt every 61 Hz to turn on the LED
// Variable Compare_A interrupt in between OVF to turn off the LED
TCCR0A |= (0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(0<<WGM01)|(0<<WGM00); // Set OC0A/OC0B on Compare Match, clear OC0A/OC0B at BOTTOM (inverting mode) ,Fast PWM Mode
TCCR0B |= (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(0<<CS02)|(1<<CS01)|(0<<CS00); //0x02;	pre-scale= CK/8
//OCR0A = 0x00; // Governs Duty Cycle (192 ~75% of 256) for inverting mode OCR0A = (255-duty-cycle)
//OCR0B = 0x00; // Governs Duty Cycle (192 ~75% of 256) for inverting mode OCR0B = (255-duty-cycle)
TIMSK |= (0<<OCIE1A)|(0<<OCIE1B)|(0<<OCIE0A)|(0<<OCIE0B)|(0<<TOIE1)|(1<<TOIE0);	// Compare A and OVF Interrupt enabled
TCNT0 = 256 - 125;
}


// Timer 1  timeout= 1.000 ms, fosc = 8.0000 MHz 
//=========================================
void Timer1_Init()
{
   /*
    TCCR1A |= (0<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10);
	TCCR1B |= (0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10); // divider 1    
	TCCR1C |= (0<<FOC1A)|(0<<FOC1B);
	TIMSK1 |= (0<<ICIE1)|(0<<OCIE1B)|(0<<OCIE1A)|(1<<TOIE1);
	TCNT1  = 65536 - 8000;
	*/
   TCCR1 |= (0<<CTC1)|(0<<PWM1A)|(0<<COM1A1)|(0<<COM1A0)|(0<<CS13)|(0<<CS12)|(0<<CS11)|(1<<CS10); // PWM1A Disabled, Timer/Counter Comparator A disconnected from output pin OC1A, prescale= CK/128 right now was CK/8 1000b ~ 1100b
   GTCCR |= (0<<TSM)|(0<<PWM1B)|(0<<COM1B1)|(0<<COM1B0)|(0<<FOC1B)|(0<<FOC1A)|(0<<PSR1)|(0<<PSR0); //PWM1B Enabled, OC1B cleared on compare match. Set when TCNT1 = $00
   //OCR1A = 0x00; // Disabled Governs Duty Cycle (128 ~50% of 256)
   //OCR1B = 0x00; // Governs Duty Cycle (192 ~75% of 256)
   //OCR1C = PWM_200Hz;	// OCR1C = Governs PWM Frequency :: (1Mhz/prescaler)/(OCR1C+1)= PWM Frequency :: 1M/128/39+1 = 200Hz	0x27 1M/128/252+1 = 33Hz	0xFC
   TIMSK |= (0<<OCIE1A)|(0<<OCIE1B)|(0<<OCIE0A)|(0<<OCIE0B)|(1<<TOIE1)|(1<<TOIE0);	// Compare A and OVF Interrupt enabled
   TCNT1  = 65536 - 8000;
   //timer1Overflows = 0; // Start timer overflows at 0
}

// Timer 2  timeout= 1.000 ms, fosc = 8.0000 MHz 
//=========================================

ISR (TIMER0_OVF_vect)
{
	TCNT0 = 256 - 125; 

    if (++repeat_cnt0 == 1) 
    {
        repeat_cnt0 = 0; 
		
         msCountedTimer0++;
		 if(msCountedTimer0 % 50 == 0)
		 {
			  msCountedTimer0 = 0;
			  adcValue = 0;
			  //adcValue = adc_get_value(3);
			  adcValue = adc_get_value_ref(2, 3);
			  if(adcValue <= 670)
			  {
				   Led2_on();
			  }
			  else
			  {
				  Led2_off();
			  }
		 }
         if(msCountedTimer0 % 100 == 0)
         {
			  if(buttonIsPressed(1))
			  {
				  msCountedTimer0 = 0;
				  Led1_on();
			  }
			  else
			  {
				  Led1_off();
			  }
		 }
		 
     }
}

ISR(TIMER0_COMPA_vect) // timer 0 compare A interrupt
{
	
}

ISR(TIMER0_COMPB_vect) // timer 0 compare B interrupt
{

}

// Timer 1 interrupt service routine 
// timeout 1.000 ms , fosc = 8.0000 MHz 
//====================================
ISR (TIMER1_OVF_vect)
{
    TCNT1 = 65536 - 8000; 
    if (++repeat_cnt1 == 1) 
    {
        repeat_cnt1 = 0; 
		
         msCountedTimer1++;
         if(msCountedTimer1 == 1500)
         {
	         msCountedTimer1 = 0;
	          
		 }
	
	}
}

ISR(TIMER1_COMPA_vect) // timer 1 compare A interrupt
{

}

ISR(TIMER1_COMPB_vect) // timer 1 compare B interrupt
{

}





