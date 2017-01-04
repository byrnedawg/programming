/*
 * cyrf.h
 *
 *  Created on: 18 Feb 2010
 *      Author: duncan
 */

#ifndef CYRF_H_
#define CYRF_H_

#define BUILDING_LIBRARY

#undef BUILDING_LIBRARY




// cyrf event defines.
#define RXC			0x02
#define RXE			0x01
#define SOPDET		0x40
#define TXC			0x02
#define TXE			0x01
#define badCRC		0x08
#define RXGO		0x80		// Start Receive. Setting this bit causes the device to transition to receive mode.
#define RXC_IRQEN	0x02		// Packet Reception Complete Interrupt Enable.
#define RXBERR		0x04
#define TXGO		0x80
#define ABORT_EN	0x20		// Receive Abort Enable. Typical applications disrupt any pending receive by first setting this bit, otherwise this bit is cleared.

#define RX_ABORT_ADR	0x29	// This register provides the ability to override some automatic features of the device.

// data buffer defines.

#define	 	TX_FRA_NUM			1
#define		TX_PS2_0			2
#define		TX_PS2_1			3
#define		TX_PS2_RX			4
#define		TX_PS2_RY			5
#define		TX_PS2_LX			6
#define		TX_PS2_LY			7
#define		TX_CONTROLER		8

#define	RX_ACCEL_X		65					// accelerometer output
#define	RX_ACCEL_Y		66
#define	RX_ACCEL_Z		67
#define	RX_GYRO_X		68					// gyro output
#define	RX_GYRO_Y		69
#define	RX_GYRO_Z		70
#define	RX_EST_X		71					// estimated attitude
#define	RX_EST_Y		72
#define	RX_EST_Z		73
#define	RX_BAT_CUR		74					// current draw on RX battery
#define	RX_RSSI_1S		75					// average signal strength over 1 second
#define	RX_PAC_P1_1S	76					// number of successful control frames last second
#define	RX_PAC_ERR_1S	77					// number of frames with errors last second
#define	RX_PAC_CON_ERR_1S	78				// largest number of consecutive errors last second
#define RX_BAT_VOLT	 	79					// RX battery voltage
#define	RX_RSSI_1F		80					// signal strength at last RX
#define	RX_RSSI_10S		81					// average signal strength over 10 seconds




#define	RX_AUTOP_X_TRIM		88
#define	RX_AUTOP_Y_TRIM		89
#define RX_GYRO_X_OFFSET_L    90
#define RX_GYRO_X_OFFSET_H    91
#define RX_GYRO_Y_OFFSET_L    92
#define RX_GYRO_Y_OFFSET_H    93


#define RX_FRA_NUM	 	97
#define	RX_UPTIME_L		98
#define	RX_UPTIME_H		99
#define	RX_AUTOP_X_MUL	100
#define	RX_AUTOP_Y_MUL	101


#define RX_SERVO_0_CH	113
#define RX_SERVO_0_CL	114
#define RX_SERVO_1_CH	115
#define RX_SERVO_1_CL	116
#define RX_SERVO_2_CH	117
#define RX_SERVO_2_CL	118
#define RX_SERVO_3_CH	119
#define RX_SERVO_3_CL	120
#define RX_SERVO_4_CH	121
#define RX_SERVO_4_CL	122
#define RX_SERVO_0_MUL	123
#define RX_SERVO_1_MUL	124
#define RX_SERVO_2_MUL	125
#define RX_SERVO_3_MUL	126
#define RX_SERVO_4_MUL	127
#define RX_SERVO_5_MUL	128


#define RX_PAC_P1_CNT	129
#define	RX_PAC_ERR_CNT	131
#define	RX_PAC_GAP		132
#define	RX_PAC_GAP_CNT	133
#define	RX_ERR_CODE		134
#define	RX_RX_FLAG		135
#define	RX_RSSI_CNT_L	136
#define	RX_RSSI_CNT_H	137
#define	RX_SEND_ADD		138








#endif /* CYRF_H_ */
