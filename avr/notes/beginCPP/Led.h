/*
 * Led.h
 *
 * Created: 1/15/2012 7:12:19 PM
 *  Author: Peter.Brown
 */ 


#ifndef LED_H_
#define LED_H_

#include "ATmega1284P.h"
#include "IOPort.h"

enum LedConfiguration
{
	LedCurrentSource,
	LedCurrentSink	
};

class Led
{
protected:
	IOPort*				_port;
	LedConfiguration	_configuration;
	uint8_t				_pin;
			
public:
	Led(IOPort* port, uint8_t pin, LedConfiguration configuration) :
		_port((IOPort *)port),
		_configuration(configuration),
		_pin(pin) {};
			
	
	void TurnOn();
	void TurnOff();
};



#endif /* LED_H_ */