#ifndef _PIN_CONFIG_H
#define _PIN_CONFIG_H

#include "prj.h"

//                 ATMEGA644            
//      +---------------------------+   
//     1|PB0 (XCK/T0)     (ADC0) PA0|40 
//     2|PB1 (T1)         (ADC1) PA1|39 
//     3|PB2 (INT2/AIN0)  (ADC2) PA2|38 
//     4|PB3 (OC0/AIN1)   (ADC3) PA3|37 
//     5|PB4 (nSS)        (ADC4) PA4|36 
//     6|PB5 (MOSI)       (ADC5) PA5|35 
//     7|PB6 (MISO)       (ADC6) PA6|34 
//     8|PB7 (SCK)        (ADC7) PA7|33 
//     9|nRST                   AREF|32 
//    10|VCC                     GND|31 
//    11|GND                    AVCC|30 
//    12|XTAL2           (TOSC2) PC7|29 
//    13|XTAL1           (TOSC1) PC6|28 
//    14|PD0 (RXD)         (TDI) PC5|27 
//    15|PD1 (TXD)         (TDO) PC4|26 
//    16|PD2 (INT0)        (TMS) PC3|25 
//    17|PD3 (INT1)        (TCK) PC2|24 
//    18|PD4 (OC1B)        (SDA) PC1|23 
//    19|PD5 (OC1A)        (SCL) PC0|22 
//    20|PD6 (ICP1)        (OC2) PD7|21 
//      +---------------------------+   

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



*/
#define Button1_PORT    PORTB
#define Button1_DIR     DDRB
#define Button1_PIN     PINB
#define Button1_BIT     4
#define Button1_PIN_BIT PB4

#define Button2_PORT    PORTD
#define Button2_DIR     DDRD
#define Button2_PIN     PIND
#define Button2_BIT     7
#define Button2_PIN_BIT PD7

/*
#define Keypad1_PORT    PORTC
#define Keypad1_DIR     DDRC
#define Keypad1_PIN     PINC
*/

#define Lcm1_DATA_PORT    PORTA
#define Lcm1_DATA_DIR     DDRA
#define Lcm1_DATA_PIN     PINA
#define Lcm1_RS_PORT    PORTB
#define Lcm1_RS_DIR     DDRB
#define Lcm1_RS_PIN     PINB
#define Lcm1_RS_BIT     0
#define Lcm1_RW_PORT    PORTB
#define Lcm1_RW_DIR     DDRB
#define Lcm1_RW_PIN     PINB
#define Lcm1_RW_BIT     1
#define Lcm1_EN_PORT    PORTB
#define Lcm1_EN_DIR     DDRB
#define Lcm1_EN_PIN     PINB
#define Lcm1_EN_BIT     3

#define Led1_PORT    PORTB
#define Led1_DIR     DDRB
#define Led1_PIN     PINB
#define Led1_BIT     5
#define Led1_PIN_BIT PB5

#define Led2_PORT    PORTC
#define Led2_DIR     DDRC
#define Led2_PIN     PINC
#define Led2_BIT     0
#define Led2_PIN_BIT PC0


/*
#define Relay1_PORT    PORTB
#define Relay1_DIR     DDRB
#define Relay1_PIN     PINB
#define Relay1_BIT     6

#define UART_TXD_PORT    PORTD
#define UART_TXD_DIR     DDRD
#define UART_TXD_PIN     PIND
#define UART_TXD_BIT     1
#define UART_RXD_PORT    PORTD
#define UART_RXD_DIR     DDRD
#define UART_RXD_PIN     PIND
#define UART_RXD_BIT     0
*/

#endif
