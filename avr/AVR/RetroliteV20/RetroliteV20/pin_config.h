#ifndef _PIN_CONFIG_H
#define _PIN_CONFIG_H

#include "prj.h"

// 												   ______
//	(PCINT5/~RESET/ADC0/dW)			Reset	--PB5 |		 |VCC--	5V
//	(PCINT3/XTAL1/CLKI/~OC1B/ADC3)	LDR		--PB3 |		 |PB2--	RelayOFF (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
//	(PCINT4/XTAL2/CLKO/OC1B/ADC2)	BLU LED	--PB4 |		 |PB1--	RelayON	(MISO/DO/AIN1/OC0B/OC1A/PCINT1)
//											--GND |______|PB0--	Switch	(MOSI/DI/SDA/AIN0/OC0A/~OC1A/AREF/PCINT0)
// 

/* 9600 baud */
//#define UART_BAUD_RATE      9600


#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);

#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);

#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)

/*
#define Bz1_PORT    PORTB
#define Bz1_DIR     DDRB
#define Bz1_PIN     PINB
#define Bz1_BIT     7

#define Switch1 PB0
#define RelayStatus PB1
#define TransmissionEn PB2
#define ReadADC2 PB4

*/
#define Button1_PORT    PORTB
#define Button1_DIR     DDRB
#define Button1_PIN     PINB
#define Button1_BIT     0
#define Button1_PIN_BIT PB0

#define ADC2_PORT    PORTB
#define ADC2_DIR     DDRB
#define ADC2_PIN     PINB
#define ADC2_BIT     4
#define ADC2_PIN_BIT PB4




#define Led1_PORT    PORTB
#define Led1_DIR     DDRB
#define Led1_PIN     PINB
#define Led1_BIT     1
#define Led1_PIN_BIT PB1

#define Led2_PORT    PORTB
#define Led2_DIR     DDRB
#define Led2_PIN     PINB
#define Led2_BIT     2
#define Led2_PIN_BIT PB2

#endif
