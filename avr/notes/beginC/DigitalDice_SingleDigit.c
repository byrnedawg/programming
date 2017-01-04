/*
 * DigitalDice_SingleDigit.c
 * Written in AVR Studio 5
 * Compiler: AVR GNU C Compiler (GCC)
 *
 * Created: 5/27/2011 10:50:48 PM
 * Author: AVR Tutorials
 * Website: www.AVR-Tutorials.com
 *
 * Description: AVR C program for ATMega16 Seven Segment 
 * Single Digit Digital Dice Project
 */ 
 
#define DicePort		PORTA
#define DicePortDDR		DDRA
 
#define SwitchPortDDR		DDRD
#define SwitchPort		PORTD
#define SwitchPin		PIND
 
#define RollSwitch		7
 
#define F_CPU 4000000UL
 
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
 
unsigned char seed = 0;
 
/*Declaration of functions. The implementation of these */
/*function are done below the main() function*/
unsigned char digit_to_7segval(unsigned char digit);
void roll_dice(unsigned char times);
void init();
ISR(TIMER0_OVF_vect);
 
 
int main(void)
{	
	unsigned char digit = 0;
 
	init();	// Configure the Microcontroller
 
    /*Do forever*/
	while(1)
	{
 
		if(!(SwitchPin & (1<<RollSwitch)))
		{
			/*Roll the dice 2 times*/
			roll_dice(2);
 
			/*Generate an integer based random number between */
			/*1 and 6 inclusive and store it in digit*/
			seed = (seed*17)+21;
			digit = seed % 6;
			digit++;
 
			/*Get the Seven Segment value of the random number*/
			/*and output it to the pins of port B*/	
			DicePort = digit_to_7segval(digit); 
		}			
	}		
 
}
 
/*Configuration for the microcontroller*/
void init()
{
	/*Configure DicePort as an output port*/
	DicePortDDR = 0xFF;
 
	/*Configure SwitchPort as an input port*/
	SwitchPortDDR = 0x00;
	/*Enable SwitchPort pull-up resistors*/
	SwitchPort = 0xFF;
 
	TCCR0 = 0x01;   // Enable Timer0 with no prescalar
	TIMSK = 0x01;   // Enable Timer0 interrupt
	sei();			// Enable Global Interrupt
}	
 
/*This function takes an character value and return the */
/*and return the value to be outputted to display that */
/*character on a seven segment display*/
unsigned char digit_to_7segval(unsigned char digit)
{
	unsigned char segval = '0';
 
	if(digit == 1)
		segval = 0x06;		// 7-Seg Value for Digit 1
	else if (digit == 2)
		segval = 0x5B;		// 7-Seg Value for Digit 2
	else if (digit == 3)
		segval = 0x4F;		// 7-Seg Value for Digit 3
	else if (digit == 4)
		segval = 0x66;		// 7-Seg Value for Digit 4
	else if (digit == 5)
		segval = 0x6D;		// 7-Seg Value for Digit 5
	else if (digit == 6)
		segval = 0x7D;		// 7-Seg Value for Digit 6
 
	return segval;
}
 
/*This function simulates the rolling of the dice*/
void roll_dice(unsigned char times)
{
	unsigned char i;
 
	for(i=0; i<times; i++)
	{
		DicePort = 0x06;	// Display #1 on dice
		_delay_ms(100);
		DicePort = 0x5B;	// Display #2 on dice
		_delay_ms(100);
		DicePort = 0x4F;	// Display #3 on dice
		_delay_ms(100);
		DicePort = 0x66;	// Display #4 on dice
		_delay_ms(100);
		DicePort = 0x6D;	// Display #5 on dice
		_delay_ms(100);
		DicePort = 0x7D;	// Display #6 on dice
		_delay_ms(100);
 
	}		
}
 
/*Interrupt Service Routine for timer overflow*/
ISR(TIMER0_OVF_vect)
{
	seed++; // Increment the value in the variable seed
}		