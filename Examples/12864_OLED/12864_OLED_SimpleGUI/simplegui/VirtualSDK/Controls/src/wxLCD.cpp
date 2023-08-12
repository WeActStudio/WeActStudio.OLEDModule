/*************************************************************************/
/** Copyright.															**/
/** FileName: wxLCD.cpp													**/
/** Author: Polarix														**/
/** Description: LCD display panel in wxWidgets frame.					**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "wxLCD.h"

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
wxLCD::wxLCD(wxWindow *pclsParent, wxWindowID iWinID, const wxPoint& clsPosition, const wxSize& clsSizeInPixel):
wxLCDBase(pclsParent, iWinID, clsPosition, clsSizeInPixel)
{
	m_clsPixelLColour.Set(0x00000000);
	m_clsPixelHColour.Set(0x00000000);
}

wxLCD::~wxLCD(void)
{

}

void wxLCD::SetPanelColour(const wxColour& clsPanelColour, bool bRefreshNow)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Replace old panel color.
	ReplaceColour(m_clsPixelLColour, clsPanelColour);
	// Set new panel color.
	m_clsPixelLColour = clsPanelColour;
	// Refresh display.
	if(true == bRefreshNow)
	{
		RefreshDisplay();
	}
}

void wxLCD::SetPixelColour(const wxColour& clsPixelColour, bool bRefreshNow)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Replace old pixel color.
	ReplaceColour(m_clsPixelHColour, clsPixelColour);
	// Set new pixel color.
	m_clsPixelHColour = clsPixelColour;
	// Refresh display.
	if(true == bRefreshNow)
	{
		RefreshDisplay();
	}
}

int wxLCD::GetPixel(const int iX, const int iY)
{
	int				iReturn;

	if(WX_LCD_PIX_RGB(GetPixelUnitColor(iX, iY)) == WX_LCD_PIX_RGB(m_clsPixelHColour.GetRGBA()))
	{
		iReturn = 1;
	}
	else
	{
		iReturn = 0;
	}

	return iReturn;
}

void wxLCD::SetPixel(const int iX, const int iY, const int iValue)
{
	if(1 == iValue)
	{
		SetPixelUnitColor(iX, iY, m_clsPixelHColour);
	}
	else
	{
		SetPixelUnitColor(iX, iY, m_clsPixelLColour);
	}
}

void wxLCD::CleanScreen(void)
{
	SetDisplayBuffer(m_clsPixelLColour);
}

void wxLCD::SetParameter(PixelPanelParameter* pstPanelParameter)
{
	if(NULL != pstPanelParameter)
	{
		SetPixelUnitSize(wxSize(pstPanelParameter->PixelUnitWidth, pstPanelParameter->PixelUnitHeight));
	    SetGridVisibled(pstPanelParameter->EnableGrid);
		SetPixelNumber(pstPanelParameter->HorizontalPixelNumber, pstPanelParameter->VerticalPixelNumber);
		SetBorderWidth(pstPanelParameter->BorderWidth);

		SetPixelColour(wxColor(pstPanelParameter->PixelColor), false);
		SetPanelColour(wxColor(pstPanelParameter->PanelColor), false);
		SetGridColor(wxColor(pstPanelParameter->GridColor));

		Layout();
	}
}
