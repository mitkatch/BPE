
/************************************************************************
||
|| DATE:                $Date: 2004/01/08 20:51:46 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/TimerCallback.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: TimerCallback.cpp,v 1.1 2004/01/08 20:51:46 tomcat Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: TimerCallback.cpp,v $
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include <algorithm>
#include "TimerCallback.hpp"
#include "Application.hpp"

TimerCallback::TimerCallback():_timer(0),
				_repeat(TimerCallback::NONE),
				_interval(0)
{

}


TimerCallback::~TimerCallback()
{
	removeTimer(_timer);
}

void
TimerCallback::addTimer(unsigned long interval, int repeat, TimerCallback *tc)
{
	//cout<<"addTimer for "<<interval<<", "<<repeat<<endl;
	if (tc == NULL || repeat == TimerCallback::NONE || interval == 0L)
		return;

	_repeat = repeat;
	_interval = interval;
	_timer = XtAppAddTimeOut(theApplication->appContext(), interval,
		tc->regTimerCallback, tc);
}

void
TimerCallback::removeTimer ( TimerId timer)
{
	if (timer)
		XtRemoveTimeOut(timer);
}

void //static
TimerCallback::regTimerCallback(XtPointer clientData, TimerId *id)
{
	TimerCallback *tc = (TimerCallback *) clientData;
	if (tc != NULL){
		tc->_timer = 0; //reset to 0, if we need to remove pending timer
		tc->onTimer(*id);
		if (tc->_repeat ){
			tc->addTimer(tc->_interval,
				(tc->_repeat == TimerCallback::FOREVER
					?tc->_repeat:tc->_repeat -1), tc);
		}
	}
}

