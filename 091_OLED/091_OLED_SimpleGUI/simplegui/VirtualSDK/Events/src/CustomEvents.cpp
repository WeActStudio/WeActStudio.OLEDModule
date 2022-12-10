/*************************************************************************/
/** Copyright.															**/
/** FileName: CustomEvents.cpp											**/
/** Author: Polarix														**/
/** Description: Custom event class member define.						**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "CustomEvents.h"

//=======================================================================//
//= Function define.										            =//
//=======================================================================//

//-----------------------------------------------------------------------//
//= Initialize event class define.										=//
//-----------------------------------------------------------------------//
IMPLEMENT_DYNAMIC_CLASS(InitEvent, wxEvent)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_SDK_INIT)

InitEvent::InitEvent(int iWinID, wxEventType iCommandType):
wxEvent(iWinID, iCommandType)
{

}

InitEvent::InitEvent(const InitEvent& clsEvent):
wxEvent(clsEvent)
{

}

//-----------------------------------------------------------------------//
//= RTC status switch event class define.								=//
//-----------------------------------------------------------------------//
IMPLEMENT_DYNAMIC_CLASS(RTCSwitchEvent, wxEvent)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_RTC_SWITCH)

RTCSwitchEvent::RTCSwitchEvent(int iWinID, wxEventType iCommandType):
wxEvent(iWinID, iCommandType)
{
	m_bRTCEnabled = false;
}

RTCSwitchEvent::RTCSwitchEvent(const RTCSwitchEvent& clsEvent):
wxEvent(clsEvent)
{
	SetReqState(clsEvent.GetReqState());
}

//-----------------------------------------------------------------------//
//= Timer status switch event class define.								=//
//-----------------------------------------------------------------------//
IMPLEMENT_DYNAMIC_CLASS(TimerSetEvent, wxEvent)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_TIMER_SET)

TimerSetEvent::TimerSetEvent(int iWinID, wxEventType iCommandType):
wxEvent(iWinID, iCommandType)
{
	m_iIntervalMs = 0;
}

TimerSetEvent::TimerSetEvent(const TimerSetEvent& clsEvent):
wxEvent(clsEvent)
{
    SetInterval(clsEvent.GetInterval());
}


