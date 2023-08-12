/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: HMI_Demo04_Graph.c                                        **/
/** Author: Polarix                                                     **/
/** Version: 1.0.0.0                                                    **/
/** Description: HMI demo for graph interface.                          **/
/*************************************************************************/
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "DemoProc.h"
#include "SGUI_Text.h"
#include "SGUI_RealtimeGraph.h"
#include "SGUI_FontResource.h"

//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//
static HMI_ENGINE_RESULT    HMI_DemoRealGraph_Initialize(SGUI_SCR_DEV* pstDeviceIF);
static HMI_ENGINE_RESULT    HMI_DemoRealGraph_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoRealGraph_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoRealGraph_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT    HMI_DemoRealGraph_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
SGUI_RTGRAPH_CONTROL        s_stRealtimeGraphControl =  {50, -50, SGUI_TRUE, 3, 0};
SGUI_RTGRAPH_DATA           s_stRealtimeGraphData =     {{0}, {0}, {0}, 0, 0};
SGUI_RTGRAPH                s_stRealtimeGraph =         {{1, 9, 126, 49}, &s_stRealtimeGraphData, &s_stRealtimeGraphControl};
static HMI_SCREEN_ACTION    s_stDemoRealtimeGraphActions = { HMI_DemoRealGraph_Initialize,
                                                        HMI_DemoRealGraph_Prepare,
                                                        HMI_DemoRealGraph_RefreshScreen,
                                                        HMI_DemoRealGraph_ProcessEvent,
                                                        HMI_DemoRealGraph_PostProcess,
                                                        };

//=======================================================================//
//= Global variable declaration.                                        =//
//=======================================================================//

HMI_SCREEN_OBJECT           g_stHMIDemo_RealtimeGraph = {HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH,
                                                        &s_stDemoRealtimeGraphActions
                                                        };

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
HMI_ENGINE_RESULT HMI_DemoRealGraph_Initialize(SGUI_SCR_DEV* pstDeviceIF)
{
    //SGUI_RealtimeGraph_Initialize(&s_stRealtimeGraph);
    s_stRealtimeGraph.stLayout.iWidth = pstDeviceIF->stSize.iWidth-2;
    s_stRealtimeGraph.stLayout.iHeight = pstDeviceIF->stSize.iHeight-15;
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoRealGraph_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    // Reinitialize data.
    SGUI_RealtimeGraph_Initialize(&s_stRealtimeGraph);
    // Paint frame.
    SGUI_Basic_DrawRectangle(pstDeviceIF, 0, 0, RECT_WIDTH(pstDeviceIF->stSize), RECT_HEIGHT(pstDeviceIF->stSize), SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
    // Update screen display.
    SGUI_RealtimeGraph_Repaint(pstDeviceIF, &s_stRealtimeGraph);
    // Start dummy heart-beat timer.
    SysTickTimerEnable(true);

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoRealGraph_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    SGUI_CHAR           szTextBuffer[16];
    SGUI_RECT           stTextDisplayArea;
    SGUI_POINT          stInnerPos;

    // Paint frame.
    SGUI_Basic_DrawRectangle(pstDeviceIF, 0, 0, RECT_WIDTH(pstDeviceIF->stSize), RECT_HEIGHT(pstDeviceIF->stSize), SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
    // Paint title
    stTextDisplayArea.iX = 1;
    stTextDisplayArea.iY = 1;
    stTextDisplayArea.iWidth = RECT_WIDTH(pstDeviceIF->stSize)-2;
    stTextDisplayArea.iHeight = 8;
    stInnerPos.iX = 0;
    stInnerPos.iY = 0;
    SGUI_Text_DrawText(pstDeviceIF, SCR5_RT_GRAPH_TITLE, &SGUI_DEFAULT_FONT_8, &stTextDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);
    // Paint value.
    SGUI_Common_IntegerToString(s_stRealtimeGraph.Data->ValueArray[s_stRealtimeGraph.Data->ValueCount-1], szTextBuffer, 10, 4, ' ');
    stTextDisplayArea.iX = 1;
    stTextDisplayArea.iY = RECT_HEIGHT(pstDeviceIF->stSize)-6;
    stTextDisplayArea.iWidth = RECT_WIDTH(pstDeviceIF->stSize)-2;
    stTextDisplayArea.iHeight = 5;
    stInnerPos.iX = 0;
    stInnerPos.iY = 0;
    SGUI_Text_DrawText(pstDeviceIF, szTextBuffer, &SGUI_DEFAULT_FONT_MiniNum, &stTextDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);
    SGUI_RealtimeGraph_Repaint(pstDeviceIF, &s_stRealtimeGraph);
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoRealGraph_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    SGUI_INT                    iNewValue;
    SGUI_UINT16                 uiKeyCode;
    SGUI_UINT16                 uiKeyValue;
    KEY_PRESS_EVENT*            pstKeyEvent;
    DATA_EVENT*                 pstDataEvent;
    SGUI_INT                    iProcessAction;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;
    iProcessAction =            HMI_DEMO_PROC_NO_ACT;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    switch(pstEvent->iID)
    {
        case EVENT_ID_KEY_PRESS:
        {
            pstKeyEvent = (KEY_PRESS_EVENT*)pstEvent;
            if(SGUI_FALSE == HMI_EVENT_SIZE_CHK(*pstKeyEvent, KEY_PRESS_EVENT))
            {
                // Event data is invalid.
                eProcessResult = HMI_RET_INVALID_DATA;
            }
            else
            {
                uiKeyCode = pstKeyEvent->Data.uiKeyValue;
                uiKeyValue = KEY_CODE_VALUE(uiKeyCode);

                if(KEY_VALUE_ESC == uiKeyValue)
                {
                    iProcessAction = HMI_DEMO_PROC_CANCEL;
                }
            }
            break;
        }
        case EVENT_ID_TIMER:
        {
            pstDataEvent = (DATA_EVENT*)pstEvent;
            if(SGUI_FALSE == HMI_EVENT_SIZE_CHK(*pstDataEvent, DATA_EVENT))
            {
                // Event data is invalid.
                eProcessResult = HMI_RET_INVALID_DATA;
            }
            else
            {
                iNewValue = pstDataEvent->Data.iValue;
                SGUI_RealtimeGraph_AppendValue(pstDeviceIF, &s_stRealtimeGraph, iNewValue);
                HMI_DemoRealGraph_RefreshScreen(pstDeviceIF, NULL);
            }
            break;
        }
        default:
        {
            /* No process. */
            break;
        }
    }

    if(NULL != piActionID)
    {
        *piActionID = iProcessAction;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoRealGraph_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(HMI_PROCESS_SUCCESSFUL(eProcResult))
    {
        if(HMI_DEMO_PROC_CANCEL == iActionID)
        {
            // Stop heart-beat timer.
            SysTickTimerEnable(false);
            // Go back to last screen.
            HMI_GoBack(NULL);
        }
    }

    return HMI_RET_NORMAL;
}
