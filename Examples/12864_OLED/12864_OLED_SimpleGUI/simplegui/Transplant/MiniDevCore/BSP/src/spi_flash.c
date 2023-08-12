#include <spi_flash.h>
#include <stdio.h>

#define SPI_FLASHx                      SPI1
#define SPI_FLASH_APBxClock_FUN         RCC_APB2PeriphClockCmd
#define SPI_FLASH_CLK                   RCC_APB2Periph_SPI1
//GPIO
#define SPI_FLASH_GPIO_APBxClock_FUN    RCC_APB2PeriphClockCmd
#define SPI_FLASH_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SPI_FLASH_GPIO_PORT             GPIOA
//CS(NSS)
#define SPI_FLASH_CS_PIN                GPIO_Pin_4
//SCK
#define SPI_FLASH_SCK_PIN               GPIO_Pin_5
//MISO
#define SPI_FLASH_MISO_PIN              GPIO_Pin_6
//MOSI
#define SPI_FLASH_MOSI_PIN              GPIO_Pin_7

#define SPI_FLASH_CS_LOW()              GPIO_ResetBits(SPI_FLASH_GPIO_PORT, SPI_FLASH_CS_PIN)
#define SPI_FLASH_CS_HIGH()             GPIO_SetBits(SPI_FLASH_GPIO_PORT, SPI_FLASH_CS_PIN)

#define SPI_FLASH_DUMMY_BYTE            (0xFF)
#define SPIT_FLAG_TIMEOUT               ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT               ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;
static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

#define FLASH_INFO(fmt,arg...)          printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)         printf("<<-FLASH-ERROR->> "fmt"\n",##arg)


void SPI_FLASH_Initialize(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    SPI_FLASH_APBxClock_FUN(SPI_FLASH_CLK, ENABLE);
    SPI_FLASH_GPIO_APBxClock_FUN(SPI_FLASH_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SPI_FLASH_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_FLASH_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI_FLASH_SCK_PIN|SPI_FLASH_MISO_PIN|SPI_FLASH_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_FLASH_GPIO_PORT, &GPIO_InitStructure);

    SPI_FLASH_CS_HIGH();

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI_FLASHx, &SPI_InitStructure);

    SPI_Cmd(SPI_FLASHx, ENABLE);
}

uint8_t SPI_FLASH_TransferByte(uint8_t uiByte)
{
    SPITimeout = SPIT_FLAG_TIMEOUT;

    while (SPI_I2S_GetFlagStatus(SPI_FLASHx, SPI_I2S_FLAG_TXE) == RESET)
    {
        if ((SPITimeout--) == 0)
        {
            return SPI_TIMEOUT_UserCallback(0);
        }
    }
    SPI_I2S_SendData(SPI_FLASHx, uiByte);
    SPITimeout = SPIT_FLAG_TIMEOUT;

    while (SPI_I2S_GetFlagStatus(SPI_FLASHx, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if ((SPITimeout--) == 0)
        {
            return SPI_TIMEOUT_UserCallback(1);
        }
    }

    return SPI_I2S_ReceiveData(SPI_FLASHx);
}

uint32_t SPI_FLASH_ReadID(void)
{
    uint32_t uiID = 0x00;

    SPI_FLASH_CS_LOW();
    (void)SPI_FLASH_TransferByte(W25X_JedecDeviceID);
    uiID = SPI_FLASH_TransferByte(SPI_FLASH_DUMMY_BYTE);
    uiID <<= 8;
    uiID |= SPI_FLASH_TransferByte(SPI_FLASH_DUMMY_BYTE);
    uiID <<= 8;
    uiID |= SPI_FLASH_TransferByte(SPI_FLASH_DUMMY_BYTE);
    SPI_FLASH_CS_HIGH();
    return uiID;
}

void SPI_FLASH_Erase_Sector(uint32_t uiAddr)
{
    SPI_FLASH_WriteEnable();
    SPI_FLASH_CS_LOW();
    (void)SPI_FLASH_TransferByte(W25X_SectorErase);
    SPI_FLASH_TransferByte((uiAddr>>16)&0xff);
    SPI_FLASH_TransferByte((uiAddr>>8)&0xff);
    SPI_FLASH_TransferByte(uiAddr&0xff);
    SPI_FLASH_CS_HIGH();
    SPI_FLASH_WaitBusy();
}

void SPI_FLASH_WaitBusy(void)
{
    uint8_t uiStatusReg = 0;

    SPI_FLASH_CS_LOW();
    (void)SPI_FLASH_TransferByte(W25X_ReadStatusReg);
    do
    {
        uiStatusReg = SPI_FLASH_TransferByte(SPI_FLASH_DUMMY_BYTE);
    }
    while((uiStatusReg & 0x01) == 1);
    SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_WriteEnable(void)
{
    SPI_FLASH_CS_LOW();
    (void)SPI_FLASH_TransferByte(W25X_WriteEnable);
    SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_ReadData(uint32_t uiAddr,uint8_t *readBuff,uint32_t numByteToRead)
{
    SPI_FLASH_CS_LOW();
    (void)SPI_FLASH_TransferByte(W25X_ReadData);
    (void)SPI_FLASH_TransferByte((uiAddr>>16)&0xff);
    (void)SPI_FLASH_TransferByte((uiAddr>>8)&0xff);
    (void)SPI_FLASH_TransferByte(uiAddr&0xff);

    while(numByteToRead--)
    {
        *readBuff = SPI_FLASH_TransferByte(SPI_FLASH_DUMMY_BYTE);
        readBuff++;
    }
    SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_WriteData(uint32_t uiAddr,uint8_t *writeBuff,uint32_t numByteToWrite)
{
    SPI_FLASH_WriteEnable();
    SPI_FLASH_CS_LOW();
    (void)SPI_FLASH_TransferByte(W25X_PageProgram);
    (void)SPI_FLASH_TransferByte((uiAddr>>16)&0xff);
    (void)SPI_FLASH_TransferByte((uiAddr>>8)&0xff);
    (void)SPI_FLASH_TransferByte(uiAddr&0xff);
    while(numByteToWrite--)
    {
        (void)SPI_FLASH_TransferByte(*writeBuff);
        writeBuff++;
    }
    SPI_FLASH_CS_HIGH();
    SPI_FLASH_WaitBusy();
}

static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  FLASH_ERROR("SPI waiting timeout! error code is %d",errorCode);
  return 0;
}

uint8_t SPI_FLASH_ReadStatusReg(void)
{
    uint8_t uiStatusRegVal = 0;
    SPI_FLASH_CS_LOW();
    (void)SPI_FLASH_TransferByte(W25X_ReadStatusReg);
    uiStatusRegVal = SPI_FLASH_TransferByte(SPI_FLASH_DUMMY_BYTE);
    SPI_FLASH_CS_HIGH();
    return uiStatusRegVal;
}

void SPI_FLASH_EraseChip(void)
{
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitBusy();
    SPI_FLASH_CS_LOW();
    (void)SPI_FLASH_TransferByte(W25X_ChipErase);
    SPI_FLASH_CS_HIGH();
    SPI_FLASH_WaitBusy();
}

