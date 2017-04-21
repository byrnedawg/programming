#ifndef _RELAY_H
#define _RELAY_H

// 0 - active low, 1 - active high
#define RELAY1_ACTIVE_LOW    0

#if (RELAY1_ACTIVE_LOW ==0) 
    #define Relay1_on()     Relay1_PORT &= ~_BV(Relay1_BIT) 
    #define Relay1_off()    Relay1_PORT |=  _BV(Relay1_BIT) 
#else 
    #define Relay1_on()     Relay1_PORT |=  _BV(Relay1_BIT) 
    #define Relay1_off()    Relay1_PORT &= ~_BV(Relay1_BIT) 
#endif

#endif
