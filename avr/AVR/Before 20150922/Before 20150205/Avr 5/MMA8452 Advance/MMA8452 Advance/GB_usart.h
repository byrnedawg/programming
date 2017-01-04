#include <inttypes.h>
#include <stdio.h>

#ifndef USART_H
#define USART_H
/****************************************************************************************************/

//Macros
#define RECEIVE_BUFF_SIZE 64
#define USART_RXC_VECT USART_RXC_vect	//This change from mcu to mcu


//#define UBRR_VALUE(baud) ((1000000UL/(baud<<4))-1)


//Varriables
volatile uint8_t URBuff[RECEIVE_BUFF_SIZE];	//USART Receive Buffer
volatile int8_t UQFront;
volatile int8_t UQEnd;


//Functions
void USARTInit(uint16_t baudrate);

uint8_t UDataAvailable();
uint8_t UReadData();

void UWriteData(uint8_t data);
void UWriteString(uint8_t *str);
int usart_putchar_printf(char var, FILE *stream);




/****************************************************************************************************/


#endif




