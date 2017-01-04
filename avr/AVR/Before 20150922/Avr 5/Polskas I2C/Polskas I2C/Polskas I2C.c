/*
 * Polskas_I2C.c
 *
 * Created: 9/10/2014 1:41:06 PM
 *  Author: gregory.byrne
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "i2cmaster.h"

#define MMA8452  0x1D      // device address of EEPROM 24C02, see datasheet
#define MMA8452_CNTRL 0x2A      // device address of EEPROM 24C02, see datasheet
#define WHO_AM_I   0x0D
#define LED PB1

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)

#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)
 

 int main(void)
 {
     unsigned char ret;

     i2c_init();                             // initialize I2C library
	 set_output(DDRB, LED);
	 
	 while(1)
	 {
		// write 0x75 to EEPROM address 5 (Byte Write) 
		 i2c_start(MMA8452 + I2C_WRITE);     // set device address and write mode	
		 i2c_write(WHO_AM_I);  
		 i2c_stop();                      // write address = 5
		 _delay_ms(2);
		 
		 i2c_start(MMA8452 + I2C_READ);       // set device address and read mod
	     ret = i2c_readNak(); 
		 i2c_stop();
		 _delay_ms(2);
		 /*
		 if(ret != 0x2A)                   // read one byte from EEPROM
		 {
			 for(int i = 0; i < 10 ; i++)
			 {
				 output_high(PORTB, LED);
				_delay_ms(500);
				output_low(PORTB, LED);
				_delay_ms(500);
			 }
		 }
		 */
		// output_high(PORTB, LED);
		//_delay_ms(2000);
		
		 i2c_start(MMA8452 + I2C_WRITE);     // set device address and write mode
		 i2c_write(MMA8452_CNTRL);                        // write address = 5
		 i2c_write(0x01);                        // write value 0x75 to EEPROM
		 i2c_stop();
		 _delay_ms(2);   
	                           // set stop conditon = release bus
		// output_low(PORTB, LED);
		 //read previously written value back from EEPROM address 5 
		 i2c_start(MMA8452 + I2C_WRITE);     // set device address and write mode	
		 i2c_write(MMA8452_CNTRL); 
		 i2c_stop();
		 _delay_ms(2);
		                        // write address = 5
		 i2c_start(MMA8452 + I2C_READ);       // set device address and read mode
	     ret = i2c_readNak();                    // read one byte from EEPROM
		 i2c_stop();
		 
		 _delay_ms(500);

     }
 }