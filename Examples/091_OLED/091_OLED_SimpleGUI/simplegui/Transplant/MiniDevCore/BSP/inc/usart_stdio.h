#ifndef _INCLUDE_USART_STDIO_H_
#define _INCLUDE_USART_STDIO_H_
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

struct __FILE
{
    int handle;
};

void        USART_Stdio_Initialize(uint32_t Baudrate);

#endif
