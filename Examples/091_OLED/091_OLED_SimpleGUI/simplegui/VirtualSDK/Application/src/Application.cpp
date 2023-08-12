/*************************************************************************/
/** Copyright.															**/
/** FileName: Application.cpp											**/
/** Author: Polarix														**/
/** Description: Custom event class member define.						**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "Application.h"
#include "Common.h"

//=======================================================================//
//= Class member function define.							            =//
//=======================================================================//
IMPLEMENT_APP(Application);

bool Application::OnInit(void)
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
	// Register image media handle.
	wxInitAllImageHandlers();
	// Read virtual device parameter.
	SetLCDPanelParameter(&g_stParameters);
	// Create main frame instance.
	m_pclsMainFrame = LCDFrame::GetInstance();
	if(NULL != m_pclsMainFrame)
	{
		SetTopWindow(m_pclsMainFrame);
		m_pclsMainFrame->Show();
	}
	else
	{
		// Create instance failed.
		bReturn = false;
	}

    return bReturn;
}

int Application::OnRun(void)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	int				iStartRet;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(false == m_pclsMainFrame->StartDummyMain())
	{
		iStartRet = APP_PROC_FAILURE;
	}
	else
	{
		// Base run.
		iStartRet = wxApp::OnRun();
	}

	return iStartRet;
}

int Application::OnExit()
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	int				iTerminateRet;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	iTerminateRet = wxApp::OnExit();

    return iTerminateRet;
}

