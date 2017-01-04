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
#ifndef _PS2_CONTROLLER_H_
#define _PS2_CONTROLLER_H_

// Button enum (Do not alter order)
typedef enum 
{
	PS2_Select = 0,
	PS2_L3,
	PS2_R3,
	PS2_Start,
	PS2_DpadU,
	PS2_DpadR,
	PS2_DpadD,
	PS2_DpadL,
	PS2_L2,
	PS2_R2,
	PS2_L1,
	PS2_R1,
	PS2_Triangle,
	PS2_Circle,
	PS2_X,
	PS2_Square
} ePS2Button;

// Axis enum (Do not alter order)
typedef enum 
{
	// Analog stick axes
	PS2_RX = 4,
	PS2_RY,
	PS2_LX,
	PS2_LY,

	// Simulated axes using Dpad
	PS2_DX,
	PS2_DY
} ePS2Axis;

// Initializes the PS2 controller pins (returns 'true' if a controller is connected)
boolean InitPS2Controller( void );

// Polls and caches the PS2 controller data (returns 'true' if a controller is connected)
boolean PollPS2Controller( void );

// Returns 'true' if analog mode is on (and a controller is connected)
BOOL GetPS2AnalogMode( void );

// Returns 'true' if analog mode is activated (and a controller is connected)
// Controller should not be able to disable analog mode after this is called.
BOOL ForcePS2AnalogMode( void );

// Returns 'true' if button is pressed
BOOL GetPS2Button( ePS2Button Button );

// Returns raw -128 <-> 127 (no deadzone)
// -X = Left, +X = Right, -Y = Up, +Y = Down
short int GetRawPS2Axis( ePS2Axis Axis ); 

// Returns -100 <-> 100 (~27 deadzone)
// -X = Left, +X = Right, -Y = Up, +Y = Down
short int GetPS2Axis( ePS2Axis Axis );

// Debug and log PS2 controller data
void PrintPS2Controller( void );

#endif
