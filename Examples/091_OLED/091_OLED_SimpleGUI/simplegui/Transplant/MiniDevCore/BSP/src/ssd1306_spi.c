#include "ssd1306_spi.h"

#define SPI_SSD1306_SPI                 SPI2
#define SPI_SSD1306_SPI_APBxClock_FUN   RCC_APB1PeriphClockCmd
#define SPI_SSD1306_SPI_CLK             RCC_APB1Periph_SPI2

#define SPI_SSD1306_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd
#define SPI_SSD1306_GPIO_CLK            RCC_APB2Periph_GPIOB
#define SPI_SSD1306_GPIO_PORT           GPIOB

//RST
#define SPI_SSD1306_RST_PIN             GPIO_Pin_10
#define SPI_SSD1306_RST_LOW()           GPIO_ResetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_RST_PIN)
#define SPI_SSD1306_RST_HIGH()          GPIO_SetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_RST_PIN)
//DC
#define SPI_SSD1306_DC_PIN              GPIO_Pin_11
#define SPI_SSD1306_DC_LOW()            GPIO_ResetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_DC_PIN)
#define SPI_SSD1306_DC_HIGH()           GPIO_SetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_DC_PIN)
#define SPI_SSD1306_MODE_CMD()          SPI_SSD1306_DC_LOW()
#define SPI_SSD1306_MODE_DAT()          SPI_SSD1306_DC_HIGH()
//CS(NSS)
#define SPI_SSD1306_CS_PIN              GPIO_Pin_12
#define SPI_SSD1306_CS_LOW()            GPIO_ResetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_CS_PIN)
#define SPI_SSD1306_CS_HIGH()           GPIO_SetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_CS_PIN)
//SCLK
#define SPI_SSD1306_SCLK_PIN            GPIO_Pin_13
#define SPI_SSD1306_SCLK_LOW()          GPIO_ResetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_SCLK_PIN)
#define SPI_SSD1306_SCLK_HIGH()         GPIO_SetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_SCLK_PIN)
//MOSI
#define SPI_SSD1306_MOSI_PIN            GPIO_Pin_15
#define SPI_SSD1306_MOSI_LOW()          GPIO_ResetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_MOSI_PIN)
#define SPI_SSD1306_MOSI_HIGH()         GPIO_SetBits(SPI_SSD1306_GPIO_PORT, SPI_SSD1306_MOSI_PIN)

#define SPI_SSD1306_DUMMY_BYTE          (0xFF)

static void SSD1306_InitializeGPIO(void);
#ifndef SSD1306_SOFT_SPI
static void SSD1306_InitializeSPI(void);
#endif
static void SSD1306_WriteByte(uint8_t uiData);

/*****************************************************************************/
/** Function Name:  SSD1306_InitializeGPIO.                                 **/
/** Purpose:        Config and initialize GPIO.                             **/
/** Params:         None.                                                   **/
/** Return:         None.                                                   **/
/** Limitation:     None.                                                   **/
/*****************************************************************************/
void SSD1306_InitializeGPIO(void)
{
    // Initialize data structure.
    GPIO_InitTypeDef GPIO_InitStructure;

    // Initialize GPIO.
    SPI_SSD1306_GPIO_APBxClock_FUN(SPI_SSD1306_GPIO_CLK, ENABLE);

    // Enable GPIO.
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
#ifndef SSD1306_SOFT_SPI
    GPIO_InitStructure.GPIO_Pin =   SPI_SSD1306_SCLK_PIN|SPI_SSD1306_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    GPIO_Init(SPI_SSD1306_GPIO_PORT, &GPIO_InitStructure);
#endif

#ifdef SSD1306_SOFT_SPI
    GPIO_InitStructure.GPIO_Pin =   SPI_SSD1306_RST_PIN|SPI_SSD1306_DC_PIN|SPI_SSD1306_CS_PIN|SPI_SSD1306_SCLK_PIN|SPI_SSD1306_MOSI_PIN;
#else
    GPIO_InitStructure.GPIO_Pin =   SPI_SSD1306_RST_PIN|SPI_SSD1306_DC_PIN|SPI_SSD1306_CS_PIN;
#endif
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
    GPIO_Init(SPI_SSD1306_GPIO_PORT, &GPIO_InitStructure);
}

/*****************************************************************************/
/** Function Name:  SSD1306_InitializeSPI.                                  **/
/** Purpose:        Initialize hardware SPI controler for OLED.             **/
/** Params:         None.                                                   **/
/** Return:         None.                                                   **/
/*****************************************************************************/
void SSD1306_InitializeSPI(void)
{
    // Initialize data structure.
    SPI_InitTypeDef SPI_InitStructure;

    // Initialize GPIO.
    SPI_SSD1306_SPI_APBxClock_FUN(SPI_SSD1306_SPI_CLK, ENABLE);

    // Config SPI control.
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI_SSD1306_SPI, &SPI_InitStructure);

    SPI_Cmd(SPI_SSD1306_SPI, ENABLE);
}

/*****************************************************************************/
/** Function Name:  SSD1306_WriteByte.                                      **/
/** Purpose:        Simulate SPI communication timing to write a byte       **/
/**                 to OLED controler.                                      **/
/** Params:                                                                 **/
/** @ uiByte:       Byte will be write.                                     **/
/** Return:         None.                                                   **/
/** Limitation:     None.                                                   **/
/*****************************************************************************/
void SSD1306_WriteByte(uint8_t uiByte)
{
#ifdef SSD1306_SOFT_SPI
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    uint16_t                i;
#endif

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
#ifdef SSD1306_SOFT_SPI
    for(i=0;i<8;i++)
    {
        if((uiByte << i) & 0x80)
        {
            SPI_SSD1306_MOSI_HIGH();
        }
        else
        {
            SPI_SSD1306_MOSI_LOW();
        }
        SPI_SSD1306_SCLK_LOW();
        SPI_SSD1306_SCLK_HIGH();
    }
#else
    while (SPI_I2S_GetFlagStatus(SPI_SSD1306_SPI, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI_SSD1306_SPI, uiByte);
    while (SPI_I2S_GetFlagStatus(SPI_SSD1306_SPI, SPI_I2S_FLAG_TXE) == RESET);
#endif
}

/*****************************************************************************/
/* Function Name:   SSD1306_WriteData.                                      **/
/* Purpose:         Write a data byte to OLED coltroler.                    **/
/* Resources:       None.                                                   **/
/* Params:                                                                  **/
/*  @ uiData:       One byte data will be write.                        **/
/* Return:          None.                                                   **/
/* Limitation:      None.                                                   **/
/*****************************************************************************/
void SSD1306_WriteData(uint8_t uiData)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SPI_SSD1306_MODE_DAT();
    SPI_SSD1306_CS_LOW();
    SSD1306_WriteByte(uiData);
    SPI_SSD1306_CS_HIGH();
}

/*****************************************************************************/
/* Function Name:   SSD1306_WriteCommand.                                   **/
/* Purpose:         Write a command byte to OLED coltroler.                 **/
/* Resources:       None.                                                   **/
/* Params:                                                                  **/
/*  @ uiCommand:        One byte command will be write.                     **/
/* Return:          None.                                                   **/
/* Limitation:      None.                                                   **/
/*****************************************************************************/
void SSD1306_WriteCommand(uint8_t uiCommand)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SPI_SSD1306_MODE_CMD();
    SPI_SSD1306_CS_LOW();
    SSD1306_WriteByte(uiCommand);
    SPI_SSD1306_CS_HIGH();
}

/*****************************************************************************/
/* Function Name:   SSD1306_SetPosition.                                    **/
/* Purpose:         Set current position of page and colomn.                **/
/* Resources:       None.                                                   **/
/* Params:                                                                  **/
/*  @ uiColumn:     Position of column.                                     **/
/*  @ uiPage:       Position of page.                                       **/
/* Return:          None.                                                   **/
/* Limitation:      None.                                                   **/
/*****************************************************************************/
void SSD1306_SetPosition(uint8_t uiColumn, uint8_t uiPage)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SSD1306_WriteCommand(0xB0 | uiPage);
    SSD1306_WriteCommand(((uiColumn & 0xF0)>>4)|0x10);
    SSD1306_WriteCommand((uiColumn & 0x0F));
}

/*****************************************************************************/
/* Function Name:   SSD1306_Fill.                                           **/
/* Purpose:         Fill screen.                                            **/
/* Resources:       None.                                                   **/
/* Params:                                                                  **/
/*  @uiByte:        Filled data for each display unit.                      **/
/* Return:          None.                                                   **/
/* Limitation:      None.                                                   **/
/*****************************************************************************/
void SSD1306_Fill(uint8_t uiByte)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    uint8_t uiPage, uiColumn;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    for(uiPage=0; uiPage<8; uiPage++)
    {
        SSD1306_WriteCommand(0xb0 | uiPage);
        SSD1306_WriteCommand(0x00);
        SSD1306_WriteCommand(0x10);
        for(uiColumn=0; uiColumn<128; uiColumn++)
        {
            SSD1306_WriteData(uiByte);
        }
    }
}

/*****************************************************************************/
/** Function Name:  SSD1306_Initialize                                      **/
/** Purpose:        Run the device initialize sequence.                     **/
/** Params:         None.                                                   **/
/** Return:         None.                                                   **/
/** Limitation:     Initialize sequence must executed after power on 500ms. **/
/*****************************************************************************/
void SSD1306_Initialize(void)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
#ifndef SSD1306_SOFT_SPI
    // Initialize SPI control.
    SSD1306_InitializeSPI();
#endif
    // Initialize GPIO
    SSD1306_InitializeGPIO();
    /* Clear reset singal. */
    SPI_SSD1306_RST_HIGH();

    SSD1306_WriteCommand(0xae);
    SSD1306_WriteCommand(0xae);//--turn off oled panel
    SSD1306_WriteCommand(0x00);//---set low column address
    SSD1306_WriteCommand(0x10);//---set high column address
    SSD1306_WriteCommand(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    SSD1306_WriteCommand(0x81);//--set contrast control register
    SSD1306_WriteCommand(0xcf); // Set SEG Output Current Brightness
    SSD1306_WriteCommand(0xa0);//--Set SEG/Column Mapping     0xa0,0xa1
    SSD1306_WriteCommand(0xc0);//Set COM/Row Scan Direction   0xc0,0xc8
    SSD1306_WriteCommand(0xa6);//--set normal display
    SSD1306_WriteCommand(0xa8);//--set multiplex ratio(1 to 64)
    SSD1306_WriteCommand(0x3f);//--1/64 duty
    SSD1306_WriteCommand(0xd3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    SSD1306_WriteCommand(0x00);//-not offset
    SSD1306_WriteCommand(0xd5);//--set display clock divide ratio/oscillator frequency
    SSD1306_WriteCommand(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    SSD1306_WriteCommand(0xd9);//--set pre-charge period
    SSD1306_WriteCommand(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    SSD1306_WriteCommand(0xda);//--set com pins hardware configuration
    SSD1306_WriteCommand(0x12);
    SSD1306_WriteCommand(0xdb);//--set vcomh
    SSD1306_WriteCommand(0x40);//Set VCOM Deselect Level
    SSD1306_WriteCommand(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    SSD1306_WriteCommand(0x02);//
    SSD1306_WriteCommand(0x8d);//--set Charge Pump enable/disable
    SSD1306_WriteCommand(0x14);//--set(0x10) disable
    SSD1306_WriteCommand(0xa4);// Disable Entire Display On (0xa4/0xa5)
    SSD1306_WriteCommand(0xa6);// Disable Inverse Display On (0xa6/a7)
    SSD1306_WriteCommand(0xaf);//--turn on oled panel
    SSD1306_Clear();
    SSD1306_SetPosition(0,0);
}
