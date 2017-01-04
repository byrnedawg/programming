// Place any #define statements here before you include ANY other files

// data buffer defines.
#define UART_TX_BUFFER_SIZE 20
#define UART_RX_BUFFER_SIZE 20

#define	 	TX_SYNC				0
#define		TX_PS2_0			1
#define		TX_PS2_1			2
#define		TX_PS2_RX			3
#define		TX_PS2_RY			4
#define		TX_PS2_LX			5
#define		TX_PS2_LY			6
#define		TX_END				7

#define	PS2_B_BTN_SELECT 		0x00
#define	PS2_B_BTN_L3     		0x01
#define	PS2_B_BTN_R3     		0x02
#define	PS2_B_BTN_START  		0x03
#define	PS2_B_DPAD_UP    		0x04
#define	PS2_B_DPAD_RIGHT 		0x05
#define	PS2_B_DPAD_DOWN  		0x06
#define	PS2_B_DPAD_LEFT  		0x07
#define	PS2_B_BTN_L2     		0x08
#define	PS2_B_BTN_R2     		0x09
#define	PS2_B_BTN_L1     		0x0A
#define	PS2_B_BTN_R1     		0x0B
#define	PS2_B_BTN_TRIANGLE		0x0C
#define	PS2_B_BTN_CIRCLE  		0x0D
#define	PS2_B_BTN_X     		0x0E
#define	PS2_B_BTN_SQUARE  		0x0F

/***********************************************************************************/
// You must ALWAYS specify the board you are using
// These are all in the 'sys' folder e.g.
#include "sys/atmega168.h" // I am using an Axon
// Now include any other files that are needed here
#include "uart.h"
#include "servos.h"
#include "rprintf.h"
#include "led.h"
#include "iopin.h"
#include "spisw.h"
#include "Controller/Sony/ps2.h"
#include "pwm.h"
#include "motorPWM.h"

// non WebbotLib includes:
//#include <avr/interrupt.h>			// needed because WebbotLib does not handle interrupts on INT[0-7] yet.
//#include <avr/wdt.h>				// haven't made use of this yet.
/***********************************************************************************/
// Now create any global variables such as motors, servos, sensors etc
// define variables

uint8_t i = 0;
uint8_t buttonPressed[0xFF];		// used to count how many frames button has been pressed for.
uint8_t buttonReleased[0xFF];		// contains how many frames button was held down for. used to de-bounce buttons


// Create a controller SELECTed using D6
SONY_PS2 controller1 = MAKE_SONY_PS2(D4);
// Create a list of devices on the SPI bus
static SPI_DEVICE_LIST spiBus_list[] = {&controller1._device_};
// Create a software SPI Bus using:-
// D5 = MOSI, D3=MISO, D2=Clock
SPI_SW spiBus = MAKE_SW_SPI(spiBus_list,D3,D2,D5);

// LEDS
LED RightRed = MAKE_LED(D7,FALSE);
LED LeftRed = MAKE_LED(B0,FALSE);
LED SpotLightWhite = MAKE_LED(B1,FALSE);
LED PowerGreen = MAKE_LED(B2,FALSE);


/***********************************************************************************/
// This routine is called once only and allows you to do set up the hardware
// Dont use any 'clock' functions here - use 'delay' functions instead

void appInitHardware(void){
// Initialise the servo controller

// Initialise the SPI bus
spiBusInit(&spiBus,true);


//servosInit(&bank1, TIMER1);
// Set UART0 to 9600 baud
uartInit(UART0, 9600);
// Tell rprintf to output to UART0
rprintfInit(&uart0SendByte);

}
/***********************************************************************************/
// This routine is called once to allow you to set up any other variables in your
//program
// You can use 'clock' function here.
// The loopStart parameter has the current clock value in ÏS
TICK_COUNT appInitSoftware(TICK_COUNT loopStart){

	LED_off(&RightRed); 
	LED_off(&LeftRed); 
	LED_off(&SpotLightWhite); 
	LED_on(&PowerGreen);

	sonyPS2_setAnalogMode(&controller1);
	sonyPS2_calibrate(&controller1, 20);

return 0; // dont pause after
}
/***********************************************************************************/
// This routine is called repeatedly - its your main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart){
	

if(sonyPS2_read(&controller1))
{
	char xButton_pressure = sonyPS2_buttonPressure(&controller1, PS2_BTN_X);
	char SqButton_pressure = sonyPS2_buttonPressure(&controller1, PS2_BTN_SQUARE);
	
	char DL_Button_pressure = sonyPS2_buttonPressure(&controller1, PS2_DPAD_LEFT);
	char DR_Button_pressure = sonyPS2_buttonPressure(&controller1, PS2_DPAD_RIGHT);
	
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_R1))
	{
		LED_on(&RightRed);
		rprintf("PS2_BTN_R1");
	}
	else
	{
		LED_off(&RightRed);
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_L1))
	{
		LED_on(&LeftRed);
		rprintf("PS2_BTN_L1");
	}
	else
	{
		LED_off(&LeftRed);
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_R2))
	{
		rprintf("PS2_BTN_R2");
	}
	else
	{
		LED_off(&RightRed);
	}

	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_L2))
	{
		rprintf("PS2_BTN_L2");
	}
	else
	{
		
	}

	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_TRIANGLE))
	{
		LED_on(&SpotLightWhite);
		rprintf("PS2_BTN_TRIANGLE");
	}
	else
	{
		LED_off(&SpotLightWhite);
	}

	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_CIRCLE))
	{
		rprintf("PS2_BTN_CIRCLE");
	}
	else
	{
	
	}

	if((sonyPS2_buttonHeld(&controller1, PS2_BTN_X)) || (sonyPS2_buttonHeld(&controller1, PS2_BTN_SQUARE)))
	{
		if(sonyPS2_buttonHeld(&controller1, PS2_BTN_SQUARE))
		{
			if(SqButton_pressure == 255)
			{
				sonyPS_setRumble(&controller1, SqButton_pressure, true);
				rprintf("Sq Button Max Speed");
			//	FrontSpeed = 128;
			}
			else
			{
				sonyPS_setRumble(&controller1, SqButton_pressure, false);
				rprintf("Sq Button held");
				//FrontSpeed = 157-(SqButton_pressure/9);
			}
		}
		if(sonyPS2_buttonHeld(&controller1, PS2_BTN_X))
		{
			if(xButton_pressure == 255)
			{
				sonyPS_setRumble(&controller1, xButton_pressure, true);
				rprintf("xButton Max Speed");
				//FrontSpeed = 127;
			}
			else
			{
				sonyPS_setRumble(&controller1, xButton_pressure, false);
				rprintf("xButton held");
				//FrontSpeed = 98+(xButton_pressure/9);
			}
		}
	}
	else
	{
		//FrontSpeed = 0;
		LED_off(&SpotLightWhite);
		sonyPS_setRumble(&controller1, 0, false);
	}
	
	if(sonyPS2_buttonHeld(&controller1, PS2_DPAD_UP))
	{
		rprintf("PS2_DPAD_UP");
	//	if(FrontSpeed <= 255)
	//	{
	//		FrontSpeed++;
		//}
//	}
//	else
//	{
		
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_DPAD_DOWN))
	{
		rprintf("PS2_DPAD_DOWN");
		//if(FrontSpeed > 0)
		//{
		//	FrontSpeed--;
		//	
		//}
	}
	else
	{
		
	}

	if(sonyPS2_buttonHeld(&controller1, PS2_DPAD_LEFT))
	{
		rprintf("PS2_DPAD_LEFT");
		if(DL_Button_pressure == 255)
		{
			rprintf("DL_Button Max Speed");
			//RudderPos = 128;
		}
		else
		{
			rprintf("DL_Button held");
		//	RudderDex = DL_Button_pressure/DexConstant;
		//	if(RudderPos > (128+RudderDex) && RudderPos <= 255)
		//	{
		//		RudderPos -= RudderDex;
		//	}
		//	else if(RudderPos >= 0 && RudderPos <= 127)
		//	{
		//		RudderPos -= RudderDex;
		//	}
		}
	}
	else
	{
		
	}

	if(sonyPS2_buttonHeld(&controller1, PS2_DPAD_RIGHT))
	{
		rprintf("PS2_DPAD_RIGHT");

		if(DR_Button_pressure == 255)
		{
			rprintf("DR_Button Max Speed");
			//RudderPos = 127;
		}
		else
		{
			//rprintf("DR_Button held");
			//RudderDex = DR_Button_pressure/DexConstant;
			//if(RudderPos >=0 && RudderPos <= (127-RudderDex))
			//{
		//		RudderPos += RudderDex;
		//	}
		//	else if(RudderPos >= 128 && RudderPos <= 255)
		//	{
		//		RudderPos += RudderDex;
		//	}
		}
	}
	else
	{
		
	}

	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_SELECT))
	{
		rprintf("PS2_B_BTN_SELECT");
	}
	else
	{
	
	}

	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_START))
	{
		rprintf("PS2_B_BTN_START");
	}
	else
	{
	
	}

	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_L3))
	{
		rprintf("PS2_B_BTN_L3");
	}
	else
	{
	
	}

	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_R3))
	{
		rprintf("PS2_B_BTN_R3");
	}
	else
	{
	
	}
}
else
{
	
}
		
		//act_setSpeed(&engine, FrontSpeed);
		//act_setSpeed(&rudder, RudderPos);
	
return 200000; // wait for 1 second before calling me again. 1000000us = 1
//second
}



