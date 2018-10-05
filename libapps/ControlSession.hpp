/************************************************************************
||
|| DATE:                $Date: 2005/01/05 17:24:48 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/ControlSession.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ControlSession.hpp,v 1.2 2005/01/05 17:24:48 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: ControlSession.hpp,v $
|| LOG:                 Revision 1.2  2005/01/05 17:24:48  mikhailt
|| LOG:                 control mc msg sent
|| LOG:
|| LOG:                 Revision 1.1  2004/07/30 16:16:06  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CONTROLSESSION_HPP
#define CONTROLSESSION_HPP

//#include "TimerCallback.hpp"
#include "UDPSession.hpp"
#include "MCMsg.hpp"

class ControlSession: public UDPSession
{
	public:
		ControlSession(const ConfigMap &confmap);
		virtual ~ControlSession();
	
		virtual bool initSession();
		virtual void broadcast(const MCMsg& msg);

	protected:
		virtual void processUDP(const string& str, const string& line);

	private:
};
#endif //CONTROLSESSION_HPP
