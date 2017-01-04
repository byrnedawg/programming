/*
 * Atmega8DRI.c
 *
 * Created: 11/25/2015 8:51:47 AM
 *  Author: ByrneDawg
 */ 


#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <math.h>
#include "i2c.h"
#include "lcd_lib.h"

#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);

#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);

#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)

#define Switch1 PC3
#define Switch2 PB1
#define Switch3 PB3
#define GreenLED PB2

#define PRINT_INT_BUFSIZE	16

// i2C Stuff
#define MMA8452_ADDRESS 0x1D
#define FALSE	0
#define TRUE	-1
const uint8_t SCALE = 2;  // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.
// Set the output data rate below. Value should be between 0 and 7
const uint8_t dataRate = 0;  // 0=800Hz, 1=400, 2=200, 3=100, 4=50, 5=12.5, 6=6.25, 7=1.56

int accelCount[3];  // Stores the 12-bit signed value
float accelG[3];  // Stores the real accel value in g's
float accelD[3];  // Stores the real accel value in degrees
float buns = 1.75;

//i2C Stuff

volatile uint16_t timer0Overflows = 0; // Stores the value for timer polling timer 0
volatile uint8_t switch_state = 1;	// Light Function state
volatile int16_t i = -1024;
volatile int16_t d = 1;
volatile uint8_t BTN_Down = 0;
volatile uint8_t updateSpeed = 32;
volatile uint32_t system_Clock = 0;
volatile uint8_t system_Second = 0;
volatile uint8_t system_Minute = 0;
volatile uint8_t system_Hour = 0;
volatile uint8_t system_Day = 0;


//Strings stored in AVR Flash memory
const uint8_t LCDanimation[] PROGMEM=" Greg Byrne \0";
const uint8_t OnScreen[] PROGMEM= "DRI V1.0";
const uint8_t State0[] PROGMEM= "State 0";
const uint8_t State1[] PROGMEM= "State 1";
const uint8_t State2[] PROGMEM= "State 2";
const uint8_t State3[] PROGMEM= "State 3";
const uint8_t State4[] PROGMEM= "State 4";
const uint8_t State5[] PROGMEM= "State 5";
const uint8_t State6[] PROGMEM= "State 6";
uint8_t numberDisplay[5]=" 0000";

// additional custom LCD characters
const uint8_t backslash[8] PROGMEM=
{
	0b00000000,//back slash
	0b00010000,
	0b00001000,
	0b00000100,
	0b00000010,
	0b00000001,
	0b00000000,
	0b00000000
};
void clearLCDScreen(uint8_t x, uint8_t y, uint8_t numchar  )
{
	LCDGotoXY(x, y);
	for(uint8_t j = 0; j<numchar; j++)
	{
		LCDsendChar(' ');
	}
}
void printTime(uint8_t x, uint8_t y)
{
	LCDGotoXY(x, y);
	char buf[PRINT_INT_BUFSIZE];
	
	buf[10] = system_Day / 10 +48 ;
	buf[9] =  system_Day % 10 +48 ;
	buf[8] =  ':' ;
	buf[7] = system_Hour / 10 +48 ;
	buf[6] = system_Hour % 10 +48 ;
	buf[5] =  ':' ;
	buf[4] = system_Minute / 10 +48 ;
	buf[3] =  system_Minute % 10 +48 ;
	buf[2] =  ':' ;
	buf[1] = system_Second / 10 +48 ;
	buf[0] =  system_Second % 10 +48 ;
	
	for(int k = 10; k >= 0; k--)
	{
		LCDsendChar(buf[k]);
	}
}
void printAccel(uint8_t x, uint8_t y, int accel)
{
	LCDGotoXY(x, y);
	char buf[PRINT_INT_BUFSIZE];
	int8_t sign = 1;
	if (accel < 0) { // look for the sign
		sign = -1;
		accel= -accel;
	}
	
	if (sign < 0)
	{	// don't forget to add the sign
		buf[3] = '-';
	}
	else
	{
		buf[3] = ' ';
	}
	buf[2] = accel / 100 +48 ;
	buf[1] = (accel / 10)%10 +48 ;
	buf[0] =  accel % 10 +48 ;
	
	for(int k = 3; k >= 0; k--)
	{
		LCDsendChar(buf[k]);
	}
}
void printInt(uint8_t x, uint8_t y, int32_t i)
{
	LCDGotoXY(x, y);
	char buf[PRINT_INT_BUFSIZE];
	int8_t sign = 1;
	//int8_t len = 0;
	if (i < 0) { // look for the sign
		sign = -1;
		i = -i;
	}
	
	buf[3] = i / 1000 + 48;
	buf[2] = (i / 100)%10 +48 ;
	buf[1] = (i / 10)%10 +48 ;
	buf[0] =  i % 10 +48 ;
	
	if (sign < 0) 
	{	// don't forget to add the sign
		buf[4] = '-';
	}
	else
	{
		buf[4] = ' ';
	}
	
	if(i < 1000)
	{
		buf[3] = ' ';
	}
	if(i < 100)
	{
		buf[2] = ' ';
	}
	if(i < 10)
	{
		buf[1] = ' ';
	}
	LCDsendChar(buf[4]);
	LCDsendChar(buf[3]);
	LCDsendChar(buf[2]);
	LCDsendChar(buf[1]);
	LCDsendChar(buf[0]);
}

void update_Display(void) //
{
	if(switch_state == 0 ) //
	{
		CopyStringtoLCD(State0, 3, 0);
		printInt(11, 0 , i);
		printInt(5, 1 , system_Clock);
		output_low(PORTB, GreenLED);
		updateSpeed = 32;
		
	}
	if(switch_state == 1)	//Red
	{
		CopyStringtoLCD(State1, 3, 0);
		printInt(11, 0 , i);
		output_high(PORTB, GreenLED);
		updateSpeed = 16;
	}
	else if(switch_state == 2)	//Orange
	{
		CopyStringtoLCD(State2, 3, 0);
		printInt(11, 0 , i);
		printTime(3,1);
		output_low(PORTB, GreenLED);
		updateSpeed = 8;
	}
	else if(switch_state == 3)	//Yellow
	{
		CopyStringtoLCD(State3, 3, 0);
		printInt(11, 0 , i);
		printInt(5, 1 , system_Clock);
		output_high(PORTB, GreenLED);
		updateSpeed = 4;
	}
	
	else if(switch_state == 4)	//Green
	{
		CopyStringtoLCD(State4, 3, 0);
		
		printAccel(0, 1 , (int)accelD[0]);
		
		printAccel(5, 1 , (int)accelD[1]);
		
		printAccel(12, 1 , (int)accelD[2]);
		
		output_low(PORTB, GreenLED);
		updateSpeed = 2;
	}
	else if(switch_state == 5)	//Blue
	{
		CopyStringtoLCD(State5, 3, 0);
		printInt(11, 0 , i);
		printInt(5, 1 , system_Clock);
		output_high(PORTB, GreenLED);
		updateSpeed = 1;
	}
	else if(switch_state == 6)	//Blue
	{
		CopyStringtoLCD(State6, 3, 0);
		printInt(11, 0 , i);
		printInt(5, 1 , system_Clock);
		output_low(PORTB, GreenLED);
		updateSpeed = 64;
	}
	else
	{
		CopyStringtoLCD(State0, 3, 0);
		printInt(11, 0 , i);
		printInt(5, 1 , system_Clock);
		output_low(PORTB, GreenLED);
		updateSpeed = 32;
		switch_state = 0; // set switch state to 0 if the state is above 9
	}

}

ISR(TIMER0_OVF_vect) // timer 0 overflow interrupt
{
	TCNT0 += 6;
	if((timer0Overflows % 8) == 0) // push button polling 195 1 sec /975 5 sec/ 11700 1 min/
	{
		if(is_clear(PINC, Switch1)) // if the push button is held down connecting pin B2 to ground
		{
			BTN_Down = 1; // button down is true
		}
	}
	if((timer0Overflows % 16) == 0) // push button polling on the release
	{

		if(is_set(PINC, Switch1) && BTN_Down == 1) //
		{
			switch_state++; // increment mode states by 1 i.e. red -> yellow, green - > blue
			BTN_Down = 0; // push button is released so the button is no longer held down
			LCDclr();
		}
	}
	if((timer0Overflows % 8) == 0) // push button polling on the release % 16
	{
		
	}
	if((timer0Overflows % 32) == 0) // push button polling on the release
	{
		
		system_Clock++;
		system_Second++;
		
		if((system_Second % 60) == 0) //
		{
			system_Minute++;
			system_Second = 0;
			if((system_Minute % 60) == 0)
			{
				system_Hour++;
				system_Minute = 0;
				if((system_Hour % 24) == 0)
				{
					system_Day++;
					system_Hour = 0;
				}
			}
		}
		
		if(is_clear(PINB, Switch2)) // if the push button is held down connecting pin B2 to ground
		{
			switch_state--;
			LCDclr();
		}
		if(is_clear(PINB, Switch3)) // if the push button is held down connecting pin B2 to ground
		{
			switch_state++;
			LCDclr();
		}
		readAccelData(accelCount);
		for (int i=0; i<3; i++)
		{
			accelG[i] = (float)accelCount[i]/((1<<12)/(2*SCALE));  // get actual g value, this depends on scale being set
			accelD[i] = asin(accelG[i])*57.2957795;
		}
		update_Display();
	}
	if((timer0Overflows % updateSpeed) == 0) // push button polling on the release
	{
		if (i == 1023) d = -1;   // count down
		if (i == -1024) d = 1;   // count up
		i += d;
		update_Display();
	}
	timer0Overflows++;
}

void timer0_init(void) // Initialize timer 0
{
	TIMSK |= (1<<TOIE0);
	TCCR0 |= (1<<CS02)|(0<<CS01)|(1<<CS00); //

	timer0Overflows = 0; // Start timer overflows at 0
}

//delay 1s
void delay1s(void)
{
	uint8_t i;
	for(i=0;i<100;i++)
	{
		_delay_ms(10);
	}
}
//demonstration of progress bar
void progress(void)
{
	LCDclr();
	CopyStringtoLCD(OnScreen, 4, 0);
	/*
	LCDsendChar('D');
	delay1s();
	LCDsendChar('R');
	delay1s();
	LCDsendChar('I');
	delay1s();
	LCDsendChar(' ');
	delay1s();
	LCDsendChar('V');
	delay1s();
	LCDsendChar('1');
	delay1s();
	LCDsendChar('.');
	delay1s();
	LCDsendChar('0');
	*/
	delay1s();
	for(uint8_t i=0;i<255;i++)
	{
		_delay_ms(10);
		LCDGotoXY(0, 1);
		LCDprogressBar(i, 255, 16);
	}
	LCDclr();
}
//demonstration of animation
void demoanimation(void)
{
	LCDclr();
	LCDdefinechar(backslash,0);
	CopyStringtoLCD(LCDanimation, 0, 0);
	for(uint8_t i=0;i<3;i++)
	{
		LCDGotoXY(8, 1);
		LCDsendChar(0);
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('-');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('/');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('|');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar(8);//backslash
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('-');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('/');
		delay1s();
		LCDGotoXY(8, 1);
		LCDsendChar('|');
		delay1s();
	}
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
void setupI2C(void)
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
			output_high(PORTB, GreenLED);
			_delay_ms(500);
			
			output_low(PORTB, GreenLED);
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

int main(void)
{

	set_input(DDRC, Switch1);
	set_input(DDRB, Switch2);
	set_input(DDRB, Switch3);
	set_output(DDRB, GreenLED);

	LCDinit();//init LCD bit, dual line, cursor right
	LCDclr();//clears LCD
	progress();
	delay1s();
	setupI2C();
	MMA8452Active();
	timer0_init();
	sei();
	
	while(1)//loop demos
	{
		//progress();
		//delay1s();
		//demoanimation();
		//_delay_ms(50);
	}
}