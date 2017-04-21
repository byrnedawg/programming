// interrupt.c

#include "prj.h"

void interrupt0_init()
{ 
    // enable external interrupt 0
    DDRD  &= ~_BV(2); // input
    PORTD |=  _BV(2); // pull-high
    MCUCR |= _BV(ISC01); 
    GICR  |= _BV(INT0);
}    

void interrupt1_init()
{ 
    // enable external interrupt 1
    DDRD  &= ~_BV(3); // input
    PORTD |=  _BV(3); // pull-high
    MCUCR |= _BV(ISC11); 
    GICR  |= _BV(INT1);
}    

void interrupt2_init()
{ 
    // enable external interrupt 2
    DDRB  &= ~_BV(2); // input
    PORTB |=  _BV(2); // pull-high
    GICR  |= _BV(INT2);
}    

ISR (INT0_vect) 
{ 
    // write your code 
 
} 

ISR (INT1_vect) 
{ 
    // write your code 
 
} 

ISR (INT2_vect) 
{ 
    // write your code 
 
} 

