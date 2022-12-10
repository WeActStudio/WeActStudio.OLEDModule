/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: RTCNotice.c                                               **/
/** Author: Polarix                                                     **/
/** Version: 1.0.0.0                                                    **/
/** Description: HMI demo for notice box interface and refresh screen.  **/
/*************************************************************************/
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "DemoProc.h"
#include "HMI_Engine.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"
#include "SGUI_FontResource.h"
#include "SGUI_PolarCoordinates.h"
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
#include<time.h>
#endif // _SIMPLE_GUI_IN_VIRTUAL_SDK_

//=======================================================================//
//= User Macro definition.                                              =//
//=======================================================================//
#define CLOCK_CENTER_X_COORD			(31)
#define CLOCK_CENTER_Y_COORD			(31)
#define CLOCK_RADIUS                    (28)

//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//
static HMI_ENGINE_RESULT    HMI_DemoPolarCoord_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoPolarCoord_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoPolarCoord_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT    HMI_DemoPolarCoord_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);
static void					HMI_DemoPolarCoord_PaintClock(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iHour, SGUI_INT iMinute, SGUI_INT iSecond);
static void					HMI_DemoPolarCoord_PaintClockText(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iHour, SGUI_INT iMinute, SGUI_INT iSecond);

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
HMI_SCREEN_ACTION       s_stDemoPolorCoordActions =     {   NULL,
                                                            HMI_DemoPolarCoord_Prepare,
                                                            HMI_DemoPolarCoord_RefreshScreen,
                                                            HMI_DemoPolarCoord_ProcessEvent,
                                                            HMI_DemoPolarCoord_PostProcess
                                                        };
//=======================================================================//
//= Global variable declaration.                                        =//
//=======================================================================//
HMI_SCREEN_OBJECT       g_stHMIDemo_PolorCoord =        {   HMI_SCREEN_ID_DEMO_POLOR_COORD,
                                                            &s_stDemoPolorCoordActions
                                                        };

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
HMI_ENGINE_RESULT HMI_DemoPolarCoord_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    /* Clean screen display. */
    pstDeviceIF->fnClear();
    /* Refresh display. */
    HMI_DemoPolarCoord_RefreshScreen(pstDeviceIF, pstParameters);
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoPolarCoord_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SGUI_Basic_DrawCircle(pstDeviceIF, CLOCK_CENTER_X_COORD, CLOCK_CENTER_Y_COORD, CLOCK_RADIUS+3, SGUI_COLOR_FRGCLR, SGUI_COLOR_TRANS);
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    RTCTimerEnable(true);

	time_t now_time=time(NULL);
	struct tm* pstLocalTime = localtime(&now_time);
    HMI_DemoPolarCoord_PaintClock(pstDeviceIF, pstLocalTime->tm_hour, pstLocalTime->tm_min, pstLocalTime->tm_sec);
    //HMI_DemoPolarCoord_PaintClock(pstDeviceIF, 15, 40, 20);
    HMI_DemoPolarCoord_PaintClockText(pstDeviceIF, pstLocalTime->tm_hour, pstLocalTime->tm_min, pstLocalTime->tm_sec);
#else
    #error Add RTC trigger process here.
#endif

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoPolarCoord_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    SGUI_INT                    iProcessAction;
    KEY_PRESS_EVENT*            pstKeyEvent;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;
    iProcessAction =            HMI_DEMO_PROC_NO_ACT;


    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
	if(EVENT_ID_KEY_PRESS == pstEvent->iID)
	{
		pstKeyEvent = (KEY_PRESS_EVENT*)pstEvent;
		// Check event is valid.
		if(SGUI_FALSE == HMI_EVENT_SIZE_CHK(*pstKeyEvent, KEY_PRESS_EVENT))
		{
			// Event data is invalid.
			eProcessResult = HMI_RET_INVALID_DATA;
		}
		else if(KEY_VALUE_ESC == pstKeyEvent->Data.uiKeyValue)
		{
			iProcessAction = HMI_DEMO_PROC_CANCEL;
		}
	}
	else if(EVENT_ID_RTC == pstEvent->iID)
	{
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
		/* RTC time is update. */
		time_t now_time=time(NULL);
		struct tm* pstLocalTime = localtime(&now_time);
		HMI_DemoPolarCoord_PaintClock(pstDeviceIF, pstLocalTime->tm_hour, pstLocalTime->tm_min, pstLocalTime->tm_sec);
		HMI_DemoPolarCoord_PaintClockText(pstDeviceIF, pstLocalTime->tm_hour, pstLocalTime->tm_min, pstLocalTime->tm_sec);
#endif
	}
    if(NULL != piActionID)
    {
        *piActionID = iProcessAction;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoPolarCoord_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(HMI_PROCESS_SUCCESSFUL(eProcResult))
    {
        if(HMI_DEMO_PROC_CANCEL == iActionID)
        {
        	RTCTimerEnable(false);
            HMI_GoBack(NULL);
        }
    }

    return HMI_RET_NORMAL;
}

static void HMI_DemoPolarCoord_PaintClock(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iHour, SGUI_INT iMinute, SGUI_INT iSecond)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
	SGUI_INT			iDialAngl = 0;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    // Paint circle.
    SGUI_Basic_DrawCircle(pstDeviceIF, CLOCK_CENTER_X_COORD, CLOCK_CENTER_Y_COORD, CLOCK_RADIUS, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
    // Paint dial.
    while(iDialAngl < (360+90))
	{
		SGUI_PolarCoord_DrawLine(pstDeviceIF, CLOCK_CENTER_X_COORD, CLOCK_CENTER_Y_COORD, iDialAngl, CLOCK_RADIUS - 3, CLOCK_RADIUS - 1, SGUI_COLOR_FRGCLR);
		iDialAngl += 30;
	}
#if 0
	// Minute angle.
	iDialAngl = (60-(iMinute % 60)) * 6;
	SGUI_PolarCoord_DrawLine(pstDeviceIF, CLOCK_CENTER_X_COORD, CLOCK_CENTER_Y_COORD, iDialAngl+90, 0, CLOCK_RADIUS - 10, SGUI_COLOR_FRGCLR);
	// Second angle.
	iDialAngl = (60-(iSecond % 60)) * 6;
	SGUI_PolarCoord_DrawLine(pstDeviceIF, CLOCK_CENTER_X_COORD, CLOCK_CENTER_Y_COORD, iDialAngl+90, 0, CLOCK_RADIUS - 5, SGUI_COLOR_FRGCLR);
	// Hour angle.
	iDialAngl = ((12-(iHour % 12)) * 30) - (iMinute / 2);
	SGUI_PolarCoord_DrawLine(pstDeviceIF, CLOCK_CENTER_X_COORD, CLOCK_CENTER_Y_COORD, iDialAngl+90, 0, CLOCK_RADIUS - 15, SGUI_COLOR_FRGCLR);
#else
	// Minute angle.
	iDialAngl = (iMinute % 60) * 6;
	SGUI_PolarCoord_DrawLine(pstDeviceIF, CLOCK_CENTER_X_COORD, CLOCK_CENTER_Y_COORD, 450 - iDialAngl, 0, CLOCK_RADIUS - 9, SGUI_COLOR_FRGCLR);
	// Second angle.
	iDialAngl = (iSecond % 60) * 6;
	SGUI_PolarCoord_DrawLine(pstDeviceIF, CLOCK_CENTER_X_COORD, CLOCK_CENTER_Y_COORD, 450 - iDialAngl, 0, CLOCK_RADIUS - 5, SGUI_COLOR_FRGCLR);
	// Hour angle.
	iDialAngl = ((iHour % 12) * 30) + (iMinute / 2) - 1;
	SGUI_PolarCoord_DrawLine(pstDeviceIF, CLOCK_CENTER_X_COORD, CLOCK_CENTER_Y_COORD, 450 - iDialAngl, 0, CLOCK_RADIUS - 18, SGUI_COLOR_FRGCLR);
#endif
}

static void HMI_DemoPolarCoord_PaintClockText(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iHour, SGUI_INT iMinute, SGUI_INT iSecond)
{
	/*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_CHAR			szTextBuffer[10] = {0x00};
    SGUI_RECT			stPaintArea = {78, 25, 48, 12};
    SGUI_POINT			stInnerPos = {0, 0};

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    snprintf(szTextBuffer, 10, "%02d:%02d:%02d", iHour, iMinute, iSecond);
    SGUI_Text_DrawText(pstDeviceIF, szTextBuffer, &SGUI_DEFAULT_FONT_12, &stPaintArea, &stInnerPos, SGUI_DRAW_NORMAL);
}

