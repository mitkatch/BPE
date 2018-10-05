/************************************************************************
||
|| DATE:                $Date: 2004/08/09 21:00:10 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/HeartbeatSession.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: HeartbeatSession.hpp,v 1.2 2004/08/09 21:00:10 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: HeartbeatSession.hpp,v $
|| LOG:                 Revision 1.2  2004/08/09 21:00:10  mikhailt
|| LOG:                 INTERVAL added
|| LOG:
|| LOG:                 Revision 1.1  2004/07/30 16:16:06  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef HEARTBEATSESSION_HPP 
#define HEARTBEATSESSION_HPP 

#include "TimerCallback.hpp"
#include "UDPSession.hpp"
#include "ConfigMap.hpp"
#include "Scheduler.hpp"

class HeartbeatSession: public UDPSession, public TimerCallback
{
	public:
		HeartbeatSession(const ConfigMap &confmap);
		virtual ~HeartbeatSession();

		virtual bool initSession();
		//virtual void initHeartbeatSession();
	protected:
		virtual void onTimer(TimerId id);

	private:
		int _heartbeat;
		string _interval;
		string _p_host;
		string _pid;
};
#endif // HEARTBEATSESSION_HPP
