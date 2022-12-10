/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: HMI_Process.c                                             **/
/** Author: Polarix                                                     **/
/** Version: 1.0.0.0                                                    **/
/** Description: This file provides ActionMap interface function.       **/
/*************************************************************************/

//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "HMI_Engine.h"

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
static HMI_ENGINE_OBJECT*           g_pstActivedEngineObject;

//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//
static HMI_SCREEN_OBJECT* HMI_GetScreenObjectInEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID);

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
/*****************************************************************************/
/** Function Name:  HMI_ActiveEngine                                        **/
/** Purpose:        Set a HMI engine object to the activated state.         **/
/** Params:                                                                 **/
/** @pstHMIEngineObject[in]: The pointer of the engine object will active.  **/
/** @iScreenID[in]:     Specif a screen ID, this screen will be show first  **/
/**                     when engine start, for HMI_SCREEN_ID_ANY to keep    **/
/**                     last screen when this engine activated, and if the  **/
/**                     screen not add into the engine object, the error    **/
/**                     HMI_RET_INVALID_DATA will be returned.              **/
/** Return:         HMI_ENGINE_RESULT                                       **/
/** @HMI_RET_NORMAL:    Engine activated.                                   **/
/** @HMI_RET_ERROR_STATE: Current engine object is in processing.           **/
/** @HMI_RET_INVALID_DATA: Specified screen ID is invalid or not add to     **/
/**                     this engine.                                        **/
/** Notice:         The process event will post to the activated engine.    **/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_ActiveEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstScreenObject;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;
    pstScreenObject =           NULL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pstHMIEngineObject)
    {
        if(HMI_SCREEN_ID_ANY != iScreenID)
        {
            pstScreenObject = HMI_GetScreenObjectInEngine(pstHMIEngineObject, iScreenID);
        }
        else
        {
            pstScreenObject = pstHMIEngineObject->ScreenObjPtr[0];
        }
        if(NULL != pstScreenObject)
        {
            pstHMIEngineObject->CurrentScreenObject = pstScreenObject;
        }
        else
        {
            eProcessResult = HMI_RET_INVALID_DATA;
        }
    }
    else
    {
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    if(HMI_RET_NORMAL == eProcessResult)
    {
        g_pstActivedEngineObject = pstHMIEngineObject;
    }
    return eProcessResult;
}

/*****************************************************************************/
/** Function Name:  HMI_StartEngine                                         **/
/** Purpose:        Start HMI engine and show screen display.               **/
/** Params:         None.                                                   **/
/** Return:         HMI_ENGINE_RESULT                                       **/
/** @ HMI_RET_NORMAL:    Engine activated.                                  **/
/** @ HMI_RET_ERROR_STATE: Current engine object is in processing.          **/
/** @ HMI_RET_INVALID_DATA: Specified screen ID is invalid or not add to    **/
/**                     this engine.                                        **/
/** Notice:         The process event will post to the activated engine.    **/
/** Notice:         Screen will only refresh when pstPreProcessData is NULL **/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_StartEngine(const void* pstParameters)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstStartScreen;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;
    pstStartScreen =            NULL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != g_pstActivedEngineObject)
    {
        pstStartScreen = g_pstActivedEngineObject->CurrentScreenObject;
        if(NULL != pstStartScreen)
        {
            if(NULL != pstStartScreen->pstActions)
            {
                if(NULL != pstStartScreen->pstActions->Prepare)
                {
                    eProcessResult = pstStartScreen->pstActions->Prepare(g_pstActivedEngineObject->Interface, pstParameters);
                    g_pstActivedEngineObject->Interface->fnSyncBuffer();
                }
            }
        }
    }
    else
    {
        // No activated HMI engine object.
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    return eProcessResult;
}

/*****************************************************************************/
/** Function Name:  HMI_ProcessEvent                                        **/
/** Purpose:        Receive posted event and post to screen object.         **/
/** Params:                                                                 **/
/** @ pstEvent[in]:     Event data, include action type and parameter data. **/
/** Return:         HMI_ENGINE_RESULT.                                      **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_ProcessEvent(const HMI_EVENT_BASE* pstEvent)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstCurrentScreen;
    SGUI_INT                    iActionID;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;
    pstCurrentScreen =          NULL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != g_pstActivedEngineObject)
    {
        // Get current displayed screen object.
        pstCurrentScreen = g_pstActivedEngineObject->CurrentScreenObject;
        if( (NULL == pstCurrentScreen) ||
            (NULL == pstCurrentScreen->pstActions) ||
            (NULL == pstCurrentScreen->pstActions->ProcessEvent))
        {
            // Screen ID is invalid or screen object is invalid.
            eProcessResult = HMI_RET_INVALID_DATA;

        }
        else
        {
            // Process event.
            eProcessResult = pstCurrentScreen->pstActions->ProcessEvent(g_pstActivedEngineObject->Interface, pstEvent, &iActionID);
        }

        // Run post process.
        eProcessResult = pstCurrentScreen->pstActions->PostProcess(g_pstActivedEngineObject->Interface, eProcessResult, iActionID);
        /*
        if(HMI_PROCESS_FAILED(eProcResult))
        {
            // Post process return failed.
        }
        */
        if( (NULL != g_pstActivedEngineObject->Interface) &&
            (NULL != g_pstActivedEngineObject->Interface->fnSyncBuffer))
        {
            g_pstActivedEngineObject->Interface->fnSyncBuffer();
        }
    }
    else
    {
        // No activated HMI engine object.
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    return eProcessResult;
}

/*****************************************************************************/
/** Function Name:  HMI_SwitchScreen                                        **/
/** Purpose:        Turn to a screen with screen index.                     **/
/** Params:                                                                 **/
/** @ iDestScreenID[in]: Screen ID witch will be going to.                  **/
/** @ pstParameters[in]: Screen prepare data pointer.                       **/
/** Return:         HMI_ENGINE_RESULT.                                      **/
/** Notice:         Screen will only refresh when pstPreProcessData is NULL **/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_SwitchScreen(SGUI_INT iDestScreenID, const void* pstParameters)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstDestScreen;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != g_pstActivedEngineObject)
    {
        pstDestScreen = HMI_GetScreenObjectInEngine(g_pstActivedEngineObject, iDestScreenID);
        if(NULL == pstDestScreen)
        {
            /* Not find screen object by ID. */
            eProcessResult = HMI_RET_INVALID_DATA;
        }
        else if(NULL != pstDestScreen->pstPrevious)
        {
            /* Cannot reenter to a screen object. */
            eProcessResult = HMI_RET_ERROR;
        }
        else
        {
            pstDestScreen->pstPrevious = g_pstActivedEngineObject->CurrentScreenObject;
            g_pstActivedEngineObject->CurrentScreenObject = pstDestScreen;
            if(NULL != pstDestScreen->pstActions)
            {
                if(NULL != pstDestScreen->pstActions->Prepare)
                {
                    eProcessResult = pstDestScreen->pstActions->Prepare(g_pstActivedEngineObject->Interface, pstParameters);
                }
            }
        }

    }
    else
    {
        // No activated HMI engine object.
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    return eProcessResult;
}

/*****************************************************************************/
/** Function Name:  HMI_GoBack.                                             **/
/** Purpose:        Go back to previous screen in actions history.          **/
/** Resources:      Screen data structure and action process function.      **/
/** Params:                                                                 **/
/** @pstPreProcessData[in]: Update screen data.                             **/
/** Return:         HMI_ENGINE_RESULT.                                      **/
/** Notice:         Screen will only refresh when pstPreProcessData is NULL **/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_GoBack(const void* pstParameters)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstPreviousScreen;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != g_pstActivedEngineObject)
    {
        pstPreviousScreen = g_pstActivedEngineObject->CurrentScreenObject->pstPrevious;
        g_pstActivedEngineObject->CurrentScreenObject->pstPrevious = NULL;

        if(NULL != pstPreviousScreen)
        {
            g_pstActivedEngineObject->CurrentScreenObject = pstPreviousScreen;
            if(NULL != pstPreviousScreen->pstActions)
            {
                if(NULL != pstPreviousScreen->pstActions->Prepare)
                {
                    eProcessResult = pstPreviousScreen->pstActions->Prepare(g_pstActivedEngineObject->Interface, pstParameters);
                }
            }
        }
        else
        {
            eProcessResult = HMI_RET_INVALID_DATA;
        }
    }
    else
    {
        // No activated HMI engine object.
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    return eProcessResult;
}

/*****************************************************************************/
/** Function Name:  HMI_GetScreenObjectInEngine.                            **/
/** Purpose:        Get a screen object pointer form a existed HMI engine   **/
/**                 object, return NULL if screen not existed.              **/
/** Params:                                                                 **/
/** @pstHMIEngineObject[in]: The pointer of the engine object.              **/
/** @iScreenID[in]:     Screen ID of screen object will be searched in HMI  **/
/**                     engine object.                                      **/
/** Return:         HMI_SCREEN_OBJECT pointer.                              **/
/** @NULL:              Screen object not existed or not add in this HMI    **/
/**                     engine.                                             **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
HMI_SCREEN_OBJECT* HMI_GetScreenObjectInEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_SCREEN_OBJECT*          pstScreenObject;
    SGUI_INT                    iIndex;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    pstScreenObject =           NULL;
    iIndex =                    0;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pstHMIEngineObject)
    {
        while(iIndex < pstHMIEngineObject->ScreenCount)
        {
            if(iScreenID == pstHMIEngineObject->ScreenObjPtr[iIndex]->iScreenID)
            {
                pstScreenObject = pstHMIEngineObject->ScreenObjPtr[iIndex];
                break;
            }
            else
            {
                iIndex++;
            }
        }
    }

    return pstScreenObject;
}

/*****************************************************************************/
/** Function Name:  HMI_SetDeviceObject.                                    **/
/** Purpose:        Set a new screen device interface object pointer to     **/
/**                 actived HMI object.                                     **/
/** Params:                                                                 **/
/** @ pstDeviceObj[in]: New device driver interface object pointer.         **/
/** Return:         HMI_ENGINE_RESULT                                       **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_SetDeviceObject(SGUI_SCR_DEV* pstDeviceObj)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((NULL == pstDeviceObj) || (NULL == g_pstActivedEngineObject))
    {
        eProcessResult = HMI_RET_INVALID_DATA;
    }
    else
    {
        g_pstActivedEngineObject->Interface = pstDeviceObj;
    }

    return eProcessResult;
}
