/*
 * NRF24_Tx.c
 *
 * Created: 1/5/2015 9:50:57 AM
 *  Author: ByrneDawg
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
 
#include "nRF24L01.h"
 
#define BIT(x)	(1 << (x)) 
#define SETBITS(x,y) ((x) |= (y))
#define CLEARBITS(x,y) ((x) &= (~(y)))
#define SETBIT(x,y) SETBITS((x), (BIT((y))))
#define CLEARBIT(x,y) CLEARBITS((x), (BIT((y))))

#define W 1
#define R 0

#define dataLen 32
uint8_t *data;
uint8_t *arr;


void InitSPI(void)
{
	//Set SCK (PB2), MISO (PB1 connects to nRF MOSI) , CSN (PB4) and Ce (PB3) as outport 
	// OBS !!! Has to be set before SPI-Enable Below
	DDRB |= (1<<PB2) | (1<<PB1) | (1<<PB4) |(1<<PB3);
	
	// Set MOSI (PB0) as input OBS: connects to nRF MISO
	DDRB &= ~(1<<PB0);
	PORTB |= (1<<PB0);
	
	USICR |= (1<<USIWM0)|(1<<USICS1)|(1<<USICLK);
	
	SETBIT(PORTB, 4);	//CSN IR_High to start with, nothing to be sent to the nRF yet
	CLEARBIT(PORTB, 3);	//CE low to start with
}

char WriteByteSPI(unsigned char cData)
{
	//Load byte to Data register
	USIDR = cData;	
	
	USISR |= (1<<USIOIF); // clear flag to be able to receive new data
		
	/* Wait for transmission complete */
	while((USISR & (1<<USIOIF)) == 0)
	{
		USICR |= (1<<USITC); //TOGGLE SCK and count a 4-bit counter from 0-15, 
	// when it reaches 15 USIOIF is set!
	}
	
	return USIDR;
}
////////////////////////////////////////////////////
 
 
/*****************in/out***************************/  

void ioinit(void)			
{
	DDRB |= (1<<DDB5); //led
}

uint8_t GetReg(uint8_t reg)
{	
	_delay_us(10); // make sure last command was a while ago...
	CLEARBIT(PORTB, 4);	//CSN low
	_delay_us(10);
	WriteByteSPI(R_REGISTER + reg);	// R_Register = set the nRF to reading mode, "reg" = this registry will be read back
	_delay_us(10);
	reg = WriteByteSPI(NOP); // send NOP (dummy Byte) once receive back the first byte in the "reg" register
	_delay_us(10);
	SETBIT(PORTB, 4);	//CSN Hi -nRF goes back to doing nothing
	return reg;	// Return the read registry
}
 
 
/*****************nrf-setup***************************/ 
/*uint8_t WriteToNrf(uint8_t reg, uint8_t Package)	
{
	_delay_us(10); //make sure last command was a while ago...
	CLEARBIT(PORTB, 4); // CSN LOW - nRF starts to listen for command
	_delay_us(10);
	WriteByteSPI(W_REGISTER + reg); //W_REGISTER = set the nRF in write mode
	_delay_us(10);
	WriteByteSPI(Package); // Send the package ti be written to the Registry "reg"
	_delay_us(10);
	SETBIT(PORTB, 4); // CSN Hi -nRF goes back to doing nothing
}
*/
uint8_t *WriteToNrf(uint8_t ReadWrite, uint8_t reg, uint8_t *val, uint8_t antVal)	//tar in "ReadWrite" (W el R), "reg" (get register), "*val" (en array) & "antVal" (antal integer i variabeln)
{
	// "ReadWrite" ("W" or "R"), "reg", (the register), "*val" (an array with the Package) & "antVal" (number of intergers in the package)
	if (ReadWrite == W)	// if "W" 
	{
		reg = W_REGISTER + reg;	// Add the write bit to the reg
	}
	
	//Create an array to be returned at the end
	//Static uint8_t is needed to be able to return an array
	
	static uint8_t ret[dataLen];
	
	_delay_us(10);		
	CLEARBIT(PORTB, 4);	//CSN low 
	_delay_us(10);
	WriteByteSPI(reg);	//set the nRF to Write or read mode of "reg"
	_delay_us(10);
	
	int i;
	for(i=0; i<antVal; i++)
	{
		if (ReadWrite == R && reg != W_TX_PAYLOAD) //Did you want to read a registry?
		{
			ret[i]=WriteByteSPI(NOP);	//send dummy bytes to read out the data
			_delay_us(10);
		}
		else
		{
			WriteByteSPI(val[i]); //Send the commands to the nRF once at a time
			_delay_us(10);
		}
	}
	SETBIT(PORTB, 4);	//CSN High
	
	return ret;	//return the array
}

void nrf24L01_init(void)
{
	_delay_ms(100);	//allow radio to reach power down if shut down
	
	uint8_t val[5];	// An array of ints to send
	
	
	val[0]=0x01; // set value
	WriteToNrf(W, EN_AA, val, 1); //W = writemode , En_AA=register to write to, val=data to write, 1=number of data bytes	
	
	//SETUP_RETR (the setup for "EN_AA")
	val[0]=0x2F;	//0b0010 00011 "2" sets it up to 750uS delay between every retry (at least 500us at 250kbps 
					//and if payload >5bytes in 1Mbps, and if payload >15byte in 2Mbps) "F" is number of retries (1-15, now 15)
	WriteToNrf(W, SETUP_RETR, val, 1);
	
	//Choose number of enabled data pipes (1-5)
	val[0]=0x01;
	WriteToNrf(W, EN_RXADDR, val, 1); //enable data pipe 0
	
	//RF_Adress width setup (how many bytes is the receiver address, the more the merrier 1-5)
	val[0]=0x03; //0b0000 00011 = 5 bytes RF_Address
	WriteToNrf(W, SETUP_AW, val, 1); 
	
	//RF channel setup - choose frequency 2,400-2,527Ghz 1Mhz/step
	val[0]=0x01; //0b0000 0001 = 2,401Ghz (same tx and rx) 
	WriteToNrf(W, RF_CH, val, 1);
	
	//RF setup	- choose power mode and data speed
	val[0]=0x07; //0000 0111 bit 3="0" 1Mbps = longer range, bit 2-1 power mode("11" = -0dB; "00"=-18dB)
	WriteToNrf(W, RF_SETUP, val, 1); 
	
	//RX RF_Adress setup 5 byte - Set Receiver address(set RX_ADDR_P0 = TX_ADDR if EN_AA is enabled
	int i;
	for(i=0; i<5; i++)
	{
		val[i]=0x12;	//0x12 x 5 to get a long and secure address
	}
	WriteToNrf(W, RX_ADDR_P0, val, 5); //since we chose pipe 0 on EN_RXADDR we give this address to the channel.
	
	for(i=0; i<5; i++) //TX RF_Adress setup 5 byte - Set Receiver address (set RX_ADDR_P0 = TX_ADDR if EN_AA is enabled
	{
		val[i]=0x12; //0x12 x 5 - same on the receiver chip and the RX-RF address above	
	}
	WriteToNrf(W, TX_ADDR, val, 5);
	
	// payload width setup - 1-32 byte how many bytes to send per transmission 
	val[0]=5; //send 5 bytes per package 
	WriteToNrf(W, RX_PW_P0, val, 1);
	
	//CONFIG reg setup - Now its time to boot up the nRF and choose if its Tx or Rx
	val[0]=0x1E;  //0b0000 1110 config registry	bit "1":1=power up,  
	//bit "0":0=transmitter (bit "0":1=Receiver) (bit "4":1=>mask_Max_RT, i.e. IRQ interrupt is not triggered if transmission failed
	WriteToNrf(W, CONFIG, val, 1);
	
	//device need 1.5ms to reach standby mode (CE=low)
	_delay_ms(100);
}

void transmit_payload(uint8_t * W_buff)
{
	WriteToNrf(R, FLUSH_TX, W_buff, 0); //Sends 0xE1 to flush registry
	
	WriteToNrf(R, W_TX_PAYLOAD, W_buff, dataLen); //Sends the data in the W_Buff
	
	_delay_ms(10);		
	SETBIT(PORTB, 3);	//CE high Transmit the data	
	_delay_us(20);		
	CLEARBIT(PORTB, 3);	//CE low stop transmitting
	_delay_ms(10);		
	
}

void receive_payload(void)
{
	
	SETBIT(PORTB, 3);	//CE IR_High = "Lyssnar"
	_delay_ms(1000);	//lyssnar i 1s och om mottaget går int0-interruptvektor igång
	CLEARBIT(PORTB, 3); //ce låg igen -sluta lyssna
	
}

void reset(void)
{
	_delay_us(10);
	CLEARBIT(PORTB, 4);	//CSN low
	_delay_us(10);
	WriteByteSPI(W_REGISTER + STATUS);	//
	_delay_us(10);
	WriteByteSPI(0b01110000);	//radedrar alla irq i statusregistret (för att kunna lyssna igen)
	_delay_us(10);
	SETBIT(PORTB, 4);	//CSN IR_High
}

void ChangeAddress(uint8_t adress)
{
	_delay_ms(100);
	uint8_t val[5];
	//RX RF_Adress setup 5 byte - väljer RF_Adressen på Recivern (Måste ges samma RF_Adress om Transmittern har EN_AA påslaget!!!)
	int i;
	for(i=0; i<5; i++)
	{
		val[i]=adress;	//RF channel registry 0b10101011 x 5 - skriver samma RF_Adress 5ggr för att få en lätt och säker RF_Adress (samma på transmitterns chip!!!)
	}
	WriteToNrf(W, RX_ADDR_P0, val, 5); //0b0010 1010 write registry - eftersom vi valde pipe 0 i "EN_RXADDR" ovan, ger vi RF_Adressen till denna pipe. (kan ge olika RF_Adresser till olika pipes och därmed lyssna på olika transmittrar)
	
	//TX RF_Adress setup 5 byte -  väljer RF_Adressen på Transmittern (kan kommenteras bort på en "ren" Reciver)
	//int i; //återanvänder föregående i...
	for(i=0; i<5; i++)
	{
		val[i]=adress;	//RF channel registry 0b10111100 x 5 - skriver samma RF_Adress 5ggr för att få en lätt och säker RF_Adress (samma på Reciverns chip och på RX-RF_Adressen ovan om EN_AA enablats!!!)
	}
	WriteToNrf(W, TX_ADDR, val, 5);
	_delay_ms(100);
}

int main(void)
{
	
	InitSPI();
	ioinit();
	
	nrf24L01_init();
	
	SETBIT(PORTB,5);		
	_delay_ms(1000);
	CLEARBIT(PORTB,5);
	
	while(1)
	{
		//Wait for USART-interrupt to send data...
		
	}
	return 0;
}




