#ifndef _ADC_H
#define _ADC_H

#include "prj.h"

void         adc_init(); 
unsigned int adc_get_value(unsigned char ch); 
unsigned int adc_get_value_ref(unsigned char ref, unsigned char ch);

#endif
