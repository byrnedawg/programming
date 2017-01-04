#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#ifndef F_CPU
//define cpu clock speed if not defined
#define F_CPU 8000000UL
#endif
//set desired baud rate
#define BAUDRATE 1200
//calculate UBRR value
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)
//define receive parameters
#define sbi(x,y) x |= _BV(y) //set bit
#define cbi(x,y) x &= ~(_BV(y)) //clear bit
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) (x & _BV(y)) == _BV(y) //(for input) checks if the 							  // input is high (Logic 1)
							  // if(is_high(PIND,PD0))

#define SYNC 0XAA// synchro signal
#define RADDR 0x44
#define LEDON 0x11//switch led on command
#define LEDOFF 0x22//switch led off command

void USART_Init(void)
{
	DDRB = (0<<PINB0);
   
    DDRD = (1<<PIND7);
	cbi(PORTD,PD7); //LED OFF;
   	
	//Set baud rate
	UBRRL=(uint8_t)UBRRVAL;		//low byte
	UBRRH=(UBRRVAL>>8);	//high byte
	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	//Enable Transmitter and Receiver and Interrupt on receive complete
	UCSRB=(1<<TXEN);

}
void USART_vSendByte(uint8_t u8Data)
{
    // Wait if a byte is being transmitted
    while((UCSRA&(1<<UDRE)) == 0);
    // Transmit data
    UDR = u8Data; 
}
//void Send_Packet(uint8_t addr, uint8_t cmd)
void Send_Packet(uint8_t cmd)
{
	USART_vSendByte(SYNC);//send synchro byte	
	USART_vSendByte(RADDR);//send receiver address
	USART_vSendByte(cmd);//send increment command
	USART_vSendByte((RADDR+cmd));//send checksum
}
void delayms(uint8_t t)//delay in ms
{
uint8_t i;
for(i=0;i<t;i++)
	_delay_ms(1);
}

int main(void)
{
USART_Init();
while(1)
	{//endless transmission
	//send command to switch led ON
	if(bit_is_clear(PINB,PB0))
	{
		//Send_Packet(RADDR, LEDON);
		sbi(PORTD,PD7); //LED ON;
		Send_Packet(LEDON);
		delayms(100);
	}
	else
	{
	//send command to switch led OFF
		//Send_Packet(RADDR, LEDOFF);
		cbi(PORTD,PD7); //LED OFF;
		Send_Packet(LEDOFF);
		delayms(100);
	}
	}
	return 0;
}
