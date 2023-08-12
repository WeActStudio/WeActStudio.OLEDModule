#ifndef _INCLUDE_USART_H_
#define _INCLUDE_USART_H_
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "stm32f10x_usart.h"

#define USART_REV_INT_CFG_DECLARE(PORT)         void PORT##_ConfigReceiveInterrupt(USART_INT_FP pfCallBack, int iPreemptionPriority, int SubPriority)

typedef enum
{
    USART_INT_REASON_UNKNOW = 0,
    USART_INT_REASON_IDLE,
    USART_INT_REASON_REV
}USART_INT_REASON;

typedef void(*USART_INT_FP)(USART_INT_REASON eReason, uint8_t uiReceiveData);

void    USART_Initialize(USART_TypeDef* pstUSARTx, uint32_t Baudrate);
void    USART_SendByte(USART_TypeDef* pstUSARTx, uint16_t uiByte);
void    USART_ActiveInterrupt(USART_TypeDef* pstUSARTx, bool bEnable);

USART_REV_INT_CFG_DECLARE(USART1);
USART_REV_INT_CFG_DECLARE(USART2);

#endif
