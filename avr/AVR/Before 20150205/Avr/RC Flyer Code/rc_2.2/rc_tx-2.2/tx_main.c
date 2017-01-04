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

#define UART_TX_BUFFER_SIZE 40
#define UART_RX_BUFFER_SIZE 50
#include "cyrf.h"
#include "tx_main.h"


// Un-comment one or both of these lines depending on whether you want to use WebbotLib or the simple native (fast) code for SPI communications on this node or the Receiver node.
// WebbotLib's SPI code is far more robust and extendible than this projects native SPI code.
// If you plan on adding your own SPI devices to this project then you may want to consider using WebbotLib's SPI code.
// If on the other hand you plan to run this project "as is" you probably want to use this project's native SPI code
// as it is quicker and will allow more retransmissions of any failed packets.
//#define USE_WEBBOTLIB_SPI
//#define USE_WEBBOTLIB_SPI_REMOTE


// this section defines the time in us between transmitted packet and reply packet.
// you may need to experiment with different values (in uS) if you change the code much.
#if defined USE_WEBBOTLIB_SPI_REMOTE && defined	USE_WEBBOTLIB_SPI
	#define REPLY_TIME 3300
#elif defined USE_WEBBOTLIB_SPI_REMOTE
	#define REPLY_TIME 3400
#elif defined	USE_WEBBOTLIB_SPI
	#define REPLY_TIME 2700
#else
	//#define REPLY_TIME 2600							// the shortest time delay needed is when we don't use WebbotLib anywhere.
	#define REPLY_TIME 2600							// the shortest time delay needed is when we don't use WebbotLib anywhere.
#endif


// Create a controller SELECTed using E5
SONY_PS2 controller1 = MAKE_SONY_PS2(E5);

// Create a list of devices on the SPI bus
static SPI_DEVICE_LIST spiBus_list[] = {&controller1._device_};
SPI_SW spi_sw = MAKE_SW_SPI(spiBus_list,B5,B6,B7);


// define CYRF6936 RF modules on SPI bus.
#define MAKE_SPI_GENERIC_DEVICE(mode,order,select,filler) { MAKE_SPI_DEVICE(mode,order,select,filler) }
CYPRESS_CYRF cyrf_0 = MAKE_SPI_GENERIC_DEVICE(SPI_MODE_0,SPI_DATA_ORDER_MSB,B0,0x00);
CYPRESS_CYRF cyrf_1 = MAKE_SPI_GENERIC_DEVICE(SPI_MODE_0,SPI_DATA_ORDER_MSB,B4,0x00);

static SPI_DEVICE_LIST devices[] = {&cyrf_0._device_, &cyrf_1._device_};
SPI bus_hw = MAKE_SPI(devices);



// define variables
uint8_t data[150];
uint32_t replyTime;
uint8_t rs232Receiving = 0;

// Now create any global variables such as motors, servos, sensors etc
// This routine is called once only and allows you to do set up the hardware
// Dont use any 'clock' functions here - use 'delay' functions instead
void appInitHardware(void){
	// set I/O pins for status LEDs
	pin_make_output(C0);			// set LED pin for output. (PCB mounted LED.)
	pin_make_output(C1);			// set LED pin for output. (PCB mounted LED.)
	pin_make_output(C2);			// set LED pin for output. (PCB mounted LED.)
	pin_high(C0);					// LED off (LED hidden inside box.)
	pin_high(C1);					// LED off (LED hidden inside box.)
	pin_high(C2);					// LED off (LED hidden inside box.)
	pin_make_output(F7);			// set LED pin for output. (case mounted LED.)
	pin_make_output(F6);			// set LED pin for output. (case mounted LED.)
	pin_make_output(F5);			// set LED pin for output. (case mounted LED.)
	pin_make_output(F4);			// set LED pin for output. (case mounted LED.)
	pin_low(F7);					// LED on
	pin_low(F6);					// LED on
	pin_low(F5);					// LED on
	pin_low(F4);					// LED on

	pin_make_input(F3, TRUE);		// set toggle switch to input. pull-up enabled.



	// set I/O pins for RF module.
	pin_make_input(D4, FALSE);		// set PACTL pin to input.
	pin_make_input(D5, FALSE);		// set PACTLn pin to input.
	pin_make_input(E7, TRUE);		// set UNIGEN RF module IRQ pin to input
	pin_make_output(G3);			// set UNIGEN RF module RST pin to output
	pin_low(G3);					// don't reset yet.

	// Set UART1 to 19200 baud
	uartInit(UART1, 115200);
	//uartInit(UART1, 38400);
    // Tell rprintf to output to UART1
    rprintfInit(&uart1SendByte);


    // PS2 controller setup.
    spiBusInit(&spi_sw,true);
	//sonyPS2_setAnalogMode(&controller1);
   // while((!(sonyPS2_isAnalogMode(&controller1))) && (pin_is_high(F3))){
    	// pin F3 is the toggle switch that lets us select PS2 controller or UART for data input.
    	// some PS2 controllers take a while to stabilise. wait here until controller responds.
    //	rprintfProgStrM("Attempting to put PS2 controller in analogue mode.\n");
    //	sonyPS2_setAnalogMode(&controller1);
    //}

    // RF module(s) setup.
    spiBusInit(&bus_hw, TRUE);
	spiDeviceSelect(&cyrf_0, TRUE);
	spiDeviceSelect(&cyrf_0, FALSE);

    // initialise RF module.
	cyrf6936_Initialise();

}

// This routine is called once to allow you to set up any other variables in your program
// You can use 'clock' function here.
// The loopStart parameter has the current clock value in μS
TICK_COUNT appInitSoftware(TICK_COUNT loopStart){

	// switch off LEDs.
	pin_toggle(F7);
	pin_toggle(F6);
    pin_toggle(F5);
    pin_toggle(F4);

    // set this bit so the main program thinks we have just changed to PS2 controller and re-initialises if the switch is set to PS2.
    data[TX_CONTROLER]=1;

	return 0; // don't pause after
}



// This routine is called repeatedly - its your main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart){

	// read status of toggle switch
	if (pin_is_high(F3)){
		// this way for PS2 controller
		if(data[TX_CONTROLER]==1){
			// position of switch has just changed
			sonyPS2_setAnalogMode(&controller1);
			while((!(sonyPS2_isAnalogMode(&controller1))) && (pin_is_high(F3))){
				// pin F3 is the toggle switch that lets us select PS2 controller or UART for data input.
				// some PS2 controllers take a while to stabilise. wait here until controller responds.
				rprintfProgStrM("Attempting to put PS2 controller in analogue mode.\n");
				sonyPS2_setAnalogMode(&controller1);
			}
		}
		//pin_low(F5);
		service_PS2_Controller(data);
		data[TX_CONTROLER]=0;
		rs232Receiving=0;
	}
	else{
		// this way for input over UART
		//pin_high(F5);
		if(service_Joystick(data) > 0){
			rs232Receiving=1;
		}
		if(rs232Receiving==0){
			// come here if no data being received on UART.
			// this stops the motor and levels the controll surfaces.
			data[TX_PS2_LX] = 0x7F;
			data[TX_PS2_LY] = 0xFF;
			data[TX_PS2_RX] = 0x7F;
			data[TX_PS2_RY] = 0x7F;
		}
		data[TX_CONTROLER]=1;

	}


	// send data and request data back.
	data[TX_SEND_ADD] = 1;								// send data from address "1" upwards
	data[RX_SEND_ADD] = 65;								// request data from address "65" upwards

	data_Synchronise(data, loopStart, TRUE, &cyrf_0);

	// send data and request data back.
	data[TX_SEND_ADD] = 15;								// send data from address "1" upwards
	data[RX_SEND_ADD] = 79;								// request data from address "65" upwards
	data_Synchronise(data, loopStart, FALSE, &cyrf_0);

	// RSSI is Received Signal Strength Indicator.
	// do this soon after a data read.
	store_RSSI(data, &cyrf_0);


	if (data[TX_FRA_NUM]==24){
		// this will happen every second 0.5 seconds behind main 1sec section.

		// output data on USB port
		dataOut(data);

		// TX battery low so speed up LED flashes.
		if(data[TX_BAT_VOLT] < 0x6B){
			pin_toggle(F4);
		}
	}


	if ((data[TX_FRA_NUM]==12)||(data[TX_FRA_NUM]==36)){
		// output data on USB port
		dataOut(data);

		// TX battery low so speed up LED flashes.
		if(data[TX_BAT_VOLT] < 0x6A){
			pin_toggle(F4);
		}

	}


	if ((data[TX_FRA_NUM]==6)||(data[TX_FRA_NUM]==18)||(data[TX_FRA_NUM]==30)||(data[TX_FRA_NUM]==42)){
		// output data on USB port
		dataOut(data);

		// TX battery low so speed up LED flashes.
		if(data[TX_BAT_VOLT] < 0x69){
			pin_toggle(F4);
		}

	}



	// once per second this stuff will happen
	if ((data[TX_FRA_NUM]++)==49){
		// reset frame counter.
		data[TX_FRA_NUM] = 0;

		// pulse status LED.
		pin_toggle(F4);
		pin_high(F7);					// signal LED

		// output data on USB port
		dataOut(data);

		// call this once per second to update various counters.
		update_Counters(data, &cyrf_0);

		// read battery and current sensor
		data[TX_BAT_VOLT] = a2dConvert8bit(ADC_CH_ADC0);


		if(data[TX_PAC_GAP] > 0x35){
			// no data from RX (signal failure) so switch off RX battery LED.
			pin_high(F5);
		}
		else if(data[RX_BAT_VOLT] < 0x73){
			// RX battery low. flash LED.
			pin_toggle(F5);
		}
		else{
			// RX battery fine. switch on LED.
			pin_low(F5);
		}

		/*for (uint8_t i=0; i<16; i++){
			rprintfu08(data[i]);
			rprintfProgStrM(" ");
		}*/
	}



	return 20000;			// repeat 20ms after last started.
}



// SPI send byte as address and return data received.
uint8_t cyrf_SPI_Read(uint8_t register_address, CYPRESS_CYRF* cyrf)
{
	// first bit sets CYRF6936 data direction. (not done here. DIR =0)
	// next bit is set if you want multiple bytes data to be written. (not done here. INC=0)
	// next 6 bits are CYRF6936 register address.
	register_address = (register_address & 0b00111111);

#ifndef USE_WEBBOTLIB_SPI
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

#ifndef USE_WEBBOTLIB_SPI
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
void cyrf6936_Initialise(CYPRESS_CYRF* cyrf)
{

	pin_high(G3);					// reset the CYRF6936. (hardware reset.)
	delay_us(10);
	pin_low(G3);					// stop reset.


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
	//cyrf_SPI_Write(0x28,0x02);		// (must)
	cyrf_SPI_Write(0x32,0x3C, cyrf);		// AUTO_CAL_TIME  (must)
	cyrf_SPI_Write(0x35,0x14, cyrf);		// AUTO_CAL_OFFSET  (must)

	cyrf_SPI_Write(0x0C,0x40, cyrf);

	cyrf_SPI_Write(0x0D,0x80, cyrf);		// IRQ pin drive strength

	//cyrf_SPI_Write(0x00,0x40);		// channel

	cyrf_SPI_Write(0x24, 0x04, cyrf);
	cyrf_SPI_Write(0x24, 0x33, cyrf);
	cyrf_SPI_Write(0x24, 0x33, cyrf);


	return;
}


// transition cyrf6936 into receive mode
void cyrf6936_RX_mode(CYPRESS_CYRF* cyrf)
{
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
	cyrf_SPI_Write(0x0F,0x27, cyrf);					// force "idle" state.
	// if TXGO not set,
	if (!(cyrf_SPI_Read(0x02, cyrf) & (TXGO))){
		// force state to TX.
		cyrf_SPI_Write(0x0F,0x27, cyrf);	// force "idle" as end state.
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
	// if you are arriving here by interrupt then this shouldn't be too critical. Use ~100us to be safe.
	// if you are poling the cyrf6936_RX_test() function to decide when a packet comes in then you will need a longer delay.
	// for debugging use something huge like 5000uS.
	TICK_COUNT timeout = clockGetus();
	while ((cyrf_SPI_Read(0x05, cyrf) & RXGO)) {
		if(clockGetus() - timeout > 100){break;}	// *** the number on this line is the timeout that may need changing. ***
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
void cyrf6936_TX_FillBuffer(uint8_t *data, uint8_t tx_data_position, uint8_t tx_data_len, CYPRESS_CYRF* cyrf)
{
	// data_position is the position of the first bit in the data structure to be sent.
	// data_len is the number of subsequent bits to be sent. maximum of 14 bits.
	if (tx_data_len > 14) tx_data_len=14;

	cyrf_SPI_Write(0x01,tx_data_len +2, cyrf);		// TX_LENGTH + 2 bytes for addresses

	cyrf_SPI_Write(0x02,0x40, cyrf);				// TX_CTRL_ADR, TX_CLR. (do this!)

	cyrf_SPI_Write(0x20,tx_data_position, cyrf);	// 1st byte of TX buffer. (which data is being sent.)
	cyrf_SPI_Write(0x20,data[RX_SEND_ADD], cyrf);	// 2nd byte of TX buffer. data to be sent from remote end.

	// i is the position in the data structure of the bit to be sent.
	// loop through the bits being sent, populating the cyrf6936 buffer.
	for (uint8_t i = tx_data_position ; i < (tx_data_position + tx_data_len); i++){
		cyrf_SPI_Write(0x20,data[i], cyrf);
	}

	return;
}


// transmits data in cyrf6936 buffer.
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
		if (tmp++ > 100){rprintfProgStrM("."); break;} 		// allow this to time out just in case.
	}
	if (RadioState == 0x02){				// TXC=1, TXE=0.
		//if (!(cyrf_SPI_Read(0x04) & TXE)){	// check TXE again. (only needed if using transaction mode.)
			return 0;						// TX successful! happy days.
		//}
	}

	return 1;								// anything else is an error.
}



// read data from cyrf6936 buffer into program data space.
void cyrf6936_RX_ReadBuffer(uint8_t *data, CYPRESS_CYRF* cyrf)
{
	cyrf_SPI_Write(0x0F,0x27, cyrf);					// force "idle" state.
	cyrf_SPI_Write(0x07,0x80, cyrf);					// need to set RXOW before data read.

	uint8_t rx_data_len = cyrf_SPI_Read(0x0A, cyrf);	// received data length.

	uint8_t rx_data_position = cyrf_SPI_Read(0x21, cyrf);// first byte received is position in data structure
	//rx_data_position = 1;						// *******this only for testing.******
	uint8_t reserved = cyrf_SPI_Read(0x21, cyrf);		// unused in the reply packet.

	// i is the position in the data structure of the bit being received.
	// loop through the bits being sent, populating the program data structure.
	for (uint8_t i = rx_data_position ; i < (rx_data_position + rx_data_len -2); i++){
		data[i] = cyrf_SPI_Read(0x21, cyrf);
	}


	return;
}


// RSSI is Received Signal Strength Indicator.
uint8_t cyrf6936_Get_RSSI(CYPRESS_CYRF* cyrf)
{
	return (0x1F & cyrf_SPI_Read(0x13, cyrf));
}


void cyrf6936_Set_Channel();


// Transmit data and wait for reply packet.
uint8_t transmit_Packet(uint8_t *data, CYPRESS_CYRF* cyrf){

	TICK_COUNT txStartTime, txFinishTime;

	//cyrf6936_TX_mode(cyrf);
	cyrf6936_TX_FillBuffer(data, data[TX_SEND_ADD], 14, cyrf);	// fill cyrf6936 buffer
	cyrf6936_TX_mode(cyrf);

	if (cyrf6936_TX(cyrf)){									// transmit data and test for error
		data[TX_ERR_CODE]=9;
	}

	// now we are expecting packet to be received on cyrf6936.
	// so put CYRF in Receive mode.
	cyrf6936_RX_mode(cyrf);


	//   poll interrupt pin to see when packet arrives.
	//   (this works out *slightly* less efficient than querying the module directly.)
	//while(pin_is_high(E7)){
	//	if(clockGetus() - loopStart > 20){break;}
	//}

	//cyrf6936_RX_mode(cyrf);
	uint8_t tmp = 6;
	txStartTime = clockGetus();
	while(tmp==6){
		// Wait until reply packet received.
		// This delay REPLY_TIME will vary depending on the clock speed of the other end of the RF link. (ie, the Receiver.)
		// 5000uS is a good starting point but be prepared to experiment. (2200uS)
		// Too long and the number of retransmissions possible will decrease.
		// Too short and the reply will not get back to the Transmitter in time and will be lost.
		tmp = cyrf6936_RX_test(cyrf);
		txFinishTime = clockGetus();
		if(txFinishTime - txStartTime > REPLY_TIME){break;}		// *** the number on this line is the timeout that may need changing. ***
	}
	if (tmp==0){
		// no errors on RX so transmission is successful.

/*		if(((txFinishTime - txStartTime) > ((replyTime>>8) +100)) ||
				((txFinishTime - txStartTime) < ((replyTime>>8) -100))
				){replyTime = (clockGetus() - txStartTime)<<8;}
		else if((txFinishTime - txStartTime) > ((replyTime>>8) +0x100)){replyTime+=0x10;}
		else if((txFinishTime - txStartTime) < ((replyTime>>8) -0x100)){replyTime-=0x10;}
		else if((txFinishTime - txStartTime) > (replyTime>>8)){replyTime++;}
		else if((txFinishTime - txStartTime) < (replyTime>>8)){replyTime--;}
*/
		//replyTime = clockGetus() - txStartTime;

		cyrf6936_RX_ReadBuffer(data, cyrf);			// read data from cyrf6936 into buffer
	}
	else{
		data[TX_ERR_CODE]=tmp;				// save any RX error code for debugging.
	}

	//cyrf6936_RX_mode();

	return tmp;
}


// sends 14bytes of data from this end starting from address saved in data[TX_SEND_ADD].
// requests 14bytes of data from remote end starting from address saved in data[RX_SEND_ADD].
// waits for return packet and retries transmission if none arrives.
//
// function returns 0 if successful.
// function returns 0xFF if timed out.
// function returns TX or RX error code if errors encountered.
uint8_t data_Synchronise(uint8_t *data, TICK_COUNT loopStart, uint8_t log, CYPRESS_CYRF* cyrf){

	uint8_t tmp = 0xFF, i=0;
	while(tmp){
		if(clockGetus() - loopStart >= 17500){ break;}	// this timer stops retransmissions outside 20ms frame.
		tmp = transmit_Packet(data, cyrf);
		i++;
	}

	if(!tmp){
		// transmission successful
		if(log){
			data[TX_PAC_P1_CNT]++;				// receive error counter
			data[TX_PAC_GAP]=0;					// consecutive error counter

			pin_high(F6);						// data received so switch off error LED.
		}
	}
	else if(log){
		pin_low(F6);						// no data received so switch on error LED.

		data[TX_PAC_ERR_CNT]++;

		// no usable data received. increase counter.
		if(data[TX_PAC_GAP]++ >= data[TX_PAC_GAP_CNT]){
			// current count is higher than previous
			data[TX_PAC_GAP_CNT] = data[TX_PAC_GAP];
		}
		if (data[TX_PAC_GAP_CNT] == 0xFF){
			data[TX_PAC_GAP] = 0xFA;
		}

	}

	return tmp;
}


// RSSI is Received Signal Strength Indicator.
// this function keeps some running totals from which to get an average RSSI.
void store_RSSI(uint8_t *data, CYPRESS_CYRF* cyrf){
	// store this frames RSSI reading.
	data[TX_RSSI_1F] = cyrf6936_Get_RSSI(cyrf);
	// add it to the total for this second.
	data[TX_RSSI_CNT_L]+=data[TX_RSSI_1F];
	// if register has rolled over, increase high bit
	if (data[TX_RSSI_CNT_L] < data[TX_RSSI_1F]){
		data[TX_RSSI_CNT_H]++;
	}
}


// this keeps track of the various counters.
// transmission success rates, number of frames, uptime, etc.
void update_Counters(uint8_t *data, CYPRESS_CYRF* cyrf){
	// store RSSI data.
	data[TX_RSSI_1S] = (   ((data[TX_RSSI_CNT_H]<<8) + data[TX_RSSI_CNT_L] - 0x210) / 8   );
	data[TX_RSSI_CNT_L]=0;
	data[TX_RSSI_CNT_H]=0;

	// uptime counter (in seconds)
	data[TX_UPTIME_L]++;
	if (data[TX_UPTIME_L]==0){
		data[TX_UPTIME_H]++;
	}

	// store successful frames last second
	data[TX_PAC_P1_1S]=data[TX_PAC_P1_CNT];
	data[TX_PAC_P1_CNT]=0;

	// store error-ed frames last second
	data[TX_PAC_ERR_1S]=data[TX_PAC_ERR_CNT];
	data[TX_PAC_ERR_CNT]=0;

	// save counter for largest number of consecutive RX errors last second
	data[TX_PAC_CON_ERR_1S] = data[TX_PAC_GAP_CNT];
	data[TX_PAC_GAP_CNT] = 0;

	// reset the cyrf6936 if no data received for a while.
	if(data[TX_PAC_GAP] > 0x35){
		// no data received for ~ 1 second so reset cyrf6936.
		// this will cause the cyrf6936 to be unresponsive for a few ms so don't do it too often.
		pin_low(F7);					// LED on
		cyrf6936_Initialise(cyrf);
	}

	return;
}


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



int rs232RXPos = 0;

uint8_t service_Joystick(uint8_t *data){
	int rs232RXPosReport = 0;				// this saves the

	int lett = 0;
    while(lett >= 0){
    	lett = uartGetByte(UART1);
    	if(lett==255){
    		// we should receive 2 start bytes with a value of 255.
    		if(rs232RXPos<2){
    			// first or second start byte
    			rs232RXPos++;
    		}
    		else{
    			// 3rd start byte so must be error.
    			// we can safely presume this is in fact the 2nd start byte. (this scenario is more likely than it being the first one.)
    			rs232RXPos=2;
    		}
    	}
    	else if((lett < 0)|(lett > 0xFF)){
    		// data received on UART is out of range so an error has occurred.
    		// we must cancel RX at this point as we no longer know which byte any subsequent data is.
    		lett=-1;
    		if(rs232RXPosReport < rs232RXPos-1){rs232RXPosReport = rs232RXPos -1;}
    		rs232RXPos = 0;
    	}
    	else if(rs232RXPos >= 2){
    		if(rs232RXPos >= 10){
    			// buffer overflow. we should have looped back to receiving a start byte by now.
    			// cancel RX.
    			lett=-1;
    			rs232RXPos = -1;
    		}
    		else if(rs232RXPos==2){
    			data[TX_PS2_RX] = lett;
    		}
    		else if(rs232RXPos==3){
    			data[TX_PS2_RY] = lett;
    		}
    		else if(rs232RXPos==4){
    			data[TX_PS2_LY] = lett;
    		}
    		else if(rs232RXPos==5){
    			data[TX_PS2_LX] = lett;
    		}
    		else if(rs232RXPos==6){
    		}
    		else if(rs232RXPos==7){
    		}
    		else if(rs232RXPos==8){
    			// we have all the data we need so don't bother processing the rest of the rs232_RX buffer.
    			lett=-1;
     			if(rs232RXPosReport<rs232RXPos){rs232RXPosReport = rs232RXPos;}
     			rs232RXPos = -1;
    		}

    		rs232RXPos++;
    	}

    }

    uartFlushReceiveBuffer(UART1);

	return rs232RXPosReport;
}


void dataOut(uint8_t *data){
	rprintfProgStrM("R");
	rprintfu08(data[RX_RSSI_1S]);
	rprintfProgStrM("S");
	rprintfu08(data[RX_PAC_P1_1S]);
	rprintfProgStrM("G");
	rprintfu08(data[RX_PAC_CON_ERR_1S]);	// max failed frames last second
	rprintfProgStrM("V");
	rprintfu08(data[RX_BAT_VOLT]);
	rprintfProgStrM("c");
	rprintfu08(data[RX_BAT_CUR]);
	rprintfProgStrM("b");
	rprintfu08(data[TX_BAT_VOLT]);
	rprintfProgStrM("s");
	rprintfu08(data[TX_PAC_P1_1S]);
	rprintfProgStrM("X");
	rprintfu08(data[RX_ACCEL_X]);
	rprintfProgStrM("Y");
	rprintfu08(data[RX_ACCEL_Y]);
	rprintfProgStrM("x");
	rprintfu08(data[RX_EST_X]);
	rprintfProgStrM("y");
	rprintfu08(data[RX_EST_Y]);
	rprintfProgStrM("\n");

}
