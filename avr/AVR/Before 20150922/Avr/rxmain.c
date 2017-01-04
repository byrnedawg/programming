#define F_CPU 4000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#ifndef F_CPU
//define cpu clock speed if not defined
#define F_CPU 4000000
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

/* UART Buffer Defines */ 
#define UART_RX_BUFFER_SIZE 4	/* 1,2,4,8,16,32,64,128 or 256 bytes */ 
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 ) 
#if ( UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK ) 
#error RX buffer size is not a power of 2 
#endif
/* Static Variables */ 
static unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE]; 
static volatile unsigned char UART_RxHead; 
static volatile unsigned char UART_RxTail;
							  
#define SYNC 0XAA// synchro signal
#define RADDR 0x44
#define LEDON 0x11//LED on command
#define LEDOFF 0x22//LED off command
volatile char sinc;
volatile char addr;
volatile char data;
volatile char chk;

unsigned char ReceiveByte( void );

void USART_Init(void)
{   
    UCSRB |= (1 << RXEN);

	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	//Set baud rate
	UBRRL=(uint8_t)UBRRVAL;		//low byte
	UBRRH=(UBRRVAL>>8);	//high byte

	//Enable Transmitter and Receiver and Interrupt on receive complete
	UCSRB |= (1<<RXCIE);
	//enable global interrupts
	sei();

}
uint8_t USART_vReceiveByte(void)
{
    // Wait until a byte has been received
    while((UCSRA&(1<<RXC)) == 0);
    // Return received data
    return UDR;
} 
ISR(USART_RXC_vect)
{
	unsigned char data; 
	unsigned char tmphead;
	data = UDR;
	tmphead = ( UART_RxHead + 1 ) & UART_RX_BUFFER_MASK;
	/* read the received data */
	/* calculate buffer index */ 
	UART_RxHead = tmphead;
	/* store new index */
	/* store received data in buffer */
	if ( tmphead == UART_RxTail ) 
	{
		/* ERROR! Receive buffer overflow */
	} 
	UART_RxBuf[tmphead] = data;
}

unsigned char ReceiveByte( void ) 
{
	unsigned char tmptail; 
	while ( UART_RxHead == UART_RxTail );
	tmptail = ( UART_RxTail + 1 ) & UART_RX_BUFFER_MASK;
	/* calculate buffer index */
	UART_RxTail = tmptail; 
	return UART_RxBuf[tmptail];
}

unsigned char DataInReceiveBuffer( void ) 
{
	return ( UART_RxHead != UART_RxTail );
}

void Main_Init(void)
{
	DDRC |= (1<<PINC0)|(1<<PINC2) ;
      //sbi(PORTC,PC0); //LED ON             
      cbi(PORTC,PC0); //LED OFF
	  cbi(PORTC,PC2); //LED OFF
	//enable global interrupts
}

void delayms(uint8_t t)//delay in ms
{
uint8_t i;
for(i=0;i<t;i++)
	_delay_ms(1);
}

int main(void)
{
	Main_Init();
	USART_Init();
	while(1)
	{ 
	
		if(data==LEDON)
	{
		sbi(PORTC,PC2); //LED ON
	}
    if(data==LEDOFF)
	{
		cbi(PORTC,PC2); //LED OFF
	}
	
   	tbi(PORTC,PC0); //LED ON
    delayms(200);
	}
	return 0;
}


