#include "OLED.h"
#include "stdlib.h"
#include "oledfont.h"
#include "i2c.h"

uint8_t OLED_GRAM[144][4];

//发送一个字节
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
	uint8_t data[2];

	if (mode)
	{
		data[0] = 0x40;
	}
	else
	{
		data[0] = 0x00;
	}

	data[1] = dat;

	HAL_I2C_Master_Transmit(&hi2c1, 0x78, data, 2, 10);
}

//开启OLED显示
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵使能
	OLED_WR_Byte(0x14, OLED_CMD); //开启电荷泵
	OLED_WR_Byte(0xAF, OLED_CMD); //点亮屏幕
}

//关闭OLED显示
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵使能
	OLED_WR_Byte(0x10, OLED_CMD); //关闭电荷泵
	OLED_WR_Byte(0xAE, OLED_CMD); //关闭屏幕
}

//更新显存到OLED
void OLED_Refresh(void)
{
	uint8_t data[128];
	HAL_StatusTypeDef status;
	uint8_t i, n;
	for (i = 0; i < 4; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置行起始地址
		OLED_WR_Byte(0x00, OLED_CMD);	  //设置低列起始地址
		OLED_WR_Byte(0x10, OLED_CMD);	  //设置高列起始地址

		for (n = 0; n < 128; n++)
		{
			data[n] = OLED_GRAM[n][i];
		}

		status = HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x40, 1, (uint8_t *)data, sizeof(data), 50);
		if (status == HAL_BUSY)
		{
			LED_Blink(10, 100);
			HAL_I2C_Init(&hi2c1);
			HAL_NVIC_SystemReset();
		}
		else if (status == HAL_ERROR)
		{
			LED_Blink(30, 500);
		}
	}
}
//清屏函数
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 4; i++)
	{
		for (n = 0; n < 128; n++)
		{
			OLED_GRAM[n][i] = 0; //清除所有数据
		}
	}
	OLED_Refresh(); //更新显示
}

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
	uint8_t i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	if (t)
	{
		OLED_GRAM[x][i] |= n;
	}
	else
	{
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
		OLED_GRAM[x][i] |= n;
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	}
}

//画线
//x1,y1:起点坐标
//x2,y2:结束坐标
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode)
{
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1; //画线起点坐标
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //设置单步方向
	else if (delta_x == 0)
		incx = 0; //垂直线
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //水平线
	else
	{
		incy = -1;
		delta_y = -delta_x;
	}
	if (delta_x > delta_y)
		distance = delta_x; //选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		OLED_DrawPoint(uRow, uCol, mode); //画点
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
	int a, b, num;
	a = 0;
	b = r;
	while (2 * b * b >= r * r)
	{
		OLED_DrawPoint(x + a, y - b, 1);
		OLED_DrawPoint(x - a, y - b, 1);
		OLED_DrawPoint(x - a, y + b, 1);
		OLED_DrawPoint(x + a, y + b, 1);

		OLED_DrawPoint(x + b, y + a, 1);
		OLED_DrawPoint(x + b, y - a, 1);
		OLED_DrawPoint(x - b, y - a, 1);
		OLED_DrawPoint(x - b, y + a, 1);

		a++;
		num = (a * a + b * b) - r * r; //计算画的点离圆心的距离
		if (num > 0)
		{
			b--;
			a--;
		}
	}
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode)
{
	uint8_t i, m, temp, size2, chr1;
	uint8_t x0 = x, y0 = y;
	if (size1 == 8)
		size2 = 6;
	else
		size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); //得到字体一个字符对应点阵集所占的字节数
	chr1 = chr - ' ';											   //计算偏移后的值
	for (i = 0; i < size2; i++)
	{
		if (size1 == 8)
		{
			temp = asc2_0806[chr1][i];
		} //调用0806字体
		else if (size1 == 12)
		{
			temp = asc2_1206[chr1][i];
		} //调用1206字体
		else if (size1 == 16)
		{
			temp = asc2_1608[chr1][i];
		} //调用1608字体
		else if (size1 == 24)
		{
			temp = asc2_2412[chr1][i];
		} //调用2412字体
		else
			return;
		for (m = 0; m < 8; m++)
		{
			if (temp & 0x01)
				OLED_DrawPoint(x, y, mode);
			else
				OLED_DrawPoint(x, y, !mode);
			temp >>= 1;
			y++;
		}
		x++;
		if ((size1 != 8) && ((x - x0) == size1 / 2))
		{
			x = x0;
			y0 = y0 + 8;
		}
		y = y0;
	}
}

//显示字符串
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
//mode:0,反色显示;1,正常显示
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1, uint8_t mode)
{
	while ((*chr >= ' ') && (*chr <= '~')) //判断是不是非法字符!
	{
		OLED_ShowChar(x, y, *chr, size1, mode);
		if (size1 == 8)
			x += 6;
		else
			x += size1 / 2;
		chr++;
	}
}

//m^n
uint32_t OLED_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
	{
		result *= m;
	}
	return result;
}

//显示数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1, uint8_t mode)
{
	uint8_t t, temp, m = 0;
	if (size1 == 8)
		m = 2;
	for (t = 0; t < len; t++)
	{
		temp = (num / OLED_Pow(10, len - t - 1)) % 10;
		if (temp == 0)
		{
			OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
		}
		else
		{
			OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
		}
	}
}

//x,y：起点坐标
//sizex,sizey,图片长宽
//BMP[]：要写入的图片数组
//mode:0,反色显示;1,正常显示
void OLED_ShowPicture(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode)
{
	uint16_t j = 0;
	uint8_t i, n, temp, m;
	uint8_t x0 = x, y0 = y;
	sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
	for (n = 0; n < sizey; n++)
	{
		for (i = 0; i < sizex; i++)
		{
			temp = BMP[j];
			j++;
			for (m = 0; m < 8; m++)
			{
				if (temp & 0x01)
					OLED_DrawPoint(x, y, mode);
				else
					OLED_DrawPoint(x, y, !mode);
				temp >>= 1;
				y++;
			}
			x++;
			if ((x - x0) == sizex)
			{
				x = x0;
				y0 = y0 + 8;
			}
			y = y0;
		}
	}
}
//OLED的初始化
void OLED_Init(void)
{
	HAL_Delay(10);

	OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
	OLED_WR_Byte(0x00, OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
	OLED_WR_Byte(0x00, OLED_CMD); /*set display start line*/
	OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
	OLED_WR_Byte(0xff, OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
	OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
	OLED_WR_Byte(0x1F, OLED_CMD); /*duty = 1/32*/
	OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
	OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
	OLED_WR_Byte(0x00, OLED_CMD);
	OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
	OLED_WR_Byte(0x80, OLED_CMD);
	OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
	OLED_WR_Byte(0x1f, OLED_CMD);
	OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
	OLED_WR_Byte(0x00, OLED_CMD);
	OLED_WR_Byte(0xdb, OLED_CMD); /*set vcomh*/
	OLED_WR_Byte(0x40, OLED_CMD);
	OLED_WR_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
	OLED_WR_Byte(0x14, OLED_CMD);
	OLED_Clear();
	OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}

struct
{
	struct
	{
		uint16_t uiStartPageIndex;
		uint16_t uiStartColumnIndex;
		uint16_t uiEndPageIndex;
		uint16_t uiEndColumnIndex;
	} stUpdateArea;
	uint16_t auiDisplayCache[LCD_SIZE_WIDTH][LCD_SIZE_PAGES];
} s_stLCDBuffer;

static void SCREEN_UpdateChangedBufferAreaRecord(uint8_t uiPageIndex, uint8_t uiColumnIndex);
static void SCREEN_ClearDisplayBuffer(void);

//发送一个字节
//mode:数据/命令标志 0,表示命令;1,表示数据;
void SSD1306_WR_Byte(uint8_t dat, uint8_t mode)
{
	uint8_t data[2];

	if (mode)
	{
		data[0] = 0x40;
	}
	else
	{
		data[0] = 0x00;
	}

	data[1] = dat;

	HAL_I2C_Master_Transmit(&hi2c1, 0x78, data, 2, 10);
}

void SSD1306_SetBrightness(uint8_t brightness)
{
	union
	{
		struct 
		{
			uint8_t mode;
			uint8_t data[2];
		} SSD1306IIC;
		uint8_t sendbyte[3];
	}sendbyte;
	
	sendbyte.SSD1306IIC.mode = SSD1306_CMD;
	sendbyte.SSD1306IIC.data[0] = 0x81;      /*contract control*/
	sendbyte.SSD1306IIC.data[1] = brightness;
	
	HAL_I2C_Master_Transmit(&hi2c1, SSD1306_ADDR, sendbyte.sendbyte, sizeof(sendbyte), 10);
	
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
	union
	{
		struct 
		{
			uint8_t mode;
			uint8_t data[3];
		} SSD1306IIC;
		uint8_t sendbyte[4];
	}sendbyte;
	
	sendbyte.SSD1306IIC.mode = SSD1306_CMD;
	sendbyte.SSD1306IIC.data[0] = 0xB0 + uiPage;              //设置行起始地址
	sendbyte.SSD1306IIC.data[1] = (((uiColumn & 0xF0)>>4)|0x10);   //设置低列起始地址
	sendbyte.SSD1306IIC.data[2] = (uiColumn & 0x0F);          //设置高列起始地址
	HAL_I2C_Master_Transmit(&hi2c1, SSD1306_ADDR, sendbyte.sendbyte, sizeof(sendbyte), 10);
}


/*************************************************************************/
/** Function Name:  SCREEN_UpdateChangedBufferAreaRecord                  **/
/** Purpose:        Check changed area recodr and update.               **/
/** Resources:      None.                                               **/
/** Params:                                                             **/
/** @ uiPageIndex:      Operated page index.                            **/
/** @ uiColumnIndex:    Operated column index.                          **/
/** Return:         None.                                               **/
/** Limitation:     None.                                               **/
/*************************************************************************/
void SCREEN_UpdateChangedBufferAreaRecord(uint8_t uiPageIndex, uint8_t uiColumnIndex)
{
	// Check and set page and column index.
	if (uiPageIndex < s_stLCDBuffer.stUpdateArea.uiStartPageIndex)
	{
		s_stLCDBuffer.stUpdateArea.uiStartPageIndex = uiPageIndex;
	}
	if (uiPageIndex > s_stLCDBuffer.stUpdateArea.uiEndPageIndex)
	{
		s_stLCDBuffer.stUpdateArea.uiEndPageIndex = uiPageIndex;
	}
	if (uiColumnIndex < s_stLCDBuffer.stUpdateArea.uiStartColumnIndex)
	{
		s_stLCDBuffer.stUpdateArea.uiStartColumnIndex = uiColumnIndex;
	}
	if (uiColumnIndex > s_stLCDBuffer.stUpdateArea.uiEndColumnIndex)
	{
		s_stLCDBuffer.stUpdateArea.uiEndColumnIndex = uiColumnIndex;
	}
}

/*************************************************************************/
/** Function Name:  LCD_ClearDisplayBuffer                              **/
/** Purpose:            Clean display buffer.                           **/
/** Resources:      None.                                               **/
/** Params:         None.                                               **/
/** Return:         None.                                               **/
/** Limitation:     None.                                               **/
/*************************************************************************/
void SCREEN_ClearDisplayBuffer(void)
{
	uint16_t uiCurrentPageIndex, uiCurrentColumnIndex;

	for (uiCurrentPageIndex = 0; uiCurrentPageIndex < LCD_SIZE_PAGES; uiCurrentPageIndex++)
	{
		for (uiCurrentColumnIndex = 0; uiCurrentColumnIndex < LCD_SIZE_WIDTH; uiCurrentColumnIndex++)
		{
			s_stLCDBuffer.auiDisplayCache[uiCurrentColumnIndex][uiCurrentPageIndex] = 0x00;
		}
	}
	s_stLCDBuffer.stUpdateArea.uiStartPageIndex = (LCD_SIZE_HEIGHT / 8) - 1;
	s_stLCDBuffer.stUpdateArea.uiEndPageIndex = 0;
	s_stLCDBuffer.stUpdateArea.uiStartColumnIndex = LCD_SIZE_WIDTH - 1;
	s_stLCDBuffer.stUpdateArea.uiEndColumnIndex = 0;
}

/*************************************************************************/
/** Function Name:  SCREEN_SetPixel                                     **/
/** Purpose:        Set a pixel color or draw a point.                  **/
/** Resources:      None.                                               **/
/** Params:                                                             **/
/** @ iPosX:        X location of point by pixels.                      **/
/** @ iPosY:        Y location of point by pixels.                      **/
/** @ iColor:       Point color, 0 is white, Nonzero is Black.          **/
/** Return:         None.                                               **/
/** Limitation:     None.                                               **/
/*************************************************************************/
void SCREEN_SetPixel(int iPosX, int iPosY, int iColor)
{
	// Operating position check.
	if ((iPosX < LCD_SIZE_WIDTH) && (iPosY < LCD_SIZE_HEIGHT))
	{
		// Check and set page and column index.
		SCREEN_UpdateChangedBufferAreaRecord(iPosY / 8, iPosX);
		// Set point data.
		if (SCREEN_COLOR_FRG == iColor)
		{
			SET_PAGE_BIT(s_stLCDBuffer.auiDisplayCache[iPosX][iPosY / 8], iPosY % 8);
		}
		else
		{
			CLR_PAGE_BIT(s_stLCDBuffer.auiDisplayCache[iPosX][iPosY / 8], iPosY % 8);
		}
	}
}

/*************************************************************************/
/** Function Name:  SCREEN_GetPixel                                     **/
/** Purpose:        Get a pixel from buffer.                            **/
/** Resources:      None.                                               **/
/** Params:                                                             **/
/** @ iPosX:            X location of point by pixels.                  **/
/** @ iPosY:            Y location of point by pixels.                  **/
/** Return:         None.                                               **/
/** Limitation:     None.                                               **/
/*************************************************************************/
int SCREEN_GetPixel(int iPosX, int iPosY)
{
	// Operating position check.
	if ((iPosX < LCD_SIZE_WIDTH) && (iPosY < LCD_SIZE_HEIGHT))
	{
		return GET_PAGE_BIT(s_stLCDBuffer.auiDisplayCache[iPosX][iPosY / 8], iPosY % 8);
	}
	else
	{
		return 0;
	}
}

/*************************************************************************/
/** Function Name:  LCD_RefreshScreen                                   **/
/** Purpose:            Update Screen with cache data.                  **/
/** Resources:      None.                                               **/
/** Params:         None.                                               **/
/** Return:         None.                                               **/
/** Limitation:     None.                                               **/
/*************************************************************************/
void SCREEN_RefreshScreen(void)
{
	uint8_t uiChangedPageIndex, uiChangedColumnIndex;
	
	HAL_StatusTypeDef status;
	uint8_t data[LCD_SIZE_WIDTH],index;
	
	if (s_stLCDBuffer.stUpdateArea.uiEndColumnIndex > LCD_SIZE_WIDTH - 1)
	{
		s_stLCDBuffer.stUpdateArea.uiEndColumnIndex = LCD_SIZE_WIDTH - 1;
	}
	if (s_stLCDBuffer.stUpdateArea.uiEndPageIndex > (LCD_SIZE_HEIGHT / 8) - 1)
	{
		s_stLCDBuffer.stUpdateArea.uiEndPageIndex = (LCD_SIZE_HEIGHT / 8) - 1;
	}
	uiChangedPageIndex = s_stLCDBuffer.stUpdateArea.uiStartPageIndex;
	// Loop for each changed page.
	while (uiChangedPageIndex <= s_stLCDBuffer.stUpdateArea.uiEndPageIndex)
	{
		uiChangedColumnIndex = s_stLCDBuffer.stUpdateArea.uiStartColumnIndex;
		SSD1306_SetPosition(s_stLCDBuffer.stUpdateArea.uiStartColumnIndex, uiChangedPageIndex);
		
		index = 0;
		// Loop for each changed column data in current page.
		while (uiChangedColumnIndex <= s_stLCDBuffer.stUpdateArea.uiEndColumnIndex)
		{
			// Write data to screen controler.
			data[index] = s_stLCDBuffer.auiDisplayCache[uiChangedColumnIndex][uiChangedPageIndex];
			uiChangedColumnIndex++;
			index ++;
		}
		status = HAL_I2C_Mem_Write(&hi2c1, SSD1306_ADDR, SSD1306_DATA, 1, (uint8_t *)data, index, 50);
		if (status == HAL_BUSY)
		{
			LED_Blink(10, 100);
			HAL_I2C_Init(&hi2c1);
			HAL_NVIC_SystemReset();
		}
		else if (status == HAL_ERROR)
		{
			LED_Blink(30, 500);
		}
		
		uiChangedPageIndex++;
	}

	// Reset global variable.
	s_stLCDBuffer.stUpdateArea.uiStartPageIndex = (LCD_SIZE_HEIGHT / 8) - 1;
	s_stLCDBuffer.stUpdateArea.uiEndPageIndex = 0;
	s_stLCDBuffer.stUpdateArea.uiStartColumnIndex = LCD_SIZE_WIDTH - 1;
	s_stLCDBuffer.stUpdateArea.uiEndColumnIndex = 0;
}

/*************************************************************************/
/** Function Name:  SCREEN_ClearDisplay                                 **/
/** Purpose:        Clean display buffer.                               **/
/** Resources:      None.                                               **/
/** Params:         None.                                               **/
/** Return:         None.                                               **/
/** Limitation:     None.                                               **/
/*************************************************************************/
void SCREEN_ClearDisplay(void)
{
	SCREEN_ClearDisplayBuffer();
	OLED_Clear();
}
