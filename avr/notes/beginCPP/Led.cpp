/*
 * Led.cpp
 *
 * Created: 1/15/2012 7:18:36 PM
 *  Author: Peter.Brown
 */ 

#include "Led.h"


void Led::TurnOn()
{
	_port->SetPinValue(_pin, (_configuration == LedCurrentSource));
}

void Led::TurnOff()
{	
	_port->SetPinValue(_pin, !(_configuration == LedCurrentSource));
}


