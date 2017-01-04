/*
 * I2C_Slave.c
 *
 * Created: 9/9/2014 3:28:46 PM
 *  Author: gregory.byrne
 */ 

//
// slave_I2C.c
//
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
 
#include "TWI.h"
 
// settings for I2C
uint8_t I2C_buffer[25];
#define I2C_SLAVE_ADDRESS 0x53
void handle_I2C_interrupt(volatile uint8_t TWI_match_addr, uint8_t status);
 
// --------------------------------------------------------------------------------------------------------
int main() {
    // Initialize I2C
    TWI_init(   F_CPU,                      // clock frequency
                100000L,                    // desired TWI/IC2 bitrate
                I2C_buffer,                 // pointer to comm buffer
                sizeof(I2C_buffer),         // size of comm buffer
                &handle_I2C_interrupt       // pointer to callback function
                );
 
    // Enable interrupts
    sei();
 
    // give our slave address and enable I2C
    TWI_enable_slave_mode(  I2C_SLAVE_ADDRESS,      // device address of slave
                            0,                      // slave address mask
                            0                       // enable general call
                        );
 
    // received data is processed in the callback
    // nothing else to do here
    while(true){
    }
}
// --------------------------------------------------------------------------------------------------------
//
void handle_I2C_interrupt(volatile uint8_t TWI_match_addr, uint8_t status){
    if(status==TWI_success){
        // increment the integer in the buffer
        // and it will be returned during the read cycle
        (*(int*)I2C_buffer)++;
    }
}
