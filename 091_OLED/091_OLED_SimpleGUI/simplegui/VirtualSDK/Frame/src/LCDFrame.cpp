/*************************************************************************/
/** Copyright.															**/
/** FileName: LCDFrame.cpp											    **/
/** Author: Polarix														**/
/** Description: Main frame class define.                               **/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx/msgdlg.h>
#include <wx/app.h>
#include "LCDFrame.h"
#include "SDKInterface.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define DUMMY_MAIN_START_DELAY_MS				(100)
#define MOUSE_WHELL_DELTA						(100)
#define MOUSE_WHELL_UP_DELTA					(MOUSE_WHELL_DELTA)
#define MOUSE_WHELL_DOWN_DELTA					(-MOUSE_WHELL_DELTA)

//=======================================================================//
//= Static class member define.			    						    =//
//=======================================================================//
LCDFrame* LCDFrame::m_pclsInstance = NULL;

//=======================================================================//
//= Event table.													    =//
//=======================================================================//
BEGIN_EVENT_TABLE(LCDFrame,wxFrame)
    EVT_CLOSE			(                       LCDFrame::OnClose)
    EVT_KEY_DOWN		(                       LCDFrame::OnKeyDown)
    EVT_MOUSE_EVENTS    (                       LCDFrame::OnMouseEvent)
    EVT_TOOL			(ID_TOOL_QUICKSHOTS,    LCDFrame::OnScreenshots)
    EVT_TOOL			(ID_TOOL_COPY,          LCDFrame::OnToolCopy)
    EVT_TOOL			(ID_TOOL_OPEN_FOLDER,   LCDFrame::OnOpenScreenshotsFolder)
    EVT_TOOL			(ID_TOOL_EXIT,          LCDFrame::OnToolClose)
    EVT_TIMER			(ID_GENERAL_TIMER,      LCDFrame::OnGeneralTimerTrigger)
    EVT_TIMER			(ID_SECOND_TIMER,       LCDFrame::OnSecondTimerTrigger)
    EVT_SDK_INIT		(                       LCDFrame::OnSDKInitialize)
    EVT_SDK_SYSTICK_SET	(                       LCDFrame::OnSDKGeneralTimerSet)
    EVT_SDK_RTC_EN		(                       LCDFrame::OnSecondTimerEnabled)
END_EVENT_TABLE()

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	LCDFrame                                            **/
/** Purpose:		Constructor of main frame object.                   **/
/** Params:																**/
/**	@ pclsParent[in]:   Parent object pointer.                          **/
/**	@ iID[in]:          Frame object ID.                                **/
/**	@ strTitle[in]:     Frame window title.                             **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
LCDFrame::LCDFrame(wxWindow* pclsParent)
: wxFrame(pclsParent, ID_MAIN_FRAME, FRAME_TITLE, wxDefaultPosition, wxDefaultSize, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
, wxThreadHelper(wxTHREAD_JOINABLE)
{
	SetEvtHandlerEnabled(false);
	SetSizeHints( wxDefaultSize, wxDefaultSize );
	// Set frame icon.
	SetIcon(wxIcon(_T("ID_ICON_MAIN_FRAME"), wxBITMAP_TYPE_ICO_RESOURCE));
	// Create tools bar and tool button.
    _createToolbar();
    // Create status bar.
    m_pclsCtrlStatusBar = CreateStatusBar(1, wxST_SIZEGRIP, ID_STATUSBAR);
    // Create LCD screen panel.
	m_pclsCtrlPaintPanel = new wxLCD(this, ID_LCD_PANEL);
    // Set frame object position on monitor.
	Centre( wxBOTH );
	// Update frame object UI.
	UpdateWindowUI();
	// Set status bar text.
	_setStatusText(_TRANS_TEXT("Preparing..."));

	if(false == wxDirExists(SCREENSHOTS_FOLDER))
	{
        wxMkdir(SCREENSHOTS_FOLDER);
	}
}

/*************************************************************************/
/** Function Name:	~LCDFrame                                           **/
/** Purpose:		Destructor of main frame object.                    **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
LCDFrame::~LCDFrame()
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	delete m_pclsCtrlStatusBar;
	delete m_pclsCtrlToolBar;
	delete m_pclsCtrlPaintPanel;
}

/*************************************************************************/
/** Function Name:	_setStatusText                                      **/
/** Purpose:		Set frame status bar text.                          **/
/** Params:																**/
/**	@ strString[in]:    Set text.                                       **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::_setStatusText(const wxString& strString)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    m_pclsCtrlStatusBar->SetStatusText(strString, 0);
}

/*************************************************************************/
/** Function Name:	_createToolbar                                      **/
/** Purpose:		Create main frame tool bar.                         **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::_createToolbar(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	wxToolBar*					pclsNewToolBar;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pclsNewToolBar				= NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    // Create tools bar.
    pclsNewToolBar = CreateToolBar(wxTB_HORIZONTAL, ID_TOOLSBAR);
    if(NULL != pclsNewToolBar)
    {
        // Screen shot button.
        pclsNewToolBar->AddTool(ID_TOOL_QUICKSHOTS, _TRANS_TEXT("Quick Shot"),
                                wxBitmap(_T("ID_TOOL_QUICKSHOTS"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxBitmap(_T("ID_TOOL_QUICKSHOTS"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxITEM_NORMAL,
                                _TRANS_TEXT("Quick Shot"),
                                _TRANS_TEXT("Quick save screen shot to file."));

        pclsNewToolBar->AddTool(ID_TOOL_COPY, _TRANS_TEXT("Copy Screen Image"),
                                wxBitmap(_T("ID_TOOL_COPYSCREENSHOT"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxBitmap(_T("ID_TOOL_COPYSCREENSHOT"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxITEM_NORMAL,
                                _TRANS_TEXT("Copy Screen Image"),
                                _TRANS_TEXT("Copy screenshots picture to clipboard."));


        pclsNewToolBar->AddTool(ID_TOOL_OPEN_FOLDER, _TRANS_TEXT("Open Screenshots Folder"),
                                wxBitmap(_T("ID_TOOL_OPENSCREENSHOTSFOLDER"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxBitmap(_T("ID_TOOL_OPENSCREENSHOTSFOLDER"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxITEM_NORMAL,
                                _TRANS_TEXT("Open Screenshots Folder"),
                                _TRANS_TEXT("Open screenshots saved folder."));
        // Add a separator.
        pclsNewToolBar->AddSeparator();

        pclsNewToolBar->AddTool(ID_TOOL_EXIT, _TRANS_TEXT("Exit"),
                                wxBitmap(_T("ID_TOOL_EXIT"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxBitmap(_T("ID_TOOL_EXIT"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxITEM_NORMAL,
                                _TRANS_TEXT("Exit(Alt+F4)"),
                                _TRANS_TEXT("Exit."));

        pclsNewToolBar->Realize();
    }
	m_pclsCtrlToolBar = pclsNewToolBar;
}

/*************************************************************************/
/** Function Name:	OnKeyDown                                           **/
/** Purpose:		Key press event process.                            **/
/** Params:																**/
/**	@ clsEvent[in]:		Event data object.								**/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::OnKeyDown(wxKeyEvent& clsEvent)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    SDK_KB_EVENT                stSDKEvent;

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Union option key value to key code.
	stSDKEvent.iKeyCode = clsEvent.GetKeyCode();
	stSDKEvent.bShift = clsEvent.ShiftDown();
	stSDKEvent.bAlt = clsEvent.AltDown();
	stSDKEvent.bCtrl = clsEvent.ControlDown();
	// Set key code and sync flag.
	SGUI_SDK_SyncKeyEventData(&stSDKEvent);
	SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_KEY_EVENT, true);
	SGUI_SDK_KeyboardHandler();
	clsEvent.Skip();
}

/*************************************************************************/
/** Function Name:	OnMouseEvent										**/
/** Purpose:		Mouse event process.								**/
/** Params:																**/
/**	@ clsEvent[in]:		Event data object.								**/
/** Return:			None.                                               **/
/** Notice:			Used to process mouse wheel scroll event, process	**/
/**					same as up/down key pressed.						**/
/*************************************************************************/
void LCDFrame::OnMouseEvent(wxMouseEvent& clsEvent)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    int                     iMouseWhellDelta;
    SDK_KB_EVENT            stEvent = {0x00};

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Process wheel scroll event as Up/Down key press.
	iMouseWhellDelta = clsEvent.GetWheelRotation();
	if(0 != iMouseWhellDelta)
	{
	    stEvent.iKeyCode = WXK_NONE;
		// Same as Down key pressed.
		if(iMouseWhellDelta < MOUSE_WHELL_DOWN_DELTA)
		{
			stEvent.iKeyCode = WXK_DOWN;
		}
		// Same as Up key pressed.
		if(iMouseWhellDelta > MOUSE_WHELL_UP_DELTA)
		{
			stEvent.iKeyCode = WXK_UP;
		}
		if(WXK_NONE != stEvent.iKeyCode)
		{
			SGUI_SDK_SyncKeyEventData(&stEvent);
			SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_KEY_EVENT, true);
		}
	}
}

/*************************************************************************/
/** Function Name:	Copy                                                **/
/** Purpose:		Copy screen image to clipboard.                     **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::Copy(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	bool                        bResult;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	bResult = m_pclsCtrlPaintPanel->CopyImage();
	if(true == bResult)
	{
		_setStatusText(_TRANS_TEXT("Copied to clipboard."));
	}
	else
	{
		_setStatusText(_TRANS_TEXT("Copy failed."));
	}

}

/*************************************************************************/
/** Function Name:	Screenshots                                         **/
/** Purpose:		Save now time screen image to a bit map file.       **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::Screenshots(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint32_t                    uiFileCounter;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiFileCounter =             1;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Check image file existed and
	while(true == wxFileExists(SCREENSHOTS_FILE_FULLNAME(uiFileCounter)))
	{
		uiFileCounter++;
	}
    // Try to save image file.
	if(true == m_pclsCtrlPaintPanel->SaveImage(SCREENSHOTS_FILE_FULLNAME(uiFileCounter)))
	{
		_setStatusText(wxString::Format(_TRANS_TEXT("Save screen to %s."), SCREENSHOTS_FILE_NAME(uiFileCounter)));
	}
	else
	{
		_setStatusText(_TRANS_TEXT("Save screen failed."));
	}
}

/*************************************************************************/
/** Function Name:	OpenScreenshotsFolder                               **/
/** Purpose:		Open screen shots image file folder.                **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::OpenScreenshotsFolder(void)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    wxExecute(wxString::Format(_TRANS_TEXT("explorer %s\\%s"), wxGetCwd(), _T(SCREENSHOTS_FOLDER_T)));
}

/*************************************************************************/
/** Function Name:	Close                                               **/
/** Purpose:		Called when frame close and object destroyed.       **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::Close(void)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	wxThread*				pclsThread;

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != m_pclsGeneralTimer)
    {
        m_pclsGeneralTimer->Stop();
        delete m_pclsGeneralTimer;
    }
    if(NULL != m_pclsSecondTimer)
    {
        m_pclsSecondTimer->Stop();
        delete m_pclsSecondTimer;
    }

    // Stop thread helper.
	pclsThread = GetThread();
	if(NULL != pclsThread)
	{
		pclsThread->Kill();
        if(true == pclsThread->IsRunning())
		{
			pclsThread->Wait(wxTHREAD_WAIT_DEFAULT);
		}
	}

    Destroy();
}

/*************************************************************************/
/** Function Name:	RefreshLCD                                          **/
/** Purpose:		Refresh screen display by pixel color data.         **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::RefreshLCD(void)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    m_pclsCtrlPaintPanel->RefreshDisplay();
}

/*************************************************************************/
/** Function Name:	ClearLCD                                            **/
/** Purpose:		Clear screen display.                               **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::ClearLCD(void)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    m_pclsCtrlPaintPanel->CleanScreen();
}

/*************************************************************************/
/** Function Name:	GetInstance                                         **/
/** Purpose:		Thread end event process.                           **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
LCDFrame* LCDFrame::GetInstance(void)
{
	if(NULL == m_pclsInstance)
	{
		m_pclsInstance = new LCDFrame();
	}

    return m_pclsInstance;
}

/*************************************************************************/
/** Function Name:	SetLCDPixel                                         **/
/** Purpose:		Set a pixel value.                                  **/
/** Params:																**/
/**	@ uiPosX[in]:       X-Coordinate of pixel.                          **/
/**	@ uiY[in]:       Y-Coordinate of pixel.                          **/
/**	@ strTitle[in]:     Pixel value, 0 for clear, 1 for set.            **/
/** Return:			None.                                               **/
/** Notice:			This function only change the pixel register, To    **/
/**                 update the screen display, need to call RefreshLCD  **/
/**                 function or Or use the DrawPixel function directly. **/
/*************************************************************************/
void LCDFrame::SetLCDPixel(uint32_t uiPosX, uint32_t uiY, uint32_t uiPixelValue)
{
    m_pclsCtrlPaintPanel->SetPixel(uiPosX, uiY, uiPixelValue);
}

/*************************************************************************/
/** Function Name:	GetLCDPixel                                         **/
/** Purpose:		Get a pixel value.                                  **/
/** Params:																**/
/**	@ uiPosX[in]:       X-Coordinate of pixel.                          **/
/**	@ uiY[in]:       Y-Coordinate of pixel.                          **/
/** Return:			None.                                               **/
/** Notice:			The value is got form the display buffer.			**/
/*************************************************************************/
uint32_t LCDFrame::GetLCDPixel(uint32_t uiPosX, uint32_t uiY)
{
    uint32_t                uiPixelValue;

    uiPixelValue = (uint32_t)m_pclsCtrlPaintPanel->GetPixel(uiPosX, uiY);

    return uiPixelValue;
}

/*************************************************************************/
/** Function Name:	OnSysTickTimerEvent									**/
/** Purpose:		Dummy heart-bear timer event process.				**/
/** Params:																**/
/**	@ clsEvent[in]:		Event data object.								**/
/** Return:			None.                                               **/
/** Notice:			Target for drive dummy actions.						**/
/*************************************************************************/
void LCDFrame::OnGeneralTimerTrigger(wxTimerEvent& event)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_TIM_EVENT, true);
	SGUI_SDK_GeneralTimerHandler();
}

/*************************************************************************/
/** Function Name:	OnRTCEvent											**/
/** Purpose:		RTC timer event process.							**/
/** Params:																**/
/**	@ clsEvent[in]:		Event data object.								**/
/** Return:			None.                                               **/
/** Notice:			Target per-second for get now time.					**/
/*************************************************************************/
void LCDFrame::OnSecondTimerTrigger(wxTimerEvent& event)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_SEC_EVENT, true);
	SGUI_SDK_SecondTimerHandler();
}

/*************************************************************************/
/** Function Name:	OnSDKInitialize										**/
/** Purpose:		Frame show event process.							**/
/** Params:																**/
/**	@ clsEvent[in]:		Event data object.								**/
/** Return:			None.                                               **/
/** Notice:			Do environment initialize process when main window	**/
/**					show up.											**/
/*************************************************************************/
void LCDFrame::OnSDKInitialize(InitEvent& clsEvent)
{
	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	// Set LCD panel parameter.
	m_pclsCtrlPaintPanel->SetParameter(&g_stParameters);
	// Clear display panel.
	m_pclsCtrlPaintPanel->CleanScreen();
	// Resize frame.
	SetClientSize(m_pclsCtrlPaintPanel->GetBestSize());
	// Reset frame position.
	Center(wxBOTH);
	// Create timer objects.
	m_pclsGeneralTimer = new wxTimer(this, ID_GENERAL_TIMER);
	m_pclsSecondTimer = new wxTimer(this, ID_SECOND_TIMER);
	// Set event process flag.
	SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_INIT, true);
	// Set status bar text.
	_setStatusText(_TRANS_TEXT("Initialized."));

	clsEvent.Skip();
}

void LCDFrame::OnSDKGeneralTimerSet(TimerSetEvent& clsEvent)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	int				iTimerInterval;
	bool			bReturn;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bReturn =		true;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	iTimerInterval = clsEvent.GetInterval();

	if(NULL != m_pclsGeneralTimer)
	{
		if(iTimerInterval > 0)
		{
			// Timer started.
			bReturn = m_pclsGeneralTimer->Start(iTimerInterval);
		}
		else
		{
			m_pclsGeneralTimer->Stop();
		}

		// Set event process flag when timer start successfully or stopped.
		if(true == bReturn)
		{
			SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_TIM_SET, true);
		}
	}
}

void LCDFrame::OnSecondTimerEnabled(RTCSwitchEvent& clsEvent)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	bool			bRTCEnabled;
	bool			bReturn;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bReturn =		true;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bRTCEnabled = clsEvent.GetReqState();

	if(NULL != m_pclsSecondTimer)
	{
		if(false == bRTCEnabled)
		{
			m_pclsSecondTimer->Stop();
		}
		else
		{
			bReturn = m_pclsSecondTimer->Start(1000);
		}

		// Set event process flag when timer start successfully or stopped.
		if(true == bReturn)
		{
			SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_SEC_EN, true);
		}
	}
}

/*************************************************************************/
/** Function Name:	Entry												**/
/** Purpose:		Thread helper main process.							**/
/** Params:			None.												**/
/** Return:			wxThread::ExitCode type, 0 for normal, others for	**/
/**					failed.												**/
/** Notice:			This is joinable thread body process, dummy main	**/
/**					process function will be called here.				**/
/*************************************************************************/
wxThread::ExitCode LCDFrame::Entry(void)
{
	wxThread::ExitCode		lExitCode;
	wxThread*				pclsThread;
	bool					bExit;

	pclsThread = GetThread();
	if(NULL != pclsThread)
	{
		bExit = false;
#if 0
		// Wait for frame UI update done.
		wxMilliSleep(DUMMY_MAIN_START_DELAY_MS);
#endif
		// Enable event receive.
		SetEvtHandlerEnabled(true);
        // Start dummy main process.
		while(false == bExit)
		{
			bExit = pclsThread->TestDestroy();
            lExitCode = reinterpret_cast<wxThread::ExitCode>(SGUI_SDK_DummyMainProc());
		}
	}

	lExitCode = 0;

	return lExitCode;
}

/*************************************************************************/
/** Function Name:	StartDummyMain										**/
/** Purpose:		Create and start dummy main thread.					**/
/** Params:			None.												**/
/** Return:				true: start successfully.						**/
/**						false: create or start thread failed.			**/
/** Notice:			This is joinable thread body process, dummy main	**/
/**					None.												**/
/*************************************************************************/
bool LCDFrame::StartDummyMain(void)
{
	bool					bReturn;
	wxThread*				pclsThread;
	wxThreadError			eThreadError;

	bReturn = true;

	do
	{
		// Create thread;
		eThreadError = CreateThread(wxTHREAD_JOINABLE);
		if(wxTHREAD_NO_ERROR != eThreadError)
		{
			bReturn = false;
			break;
		}
		// Start thread.
		pclsThread = GetThread();
		if(NULL != pclsThread)
		{
			eThreadError = pclsThread->Run();
			if(wxTHREAD_NO_ERROR != eThreadError)
			{
				bReturn = false;
				break;
			}
		}
		else
		{
			bReturn = false;
			break;
		}
	}while(0);

	return bReturn;
}
