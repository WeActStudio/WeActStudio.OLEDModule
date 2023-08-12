#include "uc1604c_softspi.h"
#include <stm32f10x_gpio.h>
#include <stdint.h>
#include <stddef.h>

#define SPI_UC1604C_CTRL                SPI2
#define SPI_UC1604C_APBxClock_FUN       RCC_APB1PeriphClockCmd
#define SPI_UC1604C_CLK                 RCC_APB1Periph_SPI2

#define SPI_UC1604C_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd
#define SPI_UC1604C_GPIO_CLK            RCC_APB2Periph_GPIOD
#define SPI_UC1604C_GPIO_PORT           GPIOD

//RST
#define SPI_UC1604C_RST_PIN             GPIO_Pin_1
#define SPI_UC1604C_RST_LOW()           GPIO_ResetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_RST_PIN)
#define SPI_UC1604C_RST_HIGH()          GPIO_SetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_RST_PIN)
//DC
#define SPI_UC1604C_DC_PIN              GPIO_Pin_0
#define SPI_UC1604C_DC_LOW()            GPIO_ResetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_DC_PIN)
#define SPI_UC1604C_DC_HIGH()           GPIO_SetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_DC_PIN)
#define SPI_UC1604C_MODE_CMD()          SPI_UC1604C_DC_LOW()
#define SPI_UC1604C_MODE_DAT()          SPI_UC1604C_DC_HIGH()

//CS(NSS)
#define SPI_UC1604C_CS_PIN              GPIO_Pin_2
#define SPI_UC1604C_CS_LOW()            GPIO_ResetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_CS_PIN)
#define SPI_UC1604C_CS_HIGH()           GPIO_SetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_CS_PIN)
#define SPI_UC1604C_CS_ENABLE()         SPI_UC1604C_CS_LOW()
#define SPI_UC1604C_CS_DISABLE()        SPI_UC1604C_CS_HIGH()

//SCLK
#define SPI_UC1604C_SCLK_PIN            GPIO_Pin_4
#define SPI_UC1604C_SCLK_LOW()          GPIO_ResetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_SCLK_PIN)
#define SPI_UC1604C_SCLK_HIGH()         GPIO_SetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_SCLK_PIN)
//MOSI
#define SPI_UC1604C_MOSI_PIN            GPIO_Pin_3
#define SPI_UC1604C_MOSI_LOW()          GPIO_ResetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_MOSI_PIN)
#define SPI_UC1604C_MOSI_HIGH()         GPIO_SetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_MOSI_PIN)

static void UC1604C_InitializeGPIO(void);
static void UC1604C_WriteByte(uint8_t uiData);
static void UC1604C_WriteCommand(uint8_t uiCommand);

void UC1604C_Delay(uint32_t uiDelay)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    uint32_t    uiDelayTime;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    while(uiDelay--)
    {
        uiDelayTime = 1000;
        while(--uiDelayTime);
    }
}

/*****************************************************************************/
/** Function Name:  UC1604C_InitializeGPIO.                             **/
/** Purpose:        Initialize GPIO for SPI Pin.                            **/
/** Params:         None.                                                   **/
/** Return:         None.                                                   **/
/*****************************************************************************/
void UC1604C_InitializeGPIO(void)
{
    // Initialize data structure.
    GPIO_InitTypeDef GPIO_InitStructure;
    // Initialize GPIO.
    SPI_UC1604C_GPIO_APBxClock_FUN(SPI_UC1604C_GPIO_CLK, ENABLE);
    // Enable GPIO.
    GPIO_InitStructure.GPIO_Pin =   SPI_UC1604C_RST_PIN|SPI_UC1604C_DC_PIN|SPI_UC1604C_CS_PIN|SPI_UC1604C_SCLK_PIN|SPI_UC1604C_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(SPI_UC1604C_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_DC_PIN|SPI_UC1604C_CS_PIN|SPI_UC1604C_SCLK_PIN|SPI_UC1604C_MOSI_PIN);
    GPIO_ResetBits(SPI_UC1604C_GPIO_PORT, SPI_UC1604C_RST_PIN);
}

/*****************************************************************************/
/** Function Name:  UC1604C_WriteByte.                                  **/
/** Purpose:        Simulate SPI communication timing to write a byte to    **/
/**                 LCD controller.                                         **/
/** Params:                                                                 **/
/** @ uiData:       Byte will be write.                                     **/
/** Return:         None.                                                   **/
/*****************************************************************************/
void UC1604C_WriteByte(uint8_t uiByte)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    uint16_t                i;
    uint8_t                 uiSendByte;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    uiSendByte = uiByte;
    SPI_UC1604C_CS_LOW();
    for(i=0;i<8;i++)
    {
        if(uiSendByte & 0x80)
        {
            SPI_UC1604C_MOSI_HIGH();
        }
        else
        {
            SPI_UC1604C_MOSI_LOW();
        }
        SPI_UC1604C_SCLK_LOW();
        SPI_UC1604C_SCLK_HIGH();

        uiSendByte <<= 1;
    }
    SPI_UC1604C_CS_HIGH();
}

/*****************************************************************************/
/** Function Name:  UC1604C_WriteData.                                      **/
/** Purpose:        Write a data byte to OLED coltroler.                    **/
/** Resources:      None.                                                   **/
/** Params:                                                                 **/
/** @ uiData:       One byte data will be write.                            **/
/** Return:         None.                                                   **/
/*****************************************************************************/
void UC1604C_WriteData(uint8_t uiData)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SPI_UC1604C_MODE_DAT();
    UC1604C_WriteByte(uiData);
}

/*****************************************************************************/
/** Function Name:  UC1604C_WriteCommand.                                   **/
/** Purpose:        Write a command byte to OLED coltroler.                 **/
/** Resources:      None.                                                   **/
/** Params:                                                                 **/
/** @ uiData:       One byte command will be write.                         **/
/** Return:         None.                                                   **/
/*****************************************************************************/
void UC1604C_WriteCommand(uint8_t uiCommand)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SPI_UC1604C_MODE_CMD();
    UC1604C_WriteByte(uiCommand);
}

/*****************************************************************************/
/** Function Name:  UC1604C_SetPosition.                                    **/
/** Purpose:        Set current position of page and colomn.                **/
/** Resources:      None.                                                   **/
/** Params:                                                                 **/
/** @ uiColumn:     Position of column.                                     **/
/** @ uiPage:       Position of page.                                       **/
/** Return:         None.                                                   **/
/*****************************************************************************/
void UC1604C_SetPosition(uint8_t uiColumn, uint8_t uiPage)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    UC1604C_WriteCommand(0xB0 | uiPage);
    UC1604C_WriteCommand(((uiColumn & 0xF0)>>4)|0x10);
    UC1604C_WriteCommand((uiColumn & 0x0F));
}

/*****************************************************************************/
/** Function Name:  UC1604C_Fill.                                           **/
/** Purpose:        Fill screen.                                            **/
/** Params:                                                                 **/
/** @ uiByte:       Filled data.                                            **/
/** Return:         None.                                                   **/
/*****************************************************************************/
void UC1604C_Fill(uint8_t uiByte)//全屏填充
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    uint8_t uiPage, uiColumn;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    for(uiPage=0; uiPage<PAGE_MAX; uiPage++)
    {
        UC1604C_WriteCommand(0xB0 | uiPage);
        UC1604C_WriteCommand(0x01);
        UC1604C_WriteCommand(0x10);
        for(uiColumn=0; uiColumn<COLUMN_MAX; uiColumn++)
        {
            UC1604C_WriteData(uiByte);
        }
    }
}

/*****************************************************************************/
/** Function Name:  UC1604C_Initialize                                      **/
/** Purpose:        Run the device initialize sequence.                     **/
/** Resources:      None.                                                   **/
/** Params:         None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         Initialize sequence must executed after power on 500ms. **/
/*****************************************************************************/
void UC1604C_Initialize(void)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    UC1604C_InitializeGPIO();
    // Reset
    SPI_UC1604C_RST_LOW();
    UC1604C_Delay(100);
    SPI_UC1604C_RST_HIGH();
    UC1604C_Delay(800);

    SPI_UC1604C_CS_ENABLE();
    // Soft reset.
    UC1604C_WriteCommand(0xE2);
    UC1604C_Delay(100);
    // Enable internal boost.
    UC1604C_WriteCommand(0x2F);
    UC1604C_Delay(100);

    UC1604C_WriteCommand(0x81);  //微调对比度
    UC1604C_WriteCommand(0x56);  //微调对比度的值，可设置范围0x00～0xFF
    UC1604C_WriteCommand(0xEB);  //1/9偏压比（bias）
    UC1604C_WriteCommand(0xC2);  //行列扫描顺序：从上到下 oxc2
    UC1604C_WriteCommand(0xAF);  //开显示
}
