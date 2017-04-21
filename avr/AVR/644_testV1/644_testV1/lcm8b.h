#ifndef _LCM8B_H
#define _LCM8B_H

#include "prj.h"

#define LCD_CMD_CLEAR_DISPLAY	              0x01
#define LCD_CMD_CURSOR_HOME		          0x02

// Cursor and display shift
#define LCD_CMD_ENTRY_MODE_DEC_NOSHIFT     0x04
#define LCD_CMD_ENTRY_MODE_DEC_SHIFT       0x05
#define LCD_CMD_ENTRY_MODE_INC_NOSHIFT     0x06
#define LCD_CMD_ENTRY_MODE_INC_SHIFT       0x07

// Display control
#define LCD_CMD_DISPLAY_OFF                0x08
#define LCD_CMD_DISPLAY_NO_CURSOR          0x0c
#define LCD_CMD_DISPLAY_CURSOR_NO_BLINK    0x0E
#define LCD_CMD_DISPLAY_CURSOR_BLINK       0x0F

// Cursor and display shift
#define LCD_CMD_DISPLAY_NOSHIFT_CUR_LEFT   0x10
#define LCD_CMD_DISPLAY_NOSHIFT_CUR_RIGHT  0x14
#define LCD_CMD_DISPLAY_SHIFT_LEFT         0x18
#define LCD_CMD_DISPLAY_SHIFT_RIGHT        0x1C

// Function set
#define LCD_CMD_4BIT_1ROW_5X7              0x20
#define LCD_CMD_4BIT_1ROW_5X10             0x24
#define LCD_CMD_4BIT_2ROW_5X7              0x28
#define LCD_CMD_8BIT_1ROW_5X7              0x30
#define LCD_CMD_8BIT_1ROW_5X10             0x34
#define LCD_CMD_8BIT_2ROW_5X7              0x38

void Lcm1_Init(void); 
void Lcm1_CheckBusy(void); 
void Lcm1_Write_Instruction(char instruction); 
void Lcm1_WriteData(char value); 
void Lcm1_ShowString(char *msg); 
void Lcm1_SetCursor(unsigned char x, unsigned char y); 
void Lcm1_ShowString_xy(unsigned char x, unsigned char y, char *msg); 
void Lcm1_Clearscreen(); 

#define Lcm1_LCM_COL      16 

#endif 
