#ifndef _INCLUDE_RTC_H_
#define _INCLUDE_RTC_H_
#include "misc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rtc.h"
#include <time.h>

typedef void(*RTC_INT_FP)(uint32_t uiTimeStamp);

typedef struct tm   RTC_CALENDAR_STRUCT;

void        RTC_Initialize(void);
void        RTC_UpdateCalendar(RTC_CALENDAR_STRUCT *pstCalendar);
uint32_t    RTC_GetTimeStamp(void);
RTC_CALENDAR_STRUCT* RTC_ConvertToCalendar(time_t uiTimeStamp);
void        RTC_ConfigReceiveInterrupt(RTC_INT_FP pfCallBack, int iPreemptionPriority, int SubPriority);

#endif //_INCLUDE_RTC_H_

