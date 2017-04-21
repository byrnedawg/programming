#ifndef _LED_H
#define _LED_H

// 0 - active low, 1 - active high
#define LED1_ACTIVE_LOW    0

#if (LED1_ACTIVE_LOW ==0) 
    #define Led1_on()     Led1_PORT &= ~_BV(Led1_BIT) 
    #define Led1_off()    Led1_PORT |=  _BV(Led1_BIT) 
	#define Led2_on()     Led2_PORT &= ~_BV(Led2_BIT)
	#define Led2_off()    Led2_PORT |=  _BV(Led2_BIT)
#else 
    #define Led1_on()     Led1_PORT |=  _BV(Led1_BIT) 
    #define Led1_off()    Led1_PORT &= ~_BV(Led1_BIT) 
	#define Led2_on()     Led2_PORT |=  _BV(Led2_BIT)
	#define Led2_off()    Led2_PORT &= ~_BV(Led2_BIT)
#endif

#endif

void initLEDs(void);

void toggleLED(int ledNum);