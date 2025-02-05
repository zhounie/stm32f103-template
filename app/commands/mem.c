#include <stdint.h>
#include <stdlib.h>
#include "shell.h"
#include "tlsf.h"
#include "uart.h"

extern tlsf_t tlsf;

int cmd_mem(int argc, char *argv[])
{
    int size = atoi(argv[1]);

    uint8_t *ptr = tlsf_malloc(tlsf, size);
    shellPrint(shellGetCurrent(), "ALLOCATED %d bytes at 0x%p\n", size, ptr);

    tlsf_free(tlsf, ptr);

    return 0;
}

