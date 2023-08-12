#ifndef _INCLUDE_SSD1306_SPI_H_
#define _INCLUDE_SSD1306_SPI_H_
#include <stm32f10x_gpio.h>

#define SSD1306_Clear()         SSD1306_Fill(0x00)

void    SSD1306_WriteData(uint8_t uiData);
void    SSD1306_SetPosition(uint8_t uiColumn, uint8_t uiPage);
void    SSD1306_Fill(uint8_t uiByte);
void    SSD1306_Initialize(void);

#endif
