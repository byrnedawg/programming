/*
 * adc.h
 *
 * Created: 6/27/2017 7:37:37 AM
 *  Author: TestDev
 */ 


#ifndef ADC_H_
#define ADC_H_

//#include "prj.h"
#include "portHardware.h"

void         adc_init();
unsigned int adc_get_value(unsigned char ch);
unsigned int adc_get_value_ref(unsigned char ref, unsigned char ch);
void Analog_Comparator_init(void);

#endif /* ADC_H_ */