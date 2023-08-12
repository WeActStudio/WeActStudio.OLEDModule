#ifndef _INCLUDE_COMMON_H_
#define _INCLUDE_COMMON_H_

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <stddef.h>
#include <stdint.h>

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define _TRANS_TEXT(STR)								_(STR)

// Blue OLED
/*
#define LCD_COLOR_OBJ_PIX						    	(0xFFF9EA22)
#define LCD_COLOR_OBJ_BKG						    	(0xFF342628)
#define LCD_COLOR_OBJ_GRID						    	(0xFF2A2835)
*/
// Blue LCD
/*
#define LCD_COLOR_OBJ_PIX						    	(0xFFFEF159)
#define LCD_COLOR_OBJ_BKG						    	(0xFFFF3C24)
#define LCD_COLOR_OBJ_GRID						    	(0xFFCC1209)
*/
// Yellow LCD
#define LCD_COLOR_OBJ_PIX						    	(0xFF258562)
#define LCD_COLOR_OBJ_BKG						    	(0xFF2BEEDE)
#define LCD_COLOR_OBJ_GRID						    	(0xFF38E3D0)

#define PARAM_DEFAULT_PIXEL_NUM_H                       (128)
#define PARAM_DEFAULT_PIXEL_NUM_V                       (64)
#define PARAM_DEFAULT_PANEL_BORDER_WIDTH				(8)
#define PARAM_DEFAULT_PIXEL_WIDTH					    (2)
#define PARAM_DEFAULT_PIXEL_HEIGHT					    (2)
#define PARAM_DEFAULT_GRID_ENABLE                       (true)
#define PARAM_DEFAULT_GRID_DISABLE                      (false)

//=======================================================================//
//= Data type declaration.											    =//
//=======================================================================//
typedef struct
{
	size_t					HorizontalPixelNumber;
    size_t					VerticalPixelNumber;
    size_t					PixelUnitWidth;
    size_t					PixelUnitHeight;
    size_t					BorderWidth;
    bool					EnableGrid;
	unsigned int			PanelColor;
	unsigned int			PixelColor;
	unsigned int			GridColor;
}PixelPanelParameter;

//=======================================================================//
//= Global variable declaration.									    =//
//=======================================================================//
extern PixelPanelParameter			g_stParameters;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void SetLCDPanelParameter(PixelPanelParameter* pstParameter);

#endif // _INCLUDE_LCD_COMMON_H_
