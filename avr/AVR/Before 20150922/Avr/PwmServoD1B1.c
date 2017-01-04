#define F_CPU 1000000UL   //CPU-freq 
#include <inttypes.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/sleep.h> 
#include <util/delay.h>

ISR (TIMER0_OVF_vect)      // timer0 overflow interrupt 
{ 
	
  
} 
/*
ISR (TIMER2_OVF_vect)      // timer1 overflow interrupt 
{ 


}
*/
void ioinit (void)         //initialize pwm and enable interrupts. 
{ 
 
  	DDRA = 0x00; 
   	/* set OC0 for PWM output*/
	DDRB = (1<<PINB3);
	/*set timer counter initial value*/
	TCNT0 = 0xFF;
	/*Set timer output compare register*/
	OCR0 = 0x13; // 0F = 2 ms  13 = 2.5ms 07 = 1ms 03 =.5ms
	/*start timer:
	without presscaler
	Non inverted mode in OC2 pin;
	phase corect PWM*/
	TCCR0 = (1<<FOC0)|(1<<COM01)|(0<<COM00)|(1<<WGM01)|(1<<WGM00)|(1<<CS02)
	|(0<<CS01)|(1<<CS00);
  	sei (); 

} 

int main (void) 
{ 

   ioinit (); 

   // loop forever, the interrupts are doing the rest 

   while(1)
   {
   		if(bit_is_clear(PINA, 0) && bit_is_set(PINA, 1))
   		{   
   			DDRB = (1<<PINB3);
			OCR0 = 0x03;
  		} 
   		else if(bit_is_set(PINA, 0) && bit_is_clear(PINA, 1))
   		{ 
   			DDRB = (1<<PINB3);
			OCR0 = 0x13;
   		}
   		else if(bit_is_clear(PINA, 0) && bit_is_clear(PINA, 1))
   		{
			DDRB = (0<<PINB3);
   		}
   	
   }          
   

   return (0); 
} 
