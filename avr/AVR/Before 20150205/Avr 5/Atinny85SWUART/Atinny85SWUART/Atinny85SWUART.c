
#include "MAIN.h"
#include "SUART.C"


void init( void )
{
  DDRB = 0;
  PORTB = 0xFF;
  suart_init();
}


int main( void )
{
  init();
  sei();
  uputs( "Hallo Peter\n\r" );
  for(;;){
    _delay_ms( 5000 );
    while( kbhit() )
      uputchar( ugetchar());
  }
}
