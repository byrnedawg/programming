// lcm8b.c

#include "prj.h"

// control pin in port B bit0,1,2  
#define Lcm1_RS_1()    Lcm1_RS_PORT |= _BV(Lcm1_RS_BIT) 
#define Lcm1_RS_0()    Lcm1_RS_PORT &=~_BV(Lcm1_RS_BIT)

#define Lcm1_RW_1()    Lcm1_RW_PORT |= _BV(Lcm1_RW_BIT)
#define Lcm1_RW_0()    Lcm1_RW_PORT &=~_BV(Lcm1_RW_BIT)

#define Lcm1_EN_1()    Lcm1_EN_PORT |= _BV(Lcm1_EN_BIT)
#define Lcm1_EN_0()    Lcm1_EN_PORT &=~_BV(Lcm1_EN_BIT)

void Lcm1_Init(void) 
{

	Lcm1_DATA_DIR	 = 0xff; // output 
	Lcm1_DATA_PORT = 0x00;

	Lcm1_RS_DIR |= _BV(Lcm1_RS_BIT); // output 
	Lcm1_RW_DIR |= _BV(Lcm1_RW_BIT); // output 
	Lcm1_EN_DIR |= _BV(Lcm1_EN_BIT); // output 

	Lcm1_RS_PORT |= _BV(Lcm1_RS_BIT); 
	Lcm1_RW_PORT |= _BV(Lcm1_RW_BIT); 
	Lcm1_EN_PORT |= _BV(Lcm1_EN_BIT); 

	_delay_us(10);

    Lcm1_Write_Instruction(LCD_CMD_8BIT_2ROW_5X7); _delay_ms(15); 
    Lcm1_Write_Instruction(LCD_CMD_8BIT_2ROW_5X7); _delay_ms(15); 
    Lcm1_Write_Instruction(LCD_CMD_8BIT_2ROW_5X7); _delay_ms(15); 
    Lcm1_Write_Instruction(LCD_CMD_8BIT_2ROW_5X7); _delay_ms(15); 
    Lcm1_Write_Instruction(LCD_CMD_DISPLAY_OFF);   _delay_ms(15); 
    Lcm1_Write_Instruction(LCD_CMD_CLEAR_DISPLAY); _delay_ms(15); 
    Lcm1_Write_Instruction(LCD_CMD_DISPLAY_NO_CURSOR); _delay_ms(15); 
    Lcm1_Write_Instruction(LCD_CMD_ENTRY_MODE_INC_NOSHIFT);_delay_ms(15); 
	
	uint8_t ch=0, chn=0;
	while(ch<64)
	{
		LCDdefinechar((LcdCustomChar+ch),chn++);
		ch=ch+8;
	}
} 
 
void Lcm1_CheckBusy(void) 
{
    char status = 0x80;

	Lcm1_DATA_DIR = 0x00; // input 

    while( status & 0x80)
    {
	    Lcm1_RS_0(); Lcm1_RW_1(); 
		Lcm1_DATA_DIR = 0x00; // input
		Lcm1_EN_1(); 
		_delay_us(10);
        status  = Lcm1_DATA_PIN;
        Lcm1_EN_0();    
  }  
   Lcm1_DATA_DIR = 0xff; // output 
} 

void Lcm1_Write_Instruction(char instruction)
{

	Lcm1_RS_0(); Lcm1_RW_0();
	
	Lcm1_EN_1();	 
	Lcm1_DATA_PORT = instruction; 
	Lcm1_EN_0(); 
	Lcm1_CheckBusy(); 
} 


void Lcm1_WriteData(char value) 
{
   
	Lcm1_RS_1(); Lcm1_RW_0();
	
	Lcm1_EN_1();	 
	Lcm1_DATA_PORT = value; 
	Lcm1_EN_0(); 
	Lcm1_CheckBusy(); 
} 


void Lcm1_ShowString(char *msg) 
{
    int idx =0;
    if (msg==0)
        return;
    while (msg[idx] !=0)
    {
      Lcm1_WriteData(msg[idx++]); 
    } 
} 

// y = 0,1
// x = 0~15
void Lcm1_SetCursor(unsigned char x, unsigned char y) 
{
    if (y==0) // row 0
        Lcm1_Write_Instruction(0x80 + x); // DDRAM, Row1 
    else if (y==1)
        Lcm1_Write_Instruction(0xc0 + x); // DDRAM, Row2 
    else if (y==2)
        Lcm1_Write_Instruction(0x80 + Lcm1_LCM_COL + x);  
    else if (y==3)
        Lcm1_Write_Instruction(0xc0 + Lcm1_LCM_COL + x);  
    else 
        return;    
}

// y = 0,1
// x = 0~15
void Lcm1_ShowString_xy(unsigned char x, unsigned char y, char *msg) 
{
    if (msg==0)
        return;
    Lcm1_SetCursor(x,y); 
    Lcm1_ShowString(msg);   
}

void Lcm1_Clearscreen() 
{
    Lcm1_Write_Instruction(LCD_CMD_CLEAR_DISPLAY); 
}

void LCDdefinechar(const uint8_t *pc,uint8_t char_code){
	uint8_t a, pcc;
	uint16_t i;
	a=(char_code<<3)|0x40;
	for (i=0; i<8; i++){
		pcc=pgm_read_byte(&pc[i]);
		Lcm1_Write_Instruction(a++);
		Lcm1_WriteData(pcc);
	}
}

//adapted fro mAVRLIB
void LCDprogressBar(uint8_t progress, uint8_t maxprogress, uint8_t length)
{
	uint8_t i;
	uint16_t pixelprogress;
	uint8_t c;

	// draw a progress bar displaying (progress / maxprogress)
	// starting from the current cursor position
	// with a total length of "length" characters
	// ***note, LCD chars 0-5 must be programmed as the bar characters
	// char 0 = empty ... char 5 = full

	// total pixel length of bargraph equals length*PROGRESSPIXELS_PER_CHAR;
	// pixel length of bar itself is
	pixelprogress = ((progress*(length*PROGRESSPIXELS_PER_CHAR))/maxprogress);
	
	// print exactly "length" characters
	for(i=0; i<length; i++)
	{
		// check if this is a full block, or partial or empty
		// (u16) cast is needed to avoid sign comparison warning
		if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)+5) > pixelprogress )
		{
			// this is a partial or empty block
			if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)) > pixelprogress )
			{
				// this is an empty block
				// use space character?
				c = 0;
			}
			else
			{
				// this is a partial block
				c = pixelprogress % PROGRESSPIXELS_PER_CHAR;
			}
		}
		else
		{
			// this is a full block
			c = 5;
		}
		
		// write character to display
		Lcm1_WriteData(c);
	}

}

