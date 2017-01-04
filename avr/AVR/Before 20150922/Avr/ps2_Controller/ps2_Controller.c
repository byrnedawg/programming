/**********************************************************************\
Axon PS2 Controller Interface Library
Copyright (C) 2009 Jason Bare

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
\***********************************************************************/

/***********************************************************************\
This code was developed for the Axon MCU, however it should
work for any AVR based MCU (with minor modifications).

Low level PS2 communication code was adapted from Dunk's tutorial 
on the 'Society of Robots' website.
	
	http://www.societyofrobots.com/member_tutorials/node/200
\***********************************************************************/
#include "PS2_Controller.h"
#include "rprintf.h" //printf 

// PS2 Pin defines 
//   Wire colors are based on Lynxmotion adapter.
//   Your colors may vary, so confirm wire functionality first.
#define PS2_PORT			PORTB
#define	PS2_PIN				PINB
#define	PS2_DDR				DDRB

#define PS2_clock			3	// white
#define PS2_attention		4	// blue
#define PS2_command			5	// orange
#define PS2_data			6	// brown
// Acknowledge is not necessary (but is optimal).
// It can be commented out to save pins.
//#define PS2_acknowledge		7	// purple 


// Internal input buffer
static unsigned char PS2_Buffer[8] = { 255, 255, 255, 255, 128, 128, 128, 128 };

// Initialization state
static BOOL bPS2Initialized = FALSE;

// PS2 controller communication function.
// send a byte on the command line and receive one on the data line.
// needs Attention pin to have gone low before called to activate controller.
static short int InternalPS2GameByte(short int command)
{
	short int i;
    short int data = 0x00;				// clear data variable to save setting low bits later.

    for(i=0;i<8;i++)
    {
		if(command & _BV(i)) 			// bit bang "command" out on PS2_command wire.
			sbi(PS2_PORT, PS2_command);
		else 
			cbi(PS2_PORT, PS2_command);

		delay_us(1);                    // wait for output to stabilise
		cbi(PS2_PORT, PS2_clock);       // CLOCK LOW
		delay_us(1);                    // wait for output to stabilise
		sbi(PS2_PORT, PS2_clock);       // CLOCK HIGH

		if((PS2_PIN & _BV(PS2_data)))	// read PS2_data pin and store
			sbi(data, i);  		
    }
    sbi(PS2_PORT, PS2_command);

#ifdef PS2_acknowledge
    for(i=0;i<20;i++)
    {
		if(!(PS2_PIN & _BV(PS2_acknowledge)))	// wait for ack to go low (usually within 10 loops)
		{
			break;
		}
    }
#else
	delay_us(20);								// wait for ack to pass
#endif

    return(data);
}

// Polls and stores the PS2 controller data
BOOL PollPS2Controller( void )
{
	BOOL bConnected = FALSE;

	if( bPS2Initialized )
	{
	    sbi(PS2_PORT, PS2_command);							// start communication with PSx controller
	    sbi(PS2_PORT, PS2_clock);
	    cbi(PS2_PORT, PS2_attention);

	    InternalPS2GameByte(0x01);							// byte 0. header.
	    PS2_Buffer[0] = InternalPS2GameByte(0x42);			// byte 1. header.
	    PS2_Buffer[1] = InternalPS2GameByte(0x00);			// byte 2. header.

		if( (PS2_Buffer[0] == 0x73 || PS2_Buffer[0] == 0x41) && PS2_Buffer[1] == 0x5A )
		{
		    PS2_Buffer[2] = InternalPS2GameByte(0x00);		// byte 3. first data byte.
		    PS2_Buffer[3] = InternalPS2GameByte(0x00);		// byte 4.

			if( PS2_Buffer[0] == 0x73 )
			{
			    PS2_Buffer[4] = InternalPS2GameByte(0x00);	// byte 5.
			    PS2_Buffer[5] = InternalPS2GameByte(0x00);	// byte 6.
			    PS2_Buffer[6] = InternalPS2GameByte(0x00);	// byte 7.
			    PS2_Buffer[7] = InternalPS2GameByte(0x00);	// byte 8.
			}
			else
			{
			    PS2_Buffer[4] = 128;						// byte 5.
			    PS2_Buffer[5] = 128;						// byte 6.
			    PS2_Buffer[6] = 128;						// byte 7.
			    PS2_Buffer[7] = 128;						// byte 8.
			}

			bConnected = TRUE;
		}
		else
		{
			PS2_Buffer[2] = 255;							// byte 3. first data byte.
			PS2_Buffer[3] = 255;							// byte 4.
			PS2_Buffer[4] = 128;							// byte 5.
			PS2_Buffer[5] = 128;							// byte 6.
			PS2_Buffer[6] = 128;							// byte 7.
			PS2_Buffer[7] = 128;							// byte 8.
		}

	    sbi(PS2_PORT, PS2_command);							// close communication with PSx controller
	    delay_us(1);
	    sbi(PS2_PORT, PS2_attention);						// all done.
	}

	return bConnected;
}

// Initializes the PS2 controller
BOOL InitPS2Controller( void )
{
	rprintfInit(uart1SendByte);//USB
	rprintf("Initialize PS2 controller\r\n");

    // PSx controller I/O pin setup:
    sbi(PS2_DDR, PS2_clock);                // clock. output.

    cbi(PS2_DDR, PS2_data);                 // data. input.
    sbi(PS2_PORT, PS2_data);                //    enable pullup resistor

#ifdef PS2_acknowledge
    cbi(PS2_DDR, PS2_acknowledge);          // acknowledge. input.
    sbi(PS2_PORT, PS2_acknowledge);         //    enable pullup resistor
#endif

    sbi(PS2_DDR, PS2_command);              // command. output.

    sbi(PS2_DDR, PS2_attention);            // attention. output.

    // enable interupts
    sei();

	bPS2Initialized = 1;

	// Poll controller for connection status
	if( PollPS2Controller() )
	{
 		rprintf("PS2 Controller Initialized.\r\n");
		return TRUE;
	}
	else
	{
 		rprintf("No PS2 Controller Detected (init).\r\n");
	}

	return FALSE;
}

// Returns 'true' if analog mode is on (and a controller is connected)
BOOL GetPS2AnalogMode( void )
{
	//0x41=Digital, 0x23=NegCon, 0x73=Analogue Red LED, 0x53=Analogue Green LED
	return (PS2_Buffer[0] == 0x73);
}

// Returns 'true' if analog mode is activated (and a controller is connected)
// Controller should not be able to disable analog mode after this is called.
BOOL ForcePS2AnalogMode( void )
{
	if( PollPS2Controller() )
	{
		// this loop continues to put PSx controller into analouge mode untill the
		// controller responds with 0x73 in the 2nd byte. 
		// (PS2 controller responds with 0x73 when in analouge mode.)
		// the status LEDs will continue to count upwards untill a controller is found.
		// if everything is working correctly this should happen on the first pass of
		// this loop but occasionally errors occur and a 2nd or 3rd itteration happen.
		unsigned int cnt=0;
		do
		{
			// put controller in config mode
			sbi(PS2_PORT, PS2_command);
			sbi(PS2_PORT, PS2_clock);
			cbi(PS2_PORT, PS2_attention);

			InternalPS2GameByte(0x01);
			InternalPS2GameByte(0x43);
			InternalPS2GameByte(0x00);
			InternalPS2GameByte(0x01);
			InternalPS2GameByte(0x00);

			sbi(PS2_PORT, PS2_command);
			delay_us(1);
			sbi(PS2_PORT, PS2_attention);

			delay_ms(1);

			// put controller in analouge mode
			sbi(PS2_PORT, PS2_command);
			sbi(PS2_PORT, PS2_clock);
			cbi(PS2_PORT, PS2_attention);

			InternalPS2GameByte(0x01);
			InternalPS2GameByte(0x44);
			InternalPS2GameByte(0x00);
			InternalPS2GameByte(0x01);
			InternalPS2GameByte(0x03);
			InternalPS2GameByte(0x00);
			InternalPS2GameByte(0x00);
			InternalPS2GameByte(0x00);
			InternalPS2GameByte(0x00);

			sbi(PS2_PORT, PS2_command);
			delay_us(1);
			sbi(PS2_PORT, PS2_attention);

			delay_ms(1);

			// exit config mode
			sbi(PS2_PORT, PS2_command);
			sbi(PS2_PORT, PS2_clock);
			cbi(PS2_PORT, PS2_attention);

			InternalPS2GameByte(0x01);
			InternalPS2GameByte(0x43);
			InternalPS2GameByte(0x00);
			InternalPS2GameByte(0x00);
			InternalPS2GameByte(0x5A);
			InternalPS2GameByte(0x5A);
			InternalPS2GameByte(0x5A);
			InternalPS2GameByte(0x5A);
			InternalPS2GameByte(0x5A);

			sbi(PS2_PORT, PS2_command);
			delay_us(1);
			sbi(PS2_PORT, PS2_attention);

			delay_ms(1);

			// poll controller and check in analouge mode.
			PollPS2Controller();
			++cnt;
		}
		while( !GetPS2AnalogMode() && cnt < 10 );
	}

	return( GetPS2AnalogMode() );
}

// Returns 'true' if button is pressed
BOOL GetPS2Button( ePS2Button Button )
{
	return bit_is_clear( PS2_Buffer[2 + (Button>>3)], (Button%8) );
}

// Returns -128 <-> 127 (no deadzone)
short int GetRawPS2Axis( ePS2Axis Axis ) 
{
	switch( Axis )
	{
		case PS2_DX:
			return GetPS2Button(PS2_DpadL) ? -128 : GetPS2Button(PS2_DpadR) ? 127 : 0;
			break;

		case PS2_DY:
			return GetPS2Button(PS2_DpadU) ? -128 : GetPS2Button(PS2_DpadD) ? 127 : 0;
			break;

		default:
			return ((short int)PS2_Buffer[Axis]) - 128;
			break;
	}
}

// Returns -100 <-> 100 (~27 deadzone)
short int GetPS2Axis( ePS2Axis Axis )
{
	short int Val = GetRawPS2Axis( Axis );
	if( Val >= 27 )
	{
		return Val - 27;
	}
	else if( Val <= -28 )
	{
		return Val + 28;
	}
	else
	{
		return 0;
	}
}

// Debug and log PS2 controller 
data
void PrintPS2Controller( void )
{
	rprintfInit(uart0SendByte);//USB

	if( bPS2Initialized && (PS2_Buffer[0] == 0x73 || PS2_Buffer[0] == 0x41) && PS2_Buffer[1] == 0x5A )
	{
		rprintf( "DX=%d DY=%d", GetPS2Axis( PS2_DX ), GetPS2Axis( PS2_DY ) );

		if( GetPS2AnalogMode() )
		{
			rprintf( " LX=%d LY=%d RX=%d RY=%d", GetPS2Axis( PS2_LX ), GetPS2Axis( PS2_LY ), GetPS2Axis( PS2_RX ), GetPS2Axis( PS2_RY ) );
		}

		if( GetPS2Button( PS2_Select ) )
		{
			rprintf( " Select" );
		}

		if( GetPS2Button( PS2_R3 ) )
		{
			rprintf( " R3" );
		}

		if( GetPS2Button( PS2_L3 ) )
		{
			rprintf( " L3" );
		}

		if( GetPS2Button( PS2_Start ) )
		{
			rprintf( " Start" );
		}

		if( GetPS2Button( PS2_DpadU ) )
		{
			rprintf( " DpadU" );
		}

		if( GetPS2Button( PS2_DpadR ) )
		{
			rprintf( " DpadR" );
		}

		if( GetPS2Button( PS2_DpadD ) )
		{
			rprintf( " DpadD" );
		}

		if( GetPS2Button( PS2_DpadL ) )
		{
			rprintf( " DpadL" );
		}

		if( GetPS2Button( PS2_L2 ) )
		{
			rprintf( " L2" );
		}

		if( GetPS2Button( PS2_R2 ) )
		{
			rprintf( " R2" );
		}

		if( GetPS2Button( PS2_L1 ) )
		{
			rprintf( " L1" );
		}

		if( GetPS2Button( PS2_R1 ) )
		{
			rprintf( " R1" );
		}

		if( GetPS2Button( PS2_Triangle ) )
		{
			rprintf( " Triangle" );
		}

		if( GetPS2Button( PS2_Circle ) )
		{
			rprintf( " Circle" );
		}

		if( GetPS2Button( PS2_X ) )
		{
			rprintf( " X" );
		}

		if( GetPS2Button( PS2_Square ) )
		{
			rprintf( " Square" );
		}

		rprintf( "\r\n" );
	}
	else
	{
 		rprintf("No PS2 Controller Detected (print).\r\n");
	}
}

#undef PS2_PORT
#undef PS2_PIN
#undef PS2_DDR

#undef PS2_clock
#undef PS2_attention
#undef PS2_command
#undef PS2_data

#ifdef PS2_acknowledge
	#undef PS2_acknowledge
#endif
