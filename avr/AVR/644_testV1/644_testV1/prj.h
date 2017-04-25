#ifndef _PRJ_H
#define _PRJ_H

#define F_CPU 8000000UL
#include <avr/io.h> 
#include <avr/sfr_defs.h> 
#include <util/delay.h> 
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "pin_config.h"
#include "program_Memory.h"
#include "adc.h"
#include "led.h"
#include "buzzer.h"
#include "relay.h"
#include "button.h"
#include "keypad.h"
#include "lcm8b.h"
#include "uart.h"
#include "timer.h"
#include "interrupt.h"
#include "main.h" 

#endif
