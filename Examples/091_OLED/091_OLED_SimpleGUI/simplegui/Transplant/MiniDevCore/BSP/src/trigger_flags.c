#include "trigger_flags.h"
#include <stdio.h>

static bool __IO        s_bBaseTimerTrigger = false;
static bool __IO        s_bRTCTimerTrigger = false;
static USART_INPUT __IO s_stUsartInput = {0x00};

void USARTReceiveProc(USART_INT_REASON eReason, uint8_t uiReceiveData)
{
    if(USART_INT_REASON_IDLE == eReason)
    {
        s_stUsartInput.bIsTriggered = true;
    }
    else if(USART_INT_REASON_REV == eReason)
    {
        if(true == s_stUsartInput.bHalfRev)
        {
            s_stUsartInput.unKeyValue.uiByte[0] = uiReceiveData;
            s_stUsartInput.bIsTriggered = true;
        }
        else
        {
            s_stUsartInput.unKeyValue.uiByte[1] = uiReceiveData;
            s_stUsartInput.bHalfRev = true;
        }
    }
}

void KeyEventProc(uint16_t uiKeyCode, KEY_EVENT eEvent)
{
    if(KEY_EVENT_DOWN == eEvent)
    {
        printf("Key 0x%02X is down.", uiKeyCode);
        printf("\r\n");
    }
}

void TimerInterruptProc(void)
{
    s_bBaseTimerTrigger = true;
}

void RTCInterruptProc(uint32_t uiTimeStamp)
{
    s_bRTCTimerTrigger = true;
}

bool BaseTimerIsTrigger(void)
{
    return s_bBaseTimerTrigger;
}

void BaseTimerTriggerReset(void)
{
    s_bBaseTimerTrigger = false;
}

bool UsartIsReceived(void)
{
    return s_stUsartInput.bIsTriggered;
}

uint16_t GetReceivedCode(void)
{
    return s_stUsartInput.unKeyValue.uiKeyCode;
}

void UsartTriggerReset(void)
{
    s_stUsartInput.bIsTriggered = false;
    s_stUsartInput.bHalfRev = false;
    s_stUsartInput.unKeyValue.uiKeyCode = 0;
}

bool RTCTimerIsTrigger(void)
{
    return s_bRTCTimerTrigger;
}

void RTCTimerTriggerReset(void)
{
    s_bRTCTimerTrigger = false;
}

