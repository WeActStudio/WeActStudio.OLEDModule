#ifndef __OLED_H
#define __OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define OLED_ADDR (0x78) // 0x78/0x7A
#define OLED_CMD  (0x00) // 写命令
#define OLED_DATA (0x40) // 写数据

#define OLED_SIZE_WIDTH (128)
#define OLED_SIZE_HEIGHT (64)
#define OLED_SIZE_COLUMNS (OLED_SIZE_WIDTH)
#define OLED_SIZE_PAGES (OLED_SIZE_HEIGHT / 8)

void I2C_Error_Check(HAL_StatusTypeDef status);
void OLED_WR_Byte(uint8_t dat, uint8_t mode);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_SetBrightness(uint8_t brightness);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode);
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode);
void OLED_ShowChar6x8(uint8_t x, uint8_t y, uint8_t chr, uint8_t mode);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1, uint8_t mode);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1, uint8_t mode);
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode);
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode);
void OLED_ShowPicture(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode);
void OLED_Init(void);

#if SIMPLE_GUI
typedef enum
{
    SCREEN_COLOR_BKG = 0,
    SCREEN_COLOR_FRG = 1,
    SCREEN_COLOR_TRS = 2,
} SCREEN_COLOR;

//Common operation
#define SET_PAGE_BIT(PAGE, Bit) ((PAGE) = (PAGE) | (0x01 << (Bit)))
#define CLR_PAGE_BIT(PAGE, Bit) ((PAGE) = (PAGE) & (~(0x01 << (Bit))))
#define GET_PAGE_BIT(PAGE, Bit) ((((PAGE) & (0x01 << (Bit))) > 0) ? 1 : 0)

int SCREEN_GetPixel(int iPosX, int iPosY);
void SCREEN_SetPixel(int iPosX, int iPosY, int iColor);
void SCREEN_RefreshScreen(void);
void SCREEN_ClearDisplay(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
