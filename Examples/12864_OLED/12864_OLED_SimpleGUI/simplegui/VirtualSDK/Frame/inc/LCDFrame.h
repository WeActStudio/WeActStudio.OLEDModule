#ifndef __INCLUDE_CLASS_LCD_FRAME_H__
#define __INCLUDE_CLASS_LCD_FRAME_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/colour.h>
#include <wx/string.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/clipbrd.h>
#include <wx/filefn.h>
#include <wx/timer.h>
#include <wx/sizer.h>

#include "wxLCD.h"
#include "CustomEvents.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define FRAME_TITLE						(_T("SimpleGUI Simulator"))

enum
{
    ID_MAIN_FRAME = wxID_HIGHEST + 1,
    ID_TOOLSBAR,
    ID_TOOL_QUICKSHOTS,
    ID_TOOL_COPY,
    ID_TOOL_OPEN_FOLDER,
    ID_TOOL_ABOUT,
    ID_TOOL_EXIT,
    ID_STATUSBAR,
    ID_LCD_PANEL,
    ID_GENERAL_TIMER,
    ID_SECOND_TIMER,
};


#define SCREENSHOTS_FOLDER_T			"ScreenShots"
#define SCREENSHOTS_FILE_NAME_T			"LCD_%04u%02u%02u_%02u%02u-%u.bmp"
#define SCREENSHOTS_FILE_FULLNAME_T		wxString::Format("%s\\%s", SCREENSHOTS_FOLDER_T, SCREENSHOTS_FILE_NAME_T)
#define N_YEAR							wxDateTime::Now().GetYear()
#define N_MONTH							wxDateTime::Now().GetMonth()
#define N_DAY							wxDateTime::Now().GetDay()
#define N_HOUR							wxDateTime::Now().GetHour()
#define N_MINUTE						wxDateTime::Now().GetMinute()
#define SCREENSHOTS_FOLDER				_T(SCREENSHOTS_FOLDER_T)
#define SCREENSHOTS_FILE_NAME(IDX)		wxString::Format(SCREENSHOTS_FILE_NAME_T, N_YEAR, N_MONTH, N_DAY, N_HOUR, N_MINUTE, IDX)
#define SCREENSHOTS_FILE_FULLNAME(IDX)	wxString::Format(SCREENSHOTS_FILE_FULLNAME_T, N_YEAR, N_MONTH, N_DAY, N_HOUR, N_MINUTE, IDX)

//=======================================================================//
//= Class declare.                                                      =//
//=======================================================================//
class LCDFrame
: public wxFrame
, public wxThreadHelper
{
	wxDECLARE_NO_COPY_CLASS(LCDFrame);
	wxDECLARE_EVENT_TABLE();

	private:
	    // Controlers
		wxStatusBar*                m_pclsCtrlStatusBar;
		wxToolBar*                  m_pclsCtrlToolBar;
		wxLCD*                      m_pclsCtrlPaintPanel;
		wxTimer*                    m_pclsGeneralTimer;
        wxTimer*                    m_pclsSecondTimer;
        static LCDFrame*            m_pclsInstance;

		// Members.
		void						_createToolbar(void);
		void					    _setStatusText(const wxString& strString);

	protected:
		virtual void                Close(void);
		virtual void				OnKeyDown(wxKeyEvent& clsEvent);
		virtual void				OnMouseEvent(wxMouseEvent& clsEvent);
		virtual void				Screenshots(void);
		virtual void				Copy(void);
		virtual void				OpenScreenshotsFolder(void);
		virtual void				OnGeneralTimerTrigger(wxTimerEvent& clsEvent);
		virtual void				OnSecondTimerTrigger(wxTimerEvent& clsEvent);
		virtual void				OnSDKInitialize(InitEvent& clsEvent);
		virtual void				OnSDKGeneralTimerSet(TimerSetEvent& clsEvent);
		virtual void				OnSecondTimerEnabled(RTCSwitchEvent& clsEvent);
        virtual void                OnClose(wxCloseEvent& clsEvent)							{Close();}
		virtual void                OnScreenshots(wxCommandEvent& clsEvent)					{Screenshots();}
		virtual void                OnToolCopy(wxCommandEvent& clsEvent)					{Copy();}
		virtual void                OnOpenScreenshotsFolder(wxCommandEvent &clsEvent)		{OpenScreenshotsFolder();}
		virtual void                OnAbout(wxCommandEvent& clsEvent)						{/* Do noting */;}
		virtual void                OnToolClose(wxCommandEvent& clsEvent)					{Close();}
		virtual	wxThread::ExitCode  Entry(void);
	public:
									LCDFrame(wxWindow* pclsParent = nullptr);

									~LCDFrame(void);
        static LCDFrame*        	GetInstance(void);
        void                    	SetLCDPixel(uint32_t uiPosX, uint32_t uiY, uint32_t uiPixelValue);
        uint32_t                	GetLCDPixel(uint32_t uiPosX, uint32_t uiY);
        void                    	RefreshLCD(void);
        void						ClearLCD(void);
        bool						StartDummyMain(void);

};

#endif //__INCLUDE_CLASS_LCD_FRAME_H__
