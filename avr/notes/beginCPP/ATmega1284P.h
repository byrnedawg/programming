/*
 * ATmega1284P.h
 *
 * Created: 1/15/2012 4:41:26 PM
 *  Author: Peter.Brown
 */ 


#ifndef ATMEGA1284P_H_
#define ATMEGA1284P_H_

typedef unsigned char uint8_t;
typedef volatile uint8_t register8_t;



#define PINA	(*(register8_t*) 0x20)
#define DDRA	(*(register8_t*) 0x21)
#define PORTA	(*(register8_t*) 0x22)

#endif /* ATMEGA1284P_H_ */