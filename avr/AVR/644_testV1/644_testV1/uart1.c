// uart.c

#include "prj.h"

void uart_init() 
{
    // fosc = 8000000 Hz 
    // baud=9600, actual_baud=9615.4, err=0.2 %
    UBRR0L = 0x33; 
    UBRR0H = 0x00; 
   
    // enable uart N81  
   // UCSRB =  _BV(RXEN) | _BV(TXEN) | _BV(RXCIE) | _BV(TXCIE) ;
   // UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);
	
	UCSR0A |= (0<<RXC0)|(0<<TXC0)|(0<<UDRE0)|(0<<FE0)|(0<<DOR0)|(0<<UPE0)|(0<<U2X0)|(0<<MPCM0);
	
	UCSR0B |= (1<<RXCIE0)|(1<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02)|(0<<RXB80)|(0<<TXB80);
	
	UCSR0C |= (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
	
   
}

void uart_send_char(unsigned char ch)
{
   UDR0 = ch;

   /* Wait for empty transmit buffer */
   while (! (UCSR0A & _BV(UDRE0)) );

}

void uart_sendString(char *s)
{
   unsigned int i=0;
   while (s[i] != '\x0') 
   {
       uart_send_char(s[i++]);
    };
}

void  uart_send_hex(unsigned char ch)
{
    unsigned char i,temp;
     
    for (i=0; i<2; i++)
    {
        temp = (ch & 0xF0)>>4;
        if ( temp <= 9)
            uart_send_char ( '0' + temp);
        else
            uart_send_char(  'A' + temp - 10);
        ch = ch << 4;    
     }   
}

// UART receiver interrupt routine 
//=================================
ISR (USART0_RX_vect)
{
    char status,data;

    status = UCSR0A;
    data   = UDR0;
 
    // Check if error
    if ((status & (_BV(FE0) | _BV(DOR0) ) ) !=0)
       return ; 

    if(data == "quit")
	{
		uart_sendString("Hello Goon\r\n");
	}

}

// UART transmit interrupt routine
//=================================
ISR (USART0_TX_vect)
{

}
