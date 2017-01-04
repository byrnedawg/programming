/*
 * RetroliteSoftRx.c
 *
 * Created: 12/30/2014 10:07:52 AM
 *  Author: ByrneDawg
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "softuart.c"
#include "softuart.h"

#define PIN_LED1    PB3
#define PIN_LED2    PB4

// Define your softuart pin in softuart.h!

// Define helper macros

// write digital "high" to pin <pn> on port <prt>
#define Set_Pin_High(prt, pn) prt |= (1<<pn)

// write digital "low" to pin <pn> on port <prt>
#define Set_Pin_Low(prt, pn) prt &= ~(1<<pn)

//Display Macros
#define	CMD				254
#define Go_Home			72
#define Clear_Screen	88 
#define Return			13
#define Cursor			71


#define PRINT_NL softuart_putchar( '\n' )
#define PRINT_Space softuart_putchar( ' ' )

// Define function to write an integer to serial out
#define PRINT_INT_BUFSIZE	16

const char FName[] = "Greg";
const char LName[] = "Byrne";


static int my_stdio_putchar(char c, FILE *stream)
{
	if(c == '\n')
	{
		softuart_putchar('\r');
	}
	softuart_putchar(c);
	
	return 0;
}

FILE suart_stream = FDEV_SETUP_STREAM(my_stdio_putchar, NULL, _FDEV_SETUP_WRITE);

/*
static void stdio_out(void)
{
	stdout = &suart_stream;
	printf("This output is done with printf\n");
	printf_P(PSTR("This output is done with printf_P\n"));
}
*/
void printInt(int32_t i) {
	char buf[PRINT_INT_BUFSIZE];
	int8_t sign = 1;
	int8_t len = 0;

	if (i < 0) { // look for the sign
		sign = -1;
		i = -i;
	}

	// fill buffer with digits (in reverse order)
	do {
		buf[len++] = 48 + i % 10;	// ASCII digits start at 48
	} while ((i /= 10) > 0 && len < PRINT_INT_BUFSIZE);

	if (sign < 0) {	// don't forget to add the sign
		buf[len] = '-';
		} else {
		len--;
	}

	// reverse output of the buffer
	do {
		softuart_putchar(buf[len--]);
	} while (len >= 0);
}


// program entry point
int main(void) {
	// DDRB for the UART TX will be set in the softuart_init() function!
	
	 // we set both LED pins to "output"
	 DDRB |= (1 << PIN_LED1) | (1 << PIN_LED2);
	 
	 // initially set the pins to "low"
	 Set_Pin_Low(PORTB, PIN_LED1);
	 Set_Pin_Low(PORTB, PIN_LED2);
	
	// we neet to init softuart
	softuart_init();
	softuart_turn_rx_on();    // we receive serial data

	sei(); // this enables interupts, which are needed for softuart
	
	uint8_t toggle = 0;
	//int8_t address = 213;
	//int8_t checksum = 170;
	//int8_t sum = 0;
	//int8_t command = 85;
	
	while(1)
	{
		Set_Pin_Low(PORTB, (toggle == 0 ? PIN_LED1 : PIN_LED2));
		Set_Pin_High(PORTB, (toggle == 0 ? PIN_LED2 : PIN_LED1));
		toggle = !toggle;
		// Tests whether an input character has been received.
		
		uint8_t dataIn = softuart_getchar();
		uint8_t inData[32];
		uint8_t index = 0;
		
		
		//while(dataIn != -1)
		while(softuart_kbhit())
		{
			inData[index] = dataIn;
			index++;
			dataIn = softuart_getchar();
			
			softuart_putchar(CMD);
			softuart_putchar(Clear_Screen);
			
			softuart_putchar(inData[0]);
			PRINT_Space;
			softuart_putchar(inData[1]);
			PRINT_Space;
			softuart_putchar(inData[2]);
			PRINT_Space;
			softuart_putchar(inData[3]);
			PRINT_Space;
		}
		
		

		
		
		//sum = address+checksum+command;
		//softuart_puts(FName);
		//PRINT_NL;
		//softuart_puts(LName);
		//PRINT_Space;
		//softuart_putchar(address);
		//PRINT_Space;
		//softuart_putchar(checksum);
		//PRINT_Space;
		//softuart_putchar(command);
		//PRINT_Space;
		//softuart_putchar(sum);
		
		/*
		printInt(address);
		PRINT_Space;
		printInt(checksum);
		PRINT_Space;
		printInt(command);
		PRINT_Space;
		printInt(sum);
		*/
		
		
		// make a small delay
		_delay_ms(1000);
	}
	
	return 0;   /* never reached */
}
