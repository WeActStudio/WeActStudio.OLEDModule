/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: Text.c                                                    **/
/** Author: Polarix                                                     **/
/** Description: HMI demo for text paint interface.                     **/
/*************************************************************************/

//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "DemoProc.h"
#include "SGUI_Text.h"
#include "SGUI_FontResource.h"
#include "Resource.h"

//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//
static HMI_ENGINE_RESULT    HMI_DemoText_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoText_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoText_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT    HMI_DemoText_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
HMI_SCREEN_ACTION           s_stWeActstudioActions =   {   NULL,
                                                            HMI_DemoText_Prepare,
                                                            HMI_DemoText_RefreshScreen,
                                                            HMI_DemoText_ProcessEvent,
                                                            HMI_DemoText_PostProcess
                                                        };

//=======================================================================//
//= Global variable declaration.                                        =//
//=======================================================================//
HMI_SCREEN_OBJECT       g_stHMIWeActstudio =         {   HMI_SCREEN_ID_WeActStudio,
                                                            &s_stWeActstudioActions
                                                        };

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//

HMI_ENGINE_RESULT HMI_DemoText_Prepare (SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pstDeviceIF->fnClear)
    {
        pstDeviceIF->fnClear();
    }
    else
    {
        SGUI_Basic_DrawRectangle(pstDeviceIF, 0, 0, pstDeviceIF->stSize.iWidth, pstDeviceIF->stSize.iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
    }
    HMI_DemoText_RefreshScreen(pstDeviceIF, pstParameters);
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoText_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_RECT                   stDisplayArea;
    SGUI_POINT                  stInnerPos;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    stInnerPos.iX =         0;
    stInnerPos.iY =         0;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    stDisplayArea.iX = 1;
    stDisplayArea.iY = 1;
    stDisplayArea.iWidth = 128;
    stDisplayArea.iHeight = SGUI_DEFAULT_FONT_12.iHeight+1;
	  stInnerPos.iX =         30;
    SGUI_Text_DrawText(pstDeviceIF, "WeAct Studio", &SGUI_DEFAULT_FONT_12, &stDisplayArea, &stInnerPos, SGUI_DRAW_REVERSE);

    stDisplayArea.iY += stDisplayArea.iHeight;
    stDisplayArea.iWidth = 128;
    stDisplayArea.iHeight = SGUI_DEFAULT_FONT_12.iHeight+1;
	  stInnerPos.iX =         0;
    SGUI_Text_DrawText(pstDeviceIF, "OLED Module,IIC,5VMax", &SGUI_DEFAULT_FONT_12, &stDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);
		
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoText_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
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
    pstKeyEvent =               (KEY_PRESS_EVENT*)pstEvent;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(pstEvent->iType == EVENT_TYPE_ACTION)
    {
        // Check event is valid.
        if(SGUI_FALSE == HMI_EVENT_SIZE_CHK(*pstKeyEvent, KEY_PRESS_EVENT))
        {
            // Event data is invalid.
            eProcessResult = HMI_RET_INVALID_DATA;
        }
        else if(EVENT_ID_KEY_PRESS == pstEvent->iID)
        {
            iProcessAction = HMI_DEMO_PROC_CANCEL;
        }
    }

    if(NULL != piActionID)
    {
        *piActionID = iProcessAction;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoText_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(HMI_PROCESS_SUCCESSFUL(eProcResult))
    {
        if(HMI_DEMO_PROC_CANCEL == iActionID)
        {
            HMI_GoBack(NULL);
        }
    }

    return HMI_RET_NORMAL;
}

