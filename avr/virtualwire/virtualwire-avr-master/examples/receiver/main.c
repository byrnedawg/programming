#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <vw_avr.h>
#include <vw_4to6.h>
#include <vw_decode.h>
#include "uart.h"

unsigned char msg_buf[20];

void print_packet(uint8_t* rx_buffer) {
  uint8_t length = rx_buffer[0] - 3;
  uart_newline();
  uart_sendstr_p(PSTR("Data Length: "));
  utoa(length, (char *)msg_buf, 10);
  uart_sendstr((char *)msg_buf);
  uart_newline();

  uart_sendstr_p(PSTR("Dec\tHex\tChr"));
  uart_newline();

  uint8_t i;
  for (i = 1; i <= length; i++) {
    utoa(rx_buffer[i], (char *)msg_buf, 10);
    uart_sendstr((char *)msg_buf);
    uart_sendchar('\t');

    utoa(rx_buffer[i], (char *)msg_buf, 16);
    uart_sendstr((char *)msg_buf);
    uart_sendchar('\t');

    uart_sendchar(rx_buffer[i]);
    uart_newline();
  }
  uart_newline();
}

void print_bits(uint16_t* bits) {
  utoa(((*bits) & 0x3f) << 4, (char *)msg_buf, 2);
  uart_sendstr((char *)msg_buf);
  uart_sendchar(' ');
  utoa((*bits) >> 6, (char *)msg_buf, 2);
  uart_sendstr((char *)msg_buf);
  uart_newline();
}

int main(void)
{
  uart_init();
  int i = 0;

  for (i = 0; i < 20; i++) {
    uart_newline();
  }
  uart_sendstr_p(PSTR("===== VirtualWire Receiver ====="));
  uart_newline();

  vw_avr_setup(VW_AVR_MODE_RX);
  vw_decode_callback = &print_packet;
  //vw_decode_bits_callback = &print_bits;

  while(1) {}
  return 0;
}
