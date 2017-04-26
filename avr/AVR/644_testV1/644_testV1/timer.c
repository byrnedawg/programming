// timer.c

#include "prj.h"

volatile unsigned int repeat_cnt0 = 0;
volatile unsigned int repeat_cnt1 = 0;
volatile unsigned int repeat_cnt2 = 0;
volatile unsigned int msCountedTimer0 = 0;
volatile unsigned int msCountedTimer1 = 0;
volatile unsigned int msCountedTimer2 = 0;
volatile unsigned int state = 5;
volatile unsigned int toggleMsTime = 500;
volatile char line[128];
volatile unsigned char lineIndex = 0;


// Timer 0  timeout= 1.000 ms, fosc = 8.0000 MHz 
//=========================================
void Timer0_Init()
{
   // TCCR0 = 0x03; // divider 64

	TCCR0A |= (0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(0<<WGM01)|(0<<WGM00);
	TCCR0B |= (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(0<<CS02)|(1<<CS01)|(1<<CS00); //64
	TIMSK0 |= (0<<OCIE0B)|(0<<OCIE0A)|(1<<TOIE0);
	TIFR0 |=  (0<<OCF0B)|(0<<OCF0A)|(0<<TOV0);
	TCNT0 = 256 - 125; 	 
}


// Timer 1  timeout= 1.000 ms, fosc = 8.0000 MHz 
//=========================================
void Timer1_Init()
{
    TCCR1A |= (0<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10);
	TCCR1B |= (0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10); // divider 1    
	TCCR1C |= (0<<FOC1A)|(0<<FOC1B);
	TIMSK1 |= (0<<ICIE1)|(0<<OCIE1B)|(0<<OCIE1A)|(1<<TOIE1);
	TCNT1  = 65536 - 8000;
}

// Timer 2  timeout= 1.000 ms, fosc = 8.0000 MHz 
//=========================================
void Timer2_Init()
{
	TCCR2A |= (0<<COM2A1)|(0<<COM2A0)|(0<<COM2B1)|(0<<COM2B0)|(0<<WGM21)|(0<<WGM20);
	TCCR2B |= (0<<FOC2A)|(0<<FOC2B)|(0<<WGM22)|(0<<CS22)|(1<<CS21)|(1<<CS20); // divider 32
	TIMSK2 |= (0<<OCIE2B)|(0<<OCIE2A)|(1<<TOIE2);
	TCNT2 = 256 - 250; 
}
// Timer 0 interrupt service routine 
// timeout 1.000 ms , fosc = 8.0000 MHz 
//====================================
ISR (TIMER0_OVF_vect)
{
	TCNT0 = 256 - 125; 
	int adcValue = 0;
	char str[16];
	char *ptr;
	
    if (++repeat_cnt0 == 1) 
    {
        repeat_cnt0 = 0; 
		
         msCountedTimer0++;
         if(msCountedTimer0 % 100 == 0)
         {
			 //msCountedTimer0 = 0;
			 if(uart0_available() != 0)
			 {
				 if(uart0_peek() != '\r')
				 {
					 line[lineIndex++] = uart0_getc();
				 }
			 }
			 
			 
			 if(uart0_peek() == '\r')
			 {
				  ptr = (char *)line;
				  uart0_flush();
				  uart0_puts("\r\n");
				  uart0_puts(ptr);
				  uart0_puts("\r\n");
				  Lcm1_Clearscreen();
				  Lcm1_SetCursor(0,0);
				  Lcm1_ShowString(ptr);
				  if(strcmp(ptr,"hello") == 0)
				  {
					  uart0_puts("\r\n");
					  uart0_puts(" Hello to you Goon !!!!");
					  uart0_puts("\r\n");
				  }
				  else if(strcmp(ptr,"start") == 0)
				  {
					  uart0_puts("\r\n");
					  uart0_puts("Starting !!!!");
					  uart0_puts("\r\n");
				  }
				  else if(strcmp(ptr,"stop") == 0)
				  {
					  uart0_puts("\r\n");
					  uart0_puts("Stopping !!!!");
					  uart0_puts("\r\n");
				  }
				  else
				  {
					  uart0_puts("\r\n");
					  uart0_puts("Invalid input");
					  uart0_puts("\r\n");
				  }
				  for(int i = 0; i<128; i++)
				  {
					  line[i] = '\0';
				  }
				  lineIndex = 0;
			 }
			 
	         if(buttonIsPressed(1))
			 {
				 uart0_puts("Button 1 Pushed\r\n");
				 uart0_puts("ADC Ref Value: VCC \r\n");
				 uart0_puts("ADC Value: ");
				 adcValue = adc_get_value_ref(1,7);
				 itoa(adcValue, str, 10);
				 uart0_puts(str);
				 uart0_puts("\r\n");
				 Lcm1_Clearscreen();
				 Lcm1_SetCursor(0,0);
				 Lcm1_ShowString("ADC Ref: VCC");
				 Lcm1_SetCursor(0,1);
				 Lcm1_ShowString("ADC Value: ");
				 Lcm1_ShowString(str);
				 if(state > 2)
				 {
					 state--;
				 }
				
			 }
			 else if(buttonIsPressed(2))
			 {
				 uart0_puts("Button 2 Pushed\r\n");
				 uart0_puts("ADC Ref Value: 2.56V \r\n");
				 uart0_puts("ADC Value: ");
				 adcValue = adc_get_value_ref(3,6);
				 itoa(adcValue, str, 10);
				 uart0_puts(str);
				 uart0_puts("\r\n");
				 Lcm1_Clearscreen();
				 Lcm1_SetCursor(0,0);
				 Lcm1_ShowString("ADC Ref: 2.56V");
				 Lcm1_SetCursor(0,1);
				 Lcm1_ShowString("ADC Value: ");
				 Lcm1_ShowString(str);
				 if(state < 10)
				 {
					 state++;
				 }
				 
			 }
			 else
			 {
				 
			 }
			 
			 if(state == 1)
			 {
				 toggleMsTime = 50;
			 }
			 else if(state == 2)
			 {
				 toggleMsTime = 100;
			 }
			 else if(state == 3)
			 {
				 toggleMsTime = 200;
			 }
			  else if(state == 4)
			  {
				  toggleMsTime = 400;
			  }
			  else if(state == 5)
			  {
				  toggleMsTime = 500;
			  }
			   else if(state == 6)
			   {
				   toggleMsTime = 750;
			   }
			   else if(state == 7)
			   {
				   toggleMsTime = 1000;
			   }
			   else if(state == 8)
			   {
				   toggleMsTime = 1250;
			   }
			   else if(state == 9)
			   {
				   toggleMsTime = 1500;
			   }
			   else
			   {
				   toggleMsTime = 2000;
			   }
			
		
		 }
		 
     }
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
		/*
         msCountedTimer1++;
         if(msCountedTimer1 == 1500)
         {
	         msCountedTimer1 = 0;
	         toggleLED(1);
	         toggleLED(2); 
		 }
		 */

	}
}

// Timer 2 interrupt service routine 
// timeout 1.000 ms , fosc = 8.0000 MHz 

//====================================
ISR (TIMER2_OVF_vect)
{
    TCNT2 = 256 - 250; 
    if (++repeat_cnt2 == 1) 
    {
        repeat_cnt2 = 0; 
         msCountedTimer2++;
         if(msCountedTimer2 % toggleMsTime == 0)
         {
	         //msCountedTimer2 = 0;
	         toggleLED(1);
	         toggleLED(2); 
		 }
    }
}



