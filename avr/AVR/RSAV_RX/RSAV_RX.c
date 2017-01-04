// Place any #define statements here before you include ANY other files

// data buffer defines.
#define UART_TX_BUFFER_SIZE 32
#define UART_RX_BUFFER_SIZE 32

#define        TX_SYNC					0
#define        TX_PS2_LX				1
#define        TX_PS2_LY				2
#define        TX_PS2_RX				3
#define        TX_PS2_RY				4
#define        TX_PS2_Circle_Pressure   5
#define        TX_PS2_Triangle_Pressure 6
#define        TX_PS2_X_Pressure		7
#define        TX_PS2_SQ_Pressure		8
#define        TX_PS2_DR_Pressure		9
#define        TX_PS2_DL_Pressure		10
#define        TX_PS2_1					11
#define        TX_END					12


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
#include "pwm.h"
#include "motorPWM.h"
#include "a2d.h"
#include "Motors/Sanyo/LB1836M.h"

// non WebbotLib includes:
//#include <avr/interrupt.h>            // needed because WebbotLib does not handle interrupts on INT[0-7] yet.
#include <avr/wdt.h>                // haven't made use of this yet.
/***********************************************************************************/
// Now create any global variables such as motors, servos, sensors etc
// define variables

uint8_t i = 0;
uint8_t buttonPressed[0xFF];        // used to count how many frames button has been pressed for.
uint8_t buttonReleased[0xFF];        // contains how many frames button was held down for. used to de-bounce buttons
uint8_t SpotLightState = 0;
uint8_t CameraState = 0;
uint8_t BatteryState = 0;
char left_x = 0;
char left_y = 0;
char left_x_pos = 0;
char right_x = 0;
char right_y = 0;
char circle_pressure = 0;
char triangle_pressure = 0;
char xButton_pressure = 0;
char SqButton_pressure = 0;
char DL_Button_pressure = 0;
char DR_Button_pressure = 0;

//const uint8_t NUM_ADC_CHANNELS = 6;
//const uint16_t PROGMEM AVcc_MV = 5000;
// Create a controller SELECTed using D6
// Create a list of devices on the SPI bus

SANYO_LB1836M_MOTOR Motor1 = MAKE_SANYO_LB1836M_MOTOR(FALSE, D5, D6);
SANYO_LB1836M_MOTOR Motor2 = MAKE_SANYO_LB1836M_MOTOR(FALSE, C2, C3);/////////////////////////////////////////////////////////////////////////////////////////////////////////

SANYO_LB1836M_MOTOR_LIST PROGMEM onboard_motors[] = {&Motor1, &Motor2};
SANYO_LB1836M_MOTOR_DRIVER onboard_driver =
MAKE_SANYO_LB1836M_MOTOR_DRIVER(onboard_motors);


//SERVO Rudder = MAKE_SERVO(false,D4,1400,900);
SERVO Pan = MAKE_SERVO(false,C5,1400,900);
SERVO Tilt = MAKE_SERVO(false,C4,1400,900);
static SERVO_LIST Servo_bank1_list[] = {
   // &Rudder,
    &Pan,
    &Tilt
};
SERVO_DRIVER Servo_bank1 = MAKE_SERVO_DRIVER(Servo_bank1_list);

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

servosInit(&Servo_bank1,TIMER1);

sanyoLB1836M_Init(&onboard_driver, 5000);

//sanyoLB1836M_Init(&onboard_driver);

//servosInit(&bank1, TIMER1);
// Set UART0 to 9600 baud
uartInit(UART0, 9600);
// Tell rprintf to output to UART0
rprintfInit(&uart0SendByte);
wdt_enable (WDTO_250MS); 
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

return 0; // dont pause after
}
/***********************************************************************************/
// This routine is called repeatedly - its your main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart){
 
wdt_reset();   
int Data_In = uartGetByte(UART0);
uint8_t inData[32];
uint8_t index = 0;
uint16_t battery_voltage = a2dReadMv(ADC_CH_ADC0);

while(Data_In!=-1)
    {
        inData[index] = Data_In;
        index++;
        Data_In = uartGetByte(UART0);
    }

if(CameraState % 2 == 0)
	{
		left_x =  0;
		left_y = 0;
	}		
right_x = 0;
right_y = 0;
circle_pressure = 0;
triangle_pressure = 0;
xButton_pressure = 0;
SqButton_pressure = 0;
DL_Button_pressure = 0;
DR_Button_pressure = 0;

if(inData[TX_SYNC] == SYNC_PACKET && inData[index-1] == END_PACKET )
{
	if(CameraState % 2 == 0)
	{
		left_x = inData[TX_PS2_LX];
		left_y = inData[TX_PS2_LY];
	}	
	right_x = inData[TX_PS2_RX];
	right_y = inData[TX_PS2_RY];
	circle_pressure = inData[TX_PS2_Circle_Pressure];
	triangle_pressure = inData[TX_PS2_Triangle_Pressure];
	xButton_pressure = inData[TX_PS2_X_Pressure];
	SqButton_pressure = inData[TX_PS2_SQ_Pressure];
	DL_Button_pressure = inData[TX_PS2_DL_Pressure];
	DR_Button_pressure = inData[TX_PS2_DR_Pressure];

    //left_x_pos += (interpolate(left_x, -128, 128, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX));
	//DRIVE_SPEED Left_Pan_Pos = left_x_pos;
    DRIVE_SPEED Left_Pan_Pos = interpolate(left_x, -128, 128, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
//    DRIVE_SPEED Right_Pan_Pos = interpolate(right_x, 0, 255, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
    DRIVE_SPEED Left_Tilt_Pos = interpolate(left_y, -128, 128, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
//    DRIVE_SPEED Right_Tilt_Pos = interpolate(right_y, 0, 255, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
	//DRIVE_SPEED Rudder_Pos = interpolate(right_x, 128, -128, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
    act_setSpeed(&Pan, Left_Pan_Pos);
    act_setSpeed(&Tilt, Left_Tilt_Pos);
	//act_setSpeed(&Rudder, Rudder_Pos);
	
    
    if(battery_voltage < 4500)
    {
        BatteryState++;
        if(BatteryState % 2 == 0)
        {
          //  LED_off(&RightRed);
            //LED_off(&LeftRed);
            //LED_off(&SpotLightWhite);
            LED_off(&PowerGreen);
        }
        else
        {
            //LED_on(&RightRed);
            //LED_on(&LeftRed);
            //LED_on(&SpotLightWhite);
            LED_on(&PowerGreen);
        }
    }
    
	//if(inData[TX_PS2_1] == PS2_B_BTN_TRIANGLE)
	if(triangle_pressure > 0)
    {
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
	
    if(inData[TX_PS2_1] == PS2_B_BTN_R1)
    {
        LED_on(&RightRed);
		LED_off(&LeftRed);
      //  act_setSpeed(&Rudder, DRIVE_SPEED_MIN);
    }
    else if(DR_Button_pressure > 0)
    {
        LED_off(&RightRed);
		LED_on(&LeftRed);
       /* if(DR_Button_pressure == 255)
        {
            DRIVE_SPEED Right_Rudder_Pos = interpolate(DR_Button_pressure, 0, 255, 0, DRIVE_SPEED_MIN);
            act_setSpeed(&Rudder, Right_Rudder_Pos);
        }
        else
        {
            */

			DRIVE_SPEED Forward_Motor_Speed = interpolate(xButton_pressure, 0, 255, 0, DRIVE_SPEED_MIN);
            act_setSpeed(&Motor1, Forward_Motor_Speed);

		//	DRIVE_SPEED Right_Rudder_Pos = interpolate(DR_Button_pressure, 0, 255, 0, DRIVE_SPEED_MAX);
           // act_setSpeed(&Rudder, Right_Rudder_Pos);
        //}
	
    }
    else if(inData[TX_PS2_1] == PS2_B_BTN_L1)
    {
        LED_on(&LeftRed);
		LED_off(&RightRed);
       // act_setSpeed(&Rudder, DRIVE_SPEED_MAX);
    }
    else if(DL_Button_pressure > 0)
    {
        LED_off(&LeftRed);
		LED_on(&RightRed);
		/*
        if(DL_Button_pressure == 255)
        {
            */
			//DRIVE_SPEED Left_Rudder_Pos = interpolate(DL_Button_pressure, 0, 255, 0, DRIVE_SPEED_MIN);
            //act_setSpeed(&Rudder, Left_Rudder_Pos);

			DRIVE_SPEED Forward_Motor_Speed = interpolate(xButton_pressure, 0, 255, 0, DRIVE_SPEED_MAX);
            act_setSpeed(&Motor2, Forward_Motor_Speed);
       /* }
        else
        {
            DRIVE_SPEED Left_Rudder_Pos = interpolate(DL_Button_pressure, 0, 255, 0, DRIVE_SPEED_MAX);
            act_setSpeed(&Rudder, Left_Rudder_Pos);
        }
		*/
    }
    else 
    {
        LED_off(&RightRed);
		LED_off(&LeftRed);
//		act_setSpeed(&Rudder, Rudder_Pos);
    }
    if(inData[TX_PS2_1] == PS2_B_BTN_R2)
    {
       act_setSpeed(&Pan, DRIVE_SPEED_MIN);
    }
    else
    {
       
    }

    if(inData[TX_PS2_1] == PS2_B_BTN_L2)
    {
       act_setSpeed(&Pan, DRIVE_SPEED_MAX);
    }
    else
    {
       
    }

    //if(inData[TX_PS2_1] == PS2_B_BTN_CIRCLE)
	if(circle_pressure > 0)
    {
	   CameraState++;
    }
    else
    {
   
    }

 
    if(SqButton_pressure > 0)
    {
		DRIVE_SPEED Backward_Motor_Speed = interpolate(SqButton_pressure, 0, 255, 0, DRIVE_SPEED_MIN);
        act_setSpeed(&Motor1, Backward_Motor_Speed);
		/*
		if(DL_Button_pressure > 0)	
		{
			LED_off(&RightRed);
			LED_on(&LeftRed);
		}
		else if(DR_Button_pressure > 0)	
		{
			LED_on(&RightRed);
			LED_off(&LeftRed);
		}
		*/	
		if(DR_Button_pressure == 0 && DL_Button_pressure == 0)
		{
			LED_on(&RightRed);
			LED_on(&LeftRed);
		}
		/*
        if(SqButton_pressure == 255)
        {
            */
	
			/*
        }
        else
        {
            DRIVE_SPEED Backward_Motor_Speed = interpolate(SqButton_pressure, 0, 255, 0, DRIVE_SPEED_MIN);
            act_setSpeed(&Motor, Backward_Motor_Speed);
        }
		*/
    }
    else if(xButton_pressure > 0)
    {
        /*
		if(xButton_pressure == 255)
        {
            */
			DRIVE_SPEED Forward_Motor_Speed = interpolate(xButton_pressure, 0, 255, 0, DRIVE_SPEED_MAX);
            act_setSpeed(&Motor1, Forward_Motor_Speed);
		
			if(DR_Button_pressure == 0 && DL_Button_pressure == 0)
			{
				LED_off(&RightRed);
				LED_off(&LeftRed);
			}
		
			/*
        }
        else
        {
            DRIVE_SPEED Forward_Motor_Speed = interpolate(xButton_pressure, 0, 255, 0, DRIVE_SPEED_MAX);
            act_setSpeed(&Motor, Forward_Motor_Speed);
        }
		*/
    }
    else
    {
        /*
        if(act_getSpeed(&Motor) > 0)
        {
            act_setSpeed(&Motor, act_getSpeed(&Motor)-1);
        }
        else if(act_getSpeed(&Motor) < 0)
        {
            act_setSpeed(&Motor, act_getSpeed(&Motor)+1);
        }
        else
        {
            act_setSpeed(&Motor, 0);
        }
        */
		act_setSpeed(&Motor1, 0);
    }
   
    if(inData[TX_PS2_1] == PS2_B_DPAD_UP)
    {
       act_setSpeed(&Motor1, DRIVE_SPEED_MAX);
    }
    else
    {

    }
    if(inData[TX_PS2_1] == PS2_B_DPAD_DOWN)
    {
       act_setSpeed(&Motor1, DRIVE_SPEED_MIN);
    }
    else
    {
       
    }

    if(inData[TX_PS2_1] == PS2_B_BTN_SELECT)
    {
   //    act_setSpeed(&Rudder, 0);
	 //  act_setSpeed(&Pan, 0);
    }
    else
    {
   
    }

    if(inData[TX_PS2_1] == PS2_B_BTN_START)
    {
       
    }
    else
    {
   
    }

    if(inData[TX_PS2_1] == PS2_B_BTN_L3)
    {
       
    }
    else
    {
   
    }
	if(inData[TX_PS2_1] == PS2_B_BTN_R3)
    {
		
	}	
	else
	{
		
	}		
}
else
{
    /*
    act_setSpeed(&Rudder, 0);
    act_setSpeed(&Pan, 0);
    act_setSpeed(&Tilt,0); //-40
    act_setSpeed(&Motor, 0);
    LED_off(&RightRed);
    LED_off(&LeftRed);
	*/
	
    
}
       
      //  TICK_COUNT ms = loopStart / 1000;        // Get current time in ms
    //int16_t now = ms % (TICK_COUNT)10000;     // 10 sec for a full swing
//    if(now >= (int16_t)5000)
//    {                                        // Goes from 0ms...5000ms
//        now = (int16_t)10000 - now;            // then 5000ms...0ms
//    }
    // Map it into DRIVE_SPEED range
    //DRIVE_SPEED Servo_speed = interpolate(now, 0, 5000, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
    //DRIVE_SPEED Motor_speed = interpolate(now, 0, 255, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
    // Set speed for all motors/servos
//    act_setSpeed(&Rudder,Servo_speed);
//    act_setSpeed(&Pan,(Servo_speed/2));
//    act_setSpeed(&Tilt,(Servo_speed/4));
//    act_setSpeed(&Engine, 128);
//    act_setSpeed(&Pan_Motor,speed);
    // -------- End   Actuators -------
   
return 200000; // wait for 1 second before calling me again. 1000000us = 1
//second
}
