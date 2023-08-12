#ifndef _INCLUDED_CLASS_CUSTOM_EVENTS_H_
#define _INCLUDED_CLASS_CUSTOM_EVENTS_H_

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx/event.h>
#include <wx/string.h>

//=======================================================================//
//= Class declare.                                                      =//
//=======================================================================//

//-----------------------------------------------------------------------//
//= Event types declare.												=//
//-----------------------------------------------------------------------//
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_SDK_INIT, evtID_SDK_INIT)
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_RTC_SWITCH, evtID_FONT_STYLE)
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_TIMER_SET, evtID_NAVI_BUTTON)
END_DECLARE_EVENT_TYPES()

#define EVT_SDK_INIT(fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_SDK_INIT, wxID_ANY, -1, (wxObjectEventFunction)(wxEventFunction)(InitEventFunc)&fn, (wxObject*)NULL),

#define EVT_SDK_RTC_EN(fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_RTC_SWITCH, wxID_ANY, -1, (wxObjectEventFunction)(wxEventFunction)(RTCSwitchEventFunc)&fn, (wxObject*)NULL),

#define EVT_SDK_SYSTICK_SET(fn) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_TIMER_SET, wxID_ANY, -1, (wxObjectEventFunction)(wxEventFunction)(TimerSwitchEventFunc)&fn, (wxObject*)NULL),

//-----------------------------------------------------------------------//
//= Initialize event class declare.										=//
//-----------------------------------------------------------------------//
class InitEvent: public wxEvent
{
	DECLARE_DYNAMIC_CLASS(InitEvent)

	public:
								InitEvent(int iWinID = wxID_ANY, wxEventType iCommandType = wxEVT_SDK_INIT);
								InitEvent(const InitEvent& clsEvent);
		virtual wxEvent*		Clone() const									{return new InitEvent(*this);}
};

typedef void (wxEvtHandler::*InitEventFunc)(InitEvent&);

//-----------------------------------------------------------------------//
//= RTC status switch event class declare.								=//
//-----------------------------------------------------------------------//
class RTCSwitchEvent: public wxEvent
{
	DECLARE_DYNAMIC_CLASS(RTCSwitchEvent)

	private:
		bool					m_bRTCEnabled;

	public:
								RTCSwitchEvent(int iWinID = 0, wxEventType iCommandType = wxEVT_NULL);
								RTCSwitchEvent(const RTCSwitchEvent& clsEvent);
		virtual wxEvent*		Clone() const									{return new RTCSwitchEvent(*this);}
		bool					GetReqState(void)	const						{return m_bRTCEnabled;}
		void					SetReqState(const bool bEnabled)				{m_bRTCEnabled = bEnabled;}
};

typedef void (wxEvtHandler::*RTCSwitchEventFunc)(RTCSwitchEvent&);

//-----------------------------------------------------------------------//
//= Timer status switch event class declare.							=//
//-----------------------------------------------------------------------//
class TimerSetEvent: public wxEvent
{
	DECLARE_DYNAMIC_CLASS(TimerSetEvent)

	private:
		int						m_iIntervalMs;
	public:
								TimerSetEvent(int iWinID = 0, wxEventType iCommandType = wxEVT_NULL);
								TimerSetEvent(const TimerSetEvent& clsEvent);
		virtual wxEvent*		Clone() const									{return new TimerSetEvent(*this);}
		int						GetInterval(void)	const						{return m_iIntervalMs;}
		void					SetInterval(const int iInterval)				{m_iIntervalMs = iInterval;}
};

typedef void (wxEvtHandler::*TimerSwitchEventFunc)(TimerSetEvent&);

#endif // _INCLUDED_CLASS_CUSTOM_EVENTS_H_
