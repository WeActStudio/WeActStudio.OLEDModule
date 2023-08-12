#include "rtc.h"
#include <time.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define RTC_CONFIG_FLAG     (0x5050)
#define FREQUENCY_DIVISION  (32767)
#define START_YEAR          (1900)

static RTC_INT_FP RTC_INIT_CALLBACK = NULL;

RTC_CALENDAR_STRUCT g_stCleandar;

void        RTC_RefreshCalendar(void);
time_t      RTC_ConvertToTimestamp(RTC_CALENDAR_STRUCT* pstCalendar);

/*****************************************************************************/
/** Function Name:  RTC_ConfigReceiveInterrupt.                             **/
/** Purpose:        Update date and time to RTC controler.                  **/
/** Params:                                                                 **/
/** @uiTimeStamp:   None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         Non-thread-safe function, Please pay attention to deal  **/
/**                 with multi-thread calls.                                **/
/*****************************************************************************/
void RTC_ConfigReceiveInterrupt(RTC_INT_FP pfCallBack, int iPreemptionPriority, int SubPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = iPreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    RTC_INIT_CALLBACK = pfCallBack;
}

/*****************************************************************************/
/** Function Name:  RTC_Initialize.                                         **/
/** Purpose:        Configure RTC controler.                                **/
/** Resources:      Backup register.                                        **/
/** Params:         None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
void RTC_Initialize(void)
{
    // Read backup register to check if RTC has been configed.
    if (BKP_ReadBackupRegister(BKP_DR1) != RTC_CONFIG_FLAG)
    {
        // Configure RTC for the first time
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
        // Enable backup register access.
        PWR_BackupAccessCmd(ENABLE);
        // Reset backup register access.
        BKP_DeInit();
        // Enable low-speed external clock.
        RCC_LSEConfig(RCC_LSE_ON);
        // Wait for low-speed external clock initialized
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            // Add error handling here for LSE initialize failed.
        }
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);         // Set RTC clock source.
        RCC_RTCCLKCmd(ENABLE);                          // Enable RTC clock.
        RTC_WaitForLastTask();                          // Wait for processing to complete.
        RTC_WaitForSynchro();                           // Wait for the registers to synchronize.
        RTC_ITConfig(RTC_IT_SEC, ENABLE);               // Enable RTC interrupt.
        RTC_WaitForLastTask();                          // Wait for processing to complete.
        RTC_EnterConfigMode();                          // Enter RTC configuration mode
        RTC_SetPrescaler(FREQUENCY_DIVISION);           // Set RTC clock division factor.
        RTC_WaitForLastTask();                          // Wait for processing to complete.
        RTC_ExitConfigMode();                           // Exit RTC configuration mode
        BKP_WriteBackupRegister(BKP_DR1, RTC_CONFIG_FLAG); // Write RTC configuration tag  backup register.
    }
    else
    {
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
    RTC_RefreshCalendar();
}

/*****************************************************************************/
/** Function Name:  RTC_IRQHandler.                                         **/
/** Purpose:        RTC interrupt service function.                         **/
/** Resources:      NVIC controler.                                         **/
/** Params:         None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
void RTC_IRQHandler(void)
{
    // Second interrupt
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        RTC_RefreshCalendar();  // Update time date.
        if(NULL != RTC_INIT_CALLBACK)
        {
            RTC_INIT_CALLBACK(RTC_GetCounter());
        }
    }
    // Alarm interrupt
    if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)
    {
        // Clear Alarm interrupt tag.
        RTC_ClearITPendingBit(RTC_IT_ALR);
    }
    // Clear interrupt
    RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);
    RTC_WaitForLastTask();
}


/*****************************************************************************/
/** Function Name:  RTC_ConvertToUnix.                                      **/
/** Purpose:        Convert real time to unix timestamp.                    **/
/** Resources:      C standard library time.h file.                         **/
/** Params:                                                                 **/
/** @pstCalendar:   Calendar structure pointer.                             **/
/** Return:         Unix timestamp.                                         **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
time_t RTC_ConvertToTimestamp(RTC_CALENDAR_STRUCT* pstCalendar)
{
    pstCalendar->tm_year -= START_YEAR;
    pstCalendar->tm_mon -= 1;
    return mktime(pstCalendar);
}

/*****************************************************************************/
/** Function Name:  RTC_ConvertToCalendar.                                  **/
/** Purpose:        Refresh date or time from RTC controler.                **/
/** Resources:      C standard library time.h file.                         **/
/** Params:                                                                 **/
/** @uiTimeStamp:       Unix timestamp value.                               **/
/** Return:         Calendar structure pointer.                             **/
/** Notice:         Non-thread-safe function, Please pay attention to deal  **/
/**                 with multi-thread calls.                                **/
/*****************************************************************************/
RTC_CALENDAR_STRUCT* RTC_ConvertToCalendar(time_t uiTimeStamp)
{
    RTC_CALENDAR_STRUCT *pstCalendar;
    pstCalendar = localtime(&uiTimeStamp);
    pstCalendar->tm_year += START_YEAR;
    pstCalendar->tm_mon += 1;
    return pstCalendar;
}

/*****************************************************************************/
/** Function Name:  RTC_RefreshCalendar.                                    **/
/** Purpose:            Refresh date and time from RTC controler.           **/
/** Resources:      C standard library time.h file.                         **/
/** Params:                                                                 **/
/** @uiTimeStamp:   None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         Non-thread-safe function, Please pay attention to deal  **/
/**                 with multi-thread calls.                                **/
/*****************************************************************************/
void RTC_RefreshCalendar(void)
{
    time_t uiTimestamp;
    // Get RTC timestamp value.
    uiTimestamp = RTC_GetCounter();
    // Convert RTC timestamp value to calendar.
    memcpy(&g_stCleandar, RTC_ConvertToCalendar(uiTimestamp), sizeof(g_stCleandar));
}

/*****************************************************************************/
/** Function Name:  RTC_UpdateCalendar.                                     **/
/** Purpose:        Update date and time to RTC controler.                  **/
/** Resources:      C standard library time.h file.                         **/
/** Params:                                                                 **/
/** @uiTimeStamp:   None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         Non-thread-safe function, Please pay attention to deal  **/
/**                 with multi-thread calls.                                **/
/*****************************************************************************/
void RTC_UpdateCalendar(RTC_CALENDAR_STRUCT *pstCalendar)
{
    time_t uiTimestamp;
    // Convert calendar data to timestamp.
    uiTimestamp = RTC_ConvertToTimestamp(pstCalendar);
    // Enable backup register access.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RTC_SetCounter(uiTimestamp);
    RTC_WaitForLastTask();
}

/*****************************************************************************/
/** Function Name:  RTC_GetTimeStamp.                                       **/
/** Purpose:        Update date and time to RTC controler.                  **/
/** Params:                                                                 **/
/** @uiTimeStamp:   None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         Non-thread-safe function, Please pay attention to deal  **/
/**                 with multi-thread calls.                                **/
/*****************************************************************************/
uint32_t RTC_GetTimeStamp(void)
{
    time_t uiTimestamp;
    // Get RTC timestamp value.
    uiTimestamp = RTC_GetCounter();

    return uiTimestamp;
}





