#ifndef _BUTTON_H
#define _BUTTON_H

#include "prj.h"

#define Button1_press()     ( ( Button1_PIN & _BV(Button1_BIT)) == 0 )
#define Button1_release()   ( (Button1_PIN & _BV(Button1_BIT)) == 1 )

#define Button2_press()     ( ( Button2_PIN & _BV(Button2_BIT)) == 0 )
#define Button2_release()   ( ( Button2_PIN & _BV(Button2_BIT)) == 1 )

#endif 

void initButtons(void);

unsigned int buttonIsPressed(int button);