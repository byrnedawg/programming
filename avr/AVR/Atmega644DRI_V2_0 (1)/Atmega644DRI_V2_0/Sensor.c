/*
 * Sensor.c
 *
 * Created: 12/7/2015 9:53:56 AM
 *  Author: ByrneDawg
 */ 
#include <avr/io.h>
#include <stdlib.h>

#define MMA8452_ADDRESS 0x1D
#define FALSE	0
#define TRUE	-1

void writeRegister(uint8_t address, uint8_t data)
{
	i2cSendStart();
	i2cWaitForComplete();

	i2cSendByte((MMA8452_ADDRESS<<1)); // Write 0xB4
	i2cWaitForComplete();

	i2cSendByte(address);	// Write register address
	i2cWaitForComplete();

	i2cSendByte(data);
	i2cWaitForComplete();

	i2cSendStop();
}