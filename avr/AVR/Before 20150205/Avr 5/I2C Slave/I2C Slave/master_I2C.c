//
// master_I2C.c -   load on nerdkit with LCD
//                  note:   I2C and ATmel TWI (Two Wire Interface) are 
//                          the same thing.
//
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
 
#include "../libnerdkits/lcd.h"
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
 
// LCD stream file - enable printf in functions outside of main()
FILE lcd_stream;
 
// settings for I2C
uint8_t I2C_buffer[sizeof(int)];
#define I2C_SLAVE_ADDRESS 0x10
void handle_I2C_error(volatile uint8_t TWI_match_addr, uint8_t status);
 
// --------------------------------------------------------------------------------------------------------
int main() {
    // initialize LCD display
    lcd_init();
    fdev_setup_stream(&lcd_stream, lcd_putchar, 0, _FDEV_SETUP_WRITE); 
    lcd_clear_and_home();
    fprintf_P(&lcd_stream, PSTR("master_I2C"));
 
    // Specify startup parameters for the TWI/I2C driver
    TWI_init(   F_CPU,                      // clock frequency
                300000L,                    // desired TWI/IC2 bitrate
                I2C_buffer,                 // pointer to comm buffer
                sizeof(I2C_buffer),         // size of comm buffer
                0                           // optional pointer to callback function
                );
 
    // Enable interrupts
    sei();
 
    // reset the slave for a clean start
    OUTPUT(SLAVE_RESET);
    CLEAR(SLAVE_RESET);
    _delay_ms(500);
    SET(SLAVE_RESET);
    _delay_ms(500);
 
    // send 100 test bytes
    int i;
    for(i=1;i<100;i++){
        // set value into buffer
        *(int*)I2C_buffer=i;
 
        // transmit
        TWI_master_start_write_then_read(   I2C_SLAVE_ADDRESS,  // slave device address
                                            sizeof(I2C_buffer), // number of bytes to write
                                            sizeof(I2C_buffer)  // number of bytes to read
                                            );
 
        // wait for completion
        while(TWI_busy);
 
        // if error, notify and quit
        if(TWI_error){
            lcd_goto_position(2,0);
            fprintf_P(&lcd_stream, PSTR("TWI error at %d"), i); 
            break;
        }
 
        // check result
        lcd_goto_position(1,0);
        if(*(int*)I2C_buffer==(i+1))
            fprintf_P(&lcd_stream, PSTR("%d OK"),i); 
        else {
            fprintf_P(&lcd_stream, PSTR("Error at byte %d"), i); 
            lcd_goto_position(2,0);
            fprintf_P(&lcd_stream, PSTR("expected %d, got %d"), i, *(int*)I2C_buffer); 
            break;
        }   
    }
 
    // done
    while(true);
}
// --------------------------------------------------------------------------------------------------------
//
