#ifndef _INCLUDE_BSP_SPI_FLASH_H_
#define _INCLUDE_BSP_SPI_FLASH_H_

#include <stm32f10x_spi.h>
#include <stm32f10x_gpio.h>

#define W25X_WriteEnable        (0x06)
#define W25X_WriteDisable       (0x04)
#define W25X_ReadStatusReg      (0x05)
#define W25X_WriteStatusReg     (0x01)
#define W25X_ReadData           (0x03)
#define W25X_FastReadData       (0x0B)
#define W25X_FastReadDual       (0x3B)
#define W25X_PageProgram        (0x02)
#define W25X_BlockErase         (0xD8)
#define W25X_SectorErase        (0x20)
#define W25X_ChipErase          (0xC7)
#define W25X_PowerDown          (0xB9)
#define W25X_ReleasePowerDown   (0xAB)
#define W25X_DeviceID           (0xAB)
#define W25X_ManufactDeviceID   (0x90)
#define W25X_JedecDeviceID      (0x9F)


void        SPI_FLASH_Initialize(void);
uint8_t     SPI_FLASH_TransferByte(uint8_t uiByte);
uint32_t    SPI_FLASH_ReadID(void);
void        SPI_FLASH_Erase_Sector(uint32_t uiAddr);
void        SPI_FLASH_WriteEnable(void);
void        SPI_FLASH_ReadData(uint32_t uiAddr,uint8_t *readBuff,uint32_t numByteToRead);
void        SPI_FLASH_WriteData(uint32_t uiAddr,uint8_t *writeBuff,uint32_t numByteToWrite);
void        SPI_FLASH_WaitBusy(void);
void        SPI_FLASH_EraseChip(void);
#endif
