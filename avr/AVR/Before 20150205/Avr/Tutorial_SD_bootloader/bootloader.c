#include <avr/io.h>
#include <string.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <util/crc16.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "defines.h"
#include "sdIO.h"
#include "readFAT1216.h"

typedef struct
{
	unsigned long dev_id;
	unsigned short app_version;
	unsigned short crc;
} bootldrinfo_t;

uint16_t startcluster;
uint16_t updatecluster; //is set when update is available
bootldrinfo_t current_bootldrinfo;

// static inline void (*app_start)(void) = 0x0000;
void (*app_start)(void) = 0x0000;


/*****************************************************************/
/* shut down the watchdog timer and clear any pending interrupts */  
/*****************************************************************/
#ifdef WATCHDOG

void get_mcusr(void) \
	__attribute__((naked)) \
	__attribute__((section(".init3")));
	void get_mcusr(void)
{
	MCUSR = 0;
	wdt_disable();
}

#endif

/********************************************/
/* Check firmware size, device, and version */  
/********************************************/

static inline void check_file(void)
{
	//Check filesize
	
	if (filesize != FLASHEND - BOOTLDRSIZE + 1)
		return;

	bootldrinfo_t *file_bootldrinfo;
	fat1216_readfilesector(startcluster, (FLASHEND - BOOTLDRSIZE + 1) / 512 - 1);
	
	file_bootldrinfo =  (bootldrinfo_t*) (uint8_t*) (fat_buf + (FLASHEND - BOOTLDRSIZE - sizeof(bootldrinfo_t) + 1) % 512);
	
	//Check DEVID
	if (file_bootldrinfo->dev_id != DEVID)
		return;
		
	//Check application version
	if (file_bootldrinfo->app_version <= current_bootldrinfo.app_version)
		return;
		
	current_bootldrinfo.app_version = file_bootldrinfo->app_version;
	updatecluster = startcluster;
}


/******/
/*MAIN*/
/******/

int main(void)
{
	//=====================================
	//Define and initialize local variables
	//=====================================

	uint16_t res=1;
	
	uint16_t filesector, j;
	uint16_t *lpword;
	uint16_t adr;


	//=====================
	//Initialize components
	//=====================

	//----------------------
	//Initialize LEDs
	//----------------------
#ifdef USE_FLASH_LED
	FLASH_LED_DDR = 1<<FLASH_LED_PIN;
	#if FLASH_LED_POLARITY
	FLASH_LED_PORT |= 1<<FLASH_LED_PIN;
	#endif
#endif

	//----------------------
	//Initialize memory card
	//----------------------
	for (int i=0; (i<10) && (res & STA_NOINIT); i++){
		res=disk_initialize(0); //Initialize SD card
	}

	if (res & STA_NOINIT) //SD Card not found. WHAT TO DO???
			app_start();


	//----------------------
	//Check current firmware
	//----------------------
	
	memcpy_P(&current_bootldrinfo, (uint8_t*) FLASHEND - BOOTLDRSIZE - sizeof(bootldrinfo_t) + 1, sizeof(bootldrinfo_t));

	if (current_bootldrinfo.app_version == 0xFFFF)
		current_bootldrinfo.app_version = 0; //application not flashed yet


	//-----------------------
	//Search for new firmware
	//-----------------------
	if (fat1216_init() == 0) //Initialize file system
	{
		for (int i=0; i<512; i++)
		{
			startcluster = fat1216_readRootDirEntry(i);

			if (startcluster == 0xFFFF)
			{
				continue;
			}
			
			check_file();
		}
	
		if (updatecluster)
		{
			for (filesector = 0; filesector < (FLASHEND - BOOTLDRSIZE + 1) / 512; filesector++)
			{
#ifdef USE_FLASH_LED
				FLASH_LED_PORT ^= 1<<FLASH_LED_PIN;
#endif
		
				lpword = (uint16_t*) fat_buf;
				fat1216_readfilesector(updatecluster, filesector);
	
				//----------------------
				//Write new firmware
				//----------------------
				for (int i=0; i<(512 / SPM_PAGESIZE); i++)
				{
					adr = (filesector * 512) + i * SPM_PAGESIZE;
					boot_page_erase(adr);
					while (boot_rww_busy())
						boot_rww_enable();
			
					for (j=0; j<SPM_PAGESIZE; j+=2)
						boot_page_fill(adr + j, *lpword++);

					boot_page_write(adr);
					while (boot_rww_busy())
						boot_rww_enable();
				}
			}
	
			//LED on
#ifdef USE_FLASH_LED
	#if !FLASH_LED_POLARITY
			FLASH_LED_PORT &= ~(1<<FLASH_LED_PIN);
	#else
			FLASH_LED_PORT |= 1<<FLASH_LED_PIN;
	#endif
#endif
		}
	}


	//----------------------------------
	//Check that firmware is uncorrupted
	//----------------------------------
	unsigned short flash_crc = 0xFFFF;
	
	for (adr=0; adr<FLASHEND - BOOTLDRSIZE + 1; adr++)
		flash_crc = _crc_ccitt_update(flash_crc, pgm_read_byte(adr));
		
	if (flash_crc == 0)
	{
		//Led off
#ifdef USE_FLASH_LED
		FLASH_LED_PORT |= 1<<FLASH_LED_PIN;
#endif
		app_start();
	}

#ifdef USE_FLASH_LED

	for (;;)
	{ 
		FLASH_LED_PORT ^= 1<<FLASH_LED_PIN;
		_delay_ms(700); 
	}
#else
	while(1);
#endif

}
