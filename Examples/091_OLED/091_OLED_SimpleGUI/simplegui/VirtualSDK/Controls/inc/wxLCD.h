#ifndef _INCLUDE_CLASS_WXEX_LCD_H_
#define _INCLUDE_CLASS_WXEX_LCD_H_

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "wxLCDBase.h"
#include "Common.h"

//=======================================================================//
//= Class declare.                                                      =//
//=======================================================================//
class wxLCD: public wxLCDBase
{
	//DECLARE_EVENT_TABLE();

	private:
		wxColour				m_clsPixelHColour;
		wxColour				m_clsPixelLColour;

	protected:

	public:
		// Constructor/Destructor
								wxLCD(wxWindow *pclsParent, wxWindowID iWinID = wxID_ANY, const wxPoint& clsPosition = wxDefaultPosition, const wxSize& clsSizeInPixel = wxDefaultSizeInPixel);
								~wxLCD(void);
		int						GetPixel(const int iX, const int iY);
		void					SetPixel(const int iX, const int iY, const int iValue);
		void					CleanScreen(void);
		void					SetPanelColour(const wxColour& clsPanelColour, bool bRefreshNow = true);
		void					SetPixelColour(const wxColour& clsPixelColour, bool bRefreshNow = true);

		// Prepare to remove.
		void				    SetParameter(PixelPanelParameter* pstPanelParameter);
};

#endif // _INCLUDE_CLASS_WXEX_LCD_H_
