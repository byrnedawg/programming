#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>


#define PSclock            0               // PD2
#define PSdata             1               // PD3
#define PSacknolage  2               // PD4
#define PScommand   3               // PD5
#define PSattention     4               // PD6

unsigned char chk_ana = 0;
int data0=0,data1=0,data2=0,data3=0,data4=0,data5=0,temp=0;

int gameByte(int command)
{
	int i ;
	_delay_us(1);
	int data = 0x00;                             // clear data variable to save setting low bits later.
	for(i=0;i<8;i++)
	{
		if(command & _BV(i))
		{
			PORTB|=(1<<PScommand);//sbi(PORTB, PScommand);
		}// bit bang "command" out on PScommand wire.
		else
		{
			PORTB&=~(1<<PScommand);
		}
		PORTB&=~(1<<PSclock);                             // CLOCK LOW
		_delay_us(1);                                              // wait for output to stabilise
		if((PINB & _BV(PSdata)))
		{
			data|=(1<<i);  // read PSdata pin and store
		}//else cbi(data, i);
		PORTB|=(1<<PSclock);                             // CLOCK HIGH
	}
	PORTB|=(1<<PScommand);

	_delay_us(20);                                                   // wait for ACK to pass.

	return(data);
}

void joystick_config()
{
	// put controller in config mode
	PORTB|=(1<<PScommand);
	PORTB|=(1<<PSclock);
	PORTB&=~(1<<PSattention);

	gameByte(0x01);
	gameByte(0x43);
	gameByte(0x00);
	gameByte(0x01);
	gameByte(0x00);

	PORTB|=(1<<PScommand);
	_delay_ms(1);
	PORTB|=(1<<PSattention);

	_delay_ms(10);

	// put controller in analouge mode
	PORTB|=(1<<PScommand);
	PORTB|=(1<<PSclock);
	PORTB&=~(1<<PSattention);

	gameByte(0x01);
	gameByte(0x44);
	gameByte(0x00);
	gameByte(0x01);
	gameByte(0x03);
	gameByte(0x00);
	gameByte(0x00);
	gameByte(0x00);
	gameByte(0x00);

	PORTB|=(1<<PScommand);
	_delay_ms(1);
	PORTB|=(1<<PSattention);

	_delay_ms(10);

	// exit config mode
	PORTB|=(1<<PScommand);
	PORTB|=(1<<PSclock);
	PORTB&=~(1<<PSattention);

	gameByte(0x01);
	gameByte(0x43);
	gameByte(0x00);
	gameByte(0x00);
	gameByte(0x5A);
	gameByte(0x5A);
	gameByte(0x5A);
	gameByte(0x5A);
	gameByte(0x5A);

	PORTB|=(1<<PScommand);
	_delay_ms(1);
	PORTB|=(1<<PSattention);

	_delay_ms(10);

	// poll controller and check in analouge mode.
	PORTB|=(1<<PScommand);
	PORTB|=(1<<PSclock);
	PORTB&=~(1<<PSattention);

	gameByte(0x01);
	chk_ana = gameByte(0x42);            // the 2nd byte to be returned from the controller should = 0x73 for "red" analouge controller.
	gameByte(0x00);
	gameByte(0x00);
	gameByte(0x00);
	gameByte(0x00);
	gameByte(0x00);
	gameByte(0x00);
	gameByte(0x00);

	PORTB|=(1<<PScommand);
	_delay_ms(1);
	PORTB|=(1<<PSattention);
	_delay_ms(10);

}

void ps2_init()
{
	DDRB|=(1<<PB0);                 // clock. output. (blue)

	DDRB&=~(1<<PB1);                 // data. input. (brown)
	PORTB|=(1<<PB1);               //    enable pullup resistor

	DDRB&=~(1<<PB2);                 // acknolage. input. (green)
	PORTB|=(1<<PB2);               //    enable pullup resistor

	DDRB|=(1<<PB3);                 // command. output. (orange)

	DDRB|=(1<<PB3);                 // attention. output. (yellow)
}

void joystick_poll()
{
	PORTB|=(1<<PScommand);                          // start communication with PSx controller
	PORTB|=(1<<PSclock);
	PORTB&=~(1<<PSattention);

	gameByte(0x01);                                       // bite 0. header.
	temp = gameByte(0x42);                          // bite 1. header. (should possibly put test on this byte to detect unplugging of controller.)
	gameByte(0x00);                                       // bite 2. header.

	data0 = gameByte(0x00);                         // bite 3. first data bite.
	data1 = gameByte(0x00);                         // bite 4.
	data2 = gameByte(0x00);                         // bite 5.
	data3 = gameByte(0x00);                         // bite 6.
	data4 = gameByte(0x00);                         // bite 7.
	data5 = gameByte(0x00);                         // bite 8.

	_delay_us(1);
	PORTB|=(1<<PScommand);                      // close communication with PSx controller
	_delay_us(1);
	PORTB|=(1<<PSattention);                        // all done.
}