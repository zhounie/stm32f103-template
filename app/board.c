#include "stm32f10x.h"


void board_lowlevel_init(void)
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}
