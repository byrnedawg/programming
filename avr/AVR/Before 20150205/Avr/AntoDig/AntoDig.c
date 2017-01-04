#include <avr/io.h>
#include <util/delay.h>
#include<avr\interrupt.h>
#include "usart.h"

#define 	Backspace_Left 	8
#define 	Move_Right 		9
#define 	Clear 			12
#define		C_Return		13 
#define		Display_On		24
#define		Display_OFF		21

unsigned char message1[]; 
unsigned char message2[]; 
unsigned char status[]= "C is : ";
unsigned char Value[]= "DCV = ";
unsigned char Dot[]= ".";
unsigned char EL[]= "EL:";
uint16_t adc_result;
uint16_t whole;
uint16_t frac;
//unsigned double 


ISR(INT0_vect)
{
//	switch_temp = ~PINK;
	//if (switch_temp != 0x00){					// Skip if button release
	//	pulse_incr = 0x10;					// Maximum step = 16
	//	for (i=0x80;i > 0x00; i = i >> 1){
	//		if (switch_temp & i) break;
		//	pulse_incr -= 2;
	//	}
	//	brightness = INCREASING;
		//slider_value = 0x00;				// Start new cycle with LED off
//	}
}

ISR(ADC_vect)
{
	adc_result=ADC; 
	whole = adc_result;///204;
	//	frac = (adc_result*10)/204;  
		UWriteData(Clear);
		WriteString(Value);
		itoa(whole, message1, 10); 
	 	WriteString(message1);
//	ADCSRA|=(1<<ADSC); 
}

void WriteString(unsigned char *strPointer)
{   
   while (*strPointer)
   {
      UWriteData(*strPointer);
      strPointer++;   
   }
   
   return;
} 

 
void InitADC()
{
ADMUX=(1<<REFS0);                         // For Aref=AVcc;
ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADIE); //Rrescalar div factor =128
sei();  //start interrupt
}

uint16_t ReadADC(uint8_t ch)
{
   //Select ADC Channel ch must be 0-7
   ch=ch&0b00000111;
   ADMUX|=ch;

   //Start Single conversion
   ADCSRA|=(1<<ADSC);

   //Wait for conversion to complete
   while(!(ADCSRA & (1<<ADIF)));

   //Clear ADIF by writing one to it
   //Note you may be wondering why we have write one to clear it
   //This is standard way of clearing bits in io as said in datasheets.
   //The code writes '1' but it result in setting bit to '0' !!!

   ADCSRA|=(1<<ADIF);

   return(ADC); 
}

void Wait()
{
   uint8_t i;
   for(i=0;i<20;i++)
      _delay_loop_2(0); 
}

void main()
{
   
   //Initialize ADC
   InitADC();

	//Initialize the USART with Baud rate = 2400bps 
	USARTInit(12);// 19200 4mhz 12 2400 4mhz 103 8mhz 207 9600 4mhz 25

	//Enable Internal Pullups on PORTC
	PORTC=0xFF;

	UWriteData(Display_On);
	//uint8_t data;

	ADCSRA|=(1<<ADSC);

   while(1)
   {
		
	//	WriteString(Dot);
	//	itoa(frac, message2, 10); 
	 //	WriteString(message2);    //adc_result=ReadADC(0);           // Read Analog value from channel-0
		ADCSRA|=(1<<ADSC); 
      	Wait();
   }
}


