// keypad.c

#include "prj.h"

#ifndef KEY_RELEASE
#define KEY_RELEASE   1
#endif 
#ifndef KEY_PRESS
#define KEY_PRESS     0
#endif

unsigned char Keypad1_Data  = 0;		
unsigned char Keypad1_State = KEY_RELEASE; // KEY_RELEASE ,KEY_PRESS

unsigned char Get_Keypad1_Press() 
{ 
    unsigned char temp1 = 0xff;
   

    if (Keypad1_State == KEY_RELEASE)
    {
        // input | output
        Keypad1_DIR = 0x0F;       
        Keypad1_PORT = 0xF0;    

        _delay_loop_2(4);   

        switch (Keypad1_PIN & 0xF0)
        {
               case 0xE0 : temp1=0 ; break;
               case 0xD0 : temp1=1 ; break;
               case 0xB0 : temp1=2 ; break;
               case 0x70 : temp1=3; break;
               default : temp1=0xff; break;
         }
         if (temp1==0xff)
         {
             Keypad1_Data = KEYPAD_NO_PRESS;
             return KEYPAD_NO_PRESS; 
         }
         Keypad1_Data = temp1 * 4;

        //  output | input
        Keypad1_DIR = 0xF0;       
        Keypad1_PORT = 0x0F;   

        _delay_loop_2(4);    

        switch (Keypad1_PIN & 0x0F) 
        {
               case 0x0E : temp1=0 ; break;
               case 0x0D : temp1=1 ; break;
               case 0x0B : temp1=2 ; break;
               case 0x07 : temp1=3;  break;
               default : temp1=0xff; break;
         }
        if (temp1==0xff)
        {
            Keypad1_Data = KEYPAD_NO_PRESS; 
            return KEYPAD_NO_PRESS; 
        }
        else
        {
            _delay_ms(25);
            Keypad1_Data = Keypad1_Data + temp1; 
            Keypad1_State = KEY_PRESS;
            return  KEYPAD_NO_PRESS ;
        }   

      }
      else 
      {
          Keypad1_DIR = 0x0F;   
          Keypad1_PORT = 0xF0;  

          _delay_loop_2(4);   

             switch (Keypad1_PIN & 0xF0) 
          { 
                   case 0xE0 : temp1=0 ; break;
                   case 0xD0 : temp1=1 ; break;
                   case 0xB0 : temp1=2 ; break;
                   case 0x70 : temp1=3; break;
                   default : temp1=0xff; break;
             }
          if (temp1 != 0xff)
              return KEYPAD_NO_PRESS;
          else 
          {
              Keypad1_State = KEY_RELEASE;
              return Keypad1_Data;
          }     
     }
}

