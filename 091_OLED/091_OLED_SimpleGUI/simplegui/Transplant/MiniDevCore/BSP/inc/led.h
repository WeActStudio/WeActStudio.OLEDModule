#ifndef _INCLUDE_BSP_LED_H_
#define _INCLUDE_BSP_LED_H_

#include <stm32f10x_gpio.h>

void        LED_Initialize(void);
void        LED_On(void);
void        LED_Off(void);
void        LED_Reverse(void);
uint8_t     LED_GetState(void);

#endif
