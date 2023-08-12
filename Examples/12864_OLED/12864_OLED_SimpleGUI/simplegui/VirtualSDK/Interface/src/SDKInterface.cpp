/*************************************************************************/
/** Copyright.															**/
/** FileName: SDKInterface.cpp											**/
/** Author: Polarix														**/
/** Description: SDK swap interface function defines.					**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx/msgdlg.h>
#include "SDKInterface.h"
#include "LCDFrame.h"
#include "DemoProc.h"

//=======================================================================//
//= User definition.												    =//
//=======================================================================//
#define EventSyncLock()							{s_clsEventSyncCS.Enter();}
#define EventSyncUnlock()						{s_clsEventSyncCS.Leave();}
#define SDK_DEFAULT_EVENT_SYNC_TIMEOUT_MS		(500)

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static void					SGUI_SDK_PrepareSDK(void);
static bool					SGUI_SDK_SyncMainFrameEvent(wxEvent& clsEvent, ENV_FLAG_INDEX eSyncFlag, int iTimeOutMs);

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
static bool					s_barrEventFlag[ENV_FLAG_IDX_MAX];
static SDK_KB_EVENT         s_stKBEvent;
static wxCriticalSection	s_clsEventSyncCS;

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_SDK_SetPixel                                   **/
/** Purpose:		Set virtual device pixel register data.             **/
/** Params:																**/
/**	@ iX[in]:		Pixel x-coordinate on display panel.                **/
/**	@ iY[in]:		Pixel y-coordinate on display panel.                **/
/**	@ iPixelValue[out]:	Pixel value, 0 for clear, 1 for set.			**/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void SGUI_SDK_SetPixel(int iX, int iY, int iPixelValue)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    LCDFrame*           pclsMainFrameObjectPtr;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    pclsMainFrameObjectPtr = LCDFrame::GetInstance();

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    if(NULL != pclsMainFrameObjectPtr)
    {
        pclsMainFrameObjectPtr->SetLCDPixel(iX, iY, iPixelValue);
    }
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_GetPixel									**/
/** Purpose:		Get a pixel value form virtual device register.     **/
/** Params:																**/
/**	@ iX[in]:		Pixel x-coordinate on display panel.                **/
/**	@ iY[in]:		Pixel y-coordinate on display panel.                **/
/** Return:			Pixel state, 0 for cleared, 1 for set.              **/
/** Notice:			None.                                               **/
/*************************************************************************/
int SGUI_SDK_GetPixel(int iX, int iY)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    LCDFrame*           pclsMainFrameObjectPtr;
    int					iPixelValue;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    pclsMainFrameObjectPtr = LCDFrame::GetInstance();

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    if(NULL != pclsMainFrameObjectPtr)
    {
        iPixelValue = pclsMainFrameObjectPtr->GetLCDPixel(iX, iY);
    }
    else
    {
        iPixelValue = 0;
    }

    return iPixelValue;
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_RefreshDisplay								**/
/** Purpose:		Refresh virtual device display.                     **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void SGUI_SDK_RefreshDisplay(void)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    LCDFrame*           pclsMainFrameObjectPtr;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    pclsMainFrameObjectPtr = LCDFrame::GetInstance();

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    if(NULL != pclsMainFrameObjectPtr)
    {
        pclsMainFrameObjectPtr->RefreshLCD();
    }
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_ClearDisplay								**/
/** Purpose:		Clear screen display.                               **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void SGUI_SDK_ClearDisplay(void)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    LCDFrame*           pclsMainFrameObjectPtr;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    pclsMainFrameObjectPtr = LCDFrame::GetInstance();

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    if(NULL != pclsMainFrameObjectPtr)
    {
        pclsMainFrameObjectPtr->ClearLCD();
        pclsMainFrameObjectPtr->RefreshLCD();
    }
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_SyncMainFrameEvent							**/
/** Purpose:		Send a event to main frame and wait for process		**/
/**					complete.											**/
/** Params:																**/
/**	@ clsEvent[in]:		Event class object.								**/
/** @ eSyncFlag[in]:	Sync process flag.								**/
/** @ iTimeOutMs[in]:	Waiting time in millisecond.					**/
/** Return:																**/
/** @ true:				Process successfully.							**/
/**	@ false:			Waiting process timeout.						**/
/** Notice:			If iTimeOutMs parameter value is 0, then process	**/
/**					return immediately.									**/
/*************************************************************************/
bool SGUI_SDK_SyncMainFrameEvent(wxEvent& clsEvent, ENV_FLAG_INDEX eSyncFlag, int iTimeOutMs)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    LCDFrame*           pclsMainFrameObjectPtr;
    TimerSetEvent		clsEventObject;
    bool				bReturn;
    int					iWaitTimeMs;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    pclsMainFrameObjectPtr = LCDFrame::GetInstance();
    bReturn =			false;
    iWaitTimeMs =		iTimeOutMs;

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    do
    {
        if(NULL == pclsMainFrameObjectPtr)
        {
            // No main frame instance.
            break;
        }
        // Set window ID.
        clsEvent.SetId(pclsMainFrameObjectPtr->GetId());
        // Post event.
        SGUI_SDK_SetEvnetSyncFlag(eSyncFlag, false);
        wxPostEvent(pclsMainFrameObjectPtr, clsEvent);
        // Wait for sync
        if(iWaitTimeMs < 1)
        {
            // No wait, return immediately.
            bReturn = true;

            SGUI_SDK_SetEvnetSyncFlag(eSyncFlag, true);
            break;
        }
        else
        {
            while(iWaitTimeMs--)
            {
                if(true == SGUI_SDK_GetEventSyncFlag(eSyncFlag))
                {
                    // Sync done
                    bReturn = true;
                    break;
                }
                wxMilliSleep(1);
            }
        }
    }
    while(0);

    return bReturn;
}

void SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_INDEX eIndex, bool bValue)
{
    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    EventSyncLock();
    s_barrEventFlag[eIndex] = bValue;
    EventSyncUnlock();
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_SyncKeyEventData.							**/
/** Purpose:		Set key code value when key press event targets.	**/
/** Params:																**/
/**	@ pcstKBEvent[in]: Key event data structure.                        **/
/** Return:			None.												**/
/** Notice:			This function used to save and keep pressed key 	**/
/**					code value, must be called before SetEvnetFlag.		**/
/*************************************************************************/
void SGUI_SDK_SyncKeyEventData(const SDK_KB_EVENT* pcstKBEvent)
{
    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    EventSyncLock();
    if(nullptr != pcstKBEvent)
    {
        memcpy(&s_stKBEvent, pcstKBEvent, sizeof(SDK_KB_EVENT));
    }
    EventSyncUnlock();
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_GetKeyEventData.							**/
/** Purpose:		Set key code value when key press event targets.	**/
/** Params:			none.												**/
/** Return:			Last targeted key event data.						**/
/** Notice:			This function used to save and keep pressed key 	**/
/**					code value, must be called before SetEvnetFlag.		**/
/*************************************************************************/
const SDK_KB_EVENT* SGUI_SDK_GetKeyEventData(void)
{
	return &s_stKBEvent;
}

/*************************************************************************/
/** Function Name:	GetEventFlag.										**/
/** Purpose:		Post SDK heart-beat timer set event to main frame.	**/
/** Params:																**/
/**	@ uiIntervalMs[in]:	Timer interval in ms, 0 to stop timer.			**/
/** Return:																**/
/**	@ true:				Post message and wait sync successfully.		**/
/**	@ false:			Post message failed or wait for sync timeout.	**/
/** Notice:			This function will start a timer for demo process,	**/
/**					like screen auto scroll animation or dummy data		**/
/**					input.												**/
/*************************************************************************/
bool SGUI_SDK_GetEventSyncFlag(ENV_FLAG_INDEX eIndex)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    bool				bReturn;

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    EventSyncLock();
    bReturn = s_barrEventFlag[eIndex];
    EventSyncUnlock();
    return bReturn;
}

/*************************************************************************/
/** Function Name:	SetSDKSysTickTimer.									**/
/** Purpose:		Post SDK heart-beat timer set event to main frame.	**/
/** Params:																**/
/**	@ uiIntervalMs[in]:	Timer interval in ms, 0 to stop timer.			**/
/** Return:																**/
/**	@ true:				Post message and wait sync successfully.		**/
/**	@ false:			Post message failed or wait for sync timeout.	**/
/** Notice:			This function will start a timer for demo process,	**/
/**					like screen auto scroll animation or dummy data		**/
/**					input, the recommended setting is 20(ms) or more.	**/
/*************************************************************************/
bool SGUI_SDK_ConfigGeneralTimer(unsigned int uiIntervalMs)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    TimerSetEvent		clsEventObject;
    bool				bReturn;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    bReturn =			false;

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    // Prepare event object.

    clsEventObject.SetEventType(wxEVT_TIMER_SET);
    clsEventObject.SetInterval(uiIntervalMs);

    // Post event.
    bReturn = SGUI_SDK_SyncMainFrameEvent(clsEventObject, ENV_FLAG_IDX_SDK_TIM_SET, SDK_DEFAULT_EVENT_SYNC_TIMEOUT_MS);

    return bReturn;
}

/*************************************************************************/
/** Function Name:	SetRTCTimer.										**/
/** Purpose:		Post RTC timer set event to main frame.				**/
/** Params:																**/
/**	@ bEnabled[in]:		RTC timer enable.								**/
/** Return:																**/
/**	@ true:				Post message and wait sync successfully.		**/
/**	@ false:			Post message failed or wait for sync timeout.	**/
/** Notice:			This function will start RTC for demo process, like **/
/**					screen auto scroll animation or dummy data input.	**/
/*************************************************************************/
bool SGUI_SDK_EnableSecondInterrupt(bool bEnabled)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    RTCSwitchEvent		clsEventObject;
    bool				bReturn;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    bReturn =			false;

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    // Prepare event object.

    clsEventObject.SetEventType(wxEVT_RTC_SWITCH);
    clsEventObject.SetReqState(bEnabled);

    // Post event.
    bReturn = SGUI_SDK_SyncMainFrameEvent(clsEventObject, ENV_FLAG_IDX_SDK_SEC_EN, SDK_DEFAULT_EVENT_SYNC_TIMEOUT_MS);

    return bReturn;
}

/*************************************************************************/
/** Function Name:	PrepareSDK.											**/
/** Purpose:		Initialize global variable of SDK.					**/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			This function must only called when dummy main		**/
/**					starting.											**/
/*************************************************************************/
void SGUI_SDK_PrepareSDK(void)
{
    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    // Initialize event sync flag
    for(unsigned int i=0; i<ENV_FLAG_IDX_MAX; i++)
    {
        s_barrEventFlag[i] = false;
    }
    // Initialize key coed variable.
    memset(&s_stKBEvent, 0x00, sizeof(SDK_KB_EVENT));
}

/*************************************************************************/
/** Function Name:	InitializeSDK.										**/
/** Purpose:		Post initialize event and wait for event sync.		**/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			Post event to main frame for initialize SDK.		**/
/*************************************************************************/
bool SGUI_SDK_Initialize(void)
{
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    InitEvent			clsEventObject;
    bool				bReturn;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    bReturn =			false;

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    // Prepare event object.
    clsEventObject.SetEventType(wxEVT_SDK_INIT);

    bReturn = SGUI_SDK_SyncMainFrameEvent(clsEventObject, ENV_FLAG_IDX_SDK_INIT, SDK_DEFAULT_EVENT_SYNC_TIMEOUT_MS);

    return bReturn;
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_DummyMainProc								**/
/** Purpose:		Main process start interface.						**/
/** Params:			None.                                               **/
/** Return:			return code, 0 for no error.						**/
/** Notice:			This function simulates the main entry function of	**/
/**					the target platform, it will called by main frame	**/
/**					thread helper process.								**/
/*************************************************************************/
int SGUI_SDK_DummyMainProc(void)
{
    // Initialize SDK.
    SGUI_SDK_PrepareSDK();
    // Initialize main frame.
    SGUI_SDK_Initialize();
    // Call main process.
    DemoMainProcess();

    return 0;
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_GeneralTimerHandler						**/
/** Purpose:		SDK Dummy General timer interrupt service function.	**/
/** Params:			None.                                               **/
/** Return:			none.												**/
/** Notice:			This function simulates the SysTick timer interrupt	**/
/**					service, add custom processing here if needed.		**/
/*************************************************************************/
void SGUI_SDK_GeneralTimerHandler(void)
{
	// Add your process here.
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_SecondTimerHandler                         **/
/** Purpose:		SDK Dummy second interrupt service function.        **/
/** Params:			None.                                               **/
/** Return:			none.												**/
/** Notice:			This function simulates the RTC interrupt service,	**/
/**					add custom processing here if needed.				**/
/*************************************************************************/
void SGUI_SDK_SecondTimerHandler(void)
{
    // Add your process here.
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_KeyboardHandler                            **/
/** Purpose:		SDK Dummy Key board interrupt service function.     **/
/** Params:			None.                                               **/
/** Return:			none.												**/
/** Notice:			This function simulates the RTC interrupt service,	**/
/**					add custom processing here if needed.				**/
/*************************************************************************/
void SGUI_SDK_KeyboardHandler(void)
{
    // Add your process here.
}
