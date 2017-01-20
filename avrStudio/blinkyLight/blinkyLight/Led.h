/*
 * Led.h
 *
 *  Created on: Jan 11, 2017
 *      Author: ubuntu
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
    IOPort*                _port;
    LedConfiguration    _configuration;
    uint8_t                _pin;

public:
    Led(IOPort* port, uint8_t pin, LedConfiguration configuration) :
        _port((IOPort *)port),
        _configuration(configuration),
        _pin(pin) {};


    void TurnOn();
    void TurnOff();
};




#endif /* LED_H_ */
