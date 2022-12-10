/*************************************************************************/
/** Copyright.															**/
/** FileName: wxLCDBase.cpp												**/
/** Author: Polarix														**/
/** Description: LCD display panel in wxWidgets frame.					**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/clipbrd.h>
#include "wxLCDBase.h"

//=======================================================================//
//= Marco declare.                                                      =//
//=======================================================================//

//=======================================================================//
//= Global variable define.											    =//
//=======================================================================//
const wxSize	wxDefaultSizeInPixel(128, 64);

//=======================================================================//
//= Event table.													    =//
//=======================================================================//
BEGIN_EVENT_TABLE(wxLCDBase,wxWindow)
	EVT_PAINT		        (wxLCDBase::OnPaint)
	EVT_ERASE_BACKGROUND    (wxLCDBase::OnEraseBackGround)
	EVT_KEY_DOWN			(wxLCDBase::OnKeyDown)
	EVT_SET_FOCUS			(wxLCDBase::OnSetFocus)
END_EVENT_TABLE()

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
wxLCDBase::wxLCDBase(wxWindow *pclsParent, wxWindowID iWinID, const wxPoint& clsPosition, const wxSize& clsSizeInPixel):
wxWindow(pclsParent, iWinID, clsPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER, wxPanelNameStr),
m_clsCDC(this)
{
	/*----------------------------------*/
	/* Initialize member.				*/
	/*----------------------------------*/
	// Initialize panel size.
	if(wxDefaultSize == clsSizeInPixel)
	{
		m_clsSizeInPixel.Set(WX_LCD_DEFAULT_WIDTH_PIX, WX_LCD_DEFAULT_HEIGHT_PIX);
	}
	else
	{
		m_clsSizeInPixel = clsSizeInPixel;
	}
	// Initialize paint buffer and function pointer.
	m_ppuiDisplayBuffer = nullptr;
	m_pfDrawPoint = nullptr;
	// Process initialize.
	m_bIsOK = _initialize();
}

wxLCDBase::~wxLCDBase()
{
	_freeDisplayBuffer(m_ppuiDisplayBuffer);
}

bool wxLCDBase::_initialize(void)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	bool					bReturn;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bReturn =				true;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Create display buffer.
	m_ppuiDisplayBuffer = _createNewDisplayBuffer(m_clsSizeInPixel.GetWidth(), m_clsSizeInPixel.GetHeight());
	if(NULL == m_ppuiDisplayBuffer)
	{
		// Display buffer create failed.
		bReturn = false;
	}
	else
	{
		// Set pixel size.
		SetPixelUnitSize(wxDefaultLCDPixelUnitSize);
		// Set grid visible.
		SetGridVisibled(WX_LCD_DEFAULT_GRID_VISIBLE, false);
		// Set grid color.
		SetBorderWidth(WX_LCD_BORDER_WIDTH);
	}
	return bReturn;
}

void wxLCDBase::_getBestSize(wxSize& clsBestSize) const
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	bool					bGridIsVisible;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bGridIsVisible =		GetGridVisibled();

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Set size object value.
	clsBestSize.SetWidth(m_clsSizeInPixel.GetWidth()*m_clsPixelUnitSize.GetWidth()+(bGridIsVisible?1:0)+(2*m_iBorderWidth));
	clsBestSize.SetHeight(m_clsSizeInPixel.GetHeight()*m_clsPixelUnitSize.GetHeight()+(bGridIsVisible?1:0)+(2*m_iBorderWidth));
}

void wxLCDBase::SetBorderWidth(int iBorderWidth)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	m_iBorderWidth = iBorderWidth;
	// Repaint
    RefreshDisplay();
}

void wxLCDBase::SetPixelNumber(int iHorizontalPixelNumber, int iVerticalPixelNumber)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	unsigned int**			ppuiNewDisplayBuffer;
	int                		iCopiedRowNumber;
	int						iCopiedColumnNumber;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	ppuiNewDisplayBuffer =  NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((iHorizontalPixelNumber > 0) && (iVerticalPixelNumber > 0))
    {
        // Create a new display buffer
        ppuiNewDisplayBuffer = _createNewDisplayBuffer(iHorizontalPixelNumber, iVerticalPixelNumber);
        if(NULL != ppuiNewDisplayBuffer)
        {
        	_enterPaintCriticalSection();

            iCopiedRowNumber = m_clsSizeInPixel.GetHeight()<iVerticalPixelNumber?m_clsSizeInPixel.GetHeight():iVerticalPixelNumber;
            iCopiedColumnNumber = m_clsSizeInPixel.GetWidth()<iHorizontalPixelNumber?m_clsSizeInPixel.GetWidth():iHorizontalPixelNumber;
            // Copy old buffer content to new buffer;
            for(int iIdxV=0; iIdxV<iCopiedRowNumber; iIdxV++)
            {
                memcpy(*(ppuiNewDisplayBuffer+iIdxV), *(m_ppuiDisplayBuffer+iIdxV), sizeof(unsigned int)*iCopiedColumnNumber);
            }
            // Free current buffer.
            _freeDisplayBuffer(m_ppuiDisplayBuffer);
            m_ppuiDisplayBuffer = ppuiNewDisplayBuffer;
            m_clsSizeInPixel.Set(iHorizontalPixelNumber, iVerticalPixelNumber);

            _leavePaintCriticalSection();
        }
    }
}

void wxLCDBase::GetPixelNumber(int* piHorizontalPixelNumber, int* piVerticalPixelNumber)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != piHorizontalPixelNumber)
	{
		*piHorizontalPixelNumber = m_clsSizeInPixel.GetWidth();
	}

	if(NULL != piVerticalPixelNumber)
	{
		*piVerticalPixelNumber = m_clsSizeInPixel.GetHeight();
	}
}

void wxLCDBase::SetPixelUnitSize(const wxSize clsPixelUnitSize)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Save the pixel size value.
    m_clsPixelUnitSize = clsPixelUnitSize;

    _enterPaintCriticalSection();
    // If pixel size is 0, the pixel drawing function will set to invalid.
	if((0 == m_clsPixelUnitSize.GetWidth()) || (0 == m_clsPixelUnitSize.GetHeight()))
	{
		m_pfDrawPoint = NULL;
		m_bIsOK = false;
	}
	// If pixel size is 1, the pixel drawing function will set to draw pixel unit by point.
	/*
	else if(1 == m_iPixelSize)
	{
		m_pfDrawPoint = &_drawPointSinglePixel;
	}
	else
		*/
	{
		if(true == GetGridVisibled())
		{
			m_pfDrawPoint = &_drawPointMultiplePixelWithGrid;
		}
		else
        {
            m_pfDrawPoint = &_drawPointMultiplePixel;
        }
	}
	_leavePaintCriticalSection();
}

void wxLCDBase::SetGridVisibled(bool bGridVisible, bool bRefreshNow)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Save the grid visible value set.
    if((m_clsPixelUnitSize.GetWidth() < WX_LCD_PIX_SIZE_MIN_WITH_GRID) || (m_clsPixelUnitSize.GetHeight() < WX_LCD_PIX_SIZE_MIN_WITH_GRID))
	{
		m_bGridVisible = false;
	}
	else
	{
		m_bGridVisible = bGridVisible;
	}
    _enterPaintCriticalSection();

    if(true == m_bGridVisible)
	{
		// Pixel unit size must large then WX_LCD_PIX_SIZE_MIN_WITH_GRID.
		m_pfDrawPoint = &_drawPointMultiplePixelWithGrid;
	}
	else
	{
		if((1 == m_clsPixelUnitSize.GetWidth()) && (1 == m_clsPixelUnitSize.GetHeight()))
		{
			m_pfDrawPoint = &_drawPointSinglePixel;
		}
		else
		{
			m_pfDrawPoint = &_drawPointMultiplePixel;
		}
	}
	if(true == bRefreshNow)
	{
		RefreshDisplay();
	}
	_leavePaintCriticalSection();
}

bool wxLCDBase::GetGridVisibled(void) const
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	bool				bGridVisible;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((m_clsPixelUnitSize.GetWidth() < WX_LCD_PIX_SIZE_MIN_WITH_GRID) || (m_clsPixelUnitSize.GetHeight() < WX_LCD_PIX_SIZE_MIN_WITH_GRID))
	{
		bGridVisible = false;
	}
	else
	{
		bGridVisible = m_bGridVisible;
	}

	return bGridVisible;
}

void wxLCDBase::_cleanDisplayBuffer(void)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	for(int iIdxV=0; iIdxV<m_clsSizeInPixel.GetHeight(); iIdxV++)
    {
        memset(*(m_ppuiDisplayBuffer+iIdxV), 0x00, sizeof(unsigned int)*m_clsSizeInPixel.GetWidth());
    }
	RefreshDisplay();
}

void wxLCDBase::SetDisplayBuffer(wxColour& clsPanelColour)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != m_pfDrawPoint) && (m_clsPixelUnitSize.GetWidth() > 0) && (m_clsPixelUnitSize.GetHeight()))
	{
		_enterPaintCriticalSection();

		// Cleanup display and set display buffer .
		for(int iIdxV=0; iIdxV<m_clsSizeInPixel.GetHeight(); iIdxV++)
		{
			for(int iIdxH=0; iIdxH<m_clsSizeInPixel.GetWidth(); iIdxH++)
			{
				*(*(m_ppuiDisplayBuffer+iIdxV)+iIdxH) = clsPanelColour.GetRGBA();
			}
		}

		_leavePaintCriticalSection();
	}
}

void wxLCDBase::ReplaceColour(const wxColour& clsOldColour, const wxColour& clsNewColour)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != m_pfDrawPoint) && (m_clsPixelUnitSize.GetWidth() > 0) && (m_clsPixelUnitSize.GetHeight()))
	{
		_enterPaintCriticalSection();

		// Cleanup display and set display buffer .
		for(int iIdxV=0; iIdxV<m_clsSizeInPixel.GetHeight(); iIdxV++)
		{
			for(int iIdxH=0; iIdxH<m_clsSizeInPixel.GetWidth(); iIdxH++)
			{
				if(*(*(m_ppuiDisplayBuffer+iIdxV)+iIdxH) == clsOldColour.GetRGBA())
				{
					*(*(m_ppuiDisplayBuffer+iIdxV)+iIdxH) = clsNewColour.GetRGBA();
				}
			}
		}

		_leavePaintCriticalSection();
	}
}

void wxLCDBase::SetGridColor(const wxColor& clsColor)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	m_clsGridColor = clsColor;
}

wxColor& wxLCDBase::GetGridColor(void)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    return m_clsGridColor;
}

void wxLCDBase::OnPaint(wxPaintEvent &clsEvent)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	RefreshDisplay();
}

void wxLCDBase::OnKeyDown(wxKeyEvent& clsEvent)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	clsEvent.ResumePropagation(1);
	clsEvent.Skip();
}

void wxLCDBase::OnSetFocus(wxFocusEvent& clsEvent)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	GetParent()->SetFocus();
	clsEvent.Skip();
}
void wxLCDBase::_drawPointSinglePixel(wxDC& clsDCObject, int iX, int iY, const wxSize& clsPixelSize)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	clsDCObject.DrawPoint(wxPoint(iX+m_iBorderWidth, iY+m_iBorderWidth));
}

void wxLCDBase::_drawPointMultiplePixel(wxDC& clsDCObject, int iX, int iY, const wxSize& clsPixelSize)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	clsDCObject.DrawRectangle(wxPoint(iX*m_clsPixelUnitSize.GetWidth()+m_iBorderWidth, iY*m_clsPixelUnitSize.GetHeight()+m_iBorderWidth), clsPixelSize);
}

void wxLCDBase::_drawPointMultiplePixelWithGrid(wxDC& clsDCObject, int iX, int iY, const wxSize& clsPixelSize)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	clsDCObject.DrawRectangle(wxPoint(iX*m_clsPixelUnitSize.GetWidth()+m_iBorderWidth+1, iY*m_clsPixelUnitSize.GetHeight()+m_iBorderWidth+1), wxSize(clsPixelSize.GetWidth()-1, clsPixelSize.GetHeight()-1));
}

/*************************************************************************/
/** Function Name:	SetPixelUnitColor                                   **/
/** Purpose:		Set a pixel RGBA color value.                       **/
/** Params:																**/
/**	@ iX[in]:		X-Coordinate of pixel.                          **/
/**	@ iY[in]:		Y-Coordinate of pixel.                          **/
/**	@ clsColor[in]:     Color data object.                              **/
/**	@ bRefreshNow[in]:  Refresh display at once, default to false.      **/
/** Return:			None.                                               **/
/** Notice:			This function only change the pixel color register  **/
/**                 if bRefreshNow is false(default), and when the      **/
/**                 parameter bRefreshNow is true, all pixel blocks of  **/
/**                 LCD screen panel will be repaint. if need to draw   **/
/**                 only one pixel, please use the DrawPixel function   **/
/**                 directly.                                           **/
/*************************************************************************/
void wxLCDBase::SetPixelUnitColor(int iX, int iY, const wxColor& clsColor, bool bRefreshNow)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((iX < m_clsSizeInPixel.GetWidth()) && (iY < m_clsSizeInPixel.GetHeight()))
	{
		_enterPaintCriticalSection();

		if(nullptr != m_ppuiDisplayBuffer)
		{
		    *(*(m_ppuiDisplayBuffer+iY)+iX) = clsColor.GetRGBA();
		}
		if(true == bRefreshNow)
		{
			RefreshDisplay();
		}

		_leavePaintCriticalSection();
	}
}

/*************************************************************************/
/** Function Name:	GetPixelUnitColor                                   **/
/** Purpose:		Get color of a pixel unit.                          **/
/** Params:																**/
/**	@ uiPosX[in]:       X-Coordinate of pixel.                          **/
/**	@ uiY[in]:       Y-Coordinate of pixel.                          **/
/** Return:			RGBA color value of the pixel unit.                 **/
/** Notice:			None.                                               **/
/*************************************************************************/
unsigned int wxLCDBase::GetPixelUnitColor(int iX, int iY)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint32_t		uiReturnValue = 0x00;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((iX < m_clsSizeInPixel.GetWidth()) && (iY < m_clsSizeInPixel.GetHeight()))
	{
		_enterPaintCriticalSection();
		if(nullptr != m_ppuiDisplayBuffer)
		{
			uiReturnValue = *(*(m_ppuiDisplayBuffer+iY)+iX);
		}
		_leavePaintCriticalSection();
	}

	return uiReturnValue;
}

/*************************************************************************/
/** Function Name:	DrawPixel                                           **/
/** Purpose:		Draw a pixel.                                       **/
/** Params:																**/
/**	@ iX[in]:		X-Coordinate of pixel.                          **/
/**	@ iY[in]:		Y-Coordinate of pixel.                          **/
/**	@ clsColor[in]:     Color data object.                              **/
/** Return:			None.                                               **/
/** Notice:			Draw only one pixel directly.                       **/
/*************************************************************************/
void wxLCDBase::DrawPixel(int iX, int iY, wxColor& clsColor)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((iX < m_clsSizeInPixel.GetWidth()) && (iY < m_clsSizeInPixel.GetHeight()) && (m_clsPixelUnitSize.GetWidth() != 0) && (m_clsPixelUnitSize.GetHeight() != 0))
	{
		_enterPaintCriticalSection();

		_setDCColor(clsColor);
		_prepareDC(m_clsCDC);
		if(nullptr != m_pfDrawPoint)
		{
			(this->*m_pfDrawPoint)(m_clsCDC, iX, iY, m_clsPixelUnitSize);
		}
		if(nullptr != m_ppuiDisplayBuffer)
        {
            *(*(m_ppuiDisplayBuffer+iY)+iX) = clsColor.GetRGBA();
        }
		SetPixelUnitColor(iX, iY, clsColor);
		_releaseDC(m_clsCDC);

		_leavePaintCriticalSection();
	}
}

/*************************************************************************/
/** Function Name:	RefreshDisplay                                      **/
/** Class:			wxLCDBase											**/
/** Accessibility:	Public.												**/
/** Purpose:		Repaint LCD screen panel.                           **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			Call this function after the SetPixelColor called,  **/
/**                 all pixels of the LCD panel will be repaint by the  **/
/**                 pixels's RGBA value register array.                 **/
/*************************************************************************/
void wxLCDBase::RefreshDisplay(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	int					iPaintSizeWidth, iPaintSizeHeight;
	bool				bGridVisible;
	unsigned int		uiColorRGBA;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	iPaintSizeWidth =	m_clsSizeInPixel.GetWidth()*m_clsPixelUnitSize.GetWidth()+(2*m_iBorderWidth);
	iPaintSizeHeight =	m_clsSizeInPixel.GetHeight()*m_clsPixelUnitSize.GetHeight()+(2*m_iBorderWidth);
	bGridVisible = GetGridVisibled();

	// Set buffer size.
	if(true == bGridVisible)
	{
		iPaintSizeWidth++;
		iPaintSizeHeight++;
	}

	_enterPaintCriticalSection();
	// Create buffer image and DC object.
	wxBitmap			clsBufferImage(iPaintSizeWidth, iPaintSizeHeight);
	wxBufferedDC		clsBufferedDC(&m_clsCDC, clsBufferImage);

	// Clear background for grid line.
	if(true == bGridVisible)
	{
		_setDCColor(m_clsGridColor);
		_prepareDC(clsBufferedDC);
		clsBufferedDC.DrawRectangle(wxPoint(m_iBorderWidth, m_iBorderWidth),
							wxSize(	m_clsSizeInPixel.GetWidth()*m_clsPixelUnitSize.GetWidth()+1,
									m_clsSizeInPixel.GetHeight()*m_clsPixelUnitSize.GetHeight()+1));
	}

	// Paint pixel.
	if(nullptr != m_pfDrawPoint)
	{
		// Refresh each pixel on screen.
		for(int i_H=0; i_H<m_clsSizeInPixel.GetHeight(); i_H++)
		{
			for(int i_W=0; i_W<m_clsSizeInPixel.GetWidth(); i_W++)
			{
				uiColorRGBA = *(*(m_ppuiDisplayBuffer+i_H)+i_W);
				_setDCColor(wxColor(uiColorRGBA));
				_prepareDC(clsBufferedDC);
				(this->*m_pfDrawPoint)(clsBufferedDC, i_W, i_H, m_clsPixelUnitSize);
			}
		}
	}

	_leavePaintCriticalSection();
}

/*************************************************************************/
/** Function Name:	_getLCDPanelImage									**/
/** Class:			wxLCDBase											**/
/** Accessibility:	Private.											**/
/** Purpose:		Copy LCD panel image to bitmap object.				**/
/** Resources:		None.												**/
/** Params:																**/
/** @ clsBitmap[in/out]: Out put bitmap object.							**/
/** Return:																**/
/** @ true:				Copy image successfully.						**/
/**	@ false:			Operation failed.								**/
/** Notice:			Size and depth of Bitmap object must set before		**/
/**					this function called.								**/
/*************************************************************************/
bool wxLCDBase::_getLCDPanelImage(wxBitmap& clsBitmap)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	wxMemoryDC				clsMemoryDC(clsBitmap);
	bool                    bReturn;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bReturn =               false;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	bReturn = clsMemoryDC.Blit(wxPoint(0, 0), GetSize(), &m_clsCDC, wxPoint(0, 0));
	return bReturn;
}

/*************************************************************************/
/** Function Name:	SaveScreenImageToFile                               **/
/** Class:			wxLCDBase											**/
/** Accessibility:	Public.												**/
/** Purpose:		Save current panel image to a jpeg file.            **/
/** Params:			                                                    **/
/**	@ strFilePath[in]:  Save file path.                                 **/
/** Return:                                                             **/
/** @ true:             Save successfully.                              **/
/** @ false:            Save failed.                                    **/
/** Notice:			To support jpeg format, need add wxWidgets jpeg     **/
/**                 format handler in initialize(OnInit) function of    **/
/**                 the wxApp Object.                                   **/
/**                 wxImage::AddHandler(new wxJPEGHandler)              **/
/*************************************************************************/
bool wxLCDBase::SaveImage(const wxString& strFilePath)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	wxBitmap				clsBitMap(GetSize(), wxBITMAP_SCREEN_DEPTH);
	bool                    bReturn;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bReturn =               false;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	bReturn = _getLCDPanelImage(clsBitMap);
	if(true == bReturn)
	{
		bReturn = clsBitMap.SaveFile(strFilePath, wxBITMAP_TYPE_JPEG);
	}

	return bReturn;
}

/*************************************************************************/
/** Function Name:	CopyScreenImageToClipBoard                          **/
/** Class:			wxLCDBase											**/
/** Accessibility:	Public.												**/
/** Purpose:		Copy current screen image to clip board.            **/
/** Params:			None.                                               **/
/** Return:                                                             **/
/** @ true:             Copy successfully.                              **/
/** @ false:            Copy failed.                                    **/
/** Notice:			None.                                               **/
/*************************************************************************/
bool wxLCDBase::CopyImage(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	wxBitmap				clsBitMap(GetSize(), wxBITMAP_SCREEN_DEPTH);
	bool                    bReturn;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bReturn =               true;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(true == wxTheClipboard->Open())
	{
		do
		{
			bReturn = _getLCDPanelImage(clsBitMap);
			if(false == bReturn)
			{
                // Ger LCD panel image failed.
                break;
			}

			bReturn = wxClipboard::Get()->SetData(new wxBitmapDataObject(clsBitMap));
			if(false == bReturn)
			{
                // Set data to clipboard failed.
                break;
			}
			wxTheClipboard->Flush();
		}while(0);
		wxTheClipboard->Close();
	}
	else
	{
		bReturn = false;
	}
	return bReturn;
}

/*************************************************************************/
/** Function Name:	_freeDisplayBuffer									**/
/** Class:			wxLCDBase											**/
/** Accessibility:	Private.											**/
/** Purpose:		Release display buffer memory resource.				**/
/** Resources:		None.												**/
/** Params:																**/
/** @ ppuiDisplayBuffer: Display buffer pointer.						**/
/** Return:			None.												**/
/** Notice:			Private function, called when need to release a 	**/
/**					existed display buffer. just like object destroy or **/
/**					size in pixel changed.								**/
/*************************************************************************/
void wxLCDBase::_freeDisplayBuffer(uint32_t** ppuiDisplayBuffer)
{
    if(nullptr != ppuiDisplayBuffer)
	{
	    for(int iIdxV=0; iIdxV<m_clsSizeInPixel.GetHeight(); iIdxV++)
        {
            free(*(ppuiDisplayBuffer+iIdxV));
        }
        free(ppuiDisplayBuffer);
	}
}

/*************************************************************************/
/** Function Name:	_createNewDisplayBuffer								**/
/** Class:			wxLCDBase											**/
/** Accessibility:	Private.											**/
/** Purpose:		Allocate new display buffer.						**/
/** Resources:		None.												**/
/** Params:																**/
/** @ iHorizontalPixelNumber: The number of pixels in the horizontal	**/
/**					direction.											**/
/** @ iVerticalPixelNumber: The number of pixels in the vertical		**/
/**					direction.											**/
/** Return:			New display buffer heap memory pointer.				**/
/** Notice:			Private function, called when need to create new 	**/
/**					display buffer. just like object create or size in	**/
/**					pixel changed.										**/
/*************************************************************************/
unsigned int** wxLCDBase::_createNewDisplayBuffer(int iHorizontalPixelNumber, int iVerticalPixelNumber)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	unsigned int**			ppuiNewDisplayBuffer;
	unsigned int*			puiNewHorizontalPixelBuffer;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	ppuiNewDisplayBuffer =  nullptr;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/

	if((iHorizontalPixelNumber > 0) && (iVerticalPixelNumber > 0))
    {
        ppuiNewDisplayBuffer = (unsigned int**)malloc(sizeof(unsigned int*)*iVerticalPixelNumber);
        if(nullptr != ppuiNewDisplayBuffer)
        {
            memset(ppuiNewDisplayBuffer, 0x00, sizeof(unsigned int*)*iVerticalPixelNumber);
            for(int iIdxV=0; iIdxV<iVerticalPixelNumber; iIdxV++)
            {
                puiNewHorizontalPixelBuffer = (unsigned int*)malloc(sizeof(unsigned int)*iHorizontalPixelNumber);
                if(nullptr != puiNewHorizontalPixelBuffer)
                {
                    memset(puiNewHorizontalPixelBuffer, 0x00, sizeof(unsigned int)*iHorizontalPixelNumber);
                    *(ppuiNewDisplayBuffer+iIdxV) = puiNewHorizontalPixelBuffer;
                }
                else
                {
                    _freeDisplayBuffer(ppuiNewDisplayBuffer);
                    ppuiNewDisplayBuffer = nullptr;
                    break;
                }
            }
        }
    }

    return ppuiNewDisplayBuffer;
}

/*************************************************************************/
/** Function Name:	DoGetBestClientSize									**/
/** Class:			wxLCDBase											**/
/** Accessibility:	Protected.											**/
/** Purpose:		Get best display size with current size in pixel	**/
/**					unit size, called by base class function.			**/
/** Resources:		None.												**/
/** Params:			None.												**/
/** Return:			Size object.										**/
/** Notice:			This function will be call when user call base		**/
/**					class function GetBestSize.							**/
/*************************************************************************/
wxSize wxLCDBase::DoGetBestClientSize(void) const
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	wxSize					clsBestSize;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	_getBestSize(clsBestSize);

	return clsBestSize;
}


