#ifndef _INCLUDE_SCREEN_H__
#define _INCLUDE_SCREEN_H__

#define LCD_SIZE_WIDTH      (128)
#define LCD_SIZE_HEIGHT     (64)
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

void    SCREEN_SetPixel(int iPosX, int iPosY, int iColor);
int     SCREEN_GetPixel(int iPosX, int iPosY);
int     SCREEN_Initialize(void);
void    SCREEN_ClearDisplay(void);
void    SCREEN_RefreshScreen(void);

#endif /* _INCLUDE_SCREEN_H__ */
