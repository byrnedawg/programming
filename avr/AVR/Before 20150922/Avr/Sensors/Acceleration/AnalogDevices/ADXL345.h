/*
 * $Id: ADXL345.h,v 1.2 2010/09/30 16:49:35 clivewebster Exp $
 * Revision History
 * ================
 * $Log: ADXL345.h,v $
 * Revision 1.2  2010/09/30 16:49:35  clivewebster
 * Refactored for new hw or sw i2c bus
 *
 * Revision 1.1  2010/06/20 19:30:56  clivewebster
 * Added in Version 1.21
 *
 * ================
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
 *        File: ADXL345.h
 *  Created on: 15 Jun 2010
 *      Author: Clive Webster
 *
 *  Accelerometer using I2C or SPI bus - this lib only supports the I2C bus
 *  The address can be 0x3a or 0xa6
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include "../_acceleration_common.h"
#include "../../../i2cBus.h"

// The class used to read a raw value
extern ACCELEROMETER_CLASS c_ADXL345;

typedef struct s_ADXL345{
	ACCELEROMETER accelerometer;
	I2C_DEVICE	  i2cInfo;
} ADXL345;

// Create the sensor
#define MAKE_ADXL345(i2cAddr) { \
	MAKE_ACCELEROMETER_SENSOR(c_ADXL345), \
	MAKE_I2C_DEVICE(i2cAddr) \
	}

#endif /* ADXL345_H_ */
