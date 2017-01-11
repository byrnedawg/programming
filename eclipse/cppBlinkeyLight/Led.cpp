/*
 * Led.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: ubuntu
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



