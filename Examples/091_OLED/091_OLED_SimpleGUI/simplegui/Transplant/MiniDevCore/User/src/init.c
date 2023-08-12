#include "init.h"

void BSP_Initialize(void)
{
    /* Initialize usart 1 for stdio. */
    USART_Stdio_Initialize(115200);
    USART1_ConfigReceiveInterrupt(USARTReceiveProc, 2, 2);
    USART_ActiveInterrupt(USART1, true);
    printf("Stand IO initialized.\r\n");

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    BASE_TIMER_Initialize(TIM3, 499, 7199);
    TIM3_ConfigReceiveInterrupt(TimerInterruptProc, 2, 1);
    BASE_TIMER_ActiveInterrupt(TIM3, true);
    printf("General timer initialized.\r\n");

    RTC_Initialize();
    RTC_ConfigReceiveInterrupt(RTCInterruptProc, 0, 0);
    printf("RTC timer initialized.\r\n");

    SCREEN_Initialize();
    SCREEN_ClearDisplay();
    printf("Screen device initialized.\r\n");
}
