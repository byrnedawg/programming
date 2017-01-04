/*
 * attiny85uart.c
 *
 * Created: 2/27/2014 2:00:27 PM
 *  Author: gregory.byrne
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>
#include "USI_UART.h"
#include "USI_UART.c"

 unsigned char myString[] = "\n\rYou just sent me: ";
  
    unsigned char counter;
    
 
int main(void)
{
    
	USI_UART_Flush_Buffers();
    USI_UART_Initialise_Receiver();                                         // Initialisation for USI_UART receiver
	USI_UART_Initialise_Transmitter();
	sei();                                                  // Enable global interrupts
    
    MCUCR = (1<<SE)|(0<<SM1)|(0<<SM0);                                      // Enable Sleepmode: Idle
	while(1)
    {
         //if( USI_UART_Data_In_Receive_Buffer() )
			//{  
			for(counter = 0; counter < 20; counter++)                       // Echo myString[]
            {
               // USI_UART_Transmit_Byte( (unsigned int)myString[counter] ); 
				USI_UART_Transmit_Byte('G');
				_delay_ms(500); 
				USI_UART_Transmit_Byte('R');   
				_delay_ms(500); 
				USI_UART_Transmit_Byte('E'); 
				_delay_ms(500); 
				USI_UART_Transmit_Byte('G');
				_delay_ms(500);   
            }
           // USI_UART_Transmit_Byte( USI_UART_Receive_Byte() );              // Echo the received character      
			// }  
			   
        sleep_enable();                                                         // Sleep when waiting for next event
    }
}



    
   
