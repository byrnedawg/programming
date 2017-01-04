/*
	Defines for bootlader
	
	V1.0
	
	12/2008 Kenn Sebesta, kenn@eissq.com
	
*/

/*Firmware version*/
#define DEVID 0x00000001 		//Arbitrary ID. If the firmware ID is different from bootloader ID, the device will not flash

/*MCU bootloader info*/
#define BOOTLDRSIZE  0x1000	//Size of bootloader in [bytes]. 0x1000 = 4096 bytes


/*FAT version*/
//#define USE_FAT12 				//Uncomment for FAT12 support. Requires additional flash


/*Watchdog*/
//#define WATCHDOG 					//Uncomment if Watchdog is used in normal code


/*SPI configuration*/
#define DD_MOSI		DDB5
#define DD_SCK			DDB7
#define DDR_SPI		DDRB
#define DD_SS_PORT	PORTB
#define DD_SS			DDB4


/* Port configuration for SD card SPI access */
#define SD_CS_BLOCK		DDRC
#define SD_CS_PORT		PORTC
#define SD_CS_PIN			7


/*Visual feedback*/
#define USE_FLASH_LED 				//Uncomment if you want visual feedback

/* Port configuration for visual feedback*/
#ifdef USE_FLASH_LED
#define FLASH_LED_POLARITY 0 //1 if HI turns LED on. 0. If LO turns LED on.
#define FLASH_LED_DDR		DDRA
#define FLASH_LED_PORT		PORTA
#define FLASH_LED_PIN		0
#endif



/***********************************************/
/*        DO NOT MODIFY AFTER THIS LINE        */
/***********************************************/

/*-----------------------------------*/
/* Integer type definitions for sdIO */
/*-----------------------------------*/

#ifndef DEFINES_H
#ifndef _INTEGER

/* These types must be 16-bit, 32-bit or larger integer */
typedef int				INT;
typedef unsigned int	UINT;

/* These types must be 8-bit integer */
typedef signed char		CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

/* These types must be 16-bit integer */
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;

/* These types must be 32-bit integer */
typedef long			LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;

/* Boolean type */
typedef enum { FALSE = 0, TRUE } BOOL;

#define _INTEGER
#endif
#endif
