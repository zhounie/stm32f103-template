#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>


typedef void (*uart_rx_callback_t)(uint8_t data);


void uart_init(void);
void uart_send(uint8_t data);
void uart_recv_callback_register(uart_rx_callback_t cb);


#endif
