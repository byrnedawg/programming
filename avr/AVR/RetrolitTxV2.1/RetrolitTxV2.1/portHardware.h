/*
 * portHardware.h
 *
 * Created: 6/27/2017 8:51:46 AM
 *  Author: TestDev
 */ 


#ifndef PORTHARDWARE_H_
#define PORTHARDWARE_H_

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/wdt.h>
#include <stdlib.h>
//#include <string.h>
//#include <avr/pgmspace.h>

//#include "program_Memory.h"
#include "adc.h"
//#include "uart.h"
#include "timer.h"
//#include "portHardware.h"
//#include "interrupt.h"

//#include "prj.h"

//												   ______
//	(PCINT5/~RESET/ADC0/dW)			Reset	--PB5 |		 |VCC--	5V
//	(PCINT3/XTAL1/CLKI/~OC1B/ADC3)	ADC		--PB3 |		 |PB2--	YLW LED	(SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
//	(PCINT4/XTAL2/CLKO/OC1B/ADC2)	N/C 	--PB4 |		 |PB1--	GRN LED	(MISO/DO/AIN1/OC0B/OC1A/PCINT1)
//											--GND |______|PB0--	Switch	(MOSI/DI/SDA/AIN0/OC0A/~OC1A/AREF/PCINT0)
//

#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);

#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);

#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)


#define Button1_PORT    PORTB
#define Button1_DIR     DDRB
#define Button1_PIN     PINB
#define Button1_BIT     0
#define Button1_PIN_BIT PB0

#define ADC3_PORT    PORTB
#define ADC3_DIR     DDRB
#define ADC3_PIN     PINB
#define ADC3_BIT     3
#define ADC3_PIN_BIT PB3

#define Led1_PORT    PORTB
#define Led1_DIR     DDRB
#define Led1_PIN     PINB
#define Led1_BIT     1
#define Led1_PIN_BIT PB1

#define Digital_Out1_PORT    PORTB
#define Digital_Out1_DIR     DDRB
#define Digital_Out1_PIN     PINB
#define Digital_Out1_BIT     2
#define Digital_Out1_PIN_BIT PB2

#define Digital_Out2_PORT    PORTB
#define Digital_Out2_DIR     DDRB
#define Digital_Out2_PIN     PINB
#define Digital_Out2_BIT     4
#define Digital_Out2_PIN_BIT PB4

#define Digital_Out1_High()     Digital_Out1_PORT |=  _BV(Digital_Out1_BIT)
#define Digital_Out1_Low()    Digital_Out1_PORT &= ~_BV(Digital_Out1_BIT)

#define Digital_Out2_High()     Digital_Out2_PORT |=  _BV(Digital_Out2_BIT)
#define Digital_Out2_Low()    Digital_Out2_PORT &= ~_BV(Digital_Out2_BIT)

// 0 - active low, 1 - active high
#define LED1_ACTIVE_LOW    0

#if (LED1_ACTIVE_LOW ==0)
#define Led1_on()     Led1_PORT &= ~_BV(Led1_BIT)
#define Led1_off()    Led1_PORT |=  _BV(Led1_BIT)
#define Led2_on()     Led2_PORT &= ~_BV(Led2_BIT)
#define Led2_off()    Led2_PORT |=  _BV(Led2_BIT)
#else
#define Led1_on()     Led1_PORT |=  _BV(Led1_BIT)
#define Led1_off()    Led1_PORT &= ~_BV(Led1_BIT)
#define Led2_on()     Led2_PORT |=  _BV(Led2_BIT)
#define Led2_off()    Led2_PORT &= ~_BV(Led2_BIT)
#endif


#define Button1_press()     ( ( Button1_PIN & _BV(Button1_BIT)) == 0 )
#define Button1_release()   ( (Button1_PIN & _BV(Button1_BIT)) == 1 )

void initDigitalOutput(void);

void initLEDs(void);

void toggleLED(int ledNum);

void initButtons(void);

unsigned int buttonIsPressed(int button);



#endif /* PORTHARDWARE_H_ */