
/************************************************************************
||
|| DATE:                $Date: 2004/07/30 16:16:06 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/TimerCallback.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: TimerCallback.hpp,v 1.2 2004/07/30 16:16:06 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: TimerCallback.hpp,v $
|| LOG:                 Revision 1.2  2004/07/30 16:16:06  mikhailt
|| LOG:                 sends correct heartbeats
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef TIMERCALLBACK_HPP
#define TIMERCALLBACK_HPP


#include <assert.h>
#include "BasicComponent.hpp"

typedef XtIntervalId    TimerId;

class TimerCallback
{
	public:
		static enum {   FOREVER = -1,
				NONE = 0
		} tRepeat;

	TimerCallback();
	virtual ~TimerCallback();

	virtual void addTimer(unsigned long interval, int repeat, TimerCallback *);
	
	protected:

		virtual void removeTimer ( TimerId timer);
		static void regTimerCallback(XtPointer clientData, TimerId *timer);
		virtual void onTimer(TimerId id){
			cout<<"TimerCallback not implemented "<<endl;
		}


		int             _repeat;
		TimerId         _timer;
		unsigned long   _interval;
};

#endif //TIMERCALLBACK_HPP
			
