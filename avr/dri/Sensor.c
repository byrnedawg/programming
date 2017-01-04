/*
 * Sensor.c
 *
 * Created: 12/7/2015 9:53:56 AM
 *  Author: ByrneDawg
 */ 
#include "Sensor.h"

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

unsigned char readRegister(uint8_t address)
{
	unsigned char data;

	i2cSendStart();
	i2cWaitForComplete();

	i2cSendByte((MMA8452_ADDRESS<<1)); // Write 0xB4
	i2cWaitForComplete();

	i2cSendByte(address);	// Write register address
	i2cWaitForComplete();

	i2cSendStart();

	i2cSendByte((MMA8452_ADDRESS<<1)|0x01); // Write 0xB5
	i2cWaitForComplete();
	i2cReceiveByte(TRUE);
	i2cWaitForComplete();

	data = i2cGetReceivedByte();	// Get MSB result
	i2cWaitForComplete();
	i2cSendStop();

	cbi(TWCR, TWEN);	// Disable TWI
	sbi(TWCR, TWEN);	// Enable TWI

	return data;
}

void readRegisters(uint8_t address, int i, uint8_t * dest)
{
	i2cSendStart();
	i2cWaitForComplete();

	i2cSendByte((MMA8452_ADDRESS<<1)); // write 0xB4
	i2cWaitForComplete();

	i2cSendByte(address);	// write register address
	i2cWaitForComplete();

	i2cSendStart();
	i2cSendByte((MMA8452_ADDRESS<<1)|0x01); // write 0xB5
	i2cWaitForComplete();
	for (int j=0; j<i; j++)
	{
		i2cReceiveByte(TRUE);
		i2cWaitForComplete();
		dest[j] = i2cGetReceivedByte(); // Get MSB result
	}
	i2cWaitForComplete();
	i2cSendStop();

	cbi(TWCR, TWEN); // Disable TWI
	sbi(TWCR, TWEN); // Enable TWI
}

void MMA8452Active(void)
{
	unsigned char c = readRegister(0x2A);
	writeRegister(0x2A, c | 0x01);
}

void MMA8452Standby(void)
{
	unsigned char c = readRegister(0x2A);
	writeRegister(0x2A, c & ~(0x01));
}

void initMMA8452(uint8_t fsr, uint8_t dataRate)
{
  MMA8452Standby();  // Must be in standby to change registers

  // Set up the full scale range to 2, 4, or 8g.
  if ((fsr==2)||(fsr==4)||(fsr==8))
    writeRegister(0x0E, fsr >> 2);  
  else
    writeRegister(0x0E, 0);

  // Setup the 3 data rate bits, from 0 to 7
  writeRegister(0x2A, readRegister(0x2A) & ~(0x38));
  if (dataRate <= 7)
    writeRegister(0x2A, readRegister(0x2A) | (dataRate << 3));  

  // Set up portrait/landscap registers - 4 steps:
  // 1. Enable P/L
  // 2. Set the back/front angle trigger points (z-lock)
  // 3. Set the threshold/hysteresis angle
  // 4. Set the debouce rate
  // For more info check out this app note: http://cache.freescale.com/files/sensors/doc/app_note/AN4068.pdf
  writeRegister(0x11, 0x40);  // 1. Enable P/L
  writeRegister(0x13, 0x44);  // 2. 29deg z-lock (don't think this register is actually writable)
  writeRegister(0x14, 0x84);  // 3. 45deg thresh, 14deg hyst (don't think this register is writable either)
  writeRegister(0x12, 0x50);  // 4. debounce counter at 100ms (at 800 hz)

  /* Set up single and double tap - 5 steps:
   1. Set up single and/or double tap detection on each axis individually.
   2. Set the threshold - minimum required acceleration to cause a tap.
   3. Set the time limit - the maximum time that a tap can be above the threshold
   4. Set the pulse latency - the minimum required time between one pulse and the next
   5. Set the second pulse window - maximum allowed time between end of latency and start of second pulse
   for more info check out this app note: http://cache.freescale.com/files/sensors/doc/app_note/AN4072.pdf */
  writeRegister(0x21, 0x7F);  // 1. enable single/double taps on all axes
  // writeRegister(0x21, 0x55);  // 1. single taps only on all axes
  // writeRegister(0x21, 0x6A);  // 1. double taps only on all axes
  writeRegister(0x23, 0x20);  // 2. x thresh at 2g, multiply the value by 0.0625g/LSB to get the threshold
  writeRegister(0x24, 0x20);  // 2. y thresh at 2g, multiply the value by 0.0625g/LSB to get the threshold
  writeRegister(0x25, 0x08);  // 2. z thresh at .5g, multiply the value by 0.0625g/LSB to get the threshold
  writeRegister(0x26, 0x30);  // 3. 30ms time limit at 800Hz odr, this is very dependent on data rate, see the app note
  writeRegister(0x27, 0xA0);  // 4. 200ms (at 800Hz odr) between taps min, this also depends on the data rate
  writeRegister(0x28, 0xFF);  // 5. 318ms (max value) between taps max

  // Set up interrupt 1 and 2
  writeRegister(0x2C, 0x02);  // Active high, push-pull interrupts
  writeRegister(0x2D, 0x19);  // DRDY, P/L and tap ints enabled
  writeRegister(0x2E, 0x01);  // DRDY on INT1, P/L and taps on INT2

  MMA8452Active();  // Set to active to start reading
}

void readAccelData(int * destination)
{
	uint8_t rawData[6];  // x/y/z accel register data stored here

	readRegisters(0x01, 6, &rawData[0]);  // Read the six raw data registers into data array

	// Loop to calculate 12-bit ADC and g value for each axis
	for (int i=0; i<6; i+=2)
	{
		destination[i/2] = ((rawData[i] << 8) | rawData[i+1]) >> 4;  // Turn the MSB and LSB into a 12-bit value
		if (rawData[i] > 0x7F)
		{
			// If the number is negative, we have to make it so manually (no 12-bit data type)
			destination[i/2] = ~destination[i/2] + 1;
			destination[i/2] *= -1;  // Transform into negative 2's complement #
		}
	}
}