/*
 * $Id: GP2.h,v 1.3 2010/06/14 19:03:07 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: GP2.h,v $
 * Revision 1.3  2010/06/14 19:03:07  clivewebster
 * Add copyright license info
 *
 * Revision 1.2  2010/01/30 21:21:49  clivewebster
 * Added new sensors
 *
 * Revision 1.1  2010/01/27 18:39:54  clivewebster
 * Combine Sharp header files into one called GP2.h
 *
 * ===========
 *
 * Copyright (C) 2010 Clive Webster (webbot@webbot.org.uk)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 * GP2.h
 *
 *  Created on: 27-Jan-2010
 *      Author: Clive Webster
 */

#ifndef GP2_H_
#define GP2_H_

#include "../_distance_common.h"
#include "../../../a2d.h"
#include "../../../iopin.h"

/*
 * GP2D12
 */
// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2D12;

typedef struct s_Sharp_GP2D12{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2D12;

// Create the sensor
#define MAKE_Sharp_GP2D12(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2D12), \
	adcPin }

/*
 * GP2D120
 */

// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2D120;

typedef struct s_Sharp_GP2D120{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2D120;

// Create the sensor
#define MAKE_Sharp_GP2D120(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2D120), \
	adcPin }

/*
 * GP2D15
 */

// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2D15;

typedef struct s_Sharp_GP2D15{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2D15;

// Create the sensor
#define MAKE_Sharp_GP2D15(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2D15), \
	adcPin }


/*
 * GP2Y0A02YK
 */

// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2Y0A02YK;

typedef struct s_Sharp_GP2Y0A02YK{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A02YK;

// Create the sensor
#define MAKE_Sharp_GP2Y0A02YK(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A02YK), \
	adcPin }

/*
 * GP2Y0A21YK
 */
// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2Y0A21YK;

typedef struct s_Sharp_GP2Y0A21YK{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A21YK;

// Create the sensor
#define MAKE_Sharp_GP2Y0A21YK(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A21YK), \
	adcPin }

/*
 * GP2Y0A02YK0F
 */
// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2Y0A02YK0F;

typedef struct s_Sharp_GP2Y0A02YK0F{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A02YK0F;

// Create the sensor
#define MAKE_Sharp_GP2Y0A02YK0F(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A02YK0F), \
	adcPin }


/*
 * GP2Y0A700K0F
 */
// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2Y0A700K0F;

typedef struct s_Sharp_GP2Y0A700K0F{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A700K0F;

// Create the sensor
#define MAKE_Sharp_GP2Y0A700K0F(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A700K0F), \
	adcPin }

/*
 * GP2Y0A710K0F
 */
// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2Y0A710K0F;

typedef struct s_Sharp_GP2Y0A710K0F{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A710K0F;

// Create the sensor
#define MAKE_Sharp_GP2Y0A710K0F(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A710K0F), \
	adcPin }

/*
 * GP2Y3A002K0F
 * Pin 7 is ground
 * Pin 8 is the supply voltage
 */
// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2Y3A002K0F;

typedef struct s_Sharp_GP2Y3A002K0F{
	DISTANCE distance;		// Holds the 'average' distance across all sensors
	ADC_CHANNEL adcPin; 	// The adc pin used to read value (connect to pin 6)
	const IOPin* led[5];	// The iopins used to select each led (connect to pins 1..5)
	DISTANCE_TYPE led_cm[5];// The distance value from each led
	const IOPin* vin;		// The iopin used to turn on the device (connect to pin 9)
	uint8_t pinNo;			// The current pin being sampled
}Sharp_GP2Y3A002K0F;

// Create the sensor
#define MAKE_Sharp_GP2Y3A002K0F(adcPin, led1,led2,led3,led4,led5, vin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y3A002K0F), \
	adcPin, \
	{led1, led2, led3, led4, led5}, \
	{0, 0, 0, 0, 0}, \
	vin, 0}

/*
 * GP2Y3A003K0F
 * Pin 7 is ground
 * Pin 8 is the supply voltage
 */
// The class used to read a raw value
extern DISTANCE_CLASS c_Sharp_GP2Y3A003K0F;

typedef struct s_Sharp_GP2Y3A003K0F{
	DISTANCE distance;		// Holds the 'average' distance across all sensors
	ADC_CHANNEL adcPin; 	// The adc pin used to read value (connect to pin 6)
	const IOPin* led[5];	// The iopins used to select each led (connect to pins 1..5)
	DISTANCE_TYPE led_cm[5];// The distance value from each led
	const IOPin* vin;		// The iopin used to turn on the device (connect to pin 9)
	uint8_t pinNo;			// The current pin being sampled
}Sharp_GP2Y3A003K0F;

// Create the sensor
#define MAKE_Sharp_GP2Y3A003K0F(adcPin, led1,led2,led3,led4,led5, vin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y3A003K0F), \
	adcPin, \
	{led1, led2, led3, led4, led5}, \
	{0, 0, 0, 0, 0}, \
	vin, 0}


#endif /* GP2_H_ */
