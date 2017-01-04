/*
 * Application1.c
 *
 * Created: 11/25/2014 11:05:09 PM
 *  Author: Gregory Byrne
 */ 


#include <avr/io.h>

int main(void) {

	DDRB = 0xff;

	PORTB = 0b00000011;

	PORTB = 0b00001111;

	PORTB = 0b00111111;

	PORTB = 0xff;

	return(1);

};