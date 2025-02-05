#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "shell.h"

static volatile uint8_t rxdata;

static Shell shell;
static char shell_buffer[512];

static signed short _shell_write(char *data, unsigned short len)
{
    for (unsigned short i = 0; i < len; i++)
    {
        uart_send((uint8_t)data[i]);
    }
    
    return len;
}


extern void board_lowlevel_init(void);

static void uart_rx_handler(uint8_t data)
{
    rxdata = data;
}

int main(void)
{
    board_lowlevel_init();
    uart_init();
    uart_recv_callback_register(uart_rx_handler);


    shell.write = _shell_write;
    shellInit(&shell, shell_buffer, sizeof(shell_buffer));


    while (1)
    {
        if (rxdata != 0) {
            shellHandler(&shell, rxdata);
            rxdata = 0;
        }
    }
}
