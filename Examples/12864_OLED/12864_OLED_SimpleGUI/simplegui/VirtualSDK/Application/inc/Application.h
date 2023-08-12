/***************************************************************
 * Name:      LCDSimulatorMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2017-02-25
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef __INCLUDE_CLASS_APPLICATION_H__
#define __INCLUDE_CLASS_APPLICATION_H__

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx/dialog.h>
#include <wx/app.h>
#include <wx/log.h>
#include "LCDFrame.h"

enum
{
	APP_PROC_NORMAL						= 0,
	APP_PROC_FAILURE					= -1,
	APP_PROC_ABNORMAL					= -2,
};

//=======================================================================//
//= Class declare.                                                      =//
//=======================================================================//
class Application : public wxApp
{
    private:
        LCDFrame*					m_pclsMainFrame;

    public:
		bool						OnInit(void);
		int							OnRun(void);
		int             			OnExit(void);

};

#endif //__INCLUDE_CLASS_APPLICATION_H__
