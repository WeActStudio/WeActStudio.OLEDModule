#ifndef _INCLUDE_UC1604C_SOFT_SPI_H_
#define _INCLUDE_UC1604C_SOFT_SPI_H_
//////////////////////////////////////////////////////////////////////////////////
// include
//////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

#define COLUMN_MAX      192
#define PAGE_MAX        8

void    UC1604C_Initialize(void);
void    UC1604C_SetPosition(uint8_t uiColumn, uint8_t uiPage);
void    UC1604C_WriteData(uint8_t uiData);
void    UC1604C_Fill(uint8_t bmp_dat);
#endif //_INCLUDE_UC1604C_SOFT_SPI_H_
