#ifndef __OLED_H
#define __OLED_H 

#include "main.h"

#define SSD1306_ADDR  (0x78)
#define SSD1306_DATA  (0x40)
#define SSD1306_CMD   (0x00)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define LCD_SIZE_WIDTH      (128)
#define LCD_SIZE_HEIGHT     (32)
#define LCD_SIZE_COLUMNS    (LCD_SIZE_WIDTH)
#define LCD_SIZE_PAGES      (LCD_SIZE_HEIGHT/8)

typedef enum
{
    SCREEN_COLOR_BKG = 0,
    SCREEN_COLOR_FRG = 1,
    SCREEN_COLOR_TRS = 2,
}SCREEN_COLOR;

//Common operation
#define SET_PAGE_BIT(PAGE, Bit)     ((PAGE) = (PAGE) | (0x01 << (Bit)))
#define CLR_PAGE_BIT(PAGE, Bit)     ((PAGE) = (PAGE) & (~(0x01 << (Bit))))
#define GET_PAGE_BIT(PAGE, Bit)     ((((PAGE) & (0x01 << (Bit)))>0)?1:0)

void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(uint8_t dat);
void OLED_WR_Byte(uint8_t dat,uint8_t mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_ShowChar6x8(uint8_t x,uint8_t y,uint8_t chr,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
void OLED_Init(void);

void SSD1306_SetBrightness(uint8_t brightness);
int SCREEN_GetPixel(int iPosX, int iPosY);
void SCREEN_SetPixel(int iPosX, int iPosY, int iColor);
void SCREEN_RefreshScreen(void);
void SCREEN_ClearDisplay(void);

#endif

