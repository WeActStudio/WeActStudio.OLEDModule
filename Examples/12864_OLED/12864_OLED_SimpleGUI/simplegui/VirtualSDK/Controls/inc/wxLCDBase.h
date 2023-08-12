#ifndef _INCLUDE_CLASS_WXEX_LCD_BASE_H_
#define _INCLUDE_CLASS_WXEX_LCD_BASE_H_

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx\panel.h>
#include <wx\dcbuffer.h>
#include <wx\colour.h>

//=======================================================================//
//= Marco declare.                                                      =//
//=======================================================================//
#define WX_LCD_DEFAULT_WIDTH_PIX					(128)
#define WX_LCD_DEFAULT_HEIGHT_PIX					(64)
#define WX_LCD_DEFAULT_GRID_VISIBLE					(true)
#define WX_LCD_PIX_SIZE_MIN							(3)
#define WX_LCD_PIX_SIZE_MIN_WITH_GRID				(4)
#define WX_LCD_BORDER_WIDTH							(5)
#define WX_LCD_PIX_RGB(RGBA)						((0x00FFFFFF) & (RGBA))
#define wxDefaultLCDPixelUnitSize					(wxSize(2, 2))

//=======================================================================//
//= Global variable declare.										    =//
//=======================================================================//
extern const wxSize	wxDefaultSizeInPixel;

//=======================================================================//
//= Class declare.                                                      =//
//=======================================================================//
class wxLCDBase
: public wxWindow
{
    wxDECLARE_NO_COPY_CLASS(wxLCDBase);
	wxDECLARE_EVENT_TABLE();

	private:
		wxClientDC				m_clsCDC;
		wxPen					m_clsPen;
		wxBrush					m_clsBrush;
		wxColour				m_clsGridColor;
		wxSize					m_clsSizeInPixel;
		wxSize					m_clsPixelUnitSize;
		bool					m_bGridVisible;
		unsigned int**			m_ppuiDisplayBuffer;
		wxCriticalSection		m_clsDisplayBufferCS;
		void					(wxLCDBase::*m_pfDrawPoint)(wxDC& clsDCObject, int iX, int iY, const wxSize& clsPixelSize);
		bool					m_bIsOK;
		int						m_iBorderWidth;

		bool					_initialize(void);
		void					_getBestSize(wxSize& clsBestSize) const;
		inline void				_drawPointSinglePixel(wxDC& clsDCObject, int iX, int iY, const wxSize& clsPixelSize);
		inline void				_drawPointMultiplePixel(wxDC& clsDCObject, int iX, int iY, const wxSize& clsPixelSize);
		inline void				_drawPointMultiplePixelWithGrid(wxDC& clsDCObject, int iX, int uiY, const wxSize& clsPixelSize);
		inline void				_releaseDC(wxMemoryDC& clsCDCObject)		{clsCDCObject.SetBrush(wxNullBrush); clsCDCObject.SetPen(wxNullPen);}
		inline void				_prepareDC(wxMemoryDC& clsCDCObject)		{clsCDCObject.SetBrush(m_clsBrush); clsCDCObject.SetPen(m_clsPen);}
		inline void				_releaseDC(wxClientDC& clsCDCObject)		{clsCDCObject.SetBrush(wxNullBrush); clsCDCObject.SetPen(wxNullPen);}
		inline void				_prepareDC(wxClientDC& clsCDCObject)		{clsCDCObject.SetBrush(m_clsBrush); clsCDCObject.SetPen(m_clsPen);}
		inline void				_setDCColor(const wxColour& clsColour)		{m_clsPen.SetColour(clsColour);m_clsBrush.SetColour(clsColour);}
		void         			_freeDisplayBuffer(unsigned int** ppuiDisplayBuffer);
		unsigned int**			_createNewDisplayBuffer(int iHorizontalPixelNumber, int iVerticalPixelNumber);
		void					_cleanDisplayBuffer(void);
		bool					_getLCDPanelImage(wxBitmap& clsBitmap);
		void					_enterPaintCriticalSection(void)						{m_clsDisplayBufferCS.Enter();}
		void					_leavePaintCriticalSection(void)						{m_clsDisplayBufferCS.Leave();}

	protected:
		// Event callback function.
		virtual void			OnPaint(wxPaintEvent &clsEvent);
		virtual void			OnEraseBackGround(wxEraseEvent &clsEvent)	{/* Do nothing. */}
		virtual void			OnKeyDown(wxKeyEvent& clsEvent);
		virtual void			OnSetFocus(wxFocusEvent& clsEvent);
		virtual wxSize			DoGetBestClientSize(void) const;

	public:
		// Constructor/Destructor
								wxLCDBase(wxWindow *pclsParent, wxWindowID iWinID = wxID_ANY, const wxPoint& clsPosition = wxDefaultPosition, const wxSize& clsSizeInPixel = wxDefaultSizeInPixel);
								~wxLCDBase();
		// Public interface
		void					SetBorderWidth(int iBorderWidth);
		int						GetBorderWidth(void)						{return m_iBorderWidth;}
		void					SetPixelNumber(int iHorizontalPixelNumber, int iVerticalPixelNumber);
		void					GetPixelNumber(int* piHorizontalPixelNumber, int* piVerticalPixelNumber);
		void					SetPixelUnitSize(const wxSize clsPixelUnitSize);
		wxSize&					GetPixelUnitSize(void)						{return m_clsPixelUnitSize;}
		void					SetGridVisibled(bool bGridVisible, bool bRefreshNow = true);
		bool					GetGridVisibled(void) const;
		void					SetGridColor(const wxColor& clsColor);
		wxColor&				GetGridColor(void);
		void					SetPixelUnitColor(int iX, int iY, const wxColor& clsColor, bool bRefreshNow = false);
		void					DrawPixel(int iX, int iY, wxColor& clsColor);
		unsigned int			GetPixelUnitColor(int iX, int iY);
		void					RefreshDisplay(void);
		void					SetDisplayBuffer(wxColour& clsNewColour);
		void					ReplaceColour(const wxColour& clsOldColour, const wxColour& clsNewColour);
		bool					SaveImage(const wxString& strFilePath);
		bool					CopyImage(void);
		bool					Create(wxWindow *pclsParent, wxWindowID iWinID, const wxPoint& clsPosition);

		bool					IsOK(void)									{return m_bIsOK;}
};

#endif // _INCLUDE_CLASS_WXEX_LCD_BASE_H_
