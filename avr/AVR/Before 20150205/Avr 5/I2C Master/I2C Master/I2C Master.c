//
// master_I2C.c -   load on nerdkit with LCD
//                  note:   I2C and ATmel TWI (Two Wire Interface) are 
//                          the same thing.
//
#define F_CPU 8000000UL
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
 
#include "TWI.h"
 
// preprocessor macros for port/pin manulipulation
//
#define INPUT2(port,pin) DDR ## port &= ~_BV(pin) 
#define OUTPUT2(port,pin) DDR ## port |= _BV(pin) 
#define CLEAR2(port,pin) PORT ## port &= ~_BV(pin) 
#define SET2(port,pin) PORT ## port |= _BV(pin) 
#define TOGGLE2(port,pin) PORT ## port ^= _BV(pin) 
#define READ2(port,pin) ((PIN ## port & _BV(pin))?1:0)
//
#define INPUT(x) INPUT2(x) 
#define OUTPUT(x) OUTPUT2(x)
#define CLEAR(x) CLEAR2(x)
#define SET(x) SET2(x)
#define TOGGLE(x) TOGGLE2(x)
#define READ(x) READ2(x)
#define PULLUP_ON(x) INPUT2(x); SET2(x)
#define PULLUP_OFF(x) INPUT2(x); CLEAR2(x)
 
// define ports, pins
// wire master SCL (PC5) to slave SCL (PC5)
// wire master SDA (PC4) to slave SDA (PC4)
// put a 4.7k external pull-up resistor on SCL and another on SDA
// wire master PC1 to slave reset (PC6)
#define SLAVE_RESET     C,1
 
 
// settings for I2C
uint8_t I2C_buffer[sizeof(int)];
#define I2C_SLAVE_ADDRESS 0x1D
void handle_I2C_error(volatile uint8_t TWI_match_addr, uint8_t status);
 
// --------------------------------------------------------------------------------------------------------
int main() {
    // initialize LCD display
    
 
    // Specify startup parameters for the TWI/I2C driver
    TWI_init(   F_CPU,                      // clock frequency
                300000L,                    // desired TWI/IC2 bitrate
                I2C_buffer,                 // pointer to comm buffer
                sizeof(I2C_buffer),         // size of comm buffer
                0                           // optional pointer to callback function
                );
 
    // Enable interrupts
    sei();
 
   
 
    // send 100 test bytes
    
        TWI_master_start_write(I2C_SLAVE_ADDRESS, 0x2A);
                                     
        // wait for completion
        while(TWI_busy);
		
		TWI_master_start_write(I2C_SLAVE_ADDRESS, 0x01);
                                     
        // wait for completion
        while(TWI_busy);
 
      
     
       
        
    
 
    // done
    while(true);
}
