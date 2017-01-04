/*
 * Magno_Sensor.c
 *
 * Created: 8/22/2014 9:33:40 AM
 *  Author: gregory.byrne
 */ 

/*
 * RI_Test_Stepper_Control.c
 *
 * Created: 4/2/2014 1:55:10 PM
 *  Author: gregory.byrne
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/eeprom.h> 
#include "GB_usart.c"
#include "i2csoft.c"

#define MAG_ADDR  0x0E //7-bit address for the MAG3110, doesn't change

#define 	CMD				254 
#define		Bauderate       57
#define 	Backspace_Left 	8
#define 	Move_Right 		9
#define		Line_Feed 		11 
#define 	New_Page		12
#define		Cariage_Return	13 
#define 	Clear_Screen	88
#define		Go_Home			72
#define 	New_Char		78
#define		Down_Arrow		0
#define		Up_Arrow		1
#define		Display_On		66
#define		Display_OFF		70
#define		Set_Font		49
#define		Small_Filled	1
#define		Futura_16		2
#define 	Cursor			71
#define		Cursor_Cord		121
#define		Auto_Scroll_On	81
#define		Auto_Scroll_Off	82
#define		Drawing_Color	99
#define		Draw_Pixel		112
#define		Drawing_Line	108
#define		Continue_Line	101
#define		Draw_Rectangle	114
#define		DRW_Solid_Rect	120
#define		Touch_Calibrat 	139
#define		Touch_Region	132
#define		Touch_Mode		135
#define		Co_ordinate		1
#define		Region			0

#define sbi(x,y) x |= _BV(y) //set bit (original sbi)
#define cbi(x,y) x &= ~(_BV(y)) //(original)cbi clear bit //reverse cause of 3904
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) (x & _BV(y)) == _BV(y) //(for input) checks if the // input is high (Logic 1) i.e. if(is_high(PIND,PD0))						
#define RESET &=~
#define SET |=
#define TOGGLE ^=

#define BAUDRATE 19200
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

uint32_t sensor_Data = 0; // 0
uint32_t aNumberAsString[10];
uint8_t No_Data_String[]= "No Data";
uint8_t pause_String[]= "Paused";
uint8_t uTeslas_String[]= "uTeslas";
uint8_t start_Screen1[]= "Sensor";
uint8_t start_Screen2[]= "Display";
uint8_t first_Pause = 0;


void Command_Display(uint8_t command)
{
	UWriteData(CMD);
	UWriteData(command);
}

void Change_Baudrate(uint8_t speed)
{	
	UWriteData(CMD);
	UWriteData(Bauderate);
	UWriteData(speed);
}

void Set_Cursor_Position(uint8_t column, uint8_t row)
{	
	UWriteData(CMD);
	UWriteData(71);
	UWriteData(column);
	UWriteData(row);
}

void PortInit(void) // initialize port B inputs and outputs 
{
	DDRB = (0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);// set PB1  PB2 Enable pins on L298n PB0 motor drive  
	PORTB = (0<<PB7) | (0<<PB6) | (0<<PB5) | (0<<PB4) | (0<<PB3) | (0<<PB2) | (0<<PB1) | (0<<PB0); // set all pins we are using high
	
	DDRD = 0xFF;
	DDRD = (1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);// set PD7 - PD5 motor drive  
	PORTD = (0<<PD7) | (0<<PD6) | (0<<PD5) | (0<<PD4) | (0<<PD3) | (0<<PD2) | (0<<PD1) | (0<<PD0); // set all pins we are using high 
	 
}

void config_MAG3110(void)
{
  SoftI2CStart(); // transmit to device 0x0E
  SoftI2CWriteByte(0x11);              // cntrl register2
  SoftI2CWriteByte(0x80);              // send 0x80, enable auto resets
  SoftI2CStop();       // stop transmitting
  
  _delay_ms(15);
  
  SoftI2CStart(); // transmit to device 0x0E
  SoftI2CWriteByte(0x10);              // cntrl register1
  SoftI2CWriteByte(1);                 // send 0x01, active mode
  SoftI2CStop();       // stop transmitting
}

void SystemInit(void) // Power on variable initialization 
{
	USARTInit(UBRRVAL);
	sensor_Data = 0; // 0 
	Change_Baudrate(51); // 19200
	Command_Display(Clear_Screen);
	Command_Display(Go_Home);
	//Change_Baudrate(83); // 1200
	
}

int readx(void)
{
  int xl, xh;  //define the MSB and LSB
  
  SoftI2CStart(); // transmit to device 0x0E
  SoftI2CWriteByte(0x01);              // x MSB reg
  SoftI2CStop();       // stop transmitting
 
  _delay_us(2); //needs at least 1.3us free time between start and stop
  
  SoftI2CReadByte(1); // request 1 byte
  //while(Wire.available())    // slave may send less than requested
 // { 
   // xh = Wire.receive(); // receive the byte
   xh = SoftI2CReadByte(1);
 // }
  
 _delay_us(2); //needs at least 1.3us free time between start and stop
  
  SoftI2CStart(); // transmit to device 0x0E
  SoftI2CWriteByte(0x02);              // x LSB reg
  SoftI2CStop();       // stop transmitting
 
  _delay_us(2); //needs at least 1.3us free time between start and stop
  
  SoftI2CReadByte(1); // request 1 byte
 // while(Wire.available())    // slave may send less than requested
 // { 
 //   xl = Wire.receive(); // receive the byte
 // }
	xl = SoftI2CReadByte(1);
  int xout = (xl|(xh << 8)); //concatenate the MSB and LSB
  return xout;
}
/*
int ready(void)
{
  int yl, yh;  //define the MSB and LSB
  
  SoftI2CStart(); // transmit to device 0x0E
  SoftI2CWriteByte(0x03);              // y MSB reg
  SoftI2CStop();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  SoftI2CReadByte(1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    yh = Wire.receive(); // receive the byte
  }
  
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  SoftI2CStart(); // transmit to device 0x0E
  SoftI2CWriteByte(0x04);              // y LSB reg
  SoftI2CStop();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  SoftI2CReadByte(1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    yl = Wire.receive(); // receive the byte
  }
  
  int yout = (yl|(yh << 8)); //concatenate the MSB and LSB
  return yout;
}

int readz(void)
{
  int zl, zh;  //define the MSB and LSB
  
  SoftI2CStart(); // transmit to device 0x0E
  SoftI2CWriteByte(0x05);              // z MSB reg
  SoftI2CStop();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  SoftI2CReadByte(1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    zh = Wire.receive(); // receive the byte
  }
  
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  SoftI2CStart(); // transmit to device 0x0E
  SoftI2CWriteByte(0x06);              // z LSB reg
  SoftI2CStop();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  SoftI2CReadByte(1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    zl = Wire.receive(); // receive the byte
  }
  
  int zout = (zl|(zh << 8)); //concatenate the MSB and LSB
  return zout;
}
*/

void Display_Sensor_Data(void)
{
	
		sensor_Data = readx();
		Command_Display(Clear_Screen);	
		Command_Display(Go_Home);
		ltoa(sensor_Data, aNumberAsString, 10);
		UWriteString(aNumberAsString);	
		Set_Cursor_Position(1,2);
		UWriteString(uTeslas_String);
	//}
	//else
	//{
		
		//UWriteString(No_Data_String);

	//}
}
int main(void) 
{
	//timer0_init(); // timer 0 initialize 
	SystemInit(); // system variables and state initialize
	PortInit(); // inputs and outputs initialize
	SoftI2CInit();
	_delay_ms(500);
	while(1)
	{
		Display_Sensor_Data();
		_delay_ms(500);
	}
}

   