#ifndef __INCLUDE_TIMER_H__
#define __INCLUDE_TIMER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "stm32f10x_tim.h"

#define TIMER_REV_INT_CFG_DECLARE(TIMER)            void TIMER##_ConfigReceiveInterrupt(TIMER_INT_FP pfCallBack, int iPreemptionPriority, int SubPriority)

typedef void(*TIMER_INT_FP)(void);

void    BASE_TIMER_Initialize(TIM_TypeDef* pstTimerBase, uint16_t uiReloadValue, uint16_t uiPrescaler);
void    BASE_TIMER_ActiveInterrupt(TIM_TypeDef* pstTimerBase, bool bEnable);

TIMER_REV_INT_CFG_DECLARE(TIM3);
TIMER_REV_INT_CFG_DECLARE(TIM4);

#endif /* __INCLUDE_TIMER_H__ */
