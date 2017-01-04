/*
 * MMA8452_Advance.c
 *
 * Created: 9/12/2014 10:45:22 AM
 *  Author: gregory.byrne
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include "i2c.h"
#include "GB_Display.h"
#include "GB_usart.h"


#define BAUDRATE 19200
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1) 


#define MMA8452_ADDRESS 0x1D
#define FALSE	0
#define TRUE	-1

#define LED PB1
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)

#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

const uint8_t SCALE = 2;  // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.
// Set the output data rate below. Value should be between 0 and 7
const uint8_t dataRate = 0;  // 0=800Hz, 1=400, 2=200, 3=100, 4=50, 5=12.5, 6=6.25, 7=1.56

int accelCount[3];  // Stores the 12-bit signed value
float accelG[3];  // Stores the real accel value in g's
float accelD[3];  // Stores the real accel value in degrees
float buns = 1.75;


static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

/********************************************************************************
Main
********************************************************************************/


int main(void)
{
	stdout = &mystdout;
	USARTInit(UBRRVAL);
	DisplayInit();
	Command_Display(Clear_Screen);
	Command_Display(Go_Home);
	printf("I2C");
	Set_Cursor_Position(1,2);
	printf("MMA8452");
	_delay_ms(2000);
	set_output(DDRB, LED);
	void setup();
	MMA8452Active();

    while(1)
    {
		//static uint8_t source;

  // If int1 goes high, all data registers have new data
  //if (digitalRead(int1Pin)==1)  // Interrupt pin, should probably attach to interrupt function
 // {
		readAccelData(accelCount);  // Read the x/y/z adc values

		// Now we'll calculate the accleration value into actual g's
		Command_Display(Clear_Screen);
		Command_Display(Go_Home);
		//printf("Buns = %.2f", buns);
		//_delay_ms(2000);
		for (int i=0; i<3; i++)
		{
			accelG[i] = (float)accelCount[i]/((1<<12)/(2*SCALE));  // get actual g value, this depends on scale being set
			accelD[i] = asin(accelG[i])*57.2957795;
		}			
			//switch (i)
			//{
			//case 0 : 
				//printf("X:%.2f ", accelG[0]);
				printf("X:%.2f\n", accelD[0]);
				//break;
			//case 1 : 
				//printf("Y:%.2f\n", accelG[1]);
			//	break;
			//case 2 : 
				printf("Z:%.2f", accelG[2]);
				//break;
			//}
			//printf("Numbers %d\n", accelG[i]);
			_delay_ms(500);
			//itoa(accelG[i], aNumberAsString, 16);
			//UWriteString(aNumberAsString);	
			//Set_Cursor_Position(1,2);
			
				//UWriteString(Space);
			
			//Command_Display(Go_Home);
			
		
		/*
		uint8_t rawData[6];  // x/y/z accel register data stored here

		readRegisters(0x01, 6, &rawData[0]);  // Read the six raw dat registers into data array
		
		
		Command_Display(Clear_Screen);
		Command_Display(Go_Home);
		for (int i=0; i<6; i++)
		{	
			//accelG[i] = accelCount[i] << 4;  // get actual g value, this depends on scale being set
			//ltoa(accelG[i], aNumberAsString, 16);
			itoa(rawData[i], aNumberAsString, 16);
			UWriteString(aNumberAsString);	
			//Set_Cursor_Position(1,2);
			if((i % 2) == 1)
			{
				UWriteString(Space);
			}
			//Command_Display(Go_Home);
		}
   */
		//output_high(PORTB, LED);
		//_delay_ms(750);
		
		//output_low(PORTB, LED);
		//_delay_ms(750);
		
		//_delay_ms(1000);
	}		
		
}

void DisplayInit(void) // Power on variable initialization 
{
	Change_Baudrate(51); // 19200
	Command_Display(Clear_Screen);
	Command_Display(Go_Home);
	//Change_Baudrate(83); // 1200	
}

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

void MMA8452Active()
{
  unsigned char c = readRegister(0x2A);
  writeRegister(0x2A, c | 0x01);
}

void MMA8452Standby()
{
  unsigned char c = readRegister(0x2A);
  writeRegister(0x2A, c & ~(0x01));
}

void setup()
{
  unsigned char c;

  // Read the WHO_AM_I register, this is a good test of communication
  c = readRegister(0x0D);  // Read WHO_AM_I register
  if (c == 0x2A) // WHO_AM_I should always be 0x2A
  {  
    initMMA8452(SCALE, dataRate);  // init the accelerometer if communication is OK
  }
  else
  {
    while(1)
	{
		output_high(PORTB, LED);
		_delay_ms(500);
		
		output_low(PORTB, LED);
		_delay_ms(500);
	} // Loop forever if communication doesn't happen
  }
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

