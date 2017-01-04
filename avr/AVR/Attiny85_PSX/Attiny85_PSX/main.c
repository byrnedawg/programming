/*
 * Attiny85_PSX.c
 *
 * Created: 1/15/2016 7:08:56 AM
 * Author : ByrneDawg
 */ 

#define F_CPU 8000000UL
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "softuart.h"
#include "psx.h"

#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);

#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);

#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)

void LCD_String (char *string)
{
	while (*string)
	dbg_putchar(*string++);
}


int main(void)
{
	//char c = 'A';

	dbg_tx_init();
	
	psx_init(&PORTB, PB1, &PORTB, PB2, &PORTB, PB3, &PORTB, PB4);
	_delay_ms(10);
	
	sei();
	
	LCD_String("Greg ");
	while (1)
	{
		psx_read_gamepad();
	
		_delay_ms(10);
		//printInt(psx_buttons());
		if(psx_button(PSB_CROSS))
		{
			LCD_String("X Button ");
		}
		if(psx_button(PSB_CIRCLE))
		{
			LCD_String("Circle ");
		}
		if(psx_button(PSB_SQUARE))
		{
			LCD_String("Square ");
		}
		if(psx_button(PSB_TRIANGLE))
		{
			LCD_String("Triangle ");
		}
		if(psx_button(PSB_R1))
		{
			LCD_String("R1 ");
		}
		if(psx_button(PSB_R2))
		{
			LCD_String("R2 ");
		}
		if(psx_button(PSB_L1))
		{
			LCD_String("L1 ");
		}
		if(psx_button(PSB_L2))
		{
			LCD_String("L2 ");
		}
		if(psx_button(PSB_START))
		{
			LCD_String("Start ");
		}
		if(psx_button(PSB_SELECT))
		{
			LCD_String("Select ");
		}
		if(psx_button(PSB_PAD_UP))
		{
			LCD_String("UP ");
		}
		if(psx_button(PSB_PAD_DOWN))
		{
			LCD_String("DOWN ");
		}
		if(psx_button(PSB_PAD_LEFT))
		{
			LCD_String("LEFT ");
		}
		if(psx_button(PSB_PAD_RIGHT))
		{
			LCD_String("RIGHT ");
		}
		if(psx_button(PSB_L3))
		{
			LCD_String("L3 ");
		}
		if(psx_button(PSB_R3))
		{
			LCD_String("R3 ");
		}
		
		/*
		if(psx_button(PSB_L2)&&psx_button(PSB_R2))
		{
			commandDisplay(Cariage_Return);
			uart_putc('R');
			uart_putc('X');
			uart_putc(':');
			printInt(psx_stick(PSS_RX));
			uart_putc(' ');
			uart_putc('R');
			uart_putc('Y');
			uart_putc(':');
			printInt(psx_stick(PSS_RY));
			uart_putc(' ');
		}
		else
		{
			//printInt(psx_buttons());
			commandDisplay(Cariage_Return);
			uart_putc('L');
			uart_putc('X');
			uart_putc(':');
			printInt(psx_stick(PSS_LX));
			uart_putc(' ');
			uart_putc('L');
			uart_putc('Y');
			uart_putc(':');
			printInt(psx_stick(PSS_LY));
			uart_putc(' ');
		}
		*/
		_delay_ms(200);
	
	} // while(1)
} //main
