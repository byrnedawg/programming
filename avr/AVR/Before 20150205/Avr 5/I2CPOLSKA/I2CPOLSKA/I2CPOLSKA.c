/*
 * I2CPOLSKA.c
 *
 * Created: 9/9/2014 1:53:35 PM
 *  Author: gregory.byrne
 */ 

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
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "i2cmaster.h"
//#include "uart.h"
//#include "GY26.h"
//#include "LIS302DL.h"
//#include "DS1307.h"
//#include "i2c_lcd.h"
//#include "24cXX.h"

#define MMA8452  0x1D     // device address of EEPROM 24C02, see datasheet
#define LED PB1
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)

#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)


//void placeEmptyCharacters (char charsNumber[8], unsigned char emptySpacesNumber);

//--------------------------------------------------------------------

//--------------------------------------------------------------------
//int main(void)
//{
	//char data[16];
	//int axe_value;
	
	//set_output(DDRB, LED);
	
	//output_high(PORTB, LED);
	//LIS302DL_init();				// initialize I2C LIS302DL XYZ accelerometer.
	//_delay_ms(2000);
	//output_low(PORTB, LED);	
	//_delay_ms(2000);
  //  while(1)
   // {	
	//	output_high(PORTB, LED);
		


 int main(void)
 {
     set_output(DDRB, LED);
	 unsigned char ret;
     output_high(PORTB, LED);
     i2c_init();                             // initialize I2C library

     // write 0x75 to EEPROM address 5 (Byte Write) 
     i2c_start_wait(MMA8452+I2C_WRITE);     // set device address and write mode
     i2c_write(0x2A);                        // write address = 5
     i2c_write(0x01);                        // write value 0x75 to EEPROM
     i2c_stop();                             // set stop conditon = release bus


     // read previously written value back from EEPROM address 5 
     i2c_start_wait(MMA8452+I2C_WRITE);     // set device address and write mode

     i2c_write(0x2A);                        // write address = 5
     i2c_rep_start(MMA8452+I2C_READ);       // set device address and read mode

     ret = i2c_readNak();                    // read one byte from EEPROM
     i2c_stop();
	//_delay_ms(2000);
	  output_low(PORTB, LED);
     for(;;);
 }
		
		/*
		//-------------------------------------------------------------------
		// Read accelerometer and print X,Y,Z axis value on LCD.	
		//-------------------------------------------------------------------
		axe_value = LIS302DL_read(outX); //Read X axe value.
		itoa(axe_value, data, 10);		//Convert the X value integer into string.
		
		axe_value = LIS302DL_read(outY); //Read Y axe value.
		itoa(axe_value, data, 10);
		
		axe_value = LIS302DL_read(outZ); //Read Z axe value.
		itoa(axe_value , data, 10);
			
			
			*/
			
			
			
			
		//_delay_ms(2000);
		//output_low(PORTB, LED);		
   // }
//}
