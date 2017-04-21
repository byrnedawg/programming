// key.c

#include "prj.h"

void initButtons(void)
{
	set_input(Button1_DIR, Button1_PIN_BIT);
	set_input(Button2_DIR, Button2_PIN_BIT);
}

unsigned int buttonIsPressed(int button)
{
	if(button == 1)
	{
		if(Button1_press())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	if(button == 2)
	{
		if(Button2_press())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}