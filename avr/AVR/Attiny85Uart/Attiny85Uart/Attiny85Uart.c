/*
 * Attiny85Uart.c
 *
 * Created: 11/16/2015 8:15:56 AM
 *  Author: ByrneDawg
 */ 

#define F_CPU 8000000UL
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "softuart.h"


int main(void)
{
	char c = 'A';

	dbg_tx_init();

	while(1) {
		dbg_putchar(c);
		if(++c > 'Z') c = 'A';
		_delay_ms(500);
	}

	return 0;
}

