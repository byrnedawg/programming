/*
 * Demonstration for using I2C devices/sensors such as compass, eeprom, accelerometer etc.
 *
 * Target: Any AVR with hardware I2C (TWI). TWI : Two Wire Interface.
 *
 * Created: on 29 September 2013
 * Author: Vassilis Serasidis
 *
 * Home: http://www.serasidis.gr
 * e-mail: avrsite@yahoo.gr, info@serasidis.gr 
 *
 * Devices was used:
 * - GY26:     I2C digital compass.
 * - LIS302DL: 3-axes I2C/SPI accelerometer.
 * - PCF8574T: I2C-to-parallel converter. Usually used to drive dot matrix LCDs such as 16x2 or 20x4 characters.
 * - DS1307:   I2C Real-Time-Clock. 
 * - 24C32:    I2C 32kbit eeprom (4 kByte x 8 bit = 32 kBits). 
 *
 *
 */ 

#include <avr/io.h>
//#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "i2cmaster.h"
#include "uart.h"
//#include "GY26.h"
#include "LIS302DL.h"
//#include "DS1307.h"
//#include "i2c_lcd.h"
//#include "24cXX.h"

#define LED1 PB0
#define sbi(x,y) x |= _BV(y) //set bit (original sbi)
#define cbi(x,y) x &= ~(_BV(y)) //(original)cbi clear bit //reverse cause of 3904
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) (x & _BV(y)) == _BV(y) //(for input) checks if the // input is high (Logic 1) i.e. if(is_high(PIND,PD0))						
#define RESET &=~
#define SET |=
#define TOGGLE ^=


//--------------------------------------------------------------------

//--------------------------------------------------------------------
int main(void)
{
	char data[16];
	int axe_value;
	i2c_init();
	DDRB |= (1 << 0); // Set LED as output
	//gy26_init();					// Initialize GY-26 digital compass.
	//lcd_init(LCD_BACKLIGHT_ON);		// Initialize I2C LCD.
	LIS302DL_init();				// initialize I2C LIS302DL XYZ accelerometer.
	//sei();						// Enable interrupts.
	
	
    while(1)
    {	
		PORTB ^= (1 << 0); // Toggle the LED 
		_delay_ms(1000);
		axe_value = LIS302DL_read(outX); //Read X axe value.
		itoa(axe_value, data, 10);		//Convert the X value integer into string.
		//lcd_goto_xy(2,2);				//Go to character 2 and line 2 on LCD.
		//lcd_puts(data);					//Print X value number on LCD.

		_delay_ms(200);
    }
}

//==================================================================================
// Prints space characters on LCD starting from the last used LCD address.
// This function is used to erase previous printed data on LCD from specific location.
//==================================================================================