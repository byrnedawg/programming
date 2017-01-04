// Place any #define statements here before you include ANY other files

// data buffer defines.
#define UART_TX_BUFFER_SIZE 32
#define UART_RX_BUFFER_SIZE 32

#define    SYNC_PACKET              0x53
#define    END_PACKET               0x54
#define    PS2_B_BTN_SELECT         0x00
#define    PS2_B_BTN_L3             0x01
#define    PS2_B_BTN_R3             0x02
#define    PS2_B_BTN_START          0x03
#define    PS2_B_DPAD_UP            0x04
#define    PS2_B_DPAD_RIGHT         0x05
#define    PS2_B_DPAD_DOWN          0x06
#define    PS2_B_DPAD_LEFT          0x07
#define    PS2_B_BTN_L2             0x08
#define    PS2_B_BTN_R2             0x09
#define    PS2_B_BTN_L1             0x1A
#define    PS2_B_BTN_R1             0x0B
#define    PS2_B_BTN_TRIANGLE       0x0C
#define    PS2_B_BTN_CIRCLE         0x0D
#define    PS2_B_BTN_X              0x0E
#define    PS2_B_BTN_SQUARE         0x0F
#define    PS2_B_BTN_NOT            0xBD

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
//#include "Displays/Generic/HD44780.h"

// non WebbotLib includes:
//#include <avr/interrupt.h>            // needed because WebbotLib does not handle interrupts on INT[0-7] yet.
#include <avr/wdt.h>                // haven't made use of this yet.
/***********************************************************************************/
// Now create any global variables such as motors, servos, sensors etc
// define variables

char left_x = 0;
char left_y = 0;
char right_x = 0;
char right_y = 0;
char xButton_pressure = 0;
char SqButton_pressure = 0;
char circle_pressure = 0;
char triangle_pressure = 0;
char DL_Button_pressure = 0;
char DR_Button_pressure = 0;
char PS2_PS1 = PS2_B_BTN_NOT;

uint8_t none_pressed = 0;   
uint8_t i = 0;
uint8_t buttonPressed[0xFF];        // used to count how many frames button has been pressed for.
uint8_t buttonReleased[0xFF];        // contains how many frames button was held down for. used to de-bounce buttons
uint8_t SpotLightState = 0;


// Create a controller SELECTed using D6
SONY_PS2 controller1 = MAKE_SONY_PS2(D4);
// Create a list of devices on the SPI bus
static SPI_DEVICE_LIST spiBus_list[] = {&controller1._device_};
// Create a software SPI Bus using:-
// D5 = MOSI, D3=MISO, D2=Clock
SPI_SW spiBus = MAKE_SW_SPI(spiBus_list,D3,D2,D5);

// LEDS
LED FunLED = MAKE_LED(B2,FALSE);
//LED RightRed = MAKE_LED(D4,FALSE);
//LED LeftRed = MAKE_LED(B0,FALSE);
LED SpotLightWhite = MAKE_LED(B1,FALSE);
//LED PowerGreen = MAKE_LED(D2,FALSE);

void SendData(char PS2_PS1)
{
    rprintfChar(SYNC_PACKET);
    rprintfChar(left_x);
    rprintfChar(left_y);
    rprintfChar(right_x);
    rprintfChar(right_y);
	rprintfChar(circle_pressure);
	rprintfChar(triangle_pressure);
    rprintfChar(xButton_pressure);
    rprintfChar(SqButton_pressure);
    rprintfChar(DL_Button_pressure);
    rprintfChar(DR_Button_pressure);
    rprintfChar(PS2_PS1);               
    rprintfChar(END_PACKET);
	none_pressed = 0;
}

void SendDataNone(char PS2_PS1)
{
    if(none_pressed < 5)
	{
		none_pressed++;
		rprintfChar(SYNC_PACKET);
		rprintfChar(left_x);
		rprintfChar(left_y);
		rprintfChar(right_x);
		rprintfChar(right_y);
		rprintfChar(circle_pressure);
		rprintfChar(triangle_pressure);
		rprintfChar(xButton_pressure);
	    rprintfChar(SqButton_pressure);
	    rprintfChar(DL_Button_pressure);
	    rprintfChar(DR_Button_pressure);
		rprintfChar(PS2_PS1);               
		rprintfChar(END_PACKET);	
	}
	
}

/***********************************************************************************/
// This routine is called once only and allows you to do set up the hardware
// Dont use any 'clock' functions here - use 'delay' functions instead

void appInitHardware(void){
// Initialize the servo controller

// Initialize the SPI bus
spiBusInit(&spiBus,true);


//servosInit(&bank1, TIMER1);
// Set UART0 to 9600 baud
uartInit(UART0, 9600);
// Tell rprintf to output to UART0
rprintfInit(&uart0SendByte);
rprintf("Start\n");
wdt_enable (WDTO_250MS);
}
/***********************************************************************************/
// This routine is called once to allow you to set up any other variables in your
//program
// You can use 'clock' function here.
// The loopStart parameter has the current clock value in ìS
TICK_COUNT appInitSoftware(TICK_COUNT loopStart){
	LED_off(&FunLED);
    //LED_off(&RightRed);
    //LED_off(&LeftRed);
    LED_on(&SpotLightWhite);
//    LED_on(&PowerGreen);

    sonyPS2_setAnalogMode(&controller1);
    sonyPS2_calibrate(&controller1, 25);

return 0; // dont pause after
}
/***********************************************************************************/
// This routine is called repeatedly - its your main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart){
wdt_reset(); 


if(sonyPS2_read(&controller1))
{
	xButton_pressure = sonyPS2_buttonPressure(&controller1, PS2_BTN_X);
    SqButton_pressure = sonyPS2_buttonPressure(&controller1, PS2_BTN_SQUARE);
	
	circle_pressure = sonyPS2_buttonPressure(&controller1, PS2_B_BTN_CIRCLE);
    triangle_pressure = sonyPS2_buttonPressure(&controller1, PS2_BTN_TRIANGLE);
   
    DL_Button_pressure = sonyPS2_buttonPressure(&controller1, PS2_DPAD_LEFT);
    DR_Button_pressure = sonyPS2_buttonPressure(&controller1, PS2_DPAD_RIGHT);
   
    if(sonyPS2_setAnalogMode(&controller1))
    {
		left_x = sonyPS2_joystick(&controller1, PS2_STICK_LEFT_X);
        left_y = sonyPS2_joystick(&controller1, PS2_STICK_LEFT_Y);
        right_x = sonyPS2_joystick(&controller1, PS2_STICK_RIGHT_X);
        right_y = sonyPS2_joystick(&controller1, PS2_STICK_RIGHT_Y);
    }
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_TRIANGLE))
    {       
        SendData(PS2_B_BTN_TRIANGLE);
        SpotLightState++;
        if(SpotLightState % 2 == 0)
        {
            LED_off(&SpotLightWhite);
        }
        else
        {
            LED_on(&SpotLightWhite);
        }
    }
    else
    {
       
    }
    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_R1))
    {    
		  // LED_on(&RightRed);
    }
    else if(sonyPS2_buttonHeld(&controller1, PS2_DPAD_RIGHT))
    {
        SendData(PS2_B_DPAD_RIGHT);
       // LED_on(&RightRed);
    }
    else
    {
       // LED_off(&RightRed);
    }

    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_L1))
    {
        SendData(PS2_B_BTN_L1);
        //LED_on(&LeftRed);
    }
    else if(sonyPS2_buttonHeld(&controller1, PS2_DPAD_LEFT))
    {
        SendData(PS2_B_DPAD_LEFT);
        //LED_on(&LeftRed);
    }
    else
    {
       // LED_off(&LeftRed);
    }
    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_R2))
    {
        SendData(PS2_B_BTN_R2);
    }
    else
    {
       
    }

    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_L2))
    {
        SendData(PS2_B_BTN_L2);
    }
    else
    {
      
    }
    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_CIRCLE))
    {
        SendData(PS2_B_BTN_CIRCLE);
    }
    else
    {
       
    }
   
    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_SQUARE))
    {
        SendData(PS2_B_BTN_SQUARE);
       // LED_on(&RightRed);
        //LED_on(&LeftRed);
        if(SqButton_pressure == 255)
        {
            sonyPS_setRumble(&controller1, SqButton_pressure/2, false);
        }
        else
        {
            sonyPS_setRumble(&controller1, SqButton_pressure/2, false);
        }
    }
    else if(sonyPS2_buttonHeld(&controller1, PS2_BTN_X))
    {
        SendData(PS2_B_BTN_X);
       LED_off(&FunLED);
	   // LED_off(&RightRed);
       // LED_off(&LeftRed);
        if(xButton_pressure == 255)
        {
            sonyPS_setRumble(&controller1, xButton_pressure/2, false);
        }
        else
        {
            sonyPS_setRumble(&controller1, xButton_pressure/2, false);
        }
    }
    else
    {
        sonyPS_setRumble(&controller1, 0, false);
    }
   
    if(sonyPS2_buttonHeld(&controller1, PS2_DPAD_UP))
    {
       SendData(PS2_B_DPAD_UP);
    }
    else
    {
       
    }
    if(sonyPS2_buttonHeld(&controller1, PS2_DPAD_DOWN))
    {
        SendData(PS2_B_DPAD_DOWN);
    }
    else
    {
      
    }

    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_SELECT))
    {
        SendData(PS2_B_BTN_SELECT);       
    }
    else
    {
   
    }

    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_START))
    {
        SendData(PS2_B_BTN_START);
    }
    else
    {
       
    }

    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_L3))
    {
        SendData(PS2_B_BTN_L3);
    }
    else
    {
       
    }

    if(sonyPS2_buttonHeld(&controller1, PS2_BTN_R3))
    {
        SendData(PS2_B_BTN_R3);
    }
    else
    {
       
    }
	if((left_x + left_y + right_x + right_y) != 0)
    {
		SendData(PS2_B_BTN_NOT);
    }
	//else if((xButton_pressure + SqButton_pressure + DL_Button_pressure + DR_Button_pressure ) == 0)
	else
	{
		SendDataNone(PS2_B_BTN_NOT);
	}
	
}
else
{
   LED_off(&SpotLightWhite);
   LED_on(&FunLED);
   rprintf("Fail\n");
}
          
return 200000; // wait for 1 second before calling me again. 1000000us = 1
//second
}
