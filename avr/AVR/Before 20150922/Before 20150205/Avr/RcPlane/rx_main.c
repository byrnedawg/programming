/*
 * main.c
 *
 *  Created on: 15 Feb 2010
 *  Last modified: 14 Nov 2010
 *      Author: Duncan Law
 *
 *
 *      Copyright (C) 2010 Duncan Law
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *
 *  Fuse bits:
 *  Internal clock @ 8MHz. Bootloader enabled.
 *  	avrdude -p m2561 -c dapa -P /dev/parport0 -U lfuse:w:0xc2:m -U hfuse:w:0xd6:m -U efuse:w:0xfd:m
 *  External crystal @ 8-16MHz. Bootloader enabled.
 *  	avrdude -p m2561 -c dapa -P /dev/parport0 -U lfuse:w:0xce:m -U hfuse:w:0xd6:m -U efuse:w:0xfd:m
 */


// Un-comment this line depending on whether you want to use WebbotLib or the simple native (fast) code for SPI communications on the Receiver node.
// WebbotLib's SPI code is far more robust and extendible than this projects native SPI code.
// If you plan on adding your own SPI devices to this project then you may want to consider using WebbotLib's SPI code.
// If on the other hand you plan to run this project "as is" you probably want to use this project's native SPI code
// as it is quicker and will allow more retransmissions of any failed packets.
//#define USE_WEBBOTLIB_SPI

#include "cyrf.h"
#include "rx_main.h"
#include <avr/wdt.h>
#include <math.h>

// define variables
//#define MODEL_FLYINGWING
#define RF_MODULE_ARTAFLEX			// don't know if this works with latest version of artaflex modules


TICK_COUNT frameStart;				// used to remember system clock time at start of frame.

uint8_t data[255];
uint8_t data_Flag[255];				// not yet implemented. will use this to keep track of changes in data.
uint8_t EEMEM data_eeprom[255];		// wasteful but we have plenty.

uint8_t buttonPressed[0xFF];		// used to count how many frames button has been pressed for.
uint8_t buttonReleased[0xFF];		// contains how many frames button was held down for. used to de-bounce buttons

uint16_t servoBuffer[16];			// 16bit buffer containing current servo position.

int sensorCount, sensorCount1;				// keep track of number of times ADC has been read.
long int rcAccelX, rcAccelY, rcAccelZ, rcGyroX, rcGyroY;		// total of X axis gyro readings.
long int currDraw;
struct RACC RAcc1;
struct ATTITUDE AtAcc, AtGyr, AtEst0, AtEst1;
float  RAccSize1;					// size of accelerometer vector
static int AccOffset = 0x200;		// offset of accelerometer sensor
float GyroBias = 100;					// effect of Gyro vs Accelerometer.
float GyroAmp = .1;					// amplitude of Gyro output.

#define DATA_FLAG_CHANGED	 	0x01
#define DATA_FLAG_TX_EEPROM 	0x02
#define DATA_FLAG_RX_EEPROM 	0x04

uint16_t uptime;

// Define servos
SERVO servo_0 	= MAKE_SERVO(TRUE , B7, 1000, 1000);
SERVO servo_1 	= MAKE_SERVO(TRUE , B6, 1500, 500);
SERVO servo_2 	= MAKE_SERVO(TRUE , B5, 1500, 500);
SERVO servo_3 	= MAKE_SERVO(TRUE , E5, 1500, 500);
SERVO servo_4 	= MAKE_SERVO(TRUE , E4, 1500, 500);
SERVO servo_5 	= MAKE_SERVO(TRUE , E3, 1500, 500);
// Create the list - remember to place an & at the
// start of each servo name
SERVO_LIST servos[] = {&servo_0,&servo_1,&servo_2,&servo_3,&servo_4,&servo_5};
// Create a driver for the list of servos
SERVO_DRIVER bank1 = MAKE_SERVO_DRIVER(servos);


// define CYRF6936 RF modules on SPI bus.
#define MAKE_SPI_GENERIC_DEVICE(mode,order,select,filler) { MAKE_SPI_DEVICE(mode,order,select,filler) }
CYPRESS_CYRF cyrf_0 = MAKE_SPI_GENERIC_DEVICE(SPI_MODE_0,SPI_DATA_ORDER_MSB,B0,0x00);
CYPRESS_CYRF cyrf_1 = MAKE_SPI_GENERIC_DEVICE(SPI_MODE_0,SPI_DATA_ORDER_MSB,B4,0x00);


static SPI_DEVICE_LIST devices[] = {&cyrf_0._device_, &cyrf_1._device_};
SPI bus = MAKE_SPI(devices);


// Now create any global variables such as motors, servos, sensors etc
// This routine is called once only and allows you to do set up the hardware
// Dont use any 'clock' functions here - use 'delay' functions instead
void appInitHardware(void){
	a2dSetPrescaler(ADC_PRESCALE_DIV128);

	cyrf6936_Initialise_hard();


	// Initialise SPI bus as master. (RF modules connected to hardware SPI)
    spiBusInit(&bus, TRUE);
	spiDeviceSelect(&cyrf_0, TRUE);
	spiDeviceSelect(&cyrf_0, FALSE);
	spiDeviceSelect(&cyrf_1, TRUE);
	spiDeviceSelect(&cyrf_1, FALSE);

	// set I/O pins for RF module(s).
	pin_make_input(D4, FALSE);		// set PACTL pin to input. (module on connector J1)
	pin_make_input(D5, FALSE);		// set PACTLn pin to input. (module on connector J1)
	pin_make_input(D6, FALSE);		// set PACTL pin to input. (module on connector J2)
	pin_make_input(D7, FALSE);		// set PACTLn pin to input. (module on connector J2)

#ifdef RF_MODULE_ARTAFLEX
	//
	pin_make_output(D4, FALSE);			// set RXPA pin to output. (module on connector J1)
	pin_make_output(D5, FALSE);			// set TXPA pin to output. (module on connector J1)
	pin_make_output(D6, FALSE);			// set RXPA pin to output. (module on connector J2)
	pin_make_output(D7, FALSE);			// set TXPA pin to output. (module on connector J2)
#endif

	pin_make_input(E7, TRUE);		// set UNIGEN RF module IRQ pin to input. (module on connector J1)
	pin_make_input(E6, TRUE);		// set UNIGEN RF module IRQ pin to input. (module on connector J2)
	pin_make_output(G3, FALSE);			// set UNIGEN RF module RST pin to output. (both modules)
	//pin_low(G3);					// don't reset yet.

	// set I/O pins for status LEDs
	pin_make_output(C0, TRUE);			// set LED pin for output
	pin_make_output(C1, TRUE);			// set LED pin for output
	pin_make_output(C2, FALSE);			// set LED pin for output
	//pin_high(C0);					// LED off
	//pin_high(C1);					// LED off
	//pin_low(C2);					// LED on


	// Set UART1 to 19200 baud
    uartInit(UART1, 38400);
    // Tell rprintf to output to UART1
    rprintfInit(&uart1SendByte);


    // Initialise the servo controller using Hardware PWM
    servoPWMInit(&bank1);


    // Initialise WatchDog Timer
    wdt_enable( WDTO_500MS );

}

// This routine is called once to allow you to set up any other variables in your program
// You can use 'clock' function here.
// The loopStart parameter has the current clock value in μS
TICK_COUNT appInitSoftware(TICK_COUNT loopStart){

	act_setSpeed(&servo_0,DRIVE_SPEED_CENTER);
	act_setSpeed(&servo_1,DRIVE_SPEED_CENTER);
	act_setSpeed(&servo_2,DRIVE_SPEED_CENTER);
	act_setSpeed(&servo_3,DRIVE_SPEED_CENTER);
	act_setSpeed(&servo_4,DRIVE_SPEED_CENTER);
	act_setSpeed(&servo_5,DRIVE_SPEED_CENTER);

	// starting values for trims.
	data[RX_SERVO_0_CH] = eeprom_read_byte(&data_eeprom[RX_SERVO_0_CH]);
	data[RX_SERVO_0_CL] = eeprom_read_byte(&data_eeprom[RX_SERVO_0_CL]);
	data[RX_SERVO_1_CH] = eeprom_read_byte(&data_eeprom[RX_SERVO_1_CH]);
	data[RX_SERVO_1_CL] = eeprom_read_byte(&data_eeprom[RX_SERVO_1_CL]);
	data[RX_SERVO_2_CH] = eeprom_read_byte(&data_eeprom[RX_SERVO_2_CH]);
	data[RX_SERVO_2_CL] = eeprom_read_byte(&data_eeprom[RX_SERVO_2_CL]);
	data[RX_SERVO_3_CH] = eeprom_read_byte(&data_eeprom[RX_SERVO_3_CH]);
	data[RX_SERVO_3_CL] = eeprom_read_byte(&data_eeprom[RX_SERVO_3_CL]);
	data[RX_SERVO_4_CH] = eeprom_read_byte(&data_eeprom[RX_SERVO_4_CH]);
	data[RX_SERVO_4_CL] = eeprom_read_byte(&data_eeprom[RX_SERVO_4_CL]);
	data[RX_SERVO_0_MUL] = eeprom_read_byte(&data_eeprom[RX_SERVO_0_MUL]);
	data[RX_SERVO_1_MUL] = eeprom_read_byte(&data_eeprom[RX_SERVO_1_MUL]);
	data[RX_SERVO_2_MUL] = eeprom_read_byte(&data_eeprom[RX_SERVO_2_MUL]);
	data[RX_SERVO_3_MUL] = eeprom_read_byte(&data_eeprom[RX_SERVO_3_MUL]);
	data[RX_SERVO_4_MUL] = eeprom_read_byte(&data_eeprom[RX_SERVO_4_MUL]);


	data[RX_AUTOP_X_MUL] = eeprom_read_byte(&data_eeprom[RX_AUTOP_X_MUL]);
	data[RX_AUTOP_Y_MUL] = eeprom_read_byte(&data_eeprom[RX_AUTOP_Y_MUL]);
	data[RX_AUTOP_X_TRIM] = eeprom_read_byte(&data_eeprom[RX_AUTOP_X_TRIM]);
	data[RX_AUTOP_Y_TRIM] = eeprom_read_byte(&data_eeprom[RX_AUTOP_Y_TRIM]);

	data[RX_GYRO_X_OFFSET_L] = eeprom_read_byte(&data_eeprom[RX_GYRO_X_OFFSET_L]);
	data[RX_GYRO_X_OFFSET_H] = eeprom_read_byte(&data_eeprom[RX_GYRO_X_OFFSET_H]);
	data[RX_GYRO_Y_OFFSET_L] = eeprom_read_byte(&data_eeprom[RX_GYRO_Y_OFFSET_L]);
	data[RX_GYRO_Y_OFFSET_H] = eeprom_read_byte(&data_eeprom[RX_GYRO_Y_OFFSET_H]);



	if ((data[RX_SERVO_1_CH] < 0x60) | (data[RX_SERVO_1_CH] > 0x80)){
		reset_trims();
	}

	if ((data[RX_SERVO_3_MUL] > 137) || (data[RX_SERVO_3_MUL] < 117)){ data[RX_SERVO_3_MUL] = 127;}
	if ((data[RX_SERVO_4_MUL] > 137) || (data[RX_SERVO_4_MUL] < 117)){ data[RX_SERVO_4_MUL] = 127;}


	// initialise RF module.
	cyrf6936_Initialise_soft(&cyrf_0);
	cyrf6936_Initialise_soft(&cyrf_1);



	// enable interrupt on pin connected to cyrf6936 interrupt pin.
	cyrfIntEnable();

	frameStart=clockGetus();

	data[RX_BAT_VOLT] = a2dConvert8bit(ADC_CH_ADC0);

	AtEst1.AYZ = AtEst0.AYZ = 0x2A7FF;
	AtEst1.AXZ = AtEst1.AXZ = 0x38DFF;


	return 0; // don't pause after
}



// This routine is called repeatedly - its your main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart){
	TICK_COUNT frameEnd = frameStart + 20000;

	wdt_reset();

	// very occasionally RF module misses interrupt.
	// this usually happens when signal is weak.
	// testing interrupt pin manually here is an effective workaround.
	//if (pin_is_low(E7)){
	//	cyrfInterupt(&cyrf_0);
	//}

	//cyrf6936_RX_mode(&cyrf_0);

	// poll sensors as often as possible
	// get readings from A2D.
	//int tmp = a2dConvert8bit(ADC_CH_ADC6);
	// = ;
	currDraw += a2dConvert10bit(ADC_CH_ADC1);

	rcAccelX +=	a2dConvert10bit(ADC_CH_ADC7);
	rcAccelY +=	a2dConvert10bit(ADC_CH_ADC6);
	rcAccelZ +=	a2dConvert10bit(ADC_CH_ADC5);
	rcGyroX += a2dConvert10bit(ADC_CH_ADC4);
	rcGyroY += a2dConvert10bit(ADC_CH_ADC3);
	sensorCount++;			// keep track of number of times ADC has been read.


	int tmp;
	if(frameEnd <= clockGetus()) {
		frameStart = frameEnd;
		// this section happens every 20ms frame.

		// frame counter.
		data[RX_FRA_NUM]++;

		data[RX_BAT_CUR] = (currDraw / sensorCount) >>2;

		// average accelerometer.
		RAcc1.RXACC = (rcAccelX / sensorCount) -AccOffset;
		RAcc1.RYACC = (rcAccelY / sensorCount) -AccOffset;
		RAcc1.RZACC = (rcAccelZ / sensorCount) -AccOffset;
		RAccSize1 = sqrt((RAcc1.RXACC*RAcc1.RXACC)+(RAcc1.RYACC*RAcc1.RYACC)+(RAcc1.RZACC*RAcc1.RZACC));

		// extract roll and pitch from accelerometer's 3 axis.
		data[RX_ACCEL_Y] = AtAcc.AXZ = 90*atan2(RAcc1.RXACC,RAcc1.RZACC) + data[RX_AUTOP_Y_TRIM] - 0x7F;
		data[RX_ACCEL_Y] += 0x7F;
		data[RX_ACCEL_X] = AtAcc.AYZ = 90*atan2(RAcc1.RYACC,RAcc1.RZACC) + data[RX_AUTOP_X_TRIM] - 0x7F;
		data[RX_ACCEL_X] += 0x7F;

		// *** experimental ***
		// multiply accelerometer readings by a factor of current draw.
		data[RX_ACCEL_Y] = AtAcc.AXZ = AtAcc.AXZ - data[RX_BAT_CUR]/4.5;
		data[RX_ACCEL_Y] += 0x7F;
		data[RX_ACCEL_X] = AtAcc.AYZ = AtAcc.AYZ - data[RX_BAT_CUR]/5.5;
		data[RX_ACCEL_X] += 0x7F;

		// average gyro.
		int x_gyro_offset = data[RX_GYRO_X_OFFSET_L] + (0x100*data[RX_GYRO_X_OFFSET_L]);
		int y_gyro_offset = data[RX_GYRO_Y_OFFSET_L] + (0x100*data[RX_GYRO_Y_OFFSET_L]);

		data[RX_GYRO_X] = AtGyr.AYZ = (rcGyroX / sensorCount);
		data[RX_GYRO_X] += 0x7F - x_gyro_offset;
		data[RX_GYRO_Y] = AtGyr.AXZ = (rcGyroY / sensorCount);
		data[RX_GYRO_Y] += 0x7F - y_gyro_offset;

		// work out estimated angle based on previous estimate and sensor data.
		data[RX_EST_X] = AtEst1.AYZ = (((AtEst0.AYZ + ((AtGyr.AYZ - x_gyro_offset)*GyroAmp))*GyroBias) + (AtAcc.AYZ)) / (1 + GyroBias);
		data[RX_EST_X] += 0x7F;
		data[RX_EST_Y] = AtEst1.AXZ = (((AtEst0.AXZ + ((AtGyr.AXZ - y_gyro_offset)*GyroAmp))*GyroBias) + (AtAcc.AXZ)) / (1 + GyroBias);
		data[RX_EST_Y] += 0x7F;

		// something mental is going on at power on so stop excessive values.
		if(AtEst1.AYZ > (0x2A7FF + 90)){ AtEst1.AYZ = (0x2A7FF + 90);}
		if(AtEst1.AYZ < 0x2A7FF - 90){ AtEst1.AYZ = (0x2A7FF - 90);}
		if(AtEst1.AXZ > (0x38DFF + 90)){ AtEst1.AXZ = (0x38DFF + 90);}
		if(AtEst1.AXZ < 0x38DFF - 90){ AtEst1.AXZ = (0x38DFF - 90);}

		//data[RX_EST_X] = data[RX_ACCEL_X];
		//data[RX_EST_Y] = data[RX_ACCEL_Y];

		//rprintfu16((int)(AtGyr.AXZ/0x10000) + 0x8000);
		//rprintfu16(AtGyr.AXZ );
		//rprintfProgStrM("\n");

		currDraw = rcAccelX = rcAccelY = rcAccelZ = rcGyroX = rcGyroY = 0;
		AtEst0 = AtEst1;
		sensorCount1=sensorCount;
		sensorCount=0;


/*		// read battery and current sensor
		tmp = a2dConvert8bit(ADC_CH_ADC0);
		if(tmp > data[RX_BAT_VOLT]){
			data[RX_BAT_VOLT]++;
		}
		else if(tmp < data[RX_BAT_VOLT]){
			data[RX_BAT_VOLT]--;
		}

		data[RX_BAT_CUR] = a2dConvert8bit(ADC_CH_ADC1);
*/


		if(!(testIfPacketReceived(data)==0)){
			// this section done if a packet has been received this frame.
			// (the data[] should have arrived during an interrupt on the RF module IRQ pin.)

			uptime++;

			// set servo positions.
			set_servos(data);

			pin_high(C1);					// warning LED off

			// watch for PS2 controller button presses.
			// these are de-bounced by running a simple counter.
			uint16_t tmp = ((data[TX_PS2_0] <<8) + data[TX_PS2_1]);
			uint8_t i;
			for (i=0; i<16; i++){
				if(tmp & (1<<i)){
					// button pressed. increase counter
					//rprintfProgStrM("\nPressed:"); rprintfu08(i);
					if (buttonPressed[i] < 0xFF){ buttonPressed[i]++; }
				}
				else if(buttonPressed[i] > 0){
					// button has just been released. save value in counter to de-bounce buttons.
					//rprintfProgStrM("\nReleased:"); rprintfu08(i);
					buttonReleased[i]=buttonPressed[i];
					buttonPressed[i]=0;
				}
				else{
					buttonReleased[i]=0;
					buttonPressed[i]=0;
				}
			}

			if (buttonReleased[PS2_B_BTN_TRIANGLE] > 5){
				set_trims(data);
			}
			if (buttonReleased[PS2_B_BTN_CIRCLE] > 5){
				reset_trims();
			}
			if (buttonReleased[PS2_B_BTN_X] > 5){

				data[RX_GYRO_X_OFFSET_H] = (uint8_t)(AtGyr.AYZ / 0x100);
				data[RX_GYRO_X_OFFSET_L] = (uint8_t)(AtGyr.AYZ - (AtGyr.AYZ*0x100));
				data[RX_GYRO_Y_OFFSET_H] = (uint8_t)(AtGyr.AXZ / 0x100);
				data[RX_GYRO_Y_OFFSET_L] = (uint8_t)(AtGyr.AXZ - (AtGyr.AXZ*0x100));
				eeprom_write_byte(&data_eeprom[RX_GYRO_X_OFFSET_H], data[RX_GYRO_X_OFFSET_H]);
				eeprom_write_byte(&data_eeprom[RX_GYRO_X_OFFSET_L], data[RX_GYRO_X_OFFSET_L]);
				eeprom_write_byte(&data_eeprom[RX_GYRO_Y_OFFSET_H], data[RX_GYRO_Y_OFFSET_H]);
				eeprom_write_byte(&data_eeprom[RX_GYRO_Y_OFFSET_L], data[RX_GYRO_Y_OFFSET_L]);

			}
			if (buttonReleased[PS2_B_DPAD_UP] > 5){
				if (buttonPressed[PS2_B_BTN_R1] > 0){
					if(buttonPressed[PS2_B_BTN_R2] > 0){
						//increase autopilot sensitivity (Y axis).
						data[RX_AUTOP_Y_MUL] ++;
					}
					else{
						//increase autopilot trim (Y axis).
						data[RX_AUTOP_Y_TRIM] ++;
					}
				}
				else if (data[RX_SERVO_2_MUL] < 137){
					data[RX_SERVO_2_MUL]++;
				}
			}
			if (buttonReleased[PS2_B_DPAD_DOWN] > 5){
				if (buttonPressed[PS2_B_BTN_R1] > 0){
					if(buttonPressed[PS2_B_BTN_R2] > 0){
						if (data[RX_AUTOP_Y_MUL] > 2){
							//decrease aoutopilot sensitivity (Y axis).
							data[RX_AUTOP_Y_MUL] --;
						}
					}
					else{
						//decrease autopilot trim  (Y axis).
						data[RX_AUTOP_Y_TRIM] --;
					}
				}
				else if (data[RX_SERVO_2_MUL] > 117){
					data[RX_SERVO_2_MUL]--;
				}
			}
			if (buttonReleased[PS2_B_DPAD_RIGHT] > 5){
				if (buttonPressed[PS2_B_BTN_R1] > 0){
					if(buttonPressed[PS2_B_BTN_R2] > 0){
						//increase aoutopilot sensitivity (X axis).
						data[RX_AUTOP_X_MUL] ++;
					}
					else{
						//increase autopilot trim  (X axis).
						data[RX_AUTOP_X_TRIM] ++;
					}
				}
				else if (data[RX_SERVO_4_MUL] < 137){
					data[RX_SERVO_4_MUL]++;
					data[RX_SERVO_3_MUL] = data[RX_SERVO_4_MUL];
				}
			}
			if (buttonReleased[PS2_B_DPAD_LEFT] > 5){
				if (buttonPressed[PS2_B_BTN_R1] > 0){
					if(buttonPressed[PS2_B_BTN_R2] > 0){
						if (data[RX_AUTOP_X_MUL] > 2){
							//decrease autopilot sensitivity (X axis).
							data[RX_AUTOP_X_MUL] --;
						}
					}
					else{
						//decrease autopilot trim  (X axis).
						data[RX_AUTOP_X_TRIM] --;
					}
				}
				else if (data[RX_SERVO_4_MUL] > 117){
					data[RX_SERVO_4_MUL]--;
					data[RX_SERVO_3_MUL] = data[RX_SERVO_4_MUL];
				}
			}

		}
		else{
			// no packet received this frame
			pin_low(C1);					// warning LED on
		}

		// enable receive mode.
		cyrf6936_RX_mode(&cyrf_0);
		cyrf6936_RX_mode(&cyrf_1);

		if(data[RX_PAC_GAP] > 25){
			// no data received for 1/2 second
			// so clear PS2 controller readings
			// and put servos to default positions.
			if(data[TX_CONTROLER]==0){
				data[TX_PS2_LY] = 128;
			}
			else{
				data[TX_PS2_LY] = 254;
			}
			data[TX_PS2_LX] = 128;
			data[TX_PS2_RY] = 128;
			data[TX_PS2_RX] = 128;

			set_servos(data);
			//pin_low(C0);					// warning LED on

		}
		else{
			//pin_high(C0);					// warning LED off
		}

		// debug status LED off
		pin_high(C0);


	}



	// once per second this stuff will happen
	if ((data[RX_FRA_NUM])==49){
		// reset counter every second
		data[RX_FRA_NUM] = 0;

		// flash status LED.
		pin_toggle(C2);

		// store RSSI data.
		data[RX_RSSI_1S] = cyrfRssiCounter * 255/1550;
		cyrfRssiCounter=0;


		tmp = a2dConvert8bit(ADC_CH_ADC0);
		if (tmp > data[RX_BAT_VOLT]){
			data[RX_BAT_VOLT]++;
		}
		else if (tmp < data[RX_BAT_VOLT]){
			data[RX_BAT_VOLT]--;
		}

//		data[RX_BAT_CUR] = a2dConvert8bit(ADC_CH_ADC1);


		// uptime counter (in seconds)
		data[RX_UPTIME_L]++;
		if (data[RX_UPTIME_L]==0){
			data[RX_UPTIME_H]++;
		}

		// store successful frames last second
		data[RX_PAC_P1_1S]=data[RX_PAC_P1_CNT];
		data[RX_PAC_P1_CNT]=0;

		// store number of frames with errors last second
		data[RX_PAC_ERR_1S]=data[RX_PAC_ERR_CNT];
		data[RX_PAC_ERR_CNT]=0;

		// save counter for largest number of consecutive RX errors last second
		data[RX_PAC_CON_ERR_1S] = data[RX_PAC_GAP_CNT];
		data[RX_PAC_GAP_CNT] = 0;

/*		for (uint8_t i=1; i<=8; i++){
			rprintfu08(data[i]);
			rprintfProgStrM(" ");
		}

		rprintfProgStrM("E");
		rprintfu08(data[RX_ERR_CODE]);*/

		//rprintfProgStrM(" S");
		//rprintfu08(data[RX_PAC_P1_1S]);
/*		rprintfProgStrM(" F");
		rprintfu08(data[RX_PAC_ERR_1S]);
		rprintfProgStrM(" C");
		rprintfu08(data[RX_PAC_CON_ERR_1S]);	// max failed frames last second
		rprintfProgStrM(" S1:");
		rprintfu08(data[RX_SERVO_1_CH]);
		rprintfu08(data[RX_SERVO_1_CL]);
		rprintfProgStrM(" T");
		rprintfu08(data[RX_UPTIME_H]);
		rprintfu08(data[RX_UPTIME_L]);*/

		//rprintfu16(RAcc1.RXACC+AccOffset);
		//rprintfProgStrM(" ");
		//rprintfu16(RAcc1.RYACC+AccOffset);
		//rprintfProgStrM(" ");
		//rprintfu16(RAcc1.RZACC+AccOffset);
		//rprintfProgStrM(" ");
		//rprintfu08(sensorCount1);
		//rprintfProgStrM(" ");
		//rprintfu16(RAccSize1);
		//rprintfProgStrM(" ");
	/*	rprintfu16(0x800+AtAcc.AYZ);
		rprintfProgStrM(" ");
		rprintfu16(0x800+AtAcc.AXZ);
		rprintfProgStrM(" ");
		rprintfu16(0x800+AtEst1.AYZ);
		rprintfProgStrM(" ");
		rprintfu16(0x800+AtEst1.AXZ);
		rprintfProgStrM("  ");
		rprintfu16(AtGyr.AYZ);
		rprintfProgStrM(" ");
		rprintfu16(AtGyr.AXZ );
		rprintfProgStrM("  ");
		rprintfu16(sensorCount1);
		rprintfProgStrM("\n");*/

		if(data[RX_PAC_GAP] > 0x30){
			// no data received for ~ 1 second so reset cyrf6936.
			// this will cause the cyrf6936 to be unresponsive for a few ms so don't do it too often.
			cyrf6936_Initialise_hard();
			cyrf6936_Initialise_soft(&cyrf_0);
			cyrf6936_Initialise_soft(&cyrf_1);
			data[RX_PAC_GAP] =0;
		}


	}



	//printfu08(data[RX_ERR_CODE]);

	//cyrf6936_RX_mode(&cyrf_0);

//	return 20000;
	return 0;
}



// SPI send byte as address and return data received.
uint8_t cyrf_SPI_Read(uint8_t register_address, CYPRESS_CYRF* cyrf)
{
	// first bit sets CYRF6936 data direction. (not done here. DIR =0)
	// next bit is set if you want multiple bytes data to be written. (not done here. INC=0)
	// next 6 bits are CYRF6936 register address.
	register_address = (register_address & 0b00111111);

#ifdef USE_WEBBOTLIB_SPI
	// use WebbotLib for SPI code.
	// This option is slower but more robust and flexible if you want to add other devices to the SPI bus.
	uint8_t tmp = 0xFF & spiDeviceXferWord(cyrf, (uint16_t)(register_address<<8));
	spiDeviceSelect(cyrf, FALSE);
#else
	// Don't use WebbotLib for SPI code.
	// This option executes quicker allowing a greater number of re-tries when RF transmission fails
	// but is far less extendible if you want to add other SPI devices.
	pin_low(spiGetAbstractDevice(cyrf)->select);		// SS pin low to initiate SPI. (WebbotLib knows which pin so we get it from there.)
	//pin_low(B0);										// this is about 25uS quicker.

	SPDR = register_address;		// send byte (Address)
    while(!(SPSR & (1<<SPIF)));

    SPDR = 0x00;					// not sending anything on the next cycle.
    while(!(SPSR & (1<<SPIF)));

	pin_high(spiGetAbstractDevice(cyrf)->select);		// SS pin high to end SPI. (WebbotLib knows which pin so we get it from there.)
	//pin_high(B0);										// this is about 25uS quicker.
    uint8_t tmp = SPDR;				// data received.
#endif

	return tmp;
}


// SPI send byte as address and send following byte
void cyrf_SPI_Write(uint8_t register_address, uint8_t data, CYPRESS_CYRF* cyrf)
{
	// first bit sets CYRF6936 data direction. (we want this. DIR =1)
	// next bit is set if you want multiple bytes data to be written. (not done here. INC=0)
	// next 6 bits are CYRF6936 register address.
	// next 8 bits are data to be written to CYRF6936.
	register_address = (register_address & 0b10111111) | 0b10000000; // make sure INC not set and DIR is set..

#ifdef USE_WEBBOTLIB_SPI
	// use WebbotLib for SPI code.
	// This option is slower but more robust and flexible if you want to add other devices to the SPI bus.
	spiDeviceSendWord(cyrf, ((register_address<<8) + data));
	spiDeviceSelect(cyrf, FALSE);
#else
	// Don't use WebbotLib for SPI code.
	// This option executes quicker allowing a greater number of re-tries when RF transmission fails
	// but is far less extendible if you want to add other SPI devices.
	pin_low(spiGetAbstractDevice(cyrf)->select);		// SS pin low to initiate SPI. (WebbotLib knows which pin so we get it from there.)
	//pin_low(B0);										// this is about 25uS quicker.

	SPDR = register_address;	// send byte (Address)
    while(!(SPSR & (1<<SPIF)));

    SPDR = data;				// send byte (Address)
    while(!(SPSR & (1<<SPIF)));

	pin_high(spiGetAbstractDevice(cyrf)->select);		// SS pin high to end SPI. (WebbotLib knows which pin so we get it from there.)
	//pin_high(B0);										// this is about 25uS quicker.
#endif

    return ;
}


// initialise CYRF6936
void cyrf6936_Initialise_hard()
{
	// hardware reset of *both* RF modules
	pin_high(G3);					// reset the CYRF6936. (hardware reset.)
	delay_us(10);
	pin_low(G3);					// stop reset.
}

void cyrf6936_Initialise_soft(CYPRESS_CYRF* cyrf)
{
	spiDeviceSelect(cyrf, TRUE);
	spiDeviceSelect(cyrf, FALSE);

	// hardware reset of *both* RF modules
	//pin_high(G3);					// reset the CYRF6936. (hardware reset.)
	//delay_us(10);
	//pin_low(G3);					// stop reset.


	cyrf_SPI_Write(0x1D,0x01, cyrf);		// MODE_OVERRIDE, RST (Reset module)
	cyrf_SPI_Write(0x1D,0x01, cyrf);		// MODE_OVERRIDE, RST (Reset module)


	cyrf_SPI_Write(0x03,0x2F, cyrf);		// TX_CFG,

	// set interrupts?
	cyrf_SPI_Write(0x05,0x02, cyrf);		// set RX_CTRL_ADR, RX_GO

	// un-set overwrite enable?
	cyrf_SPI_Write(0x06,0x8A, cyrf);		// RX_CFG_ADR, overwrite (RXOW EN)
	//cyrf_SPI_Write(0x06,0x88);		// RX_CFG_ADR, overwrite (RXOW EN)  *** play with this more***
	cyrf_SPI_Write(0x0F,0x13, cyrf);		// XACT_CFG,END_STATE,ACK EN, ACK_TO_15X
	cyrf_SPI_Write(0x10,0xEE, cyrf);		// FRAMING_CFG, LEN EN
	cyrf_SPI_Write(0x11,0x08, cyrf);		// DATA32_THOLD
	cyrf_SPI_Write(0x12,0x0E, cyrf);		// DATA64_THOLD
	cyrf_SPI_Write(0x15,0x14, cyrf);		// CRC_SEED_LSB
	cyrf_SPI_Write(0x16,0x14, cyrf);		// CRC_SEED_MSB
	cyrf_SPI_Write(0x1B,0x55, cyrf);		// TX_OFFSET_LSB
	cyrf_SPI_Write(0x1C,0x05, cyrf);		// TX_OFFSET_MSB
	cyrf_SPI_Write(0x1E,0x08, cyrf);		// DIS_CRC0

	cyrf_SPI_Write(0x26,0x08, cyrf);		// XTAL_CFG  (must)
	//cyrf_SPI_Write(0x28,0x02, cyrf);		// (must)
	cyrf_SPI_Write(0x32,0x3C, cyrf);		// AUTO_CAL_TIME  (must)
	cyrf_SPI_Write(0x35,0x14, cyrf);		// AUTO_CAL_OFFSET  (must)

	cyrf_SPI_Write(0x0C,0x40, cyrf);

	cyrf_SPI_Write(0x0D,0x80, cyrf);		// IRQ pin drive strength


	//cyrf_SPI_Write(0x0F,0x33, cyrf);			// force into RX state.
	//cyrf_SPI_Write(0x0F,0x04, cyrf);			// set "idle" as end state.
	//cyrf_SPI_Write(0x0F,0x27, cyrf);					// force "idle" as end state.
	//cyrf_SPI_Write(0x0F,0x13, cyrf);			// set RX as end state.

	//cyrf_SPI_Write(0x05,(RXGO | RXC_IRQEN), cyrf);


	cyrf_SPI_Write(0x24, 0x04, cyrf);
	cyrf_SPI_Write(0x24, 0x33, cyrf);
	cyrf_SPI_Write(0x24, 0x33, cyrf);


	uptime=0;

	return;
}


// transition cyrf6936 into receive mode
void cyrf6936_RX_mode(CYPRESS_CYRF* cyrf)
{
#ifdef RF_MODULE_ARTAFLEX
	pin_low(D5);		// TXPA, module 1(Transmit Power Amplifier)
	pin_low(D7);		// TXPA, module 2
	pin_high(D4);		// RXPA, module 1(Receive Power Amplifier)
	pin_high(D6);		// RXPA, module 2
#endif

	// test if RXGO is already set.
	if(!(cyrf_SPI_Read(0x05, cyrf) & (RXGO))){
		if (!(cyrf_SPI_Read(0x02, cyrf) & (TXGO))){
			// providing TXGO and RXGO are not set, set end state.
			cyrf_SPI_Write(0x0F,0x33, cyrf);			// force into RX state.
			cyrf_SPI_Write(0x0F,0x04, cyrf);			// set "idle" as end state.
		}
		// set RXGO and RXC_IRQEN. (Put in RX mode and enable interrupt on packet received.)
		cyrf_SPI_Write(0x05,(RXGO | RXC_IRQEN), cyrf);


	}

	return;
}


// transition cyrf6936 into transmit mode
void cyrf6936_TX_mode(CYPRESS_CYRF* cyrf)
{
#ifdef RF_MODULE_ARTAFLEX
	pin_high(D5);		// TXPA, module 1(Transmit Power Amplifier)
	pin_high(D7);		// TXPA, module 2
	pin_low(D4);		// RXPA, module 1(Receive Power Amplifier)
	pin_low(D6);		// RXPA, module 2
#endif

	// if TXGO not set,
	if (!(cyrf_SPI_Read(0x02, cyrf) & (TXGO))){
		// force state to TX.
		cyrf_SPI_Write(0x0F,0x27, cyrf);	// force "idle" as end state.
		cyrf_SPI_Write(0x0F,0x10, cyrf);	// "RX" as end state.

	}

	return;
}



// check if data has been received on RF module.
// if data received and no errors returns 0
// if no data received returns 1
// if data received with errors returns > 1
uint8_t cyrf6936_RX_test(CYPRESS_CYRF* cyrf)
{
	// wait until RX_GO un-sets indicating data has been received.
	// ** the timeout value here may need tuning. **
	// if you are arriving here by interrupt then this shouldn't be needed. Use 100us to be safe.
	// if you are poling the cyrf6936_RX_test() function to decide when a packet comes in then you will need a longer delay.
	// for debugging use something huge like 5000uS.
	TICK_COUNT timeout = clockGetus();
	while ((cyrf_SPI_Read(0x05, cyrf) & RXGO)) {
		if(clockGetus() - timeout > 1){break;}	// *** the number on this line is the timeout that may need changing. ***
	}

	// RXGO has cleared so *something* has been received.
	uint8_t RadioState = cyrf_SPI_Read(0x07, cyrf) ;

	if(RadioState & RXE) {
		// receive error flag set.
		return 2;
	}
	else if(RadioState & RXBERR) {
		// Receive Buffer ERRor
		return 3;
	}
	else if(RadioState & RXC) {
		// Receive Complete. (good thing)
		if (!(cyrf_SPI_Read(0x08, cyrf) & badCRC)){
			// no CRC error either.
			if(!(RadioState & RXE)) {
				// second check for receive error. (rare but the datasheet asks for it.)
				// all is good. return 0 means successful reception.
				return 0 ;
			}
			else{
				// receive RX error missed by cyrf the first time.
				return 4 ;
			}
		}
		else{
			// CRC errors.
			return(5);
		}
	}
	// no errors but RXE not set either.
	// if here on interrupt packet is probably good.
	// if you are poling this function you normally end up here if no data received.
	return 6;
}


// puts TX data in buffer.
// the data to be transmitted should be in the data[] structure.
// data[tx_data_position] to data[tx_data_position + tx_data_len] will be loaded into the TX buffer..
void cyrf6936_TX_FillBuffer(uint8_t *data, uint8_t tx_data_position, uint8_t tx_data_len, CYPRESS_CYRF* cyrf)
{
	// data_position is the position of the first bit in the data structure to be sent.
	// data_len is the number of subsequent bits to be sent. maximum of 14 bits.
	if (tx_data_len > 14) tx_data_len=14;

	cyrf_SPI_Write(0x01,tx_data_len +2, cyrf);		// TX_LENGTH + 2 bytes for addresses

	cyrf_SPI_Write(0x02,0x40, cyrf);				// TX_CTRL_ADR, TX_CLR. (do this!)

	cyrf_SPI_Write(0x20,tx_data_position, cyrf);	// 1st byte of TX buffer. (which data is being sent.)
	cyrf_SPI_Write(0x20,0x00, cyrf);				// 2nd byte of TX buffer. (reserved)

	// i is the position in the data structure of the bit to be sent.
	// loop through the bits being sent, populating the cyrf6936 buffer.
	uint8_t i;
	for (i = tx_data_position ; i < (tx_data_position + tx_data_len); i++){
		cyrf_SPI_Write(0x20,data[i], cyrf);
	}

	return;
}


// transmits data in cyrf6936 buffer.
// Returns 0 if successful.
// Returns 1 if there was a TX error.
uint8_t cyrf6936_TX(CYPRESS_CYRF* cyrf)
{
	cyrf_SPI_Write(0x02,0x80, cyrf);				// TX_CTRL_ADR, TX_GO. Start transmit

	// **** WEIRDness going on here.
	// if we read the TX_IRQ_STATUS_ADR too soon after previous read things start to go strange.
	// the solution is just not to read it at all. TX errors are rare anyway.

	uint8_t RadioState=0x00;

	uint16_t tmp = 0;
	while (!RadioState){
		RadioState = RadioState | (cyrf_SPI_Read(0x04, cyrf) & (TXE | TXC));
		if (tmp++ > 10){ rprintfProgStrM("."); break; } 		// allow this to time out just in case.
	}
	if (RadioState == 0x02){				// TXC=1, TXE=0.
		//if (!(cyrf_SPI_Read(0x04) & TXE)){	// check TXE again. (only needed if using transaction mode.)
			return 0;						// TX successful! happy days.
			//pin_low(C0);				// debug status LED on

		//}
	}

	return 1;								// anything else is an error.
}



// read data from cyrf6936 buffer into program data space.
// the first byte received is the address in the data[] structure to store the received data.
// the 2nd byte received is the address in the data[] structure of data to be transmitted in reply at the next opportunity.
void cyrf6936_RX_ReadBuffer(uint8_t *data, CYPRESS_CYRF* cyrf)
{
	cyrf_SPI_Write(0x0F,0x24, cyrf);					// force "idle" as end state.
	cyrf_SPI_Write(0x07,0x80, cyrf);					// need to set RXOW before data read.

	uint8_t rx_data_len = cyrf_SPI_Read(0x0A, cyrf);	// received data length.

	uint8_t rx_data_position = cyrf_SPI_Read(0x21, cyrf);// first byte received is position in data structure
	//rx_data_position = 1;						// *******this only for testing.******
	data[RX_SEND_ADD] = cyrf_SPI_Read(0x21, cyrf);		// this byte is the address of data being requested in the reply packet.

	// i is the position in the data structure of the bit being received.
	// loop through the bits being sent, populating the program data structure.
	uint8_t i;
	for (i = rx_data_position ; i < (rx_data_position + rx_data_len -2); i++){
		uint8_t tmp = cyrf_SPI_Read(0x21, cyrf);

		// if data changes, set flag.
		if (data[i] == tmp){
			// no change. clear flag.
			data_Flag[i] &= (~DATA_FLAG_CHANGED);
		}
		else{
			// data changed. set flag.
			// possibly save EEPROMs here as well at a later date.
			data_Flag[i] |= DATA_FLAG_CHANGED;
		}
		// save data.
		data[i] = tmp;
	}


	return;
}


// RSSI is the Received Signal Strength Indicator.
// this returns the signal strength of the last received transmission.
uint8_t cyrf6936_Get_RSSI(CYPRESS_CYRF* cyrf)
{
	return (0x1F & cyrf_SPI_Read(0x13, cyrf));
}


// need to implement channel hopping...
// not yet implemented.
void cyrf6936_Set_Channel();



// WebbotLib does not handle interrupts on INT[0-7] yet.
// this function enables INT7 to interrupt on a falling edge.
// this interrupt is triggered when cyrf_0 receives data.
void cyrfIntEnable(void){
	EICRB=0x80;				// INT7 to interrupt on a falling edge.
	EIMSK=0xC0;				// enable INT6 and INT7
	sei();
	return;
}


// WebbotLib does not handle interrupts on INT[0-7] yet.
// this function is a the Interrupt Service Routine for INT6 and INT7.
// this interrupt is triggered when cyrf_0 receives data.
ISR(INT6_vect){
	// interrupt handler function:
	//cli();		// disable interrupts
	//EIMSK=0x00;				// disable INT6 and INT7
	cyrfInterupt(&cyrf_1);
	EIMSK=0xC0;				// enable INT6 and INT7
	//sei();		// re-enable interrupts.

}
ISR(INT7_vect){
	// interrupt handler function:
	//cli();		// disable interrupts
	//EIMSK=0x00;				// disable INT6 and INT7
	cyrfInterupt(&cyrf_0);
	EIMSK=0xC0;				// enable INT6 and INT7
	//sei();		// re-enable interrupts.

}



// this function is called by the ISR.
void cyrfInterupt(CYPRESS_CYRF* cyrf){
	// the interrupt pin has changed.
	// packet received on cyrf6936.

	if(	data[RX_RX_FLAG]==1){
		// we have already received data this frame (presumably on a different RF module)
		return;
	}


	uint8_t tmp = cyrf6936_RX_test(cyrf);
	//rprintfu08(tmp);
	if (tmp==0){
		pin_low(C0);				// debug status LED on

		// no errors on RX so transmission is successful.
		cyrf6936_RX_ReadBuffer(data, cyrf);			// read data from cyrf6936 into buffer
		// set frame received flag
		data[RX_RX_FLAG]=1;

		// store RSSI data after RX.
		data[RX_RSSI_1F] = cyrf6936_Get_RSSI(cyrf);


		// send reply packet.


		//cyrf6936_TX_mode(cyrf);

		cyrf6936_TX_FillBuffer(data, data[RX_SEND_ADD], 14, cyrf);	// fill cyrf6936 buffer
		cyrf6936_TX_mode(cyrf);										// put CYRF in Transmit mode.
		if (cyrf6936_TX(cyrf)){									// transmit data and test for error
			data[RX_ERR_CODE]=11;
			//rprintfProgStrM("~");
		}
	}
	else{
		data[RX_PAC_ERR_CNT]++;
		data[RX_ERR_CODE]=tmp;				// save any RX error code for debugging.
	}


	// for some reason the receiver locks up if we put the RF module in RX mode too soon on the first iteration.
	if (uptime > 20){
		//pin_low(C0);
		// enable Receive mode.
		cyrf6936_RX_mode(cyrf);
	}


}



// this gets called every 20ms frame to see if RF packet has been received.
uint8_t testIfPacketReceived(uint8_t *data)
{
	uint8_t tmp = 0;
	// test frame received flag.
	if (data[RX_RX_FLAG]==1){
		// data has come arrived in the last 20ms.
		data[RX_PAC_GAP]=0;
		data[RX_PAC_P1_CNT]++;
		tmp = 1;
	}
	else{
		// no usable data received. increase counter.
		if (data[RX_PAC_GAP] < 0xFF){data[RX_PAC_GAP]++;}
		if (data[RX_PAC_GAP] > data[RX_PAC_GAP_CNT]){
			// current count is higher than previous, save new maximum.
			data[RX_PAC_GAP_CNT] = data[RX_PAC_GAP];
		}

		// RSSI data in buffer is stale since no RX
		data[RX_RSSI_1F] = 0 ;
	}

	// add RSSI data to the total for this second.
	cyrfRssiCounter+=data[RX_RSSI_1F];


	// clear frame received flag.
	data[RX_RX_FLAG]=0;

	return tmp;
}



// rules for dealing with each servo.
// *** there has to be a nicer way to do this. develop.***
//
void set_servos(uint8_t *data){
	short int tmp;

	// servo 0 (motor)
	if(data[TX_CONTROLER]==0){
		if (data[TX_PS2_LY] > 127){data[TX_PS2_LY] = 127;}
		act_setSpeed(&servo_0, data[TX_PS2_LY] -127);

	}
	else{
		act_setSpeed(&servo_0, (data[TX_PS2_LY] -255)/2);
	}

	// servo 1 (rudder)
	// put PS2 controller data in servo buffer
	servoBuffer[1] =  (((float)(data[RX_SERVO_1_MUL] - 127)/10) * (0x7F - data[TX_PS2_RX])) + 0x7FFF;

	// make saved trim settings centre point for servo. apply data in servo buffer.
	tmp = ((data[RX_SERVO_1_CH]<<8) + data[RX_SERVO_1_CL]) + servoBuffer[1] - 0x7FFF;

	if(buttonPressed[PS2_B_BTN_R1] > 0){
		// autopilot active
		//tmp -= (data[RX_AUTOP_X_MUL]* AtEst1.AYZ) ;
		tmp -= (data[RX_AUTOP_X_MUL]* (data[RX_EST_X]-0x7F)) ;
	}


	// make sure servo is not past end points.
	if ((tmp - 0x7FFF) < -0x7F){
		tmp = 0x7F80;
	}
	if ((tmp - 0x7FFF) > 0x7F){
			tmp = 0x807E;
	}
	// move servo.
	act_setSpeed(&servo_1, tmp - 0x7FFF );

#if defined MODEL_FLYINGWING
	servoBuffer[2] = (((float)(data[RX_SERVO_3_MUL] - 127)/10) * (+0x7F - data[TX_PS2_RX])) + (-0x7F + data[TX_PS2_RY]) + 0x7FFF;
	servoBuffer[3] = (((float)(data[RX_SERVO_3_MUL] - 127)/10) * (+0x7F - data[TX_PS2_RX])) + (+0x7F - data[TX_PS2_RY]) + 0x7FFF;

	tmp = ((data[RX_SERVO_2_CH]<<8) + data[RX_SERVO_2_CL]) + servoBuffer[2] - 0x7FFF;

	// make sure servo is not past end points.
	if ((tmp - 0x7FFF) < -0x7F){
		tmp = 0x7F80;
	}
	if ((tmp - 0x7FFF) > 0x7F){
			tmp = 0x807E;
	}
	// move servo.
	act_setSpeed(&servo_2, tmp - 0x7FFF );

	tmp = ((data[RX_SERVO_3_CH]<<8) + data[RX_SERVO_3_CL]) + servoBuffer[3] - 0x7FFF;

	// make sure servo is not past end points.
	if ((tmp - 0x7FFF) < -0x7F){
		tmp = 0x7F80;
	}
	if ((tmp - 0x7FFF) > 0x7F){
			tmp = 0x807E;
	}
	// move servo.
	act_setSpeed(&servo_3, tmp - 0x7FFF );

#else
	// servo 2 (elevator)
	// put PS2 controller data in servo buffer
	servoBuffer[2] = (((float)(data[RX_SERVO_2_MUL] - 127)/10) * (-0x7F + data[TX_PS2_RY])) + 0x7FFF;

	// make saved trim settings centre point for servo. apply data in servo buffer.
	tmp = ((data[RX_SERVO_2_CH]<<8) + data[RX_SERVO_2_CL]) + servoBuffer[2] - 0x7FFF;

	if(buttonPressed[PS2_B_BTN_R1] > 0){
		// autopilot active
		//tmp += data[RX_AUTOP_Y_MUL]* AtEst1.AXZ ;
		tmp += (data[RX_AUTOP_Y_MUL]* (data[RX_EST_Y]-0x7F)) ;
	}

	// make sure servo is not past end points.
	if ((tmp - 0x7FFF) < -0x7F){
		tmp = 0x7F80;
	}
	if ((tmp - 0x7FFF) > 0x7F){
			tmp = 0x807E;
	}
	// move servo.
	act_setSpeed(&servo_2, tmp - 0x7FFF );



	// servo 3&4 (aerlons)
	// put PS2 controller data in servo buffer
	servoBuffer[3] =  (((float)(data[RX_SERVO_3_MUL] - 127)/10) * (-0x7F + data[TX_PS2_RX])) + 0x7FFF;

	servoBuffer[4] =  (((float)(data[RX_SERVO_4_MUL] - 127)/10) * (-0x7F + data[TX_PS2_RX])) + 0x7FFF;

	// make saved trim settings centre point for servo. apply data in servo buffer.
	tmp = ((data[RX_SERVO_3_CH]<<8) + data[RX_SERVO_3_CL]) + servoBuffer[3] - 0x7FFF;

	// make sure servo is not past end points.
	if ((tmp - 0x7FFF) < -0x7F){
		tmp = 0x7F80;
	}
	if ((tmp - 0x7FFF) > 0x7F){
			tmp = 0x807E;
	}
	// move servo.
	act_setSpeed(&servo_3, tmp - 0x7FFF );

	// make saved trim settings centre point for servo. apply data in servo buffer.
	tmp = ((data[RX_SERVO_4_CH]<<8) + data[RX_SERVO_4_CL]) + servoBuffer[4] - 0x7FFF;

	// make sure servo is not past end points.
	if ((tmp - 0x7FFF) < -0x7F){
		tmp = 0x7F80;
	}
	if ((tmp - 0x7FFF) > 0x7F){
			tmp = 0x807E;
	}
	// move servo.
	act_setSpeed(&servo_4, tmp - 0x7FFF );
#endif

}



// put some sensible defaults in trims.
void reset_trims(void){
	data[RX_SERVO_0_CH] = 0x7F;
	data[RX_SERVO_0_CL] = 0xFF;
	data[RX_SERVO_1_CH] = 0x7F;
	data[RX_SERVO_1_CL] = 0xFF;
	data[RX_SERVO_2_CH] = 0x7F;
	data[RX_SERVO_2_CL] = 0xFF;
	data[RX_SERVO_3_CH] = 0x7F;
	data[RX_SERVO_3_CL] = 0xFF;
	data[RX_SERVO_4_CH] = 0x7F;
	data[RX_SERVO_4_CL] = 0xFF;
//	data[RX_SERVO_5_CH] = 0x7F;
//	data[RX_SERVO_5_CL] = 0xFF;

	data[RX_SERVO_0_MUL] = 0x7F;
	data[RX_SERVO_1_MUL] = 0x89;
	data[RX_SERVO_2_MUL] = 0x7F;
	data[RX_SERVO_3_MUL] = 0x7F;
	data[RX_SERVO_4_MUL] = 0x7F;
//	data[RX_SERVO_5_MUL] = 0x7F;

	data[RX_AUTOP_X_MUL] = 2;
	data[RX_AUTOP_Y_MUL] = 2;
	data[RX_AUTOP_X_TRIM] = 0x7F;
	data[RX_AUTOP_Y_TRIM] = 0x7F;

	eeprom_write_byte(&data_eeprom[RX_SERVO_1_CH], data[RX_SERVO_1_CH]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_1_CL], data[RX_SERVO_1_CL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_2_CH], data[RX_SERVO_2_CH]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_2_CL], data[RX_SERVO_2_CL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_3_CH], data[RX_SERVO_3_CH]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_3_CL], data[RX_SERVO_3_CL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_4_CH], data[RX_SERVO_4_CH]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_4_CL], data[RX_SERVO_4_CL]);
//	eeprom_write_byte(&data_eeprom[RX_SERVO_5_CH], data[RX_SERVO_5_CH]);
//	eeprom_write_byte(&data_eeprom[RX_SERVO_5_CL], data[RX_SERVO_5_CL]);

	eeprom_write_byte(&data_eeprom[RX_SERVO_0_MUL], data[RX_SERVO_0_MUL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_1_MUL], data[RX_SERVO_1_MUL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_2_MUL], data[RX_SERVO_2_MUL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_3_MUL], data[RX_SERVO_3_MUL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_4_MUL], data[RX_SERVO_4_MUL]);
//	eeprom_write_byte(&data_eeprom[RX_SERVO_5_MUL], data[RX_SERVO_5_MUL]);

	eeprom_write_byte(&data_eeprom[RX_AUTOP_X_MUL], data[RX_AUTOP_X_MUL]);
	eeprom_write_byte(&data_eeprom[RX_AUTOP_Y_MUL], data[RX_AUTOP_Y_MUL]);
	eeprom_write_byte(&data_eeprom[RX_AUTOP_X_TRIM], data[RX_AUTOP_X_TRIM]);
	eeprom_write_byte(&data_eeprom[RX_AUTOP_Y_TRIM], data[RX_AUTOP_Y_TRIM]);


	rprintfProgStrM("\nTrims reset to defaults.\n");

	//data[RX_SERVO_1_CH] = eeprom_read_byte(&data_eeprom[RX_SERVO_1_CH]);

	return;

}




// set trims and save to EEPROM
void set_trims(uint8_t *data){
	// servo channel 1
	short int tmp = ((data[RX_SERVO_1_CH]<<8) + data[RX_SERVO_1_CL]) + servoBuffer[1] - 0x7FFF;
	data[RX_SERVO_1_CH] = (tmp & 0xFF00)>>8;
	data[RX_SERVO_1_CL] = tmp & 0x00FF;

	// servo channel 2
	tmp = ((data[RX_SERVO_2_CH]<<8) + data[RX_SERVO_2_CL]) + servoBuffer[2] - 0x7FFF;
	data[RX_SERVO_2_CH] = (tmp & 0xFF00)>>8;
	data[RX_SERVO_2_CL] = tmp & 0x00FF;

	// servo channel 3
	tmp = ((data[RX_SERVO_3_CH]<<8) + data[RX_SERVO_3_CL]) + servoBuffer[3] - 0x7FFF;
	data[RX_SERVO_3_CH] = (tmp & 0xFF00)>>8;
	data[RX_SERVO_3_CL] = tmp & 0x00FF;

	// servo channel 4
	tmp = ((data[RX_SERVO_4_CH]<<8) + data[RX_SERVO_4_CL]) + servoBuffer[4] - 0x7FFF;
	data[RX_SERVO_4_CH] = (tmp & 0xFF00)>>8;
	data[RX_SERVO_4_CL] = tmp & 0x00FF;

	// servo channel 5
/*	tmp = ((data[RX_SERVO_5_CH]<<8) + data[RX_SERVO_5_CL]) + servoBuffer[5] - 0x7FFF);
	data[RX_SERVO_5_CH] = (tmp & 0xFF00)>>8;
	data[RX_SERVO_5_CL] = tmp & 0x00FF;*/


	// ***** save trims to EEPROM here *****
	eeprom_write_byte(&data_eeprom[RX_SERVO_1_CH], data[RX_SERVO_1_CH]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_1_CL], data[RX_SERVO_1_CL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_2_CH], data[RX_SERVO_2_CH]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_2_CL], data[RX_SERVO_2_CL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_3_CH], data[RX_SERVO_3_CH]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_3_CL], data[RX_SERVO_3_CL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_4_CH], data[RX_SERVO_4_CH]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_4_CL], data[RX_SERVO_4_CL]);
//	eeprom_write_byte(&data_eeprom[RX_SERVO_5_CH], data[RX_SERVO_5_CH]);
//	eeprom_write_byte(&data_eeprom[RX_SERVO_5_CL], data[RX_SERVO_5_CL]);

	eeprom_write_byte(&data_eeprom[RX_SERVO_0_MUL], data[RX_SERVO_0_MUL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_1_MUL], data[RX_SERVO_1_MUL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_2_MUL], data[RX_SERVO_2_MUL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_3_MUL], data[RX_SERVO_3_MUL]);
	eeprom_write_byte(&data_eeprom[RX_SERVO_4_MUL], data[RX_SERVO_4_MUL]);
//	eeprom_write_byte(&data_eeprom[RX_SERVO_5_MUL], data[RX_SERVO_5_MUL]);

	eeprom_write_byte(&data_eeprom[RX_AUTOP_X_MUL], data[RX_AUTOP_X_MUL]);
	eeprom_write_byte(&data_eeprom[RX_AUTOP_Y_MUL], data[RX_AUTOP_Y_MUL]);
	eeprom_write_byte(&data_eeprom[RX_AUTOP_X_TRIM], data[RX_AUTOP_X_TRIM]);
	eeprom_write_byte(&data_eeprom[RX_AUTOP_Y_TRIM], data[RX_AUTOP_Y_TRIM]);


	rprintfProgStrM("\nTrims set.\n");

}
