#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "shell.h"
#include "ringbuffer8.h"
#include "tlsf.h"

extern void board_lowlevel_init(void);

static uint8_t rx_buff[128];
static ringbuffer8_t rx_ringbuffer;

static Shell shell;
static char shell_buffer[512];

static uint8_t dyl_mem_pool[10 * 1024];
tlsf_t tlsf;

static signed short _shell_write(char *data, unsigned short len)
{
    for (unsigned short i = 0; i < len; i++)
    {
        uart_send((uint8_t)data[i]);
    }
    
    return len;
}



static void uart_rx_handler(uint8_t data)
{
    if (!rb8_full(rx_ringbuffer)) {
        rb8_put(rx_ringbuffer, data);
    }
}

int main(void)
{
    tlsf = tlsf_create_with_pool(dyl_mem_pool, sizeof(dyl_mem_pool));
    if (tlsf == NULL)
    {
        while (1);
    }
    
    board_lowlevel_init();
    uart_init();
    uart_recv_callback_register(uart_rx_handler);

    rx_ringbuffer = rb8_new(rx_buff, sizeof(rx_buff));

    shell.write = _shell_write;
    shellInit(&shell, shell_buffer, sizeof(shell_buffer));

    uint8_t rx_data;
    while (1)
    {
        if (!rb8_empty(rx_ringbuffer)) {
            rb8_get(rx_ringbuffer, &rx_data);
            shellHandler(&shell, rx_data);
        }
    }
}
