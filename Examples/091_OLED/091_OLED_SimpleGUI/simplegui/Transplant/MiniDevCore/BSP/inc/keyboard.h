#ifndef _INCLUDE_BSP_KEY_BOARD_H_
#define _INCLUDE_BSP_KEY_BOARD_H_

#include <stm32f10x_gpio.h>
#include <stddef.h>
#include <stdbool.h>

#define KEY_STATE_DOWN          Bit_RESET
#define KEY_STATE_UP            Bit_SET
#define KEY_STATE               BitAction

#define KEY_CODE_F1             (0x30)
#define KEY_CODE_F2             (0x32)

typedef enum
{
    KEY_EVENT_DOWN,
    KEY_EVENT_UP
}KEY_EVENT;

typedef void(*KB_EVENT_CB_FP)(uint16_t uiKeyCode, KEY_EVENT eEvent);

void        KB_Initialize(void);
void        KB_Scan(void);
void        KB_RegisterEventProc(KB_EVENT_CB_FP fpKBEventCallBackFunc);
KEY_STATE   KB_CheckKeyState(uint8_t uiKeyCode);

#endif



