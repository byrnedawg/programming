// Place any #define statements here before you include ANY other files

// data buffer defines.
#define	 	TX_FRA_NUM			1
#define		TX_PS2_0			2
#define		TX_PS2_1			3
#define		TX_PS2_RX			4
#define		TX_PS2_RY			5
#define		TX_PS2_LX			6
#define		TX_PS2_LY			7
#define		TX_CONTROLER		8

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
#include "sys/atmega32.h" // I am using an Axon
// Now include any other files that are needed here
#include "uart.h"
#include "servos.h"
#include "rprintf.h"
#include "led.h"
#include "iopin.h"
#include "spisw.h"
#include "Controller/Sony/ps2.h"
#include "led.h"
#include "pwm.h"
#include "Matrix_Orbital_GLT240128.c"

// non WebbotLib includes:
#include <avr/interrupt.h>			// needed because WebbotLib does not handle interrupts on INT[0-7] yet.
//#include <avr/wdt.h>				// haven't made use of this yet.
/***********************************************************************************/
// Now create any global variables such as motors, servos, sensors etc
// define variables

uint8_t data[150];
uint8_t buttonPressed[0xFF];		// used to count how many frames button has been pressed for.
uint8_t buttonReleased[0xFF];		// contains how many frames button was held down for. used to de-bounce buttons
uint8_t	color = 50;

// Create a controller SELECTed using D6
SONY_PS2 controller1 = MAKE_SONY_PS2(D6);
// Create a list of devices on the SPI bus
static SPI_DEVICE_LIST spiBus_list[] = {&controller1._device_};
// Create a software SPI Bus using:-
// D5 = MOSI, D3=MISO, D2=Clock
SPI_SW spiBus = MAKE_SW_SPI(spiBus_list,D5,D3,D2);

//SERVO rudder = MAKE_SERVO(false, B3,1500, 500);
//SERVO_LIST servos[] = {&rudder};
// Create a driver for the list of servos
//SERVO_DRIVER bank1 = MAKE_SERVO_DRIVER(servos);


LED boardLED0 = MAKE_LED(C0,FALSE);
LED boardLED1 = MAKE_LED(C1,FALSE);
LED boardLED2 = MAKE_LED(C2,FALSE);
LED boardLED3 = MAKE_LED(C3,FALSE);
LED boardLED4 = MAKE_LED(C4,FALSE);
LED boardLED5 = MAKE_LED(C5,FALSE);
LED boardLED6 = MAKE_LED(C6,FALSE);
LED boardLED7 = MAKE_LED(C7,FALSE);

/*
    pin_make_output(C0);			// set LED pin for output. (PCB mounted LED.)
	pin_high(C0);					// LED off (LED hidden inside box.)
	pin_low(F4);					// LED on
	pin_make_input(F3, TRUE);		// set toggle switch to input. pull-up enabled.
	pin_make_input(D4, FALSE);		// set PACTL pin to input.
*/

/***********************************************************************************/
uint8_t service_PS2_Controller(uint8_t *data){
	int ps2 = sonyPS2_read(&controller1);

    if(ps2){
    	data[TX_PS2_1] = 0xFF & sonyPS2_buttonsRaw(&controller1);
    	data[TX_PS2_0] = (0xFF00 & sonyPS2_buttonsRaw(&controller1))>>8;

    	data[TX_PS2_LX] = sonyPS2_joystickRaw(&controller1, PS2_STICK_LEFT_X);
    	data[TX_PS2_LY] = sonyPS2_joystickRaw(&controller1, PS2_STICK_LEFT_Y);
    	data[TX_PS2_RX] = sonyPS2_joystickRaw(&controller1, PS2_STICK_RIGHT_X);
    	data[TX_PS2_RY] = sonyPS2_joystickRaw(&controller1, PS2_STICK_RIGHT_Y);

    }
    else{
    	// error with PS2 controller. re-initialise.
    	sonyPS2_setAnalogMode(&controller1);
    }


	return(ps2);
}

void InitDisplay()
{
	Command_Display(Display_On);
	Choose_Font(Small_Filled);
	Choose_Color(color);
	Command_Display(Clear_Screen);

}

void Draw_Pad()
{

	Cursor_Coordinate(70,21);
	rprintf("EL:     CHG:");

	Cursor_Coordinate(70,11);
	rprintf("AZ:      RD:");

	Wait(1);
	Draw_Rect_At(255, 65, 5, 174, 30);

	Draw_Rect_At(255, 65, 35, 90, 60);  	//1
	Draw_Rect_At(255, 65, 63, 90, 88);		//4
	Draw_Rect_At(255, 65, 91, 90, 116);		//7

	Draw_Rect_At(255, 93, 35, 118, 60);		//2
	Draw_Rect_At(255, 93, 63, 118, 88);		//5
	Draw_Rect_At(255, 93, 91, 118, 116);	//8	

	Draw_Rect_At(255, 121, 35, 146, 60);	//3
	Draw_Rect_At(255, 121, 63, 146, 88);	//6
	Draw_Rect_At(255, 121, 91, 146, 116);	//9

	Draw_Rect_At(255, 149, 35, 174, 60);	//C
	Draw_Rect_At(255, 149, 63, 174, 88);	//<
	Draw_Rect_At(255, 149, 91, 174, 116);	//0
	
	Wait(1);

	Cursor_Coordinate(75,45);
	rprintf("1");
	Cursor_Coordinate(75,73);
	rprintf("4");
	Cursor_Coordinate(75,101);
	rprintf("7");

	Cursor_Coordinate(103,45);
	rprintf("2");
	Cursor_Coordinate(103,73);
	rprintf("5");
	Cursor_Coordinate(103,101);
	rprintf("8");

	Cursor_Coordinate(131,45);
	rprintf("3");
	Cursor_Coordinate(131,73);
	rprintf("6");
	Cursor_Coordinate(131,101);
	rprintf("9");

	Cursor_Coordinate(161,45);
	rprintf("C");
	Cursor_Coordinate(161,73);
	rprintf("<");
	Cursor_Coordinate(161,101);
	rprintf("0");
}

void Draw_Touch_Region()
{
	Set_Touch_Mode(Region); // Region or Co_ordinate
	Delete_All_Regions();
	
	Wait(1);
	Set_Touch_Region(1, 65, 38, 19, 19, 1, 10);		// 1, 10)
	Set_Touch_Region(4, 65, 66, 19, 19, 4, 40); 	// 4, 40)
	Set_Touch_Region(7, 65, 94, 19, 19, 7, 70);		// 7, 70)
	
	Wait(1);
	Set_Touch_Region(2, 93, 38, 19, 19, 2, 20);		// 2, 20)
	Set_Touch_Region(5, 93, 66, 19, 19, 5, 50);		// 5, 50)
	Set_Touch_Region(8, 93, 94, 19, 19, 8, 80);		// 8, 80)

	Wait(1);
	Set_Touch_Region(3, 121, 38, 19, 19, 3, 30);	// 3, 30)	
	Set_Touch_Region(6, 121, 66, 19, 19, 6, 60);	// 6, 60)
	Set_Touch_Region(9, 121, 94, 19, 19, 9, 90);	// 9, 90)	

	Wait(1);
	Set_Touch_Region(12, 152, 38, 19, 19, 13, 14);	// 13, 14)
	Set_Touch_Region(11, 152, 66, 19, 19, 11, 12);	// 11, 12)	
	Set_Touch_Region(10, 152, 94, 19, 19, 101, 100);	//101, 100)
}

/***********************************************************************************/
// This routine is called once only and allows you to do set up the hardware
// Dont use any 'clock' functions here - use 'delay' functions instead

void appInitHardware(void){
// Set UART0 to 19200 baud
uartInit(UART0, 19200);
// Tell rprintf to output to UART0
rprintfInit(&uart0SendByte);
// Initialise the SPI bus
spiBusInit(&spiBus,true);
//servoPWMInit(&bank1);
//servosInit(&bank1, TIMER0);
pwmInitHertz(B3, 65, 9, null);

}
/***********************************************************************************/
// This routine is called once to allow you to set up any other variables in your
//program
// You can use 'clock' function here.
// The loopStart parameter has the current clock value in ìS
TICK_COUNT appInitSoftware(TICK_COUNT loopStart){

LED_off(&boardLED0);
LED_off(&boardLED1);
LED_off(&boardLED2);
LED_off(&boardLED3);
LED_off(&boardLED4);
LED_off(&boardLED5);
LED_off(&boardLED6);
LED_off(&boardLED7);

	InitDisplay();
	Wait(5);
	Command_Display(Go_Home);
	//Draw_Pad();
	//Wait(5);
	//Draw_Touch_Region();
	//Wait(5);
	Cursor_Coordinate(5,20);
	rprintf("Raw BTN:");
	Cursor_Coordinate(5,30);
	rprintf("D Pad Y:");
	Cursor_Coordinate(95,30);
	rprintf("Shape Y:");
	Wait(1);
	Cursor_Coordinate(5,40);
	rprintf("D Pad X:");
	Cursor_Coordinate(95,40);
	rprintf("Shape X:");
	Wait(1);
	Cursor_Coordinate(5,50);
	rprintf("Left  Y:");
	Cursor_Coordinate(95,50);
	rprintf("BTN X P:");
	Cursor_Coordinate(5,60);
	rprintf("Left  X:");
	Cursor_Coordinate(5,70);
	rprintf("Right Y:");
	Cursor_Coordinate(5,80);
	rprintf("Right X:");
	Wait(1);
	Cursor_Coordinate(5,90);
	rprintf("Data  1:");
	Cursor_Coordinate(5,100);
	rprintf("Data  0:");
	sonyPS2_setAnalogMode(&controller1);
	sonyPS2_calibrate(&controller1, 20);

	//act_setSpeed(&rudder,DRIVE_SPEED_MAX);

return 0; // dont pause after
}
/***********************************************************************************/
// This routine is called repeatedly - its your main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart){

//int screen_output = uartGetByte(UART0);
/* 
if(screen_output!=-1)
{
	Cursor_Coordinate(5,120);
	Write_Blank(16);
	Cursor_Coordinate(5,120);
	rprintf("Screen Output");
	rprintfNum(10, 4, false, ' ', screen_output);
}
*/
	


if(sonyPS2_read(&controller1))
{
	LED_on(&boardLED0);

	int	data1 = 0xFF & sonyPS2_buttonsRaw(&controller1);
    int	data0 = (0xFF00 & sonyPS2_buttonsRaw(&controller1))>>8;
	uint16_t raw = sonyPS2_buttonsRaw(&controller1);
	
	char xButton_pressure = sonyPS2_buttonPressure(&controller1, PS2_BTN_X);
	Cursor_Coordinate(150,50);
	Write_Blank(3);
	Cursor_Coordinate(150,50);
	rprintfNum(10, 4, true, ' ', xButton_pressure);

	Wait(1);
	Cursor_Coordinate(60,20);
	Write_Blank(4);
	Cursor_Coordinate(60,20);
	rprintfNum(10, 5, false, ' ', raw);
	Cursor_Coordinate(60,90);
	Write_Blank(3);
	Cursor_Coordinate(60,90);
	rprintfNum(10, 4, false, ' ', data1);
	Wait(1);
	Cursor_Coordinate(60,100);
	Write_Blank(3);
	Cursor_Coordinate(60,100);
	rprintfNum(10, 4, false, ' ', data0);
	if(sonyPS2_setAnalogMode(&controller1))
	{
		char shape_y = sonyPS2_joystick(&controller1, PS2_STICK_SHAPE_Y);
		Cursor_Coordinate(150,30);
		Write_Blank(3);
		Cursor_Coordinate(150,30);
		rprintfNum(10, 4, true, ' ', shape_y);
	}
	Wait(1);
	if(sonyPS2_setAnalogMode(&controller1))
	{
		char shape_x = sonyPS2_joystick(&controller1, PS2_STICK_SHAPE_X);
		Cursor_Coordinate(150,40);
		Write_Blank(3);
		Cursor_Coordinate(150,40);
		rprintfNum(10, 4, true, ' ', shape_x);
	}
	if(sonyPS2_setAnalogMode(&controller1))
	{
		char dpad_y = sonyPS2_joystick(&controller1, PS2_STICK_DPAD_Y);
		Cursor_Coordinate(60,30);
		Write_Blank(3);
		Cursor_Coordinate(60,30);
		rprintfNum(10, 4, true, ' ', dpad_y);
	}

	if(sonyPS2_setAnalogMode(&controller1))
	{
		char dpad_x = sonyPS2_joystick(&controller1, PS2_STICK_DPAD_X);
		Cursor_Coordinate(60,40);
		Write_Blank(3);
		Cursor_Coordinate(60,40);
		rprintfNum(10, 4, true, ' ', dpad_x);
	}
	if(sonyPS2_setAnalogMode(&controller1))
	{
		char left_y = sonyPS2_joystick(&controller1, PS2_STICK_LEFT_Y);
		Cursor_Coordinate(60,50);
		Write_Blank(4);
		Cursor_Coordinate(60,50);
		rprintfNum(10, 4, true, ' ', left_y);
	}
	Wait(1);
	if(sonyPS2_setAnalogMode(&controller1))
	{
		char left_x = sonyPS2_joystick(&controller1, PS2_STICK_LEFT_X);
		Cursor_Coordinate(60,60);
		Write_Blank(4);
		//sonyPS_setRumble(&controller1, left_x, true);
		Cursor_Coordinate(60,60);
		rprintfNum(10, 4, true, ' ', left_x);
	}
	

	if(sonyPS2_setAnalogMode(&controller1))
	{
		char right_y = sonyPS2_joystick(&controller1, PS2_STICK_RIGHT_Y);
		Cursor_Coordinate(60,70);
		Write_Blank(4);
		Cursor_Coordinate(60,70);
		rprintfNum(10, 4, true, ' ', right_y);
	}
	
	if(sonyPS2_setAnalogMode(&controller1))
	{
		char right_x = sonyPS2_joystick(&controller1, PS2_STICK_RIGHT_X);
		Cursor_Coordinate(60,80);
		Write_Blank(4);
		Cursor_Coordinate(60,80);
		rprintfNum(10, 4, true, ' ', right_x);
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_R1))
	{
		LED_on(&boardLED1);
		pwmSetDutyCycle(B3, 6);
	}
	else
	{
		LED_off(&boardLED1);
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_L1))
	{
		LED_on(&boardLED2);
		pwmSetDutyCycle(B3, 12);
	}
	else
	{
		LED_off(&boardLED2);
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_R2))
	{
		LED_on(&boardLED3);
	}
	else
	{
		LED_off(&boardLED3);
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_L2))
	{
		LED_on(&boardLED4);
	}
	else
	{
		LED_off(&boardLED4);
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_TRIANGLE))
	{
		LED_on(&boardLED5);
	}
	else
	{
		LED_off(&boardLED5);
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_BTN_X))
	{
		LED_on(&boardLED6);
	}
	else
	{
		LED_off(&boardLED6);
	}
	if(sonyPS2_buttonHeld(&controller1, PS2_DPAD_UP))
	{
		LED_on(&boardLED7);
	}
	else
	{
		LED_off(&boardLED7);
	}
	
}
else
{
	LED_off(&boardLED0);
	LED_off(&boardLED1);
	LED_off(&boardLED2);
	LED_off(&boardLED3);
	LED_off(&boardLED4);
	LED_off(&boardLED5);
	LED_off(&boardLED6);
	LED_off(&boardLED7);
}


/*
		char left_x = sonyPS2_joystick(&controller1, PS2_STICK_LEFT_X);
		Cursor_Coordinate(5,50);
		Write_Blank(12);
		Cursor_Coordinate(5,50);
		rprintf("Left  X:");
		rprintfNum(10, 4, true, ' ', left_x);
		
		char left_y = sonyPS2_joystick(&controller1, PS2_STICK_LEFT_Y);
		Cursor_Coordinate(5,60);
		Write_Blank(12);
		Cursor_Coordinate(5,60);
		rprintf("Left  Y:");
		rprintfNum(10, 4, true, ' ', left_y);
		
		char right_x = sonyPS2_joystick(&controller1, PS2_STICK_RIGHT_X);
		Cursor_Coordinate(5,70);
		Write_Blank(12);
		Cursor_Coordinate(5,70);
		rprintf("Right X:");
		rprintfNum(10, 4, true, ' ', right_x);
	   
		
		char right_y = sonyPS2_joystickRaw(&controller1, PS2_STICK_RIGHT_Y);
		Cursor_Coordinate(40,80);
		Write_Blank(4);
		Cursor_Coordinate(40,80);
		rprintf("Right Y:");
		rprintfNum(10, 4, true, ' ', right_y);
		 */




		
return 200000; // wait for 1 second before calling me again. 1000000us = 1
//second
}



