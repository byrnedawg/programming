/*
 * $Id: $
 * Revision History
 * ================
 * $Log: $
 * ================
 *
 * Copyright (C) 2011 Clive Webster (webbot@webbot.org.uk)
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
 *        File: uartCommonTxNoBuf.c
 *  Created on: 29 Jan 2011
 *      Author: Clive Webster
 */

#include "../_uart_common.h"
#include "../core.h"
#include "../led.h"

// Send a character via a non-buffered UART
void __uartSendUnbuffered(void* _uart, uint8_t data){
	UART* uart=(UART*)_uart;

	// There is no xmit buffer - just send now

	// Wait till sending has stopped
	while(!uart->readyTx){
		breathe();
	}

	CRITICAL_SECTION{
		_uartTransmitBlockStart(uart);			// indicate we are sending
		_uartStartXmit(uart,data);				// put the byte
	}
}

void __uartSendNextUnbuffered(void* _uart){
	UART* uart=(UART*)_uart;

	CRITICAL_SECTION{
		// indicate transmit complete, back to ready
		_uartTransmitBlockEnd(uart);				// we have finished sending
	}
}
