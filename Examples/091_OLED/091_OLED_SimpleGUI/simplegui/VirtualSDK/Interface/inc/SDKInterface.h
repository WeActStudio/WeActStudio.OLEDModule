#ifndef __INCLUDE_SDK_INTERFACE__
#define __INCLUDE_SDK_INTERFACE__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef enum
{
    ENV_FLAG_IDX_SDK_INIT		= 0,
    ENV_FLAG_IDX_SDK_SEC_EN,
    ENV_FLAG_IDX_SDK_SEC_EVENT,
    ENV_FLAG_IDX_SDK_TIM_SET,
    ENV_FLAG_IDX_SDK_TIM_EVENT,
    ENV_FLAG_IDX_SDK_KEY_EVENT,
    ENV_FLAG_IDX_MAX,
} ENV_FLAG_INDEX;

typedef struct
{
    uint32_t    iKeyCode;
    bool        bShift;
    bool        bCtrl;
    bool        bAlt;
}SDK_KB_EVENT;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void			    SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_INDEX eIndex, bool bValue);
void			    SGUI_SDK_SyncKeyEventData(const SDK_KB_EVENT* pcstKBEvent);
const SDK_KB_EVENT* SGUI_SDK_GetKeyEventData(void);
bool			    SGUI_SDK_GetEventSyncFlag(ENV_FLAG_INDEX eIndex);
bool			    SGUI_SDK_Initialize(void);
void			    SGUI_SDK_SetPixel(int uiPosX, int uiY, int uiPixelValue);
int				    SGUI_SDK_GetPixel(int uiPosX, int uiY);
void			    SGUI_SDK_RefreshDisplay(void);
void			    SGUI_SDK_ClearDisplay(void);
bool			    SGUI_SDK_ConfigGeneralTimer(unsigned int uiIntervalMs);
bool			    SGUI_SDK_EnableSecondInterrupt(bool bEnabled);
int				    SGUI_SDK_DummyMainProc(void);
void			    SGUI_SDK_GeneralTimerHandler(void);
void			    SGUI_SDK_SecondTimerHandler(void);
void			    SGUI_SDK_KeyboardHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // __INCLUDE_USER_ACTION_INTERFACE__
