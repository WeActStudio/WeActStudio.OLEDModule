/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: Curve.c                                                   **/
/** Author: Polarix                                                     **/
/** Version: 1.0.0.0                                                    **/
/** Description: HMI demo for graph interface.                          **/
/*************************************************************************/
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "DemoProc.h"
#include "Resource.h"
#include "SGUI_Basic.h"
#include "SGUI_Curve.h"
#include "SGUI_Text.h"
#include "SGUI_FontResource.h"

//=======================================================================//
//= User Macro definition.                                              =//
//=======================================================================//
#define                     CURVE_POINT_MUN_MAX                 (20)

//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//
static HMI_ENGINE_RESULT    HMI_DemoCurve_Initialize(SGUI_SCR_DEV* pstDeviceIF);
static HMI_ENGINE_RESULT    HMI_DemoCurve_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoCurve_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoCurve_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT    HMI_DemoCurve_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);
static SGUI_CURVE_POINT*    HMI_DemoCurve_NewPoint(void);
static void                 HMI_DemoBasic_ReleasePoint(SGUI_CURVE_POINT* pcstPoint);
static void                 HMI_DemoCurve_RepaintText(SGUI_SCR_DEV* pstDeviceIF);

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
static SGUI_CURVE_STRUCT        s_stCurve =                 {0x00};
static SGUI_BOOL                s_arrbPointDataUsed[CURVE_POINT_MUN_MAX] = {0x00};
static SGUI_CURVE_POINT         s_arrstPointArray[CURVE_POINT_MUN_MAX] = {0x00};
HMI_SCREEN_ACTION               s_stDemoCurveActions =      {
                                                                HMI_DemoCurve_Initialize,
                                                                HMI_DemoCurve_Prepare,
                                                                HMI_DemoCurve_RefreshScreen,
                                                                HMI_DemoCurve_ProcessEvent,
                                                                HMI_DemoCurve_PostProcess,
                                                            };

//=======================================================================//
//= Global variable declaration.                                        =//
//=======================================================================//
HMI_SCREEN_OBJECT               g_stHMIDemo_Curve = {   HMI_SCREEN_ID_DEMO_CURVE,
                                                                &s_stDemoCurveActions
                                                            };

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
SGUI_CURVE_POINT* HMI_DemoCurve_NewPoint(void)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                iIndex;
    SGUI_CURVE_POINT*       pstNewPoint;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    pstNewPoint =           NULL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    for(iIndex=0; iIndex<CURVE_POINT_MUN_MAX; iIndex++)
    {
        if(SGUI_FALSE == s_arrbPointDataUsed[iIndex])
        {
            pstNewPoint = s_arrstPointArray + iIndex;
            s_arrbPointDataUsed[iIndex] = SGUI_TRUE;
            break;
        }
    }
    return pstNewPoint;
}

void HMI_DemoBasic_ReleasePoint(SGUI_CURVE_POINT* pcstPoint)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                iIndex;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    for(iIndex=0; iIndex<CURVE_POINT_MUN_MAX; iIndex++)
    {
        if(s_arrstPointArray+iIndex == pcstPoint)
        {
            SGUI_SystemIF_MemorySet(pcstPoint, 0x00, sizeof(SGUI_CURVE_POINT));
            s_arrbPointDataUsed[iIndex] = SGUI_FALSE;
        }
    }
}

void HMI_DemoCurve_RepaintText(SGUI_SCR_DEV* pstDeviceIF)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_RECT               stTextArea;
    SGUI_POINT              stInnsrPos;
    SGUI_CHAR               szNumberTextBuffer[12];
    SGUI_SIZE               sTextLength;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    SGUI_SystemIF_MemorySet(&stInnsrPos, 0x00, sizeof(SGUI_POINT));

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    stTextArea.iX = 1;
    stTextArea.iY = 1;
    stTextArea.iWidth = 48;
    stTextArea.iHeight = 8;
    SGUI_Text_DrawText(pstDeviceIF, "Focused ", &SGUI_DEFAULT_FONT_8, &stTextArea, &stInnsrPos, SGUI_DRAW_NORMAL);
    if(NULL != s_stCurve.stData.pstFocused)
    {
        //SGUI_Common_IntegerToString(s_stRealtimeGraph.Data->ValueArray[s_stRealtimeGraph.Data->ValueCount-1], szTextBuffer, 10, 4, ' ');

        stTextArea.iY =  pstDeviceIF->stSize.iHeight-9;
        stTextArea.iWidth = 48;
        sTextLength = SGUI_Common_IntegerToString(s_stCurve.stData.pstFocused->stPoint.iX, szNumberTextBuffer, 10, -1, ' ');
        SGUI_Text_DrawText(pstDeviceIF, szNumberTextBuffer, &SGUI_DEFAULT_FONT_8, &stTextArea, &stInnsrPos, SGUI_DRAW_NORMAL);
        stTextArea.iX+=(SGUI_INT)(sTextLength*SGUI_DEFAULT_FONT_8.iHalfWidth);
        SGUI_Text_DrawText(pstDeviceIF, ",", &SGUI_DEFAULT_FONT_8, &stTextArea, &stInnsrPos, SGUI_DRAW_NORMAL);
        stTextArea.iX+=SGUI_DEFAULT_FONT_8.iHalfWidth;
        sTextLength = SGUI_Common_IntegerToString(s_stCurve.stData.pstFocused->stPoint.iY, szNumberTextBuffer, 10, -1, ' ');
        SGUI_Text_DrawText(pstDeviceIF, szNumberTextBuffer, &SGUI_DEFAULT_FONT_8, &stTextArea, &stInnsrPos, SGUI_DRAW_NORMAL);
    }
}

HMI_ENGINE_RESULT HMI_DemoCurve_Initialize(SGUI_SCR_DEV* pstDeviceIF)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_CURVE_PARAM        stCurveInitParam;
    SGUI_CURVE_POINT*       pstNewPoint;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    pstNewPoint =           NULL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    stCurveInitParam.stLayout.iX = 1;
    stCurveInitParam.stLayout.iY = 10;
    stCurveInitParam.stLayout.iWidth = pstDeviceIF->stSize.iWidth-2;
    stCurveInitParam.stLayout.iHeight = pstDeviceIF->stSize.iHeight-20;

    stCurveInitParam.stXRange.iMin = -100;
    stCurveInitParam.stXRange.iMax = 100;
    stCurveInitParam.stYRange.iMin = -100;
    stCurveInitParam.stYRange.iMax = 100;

    SGUI_Curve_Initialize(&s_stCurve, &stCurveInitParam);

    pstNewPoint = HMI_DemoCurve_NewPoint();
    SGUI_Curve_InitializePoint(pstNewPoint, -100, -100);
    SGUI_Curve_AddPoint(&s_stCurve, pstNewPoint);

    pstNewPoint = HMI_DemoCurve_NewPoint();
    SGUI_Curve_InitializePoint(pstNewPoint, 0, 0);
    SGUI_Curve_AddPoint(&s_stCurve, pstNewPoint);

    pstNewPoint = HMI_DemoCurve_NewPoint();
    SGUI_Curve_InitializePoint(pstNewPoint, 100, 75);
    SGUI_Curve_AddPoint(&s_stCurve, pstNewPoint);

    pstNewPoint = SGUI_Curve_FocusPoint(&s_stCurve, 1);

    pstNewPoint = HMI_DemoCurve_NewPoint();
    SGUI_Curve_InitializePoint(pstNewPoint, -75, -100);
    SGUI_Curve_InsertBeforePoint(&s_stCurve, pstNewPoint);

    pstNewPoint = HMI_DemoCurve_NewPoint();
    SGUI_Curve_InitializePoint(pstNewPoint, 50, 25);
    SGUI_Curve_InsertAfterPoint(&s_stCurve, pstNewPoint);

    SGUI_Curve_FocusPoint(&s_stCurve, 0);

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoCurve_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    HMI_DemoCurve_RefreshScreen(pstDeviceIF, pstParameters);
    SGUI_Curve_FocusPoint(&s_stCurve, 0);

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoCurve_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SGUI_Basic_DrawRectangle(pstDeviceIF, 0, 0, pstDeviceIF->stSize.iWidth, pstDeviceIF->stSize.iHeight, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
    SGUI_Basic_DrawLine(pstDeviceIF, 1, 9, pstDeviceIF->stSize.iWidth-2, 9, SGUI_COLOR_FRGCLR);
    SGUI_Basic_DrawLine(pstDeviceIF, 1, pstDeviceIF->stSize.iHeight-10, pstDeviceIF->stSize.iWidth-2, pstDeviceIF->stSize.iHeight-10, SGUI_COLOR_FRGCLR);
    HMI_DemoCurve_RepaintText(pstDeviceIF);
    SGUI_Curve_Repaint(pstDeviceIF, &s_stCurve);
    SGUI_Curve_HighlightFocus(pstDeviceIF, &s_stCurve);

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoCurve_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT       eProcessResult;
    KEY_PRESS_EVENT*        pstKeyEvent;
    SGUI_INT                iProcessAction;
    SGUI_CURVE_POINT*       pstFocusedPoint;
    SGUI_CURVE_POINT*       pstPointObj;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;
    iProcessAction =            HMI_DEMO_PROC_NO_ACT;
    pstFocusedPoint =           s_stCurve.stData.pstFocused;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(EVENT_ID_KEY_PRESS == pstEvent->iID)
    {
        pstKeyEvent = (KEY_PRESS_EVENT*)pstEvent;

        switch(KEY_CODE_VALUE(pstKeyEvent->Data.uiKeyValue))
        {
            case KEY_VALUE_ESC:
            {
                iProcessAction = HMI_DEMO_PROC_CANCEL;
                break;
            }
            case KEY_VALUE_TAB:
            {
                if(NULL == s_stCurve.stData.pstFocused)
                {
                    s_stCurve.stData.pstFocused = s_stCurve.stData.stPoints.pstHead;
                }
                else
                {
                    if(SGUI_TRUE == SGUI_Curve_PointIsHighlight(pstDeviceIF, SGUI_Curve_FocusedPoint(&s_stCurve)))
                    {
                        SGUI_Curve_HighlightFocus(pstDeviceIF, &s_stCurve);
                    }
                    s_stCurve.stData.pstFocused = s_stCurve.stData.pstFocused->pstNext;
                }
                pstFocusedPoint = s_stCurve.stData.pstFocused;
                HMI_DemoCurve_RefreshScreen(pstDeviceIF, NULL);
                break;
            }
            case KEY_VALUE_UP:
            {
                if(NULL != pstFocusedPoint)
                {
                    SGUI_Curve_UpdateFocusPoint(&s_stCurve, pstFocusedPoint->stPoint.iX, pstFocusedPoint->stPoint.iY+1);
                    HMI_DemoCurve_RefreshScreen(pstDeviceIF, NULL);
                }
                break;
            }
            case KEY_VALUE_DOWN:
            {
                if(NULL != pstFocusedPoint)
                {
                    SGUI_Curve_UpdateFocusPoint(&s_stCurve, pstFocusedPoint->stPoint.iX, pstFocusedPoint->stPoint.iY-1);
                    HMI_DemoCurve_RefreshScreen(pstDeviceIF, NULL);
                }
                break;
            }
            case KEY_VALUE_LEFT:
            {
                if(NULL != pstFocusedPoint)
                {
                    SGUI_Curve_UpdateFocusPoint(&s_stCurve, pstFocusedPoint->stPoint.iX-1, pstFocusedPoint->stPoint.iY);
                    HMI_DemoCurve_RefreshScreen(pstDeviceIF, NULL);
                }
                break;
            }
            case KEY_VALUE_RIGHT:
            {
                if(NULL != pstFocusedPoint)
                {
                    SGUI_Curve_UpdateFocusPoint(&s_stCurve, pstFocusedPoint->stPoint.iX+1, pstFocusedPoint->stPoint.iY);
                    HMI_DemoCurve_RefreshScreen(pstDeviceIF, NULL);
                }
                break;
            }
            case KEY_VALUE_INSERT:
            {
                pstPointObj = HMI_DemoCurve_NewPoint();
                if(NULL != pstPointObj)
                {
                    SGUI_Curve_InitializePoint(pstPointObj, 0, s_stCurve.stParam.stYRange.iMin);
                    if(KEY_OPTION_SHIFT == KEY_CODE_OPT(pstKeyEvent->Data.uiKeyValue))
                    {
                        SGUI_Curve_InsertBeforePoint(&s_stCurve, pstPointObj);
                    }
                    else
                    {
                        SGUI_Curve_InsertAfterPoint(&s_stCurve, pstPointObj);
                    }
                    HMI_DemoCurve_RefreshScreen(pstDeviceIF, NULL);
                }
                break;
            }
            case KEY_VALUE_DEL:
            {
                pstPointObj = SGUI_Curve_FocusedPoint(&s_stCurve);
                if(NULL != pstPointObj)
                {
                    if(SGUI_TRUE == SGUI_Curve_RemovePoint(&s_stCurve, pstPointObj))
                    {
                        HMI_DemoBasic_ReleasePoint(pstPointObj);
                    }
                    HMI_DemoCurve_RefreshScreen(pstDeviceIF, NULL);
                }

                break;
            }
            default:
            {
                // do nothing.
                break;
            }
        }

    }

    if(NULL != piActionID)
    {
        *piActionID = iProcessAction;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoCurve_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult))
    {
        if(HMI_DEMO_PROC_CANCEL == iActionID)
        {
            HMI_GoBack(NULL);
        }
    }

    return HMI_RET_NORMAL;
}

