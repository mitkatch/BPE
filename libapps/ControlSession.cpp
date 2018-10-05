/************************************************************************
||
|| DATE:                $Date: 2005/01/05 17:24:48 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/ControlSession.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ControlSession.cpp,v 1.2 2005/01/05 17:24:48 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: ControlSession.cpp,v $
|| LOG:                 Revision 1.2  2005/01/05 17:24:48  mikhailt
|| LOG:                 control mc msg sent
|| LOG:
|| LOG:                 Revision 1.1  2004/07/30 16:14:57  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "ControlSession.hpp"
#include "Scheduler.hpp"

ControlSession::ControlSession(const ConfigMap &confmap):UDPSession(confmap)
{
	this->setMCIP(confmap.getValue("CONTROL_MULTICAST_GROUP_IP"));
	this->setMCPort(confmap.getValue("CONTROL_MULTICAST_GROUP_PORT"));


}

ControlSession::~ControlSession()
{
	if (_publisher)
		delete _publisher;
	
	removeIO(udpSendFd());
}

bool
ControlSession::initSession()
{
	bool ret = this->initUDP(UDPSession::UDP_PUBLISHER_TYPE | UDPSession::UDP_SUBSCRIBER_TYPE);
	_publisher = new UDPSessionPublisher(this, udpSendFd());

	return ret;
}

void
ControlSession::processUDP(const string& str, const string& line)
{
	//MCMsg mcmsg(str);
}

void
ControlSession::broadcast(const MCMsg& msg)
{
	if (_publisher){
		MCMsg *newmsg = new MCMsg(msg);
		if (!newmsg->isKey("TIMESTAMP")){
			string format = string("%c");
			string curtime = Scheduler::getCurrentTime(format);
			//long lcurtime = Scheduler::getTimeLong(curtime, format);
			//char buf[20];
			//sprintf(buf, "%d", lcurtime);
			newmsg->addKeyValue("TIMESTAMP", curtime.c_str()); 
		}
		_publisher->publish(newmsg);

	}
}

