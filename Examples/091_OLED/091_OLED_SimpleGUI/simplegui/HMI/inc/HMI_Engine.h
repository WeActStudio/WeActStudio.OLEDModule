#ifndef __INCLUDE_HMI_ENGINE_H__
#define __INCLUDE_HMI_ENGINE_H__
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_Typedef.h"

//=======================================================================//
//= User Macro definition.                                              =//
//=======================================================================//
// GoBack history size
#define     HMI_SCREEN_HISTORY_MAX                          (20)
#define     HMI_EVENT_KEY_VALUE_LENGTH_MAX                  (4)
// Parameter post label.
#define     HMI_SCREEN_ID_ANY                               (-1)    // This label means parameter will posted to every screen.

#define     HMI_PROCESS_SUCCESSFUL(RESULT)                  (RESULT >= 0)
#define     HMI_PROCESS_FAILED(RESULT)                      (!(HMI_PROCESS_SUCCESSFUL(RESULT)))

// Start screen definition
#define     HMI_SCREEN_START                                (0)

#define     HMI_EVENT_TYPE_DECLARE(NAME, DATA)              typedef struct                                              \
                                                            {                                                           \
                                                                HMI_EVENT_BASE Head;                                    \
                                                                DATA Data;                                              \
                                                            }NAME;

#define     HMI_EVENT_DATA_MEMSET(EVENT)                    SGUI_SystemIF_MemorySet(&EVENT, 0x00, sizeof(EVENT))

#define     HMI_EVENT_INIT(EVENT)                           {                                                           \
                                                                HMI_EVENT_DATA_MEMSET(EVENT);                           \
                                                                EVENT.Head.iSize = sizeof(EVENT);                       \
                                                                EVENT.Head.iType = EVENT_TYPE_ANY;                      \
                                                            }

#define     HMI_EVENT_SIZE_CHK(EVENT, TYPE)                 (((EVENT).Head.iSize == sizeof(TYPE))?SGUI_TRUE:SGUI_FALSE)

//=======================================================================//
//= Data type definition.                                               =//
//=======================================================================//
// HMI process result
typedef enum
{
    // Abnormal.
    HMI_RET_ERROR =                 -3,
    HMI_RET_INVALID_DATA =          -2,
    HMI_RET_ERROR_STATE =           -1,
    // Normal.
    HMI_RET_ABNORMAL =              0,
    HMI_RET_NORMAL =                1,
}HMI_ENGINE_RESULT;

typedef enum
{
    HMI_ENGINE_SCR_SWITCH =         0,// Switch screen and record to history.
    HMI_ENGINE_SCR_POPUP,           // Show up screen only.
}HMI_SCREEN_DISP_TYPE;

typedef struct
{
    SGUI_INT            iType;
    SGUI_INT            iID;
    SGUI_INT            iSize;
}HMI_EVENT_BASE;

typedef struct
{
    HMI_EVENT_BASE      Head;
}HMI_GENERAL_EVENT;

// Screen action interface function pointer structure.
typedef struct
{
    // Initialize screen data and parameter.
    HMI_ENGINE_RESULT               (*Initialize)   (SGUI_SCR_DEV* Interface);
    // Do some thing before current screen display.
    HMI_ENGINE_RESULT               (*Prepare)      (SGUI_SCR_DEV* Interface, const void* pstParameters);
    // Repaint screen if needed.
    HMI_ENGINE_RESULT               (*Repaint)      (SGUI_SCR_DEV* Interface, const void* pstParameters);
    // Process event.
    HMI_ENGINE_RESULT               (*ProcessEvent) (SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
    // Post process.
    HMI_ENGINE_RESULT               (*PostProcess)  (SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);
}HMI_SCREEN_ACTION;
// Screen data structure.
typedef struct _T_HMI_SCREEN_OBJECT_
{
    SGUI_INT                        iScreenID;
    HMI_SCREEN_ACTION*              pstActions;
    struct _T_HMI_SCREEN_OBJECT_*   pstPrevious;
}HMI_SCREEN_OBJECT;

typedef struct
{
    HMI_SCREEN_OBJECT**             ScreenObjPtr;
    SGUI_INT                        ScreenCount;
    HMI_SCREEN_OBJECT*              CurrentScreenObject;
    SGUI_SCR_DEV*                   Interface;
}HMI_ENGINE_OBJECT;

//=======================================================================//
//= Public function declaration.                                        =//
//=======================================================================//
HMI_ENGINE_RESULT   HMI_ActiveEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID);
HMI_ENGINE_RESULT   HMI_StartEngine(const void* pstParameters);
HMI_ENGINE_RESULT   HMI_ProcessEvent(const HMI_EVENT_BASE* pstEvent);
HMI_ENGINE_RESULT   HMI_SwitchScreen(SGUI_INT iDestScreenID, const void* pstParameters);
HMI_ENGINE_RESULT   HMI_GoBack(const void* pstParameters);
HMI_ENGINE_RESULT   HMI_SetDeviceObject(SGUI_SCR_DEV* pstDeviceObj);
#endif // __INCLUDE_HMI_ENGINE_H__
