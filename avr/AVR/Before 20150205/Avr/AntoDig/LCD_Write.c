#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

#define 	Backspace_Left 	8
#define 	Move_Right 		9
#define 	Clear 			12
#define		C_Return		13 
#define		Display_On		24
#define		Display_OFF		21

unsigned char message[]; 
unsigned char status[]= "C is : ";
unsigned char AZ[]= "AZ:";
unsigned char EL[]= "EL:";


void WriteString(unsigned char *strPointer)
{   
   while (*strPointer)
   {
      UWriteData(*strPointer);
      strPointer++;   
   }
   
   return;
} 

void main()
{
	//Initialize the USART with Baud rate = 2400bps 
	USARTInit(103);// 19200 4mhz 25 2400 4mhz 103 8mhz 207

	//Enable Internal Pullups on PORTC
	PORTC=0xFF;

	UWriteData(Display_On);

	/* 
	Keep transmitting the Value of Local PORTC
	to the Remote Station.

	On Remote RX station the Value of PORTC
	sent on AIR will be latched on its local PORTC
	*/
	
	uint8_t data;
	while(1)
	{
		data=PINC;

		/* 
		Now send a Packet
		Packet Format is AA<data><data inverse>Z
		total Packet size if 5 bytes.
		*/

		//Stabilize the Tx Module By Sending JUNK data
		
		
		if(data == 0xFE)
		{	
			UWriteData(Clear);
			UWriteData(250); //[250] ' Define custom character 2
			UWriteData(0);// [0] ' 0 = %00000   	0 0 0 0 0 
			UWriteData(4);// [4] ' 4 = %00100   	0 0 * 0 0
			UWriteData(14);// [14] ' 14 = %01110 	0 * * * 0
			UWriteData(31);// [31] ' 31 = %11111 	* * * * *
			UWriteData(14);// [14] ' 14 = %01110 	0 * * * 0
			UWriteData(4);// [4] ' 4 = %00100   	0 0 * 0 0
			UWriteData(0);// [0] ' 0 = %00000   	0 0 0 0 0 
			UWriteData(0);// [0] ' 0 = %00000   	0 0 0 0 0 
			UWriteData(2);// [2] ' Display the new custom character 2
		}
		else if(data == 0xFD)
		{
			UWriteData(Clear);
			WriteString(AZ);
			itoa(data, message, 16); 
			WriteString(message);
			UWriteData(2);
			UWriteData(C_Return);
			WriteString(EL);
			WriteString(message);
			UWriteData(2);
		}
		else if(data != 0xFF)
		{
			UWriteData(Clear);
			WriteString(status);
			itoa(data, message, 16); 
			WriteString(message);
		}
		else
		{
			UWriteData(Clear);
			WriteString(AZ);
			itoa(data, message, 16); 
			WriteString(message);
			UWriteData(C_Return);
			WriteString(EL);
			WriteString(message);
		}
			_delay_loop_2(0);
			_delay_loop_2(0);
			_delay_loop_2(0);
			_delay_loop_2(0);

	}
}
