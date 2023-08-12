#include "keyboard.h"
#include <stdio.h>

static KB_EVENT_CB_FP           KB_CB_FUN = NULL;

#define KB_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd
#define KB_GPIO_CLK             RCC_APB2Periph_GPIOB
#define KB_GPIO_PORT            GPIOB
#define KB_GPIO_PIN0            GPIO_Pin_9
#define KB_GPIO_PIN1            GPIO_Pin_8
#define KB_GPIO_PIN2            GPIO_Pin_7
#define KB_GPIO_PIN3            GPIO_Pin_6

typedef uint16_t                GPIO_Pin;

typedef struct
{
    GPIO_Pin                    uiPin;
    KEY_STATE                   eLastState;
}KEY;

typedef struct
{
    uint16_t                uiComPin;
    KEY                     pstKeys[3];
}KEY_IO;

KEY_IO s_arrstKeyIOGroups[4] =
{
    {KB_GPIO_PIN0, {{KB_GPIO_PIN1, KEY_STATE_UP}, {KB_GPIO_PIN2, KEY_STATE_UP}, {KB_GPIO_PIN3, KEY_STATE_UP}}},
    {KB_GPIO_PIN1, {{KB_GPIO_PIN0, KEY_STATE_UP}, {KB_GPIO_PIN2, KEY_STATE_UP}, {KB_GPIO_PIN3, KEY_STATE_UP}}},
    {KB_GPIO_PIN2, {{KB_GPIO_PIN0, KEY_STATE_UP}, {KB_GPIO_PIN1, KEY_STATE_UP}, {KB_GPIO_PIN3, KEY_STATE_UP}}},
    {KB_GPIO_PIN3, {{KB_GPIO_PIN0, KEY_STATE_UP}, {KB_GPIO_PIN1, KEY_STATE_UP}, {KB_GPIO_PIN2, KEY_STATE_UP}}}
};

#define KB_SwitchToInput(PIN)       KB_SwitchGPIOMode(PIN, GPIO_Mode_IPU)
#define KB_SwitchToOutput(PIN)      KB_SwitchGPIOMode(PIN, GPIO_Mode_Out_PP)
#define KB_PinHigh(PIN)             GPIO_SetBits(KB_GPIO_PORT, PIN);
#define KB_PinLow(PIN)              GPIO_ResetBits(KB_GPIO_PORT, PIN);

static void         KB_SwitchGPIOMode(uint16_t uiPin, GPIOMode_TypeDef eNewMode);
static KEY_STATE    KB_ReadPin(uint16_t uiPin);

/*****************************************************************************/
/** Function Name:  KB_InitializeGPIO.                                      **/
/** Purpose:        Config and initialize GPIO for key.                     **/
/** Params:         None.                                                   **/
/** Return:         None.                                                   **/
/*****************************************************************************/
void KB_Initialize(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // Enable RCC.
    KB_GPIO_APBxClock_FUN(KB_GPIO_CLK, ENABLE);
    // Loop for initialize each GPIO in array list.
    GPIO_InitStructure.GPIO_Pin = KB_GPIO_PIN0|KB_GPIO_PIN1|KB_GPIO_PIN2|KB_GPIO_PIN3;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(KB_GPIO_PORT, &GPIO_InitStructure);
}

/*****************************************************************************/
/** Function Name:  KB_SwitchGPIOMode.                                      **/
/** Purpose:        Switch GPIO working mode.                               **/
/** Params:         None.                                                   **/
/** Return:         None.                                                   **/
/*****************************************************************************/
void KB_SwitchGPIOMode(uint16_t uiPin, GPIOMode_TypeDef eNewMode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // Config GPIO.
    GPIO_InitStructure.GPIO_Pin =   uiPin;
    GPIO_InitStructure.GPIO_Mode =  eNewMode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(KB_GPIO_PORT, &GPIO_InitStructure);
}

KEY_STATE KB_ReadPin(uint16_t uiPin)
{
    return (KEY_STATE)GPIO_ReadInputDataBit(KB_GPIO_PORT, uiPin);
}

void KB_Scan(void)
{
    uint8_t     uiRowIdx;
    uint8_t     uiColumnIdx;
    uint8_t     uiKeyCode;
    KEY_STATE   eNowState;

    for(uiRowIdx=0; uiRowIdx<(sizeof(s_arrstKeyIOGroups)/sizeof(KEY_IO)); uiRowIdx++)
    {
        KB_SwitchToOutput(s_arrstKeyIOGroups[uiRowIdx].uiComPin);
        KB_PinLow(s_arrstKeyIOGroups[uiRowIdx].uiComPin);
        for(uiColumnIdx=0; uiColumnIdx<3; uiColumnIdx++)
        {
            eNowState = KB_ReadPin(s_arrstKeyIOGroups[uiRowIdx].pstKeys[uiColumnIdx].uiPin);
            uiKeyCode = ((uiRowIdx << 4) & 0xF0) | (uiColumnIdx & 0x0F);
            if(s_arrstKeyIOGroups[uiRowIdx].pstKeys[uiColumnIdx].eLastState^eNowState)
            {
                if(KEY_STATE_DOWN == eNowState)
                {

                    if(NULL != KB_CB_FUN)
                    {
                        KB_CB_FUN(uiKeyCode, KEY_EVENT_DOWN);
                    }
                }
                else if(KEY_STATE_UP == eNowState)
                {
                    if(NULL != KB_CB_FUN)
                    {
                        KB_CB_FUN(uiKeyCode, KEY_EVENT_UP);
                    }
                }
                /* Key state is changed. */
                s_arrstKeyIOGroups[uiRowIdx].pstKeys[uiColumnIdx].eLastState = eNowState;
            }

        }
        KB_SwitchToInput(s_arrstKeyIOGroups[uiRowIdx].uiComPin);
        KB_PinHigh(s_arrstKeyIOGroups[uiRowIdx].uiComPin);
    }
}

void KB_RegisterEventProc(KB_EVENT_CB_FP fpKBEventCallBackFunc)
{
    KB_CB_FUN = fpKBEventCallBackFunc;
}

KEY_STATE KB_CheckKeyState(uint8_t uiKeyCode)
{
    uint8_t     uiRowIdx;
    uint8_t     uiColumnIdx;
    KEY_STATE   eKeyState;

    uiRowIdx = (uiKeyCode>>4);
    uiColumnIdx = uiKeyCode & 0x0F;
    eKeyState = KB_ReadPin(s_arrstKeyIOGroups[uiRowIdx].pstKeys[uiColumnIdx].uiPin);

    return eKeyState;
}

