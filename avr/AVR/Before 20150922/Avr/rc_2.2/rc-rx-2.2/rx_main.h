/*
 * main.h
 *
 *  Created on: 18 Feb 2010
 *      Author: duncan
 */

#ifndef MAIN_H_
#define MAIN_H_


// include any other files that are needed here
// WebbotLib includes:
#include <uart.h>
#include <rprintf.h>
#include <iopin.h>
#include <sys/atmega2561.h>
#include "servos.h"
#include "Controller/Sony/ps2.h"
//#define ADC_DEFAULT_PRESCALE ADC_PRESCALE_DIV8
#include "a2d.h"
#include "spi.h"

// non WebbotLib includes:
#include <avr/interrupt.h>			// needed because WebbotLib does not handle interrupts on INT[0-7] yet.
//#include <avr/wdt.h>				// haven't made use of this yet.

// define variables
uint8_t data[];
uint8_t data_Flag[];
uint8_t EEMEM data_eeprom[];		// wasteful but we have plenty.
uint8_t buttonPressed[];
uint8_t buttonReleased[];
uint16_t servoBuffer[];				// 16bit buffer containing current servo position.

uint16_t cyrfRssiCounter;			// store total of RSSI measurement to get average over 1 second.

int x_gyro_offset;					// offset gyro data to center round "0"
int y_gyro_offset;


// Define constants for buttons.
// There are a similar set of defines in WebbotLib's "Controller/Sony/ps2.h" that can be ORed together
// but here we need a list from 0x00 to 0x0F
#define	PS2_B_BTN_SELECT 		0x00
#define	PS2_B_BTN_L3     		0x01
#define	PS2_B_BTN_R3     		0x02
#define	PS2_B_BTN_START  		0x03
#define	PS2_B_DPAD_UP    		0x04
#define	PS2_B_DPAD_RIGHT 		0x05
#define	PS2_B_DPAD_DOWN  		0x06
#define	PS2_B_DPAD_LEFT  		0x07
#define	PS2_B_BTN_L2     		0x08
#define	PS2_B_BTN_R2     		0x09
#define	PS2_B_BTN_L1     		0x0A
#define	PS2_B_BTN_R1     		0x0B
#define	PS2_B_BTN_TRIANGLE		0x0C
#define	PS2_B_BTN_CIRCLE  		0x0D
#define	PS2_B_BTN_X     		0x0E
#define	PS2_B_BTN_SQUARE  		0x0F


// Create the datatype for the CYRF6936 RF module in an attempt to use WebbotLib's SPI code.
typedef struct s_CYPRESS_CYRF {SPI_ABSTRACT_DEVICE _device_ ;} CYPRESS_CYRF ;


// container for accelerometer readings.
struct RACC {
	volatile float RXACC;
	volatile float RYACC;
	volatile float RZACC;
};

// container for aircraft attitude. (Roll and Pitch)
struct ATTITUDE {
	volatile float AXZ;			// rotation around the Y axis.
	volatile float AYZ;			// rotation around the X axis.
};


// initialise cyrf6936 RF module in 8DR mode.
void cyrf6936_Initialise_hard();
void cyrf6936_Initialise_soft(CYPRESS_CYRF* cyrf);

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
uint8_t cyrf6936_RX_test(CYPRESS_CYRF* cyrf);

// Transition cyrf6936 into receive mode.
// The cyrf6936 will then automatically put any incoming data in RX buffer.
void cyrf6936_RX_mode(CYPRESS_CYRF* cyrf);

// Transition cyrf6936 into transmit mode.
// This function does not actually transmit the data. (See cyrf6936_TX() for that.)
void cyrf6936_TX_mode(CYPRESS_CYRF* cyrf);

// Transmits data in cyrf6936 buffer.
// Returns 0 if successful.
// Returns 1 if there was a TX error.
uint8_t cyrf6936_TX(CYPRESS_CYRF* );

// WebbotLib does not handle interrupts on INT[0-7] yet.
// this function enables INT7 to interrupt on a falling edge.
// this interrupt is triggered when cyrf_0 receives data.
void cyrfIntEnable(void);

// this function is called by the ISR when a cyrf6936 triggers an AVR interrupt due to incoming data.
void cyrfInterupt();

// WebbotLib does not handle interrupts on INT[0-7] yet.
// this function is a the Interrupt Service Routine for INT7.
// this interrupt is triggered when cyrf_0 receives data.
ISR(INT7_vect);

// Read data from cyrf6936 buffer into program data space.
// The first byte received is the address in the data[] structure to store the received data.
// The 2nd byte received is the address in the data[] structure of data to be transmitted in reply packet at the next opportunity.
void cyrf6936_RX_ReadBuffer(uint8_t *data, CYPRESS_CYRF *cyrf);

// Puts data to be transmitted in buffer from the data[] array.
// data[tx_data_position] to data[tx_data_position + tx_data_len] will be loaded into the TX buffer.
void cyrf6936_TX_FillBuffer(uint8_t *data, uint8_t tx_data_position, uint8_t tx_data_len, CYPRESS_CYRF* cyrf);

// This gets called every 20ms frame to inform main program loop if RF packet has been received.
// (Received packet would have been handled by an interrupt so main loop will be unaware.)
uint8_t testIfPacketReceived(uint8_t *);

// RSSI is the Received Signal Strength Indicator.
// this returns the signal strength of the last received transmission.
uint8_t cyrf6936_Get_RSSI();

// rules for dealing with each servo.
void set_servos(uint8_t *data);

// put some sensible defaults in EEPROM memory for trim settings.
void reset_trims(void);

// set trims and save to EEPROM
void set_trims(uint8_t *);

// this should be called when aircraft is stationary and level to set centre points for sensors.
void setSensorDefaults(uint8_t *data);


#endif /* MAIN_H_ */
