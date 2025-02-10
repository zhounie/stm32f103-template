#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "shell.h"
#include "ringbuffer8.h"
#include "tlsf.h"
#include "elog.h"

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

    elog_init();

    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);

    elog_start();

    log_a("This is an assert log");
    log_e("This is an error log");
    log_w("This is a warning log");
    log_i("This is an info log");
    log_d("This is a debug log");
    log_v("This is a verbose log");

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
