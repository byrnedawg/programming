/*
 * IOPort.h
 *
 * Created: 1/15/2012 5:43:58 PM
 *  Author: Peter.Brown
 */ 


#ifndef IOPORT_H_
#define IOPORT_H_

#include "ATmega1284P.h"

enum PinDirection
{
	DirectionInput = 0,
	DirectionOutput = 1
};

class IOPort
{
protected:
	volatile register8_t* _port;
	volatile register8_t* _pins;
	volatile register8_t* _ddr;
public:
	IOPort(register8_t* portRegister, register8_t* pinsRegister, register8_t* ddrRegister) :
		_port(portRegister),
		_pins(pinsRegister),
		_ddr(ddrRegister) {}
			
	void SetPortDirection(register8_t directionBits);	
	void SetPortValues(register8_t valueBits);
	void GetPortValues(register8_t &valueBits);

	void SetPinDirection(int pin, PinDirection direction);
	void SetPinValue(int pin, bool value);
	void GetPinValue(int pin, bool &value);
};



#endif /* IOPORT_H_ */