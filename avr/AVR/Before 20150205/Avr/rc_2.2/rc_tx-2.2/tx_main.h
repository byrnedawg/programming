/*
 * main.h
 *
 *  Created on: 19 Feb 2010
 *      Author: duncan
 */

#ifndef MAIN_H_
#define MAIN_H_


// Now include any other files that are needed here
#include <sys/atmega2561.h>
#include <uart.h>
#include <rprintf.h>
#include <iopin.h>
#include <spi.h>
#include <Controller/Sony/ps2.h>
//#define ADC_DEFAULT_PRESCALE ADC_PRESCALE_DIV32
#include "a2d.h"
#include <spisw.h>
#include <spi.h>

// non WebbotLib includes:
#include <avr/interrupt.h>			// needed because WebbotLib does not handle interrupts on INT[0-7] yet.
//#include <avr/wdt.h>				// haven't made use of this yet.

// define variables
uint8_t data[];

// Create the datatype for the CYRF6936 RF module in an attempt to use WebbotLib's SPI code.
typedef struct s_CYPRESS_CYRF {SPI_ABSTRACT_DEVICE _device_ ;} CYPRESS_CYRF ;



// define functions.

// initialise cyrf6936 RF module in 8DR mode.
void cyrf6936_Initialise();

// Perform an SPI read on cyrf6936 RF module.
// Will read from register "register_address".
uint8_t cyrf_SPI_Read(uint8_t register_address, CYPRESS_CYRF* cyrf);

// Perform an SPI write on cyrf6936 RF module.
// Will write "data" to register "register_address".
void cyrf_SPI_Write(uint8_t register_address, uint8_t data, CYPRESS_CYRF* cyrf);

// Check if data has been received on cyrf6936 RF module.
// Returns 0 if data received with no errors.
// Returns 1 if no data received.
// Returns >1 if data received with errors.
uint8_t cyrf6936_RX_test();

// Transition cyrf6936 into receive mode.
// The cyrf6936 will then automatically put any incoming data in RX buffer.
void cyrf6936_RX_mode();

// Transition cyrf6936 into transmit mode.
// This function does not actually transmit the data. (See cyrf6936_TX() for that.)
void cyrf6936_TX_mode();

// Read data from cyrf6936 buffer into program data space.
// The first byte received is the address in the data[] structure to store the received data.
// The 2nd byte received is the address in the data[] structure of data to be transmitted in reply packet at the next opportunity.
void cyrf6936_RX_ReadBuffer(uint8_t *data, CYPRESS_CYRF* cyrf);

// Puts data to be transmitted in buffer from the data[] array.
// data[tx_data_position] to data[tx_data_position + tx_data_len] will be loaded into the TX buffer.
void cyrf6936_TX_FillBuffer(uint8_t *data, uint8_t tx_data_position, uint8_t tx_data_len, CYPRESS_CYRF* cyrf);

// Transmits data in cyrf6936 buffer.
// Returns 0 if successful.
// Returns 1 if there was a TX error.
uint8_t cyrf6936_TX();

// sends data out the USB port.
void dataOut(uint8_t *data);

void cyrfInterupt(void);
void cyrfIntEnable(void);
void testIfPacketReceived(uint8_t *);
uint8_t cyrf6936_Get_RSSI();
uint8_t transmit_Packet(uint8_t *, CYPRESS_CYRF* cyrf);
uint8_t transmit_Packet2(uint8_t *);
uint8_t data_Synchronise(uint8_t *, TICK_COUNT , uint8_t, CYPRESS_CYRF* cyrf);
void store_RSSI(uint8_t *, CYPRESS_CYRF* cyrf);
void update_Counters(uint8_t *, CYPRESS_CYRF* cyrf);
uint8_t service_PS2_Controller(uint8_t *);
uint8_t service_Joystick(uint8_t *data);


#endif /* MAIN_H_ */
