#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "prj.h"

#define KEYPAD_NO_PRESS  (unsigned char) 0xFF 
#define KEY1_PRESS       (unsigned char) 0  
#define KEY2_PRESS       (unsigned char) 1  
#define KEY3_PRESS       (unsigned char) 2  
#define KEY4_PRESS       (unsigned char) 3  
#define KEY5_PRESS       (unsigned char) 4  
#define KEY6_PRESS       (unsigned char) 5  
#define KEY7_PRESS       (unsigned char) 6  
#define KEY8_PRESS       (unsigned char) 7  
#define KEY9_PRESS       (unsigned char) 8  
#define KEY10_PRESS      (unsigned char) 9  
#define KEY11_PRESS      (unsigned char) 10  
#define KEY12_PRESS      (unsigned char) 11  
#define KEY13_PRESS      (unsigned char) 12  
#define KEY14_PRESS      (unsigned char) 13  
#define KEY15_PRESS      (unsigned char) 14  
#define KEY16_PRESS      (unsigned char) 15  
 
#ifndef KEY_RELEASE 
    #define KEY_RELEASE   1 
#endif  
#ifndef KEY_PRESS 
    #define KEY_PRESS     0 
#endif 
 
unsigned char Get_Keypad1_Press();

#endif 
