/*
 * Sensor.h
 *
 * Created: 12/7/2015 10:06:23 AM
 *  Author: ByrneDawg
 */ 

#ifndef SENSOR_H
#define SENSOR_H

#include <avr/io.h>
#include <stdlib.h>
#include "defs.h"

//#include "i2c.h"

//#define BV(bit)			(1<<(bit))
//#define cbi(reg,bit)	reg &= ~(BV(bit))
//#define sbi(reg,bit)	reg |= (BV(bit))
#define MMA8452_ADDRESS 0x1D
#define FALSE	0
#define TRUE	-1

void writeRegister(uint8_t address, uint8_t data);

unsigned char readRegister(uint8_t address);

void readRegisters(uint8_t address, int i, uint8_t * dest);

void MMA8452Active(void);

void MMA8452Standby(void);

void initMMA8452(uint8_t fsr, uint8_t dataRate);

void readAccelData(int * destination);

#endif