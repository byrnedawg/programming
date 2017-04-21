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

