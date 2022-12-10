#include "base_timer.h"

#define TIMER_REV_INT_CBPF_DEFINE(TIMER, INIT)  static TIMER_INT_FP TIMER##_INIT_CALLBACK = (INIT)

#define TIMER_REV_INT_CFG_DEFINE(TIMER)         void TIMER##_ConfigReceiveInterrupt(TIMER_INT_FP pfCallBack, int iPreemptionPriority, int SubPriority)\
                                                {\
                                                    NVIC_InitTypeDef NVIC_InitStructure;\
                                                    NVIC_InitStructure.NVIC_IRQChannel = TIMER##_IRQn;\
                                                    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=iPreemptionPriority;\
                                                    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;\
                                                    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;\
                                                    NVIC_Init(&NVIC_InitStructure);\
                                                    TIMER##_INIT_CALLBACK = pfCallBack;\
                                                }

#define TIMER_REV_INT_SRV_DEFINE(TIMER)         void TIMER##_IRQHandler(void)\
                                                {\
                                                    if (TIM_GetITStatus(TIMER, TIM_IT_Update) != RESET)\
                                                    {\
                                                        TIM_ClearITPendingBit(TIMER, TIM_IT_Update);\
                                                        if(NULL != TIMER##_INIT_CALLBACK)\
                                                        {\
                                                            TIMER##_INIT_CALLBACK();\
                                                        }\
                                                    }\
                                                }


TIMER_REV_INT_CBPF_DEFINE(TIM3, NULL);
TIMER_REV_INT_CFG_DEFINE(TIM3);
TIMER_REV_INT_SRV_DEFINE(TIM3);

TIMER_REV_INT_CBPF_DEFINE(TIM4, NULL);
TIMER_REV_INT_CFG_DEFINE(TIM4);
TIMER_REV_INT_SRV_DEFINE(TIM4);

void BASE_TIMER_Initialize(TIM_TypeDef* pstTimerBase, uint16_t uiReloadValue, uint16_t uiPrescaler)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    /* Configure timer data. */
    TIM_TimeBaseStructure.TIM_Period = uiReloadValue;
    TIM_TimeBaseStructure.TIM_Prescaler = uiPrescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(pstTimerBase, &TIM_TimeBaseStructure);
}

void BASE_TIMER_ActiveInterrupt(TIM_TypeDef* pstTimerBase, bool bEnable)
{
    TIM_Cmd(pstTimerBase, DISABLE);
    if(true == bEnable)
    {
        TIM_ITConfig(pstTimerBase,TIM_IT_Update, ENABLE);
    }
    else
    {
        TIM_ITConfig(pstTimerBase,TIM_IT_Update, DISABLE);
    }
    TIM_Cmd(pstTimerBase, ENABLE);
}
