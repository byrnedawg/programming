#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "usart.h"



void USARTInit(uint16_t ubrrvalue)
{
	//Setup q
	UQFront=UQEnd=-1;

	//Set Baud rate
	UBRR0H=(unsigned char)(ubrrvalue>>8);
	UBRR0L=(unsigned char)ubrrvalue;

	/*Set Frame Format
	
	Asynchronous mode
	No Parity
	1 StopBit
	char size 8

	*/

	UCSR0C=(0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00) |(0<<USBS0) |(1<<UCSZ01) |(1<<UCSZ00) |(0<<UCPOL0);

	/*Enable Interrupts
	RXCIE- Receive complete
	UDRIE- Data register empty

	Enable The recevier and transmitter

	*/

	UCSR0B=(1<<RXCIE0)|(1<<TXEN0)|(1<<RXEN0);
	sei();

}

//The USART ISR
ISR(USART_RXC_VECT)
{
	//Read the data
	char data=UDR0;

	//Now add it to q

	if(((UQEnd==RECEIVE_BUFF_SIZE-1) && UQFront==0) || ((UQEnd+1)==UQFront))
	{
		//Q Full
		UQFront++;
		if(UQFront==RECEIVE_BUFF_SIZE) UQFront=0;
	}
	

	if(UQEnd==RECEIVE_BUFF_SIZE-1)
		UQEnd=0;
	else
		UQEnd++;


	URBuff[UQEnd]=data;

	if(UQFront==-1) UQFront=0;

}

char UReadData()
{
	char data;
	
	//Check if q is empty
	if(UQFront==-1)
		return 0;
	
	data=URBuff[UQFront];
	
	if(UQFront==UQEnd)
	{
	//If single data is left
	//So empty q
	UQFront=UQEnd=-1;
	}
	else
	{
		UQFront++;

		if(UQFront==RECEIVE_BUFF_SIZE)
		UQFront=0;
	}

	return data;
}

void UWriteData(char data)
{
	//Wait For Transmitter to become read
	
	while(!(UCSR0A & (1<<UDR0)));

	//Now write
	UDR0=data;
}

uint8_t UDataAvailable()
{
	if(UQFront==-1) return 0;
	if(UQFront<UQEnd)
		return(UQEnd-UQFront+1);
	else if(UQFront>UQEnd)
		return (RECEIVE_BUFF_SIZE-UQFront+UQEnd+1);
	else
		return 1;
}

void UWriteString(char *str)
{
	while((*str)!='\0')
	{
		UWriteData(*str);
		str++;
	}

	UWriteData('\0');
}







