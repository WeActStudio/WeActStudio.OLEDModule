/*************************************************************************/
/** Copyright.															**/
/** FileName: Common.cpp												**/
/** Author: Polarix														**/
/** Description: Common data and functions define.						**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "Common.h"

PixelPanelParameter			g_stParameters;


void SetLCDPanelParameter(PixelPanelParameter* pstParameter)
{
	if(NULL != pstParameter)
	{
		// Appearance
		pstParameter->HorizontalPixelNumber = PARAM_DEFAULT_PIXEL_NUM_H;
		pstParameter->VerticalPixelNumber = PARAM_DEFAULT_PIXEL_NUM_V;
		pstParameter->PixelUnitWidth = PARAM_DEFAULT_PIXEL_WIDTH;
		pstParameter->PixelUnitHeight = PARAM_DEFAULT_PIXEL_HEIGHT;
		pstParameter->EnableGrid = PARAM_DEFAULT_GRID_ENABLE;
		pstParameter->BorderWidth = PARAM_DEFAULT_PANEL_BORDER_WIDTH;
		// ScreenColor
		pstParameter->PanelColor = LCD_COLOR_OBJ_BKG;
		pstParameter->PixelColor = LCD_COLOR_OBJ_PIX;
		pstParameter->GridColor = LCD_COLOR_OBJ_GRID;
	}
}
