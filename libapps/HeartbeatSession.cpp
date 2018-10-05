/************************************************************************
||
|| DATE:                $Date: 2004/08/09 21:00:10 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/HeartbeatSession.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: HeartbeatSession.cpp,v 1.2 2004/08/09 21:00:10 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: HeartbeatSession.cpp,v $
|| LOG:                 Revision 1.2  2004/08/09 21:00:10  mikhailt
|| LOG:                 INTERVAL added
|| LOG:
|| LOG:                 Revision 1.1  2004/07/30 16:14:57  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/
#include <sys/utsname.h>

#include "HeartbeatSession.hpp"

HeartbeatSession::HeartbeatSession(const ConfigMap &confmap):UDPSession(confmap)
{
	this->setMCIP(confmap.getValue("HEARTBEAT_MULTICAST_GROUP_IP"));
	this->setMCPort(confmap.getValue("HEARTBEAT_MULTICAST_GROUP_PORT"));
	_interval = confmap.getValue("HEARTBEAT_INTERVAL");
	_heartbeat = atoi(_interval.c_str());
	struct utsname name;
	uname(&name);
	_p_host = name.nodename; 
	char pid[10];
	sprintf(pid, "%d", getpid());
	_pid = pid; 


}

HeartbeatSession::~HeartbeatSession()
{
	if (_publisher)
		delete _publisher;
	
}

bool
HeartbeatSession::initSession()
{
	bool ret = this->initUDP(UDPSession::UDP_PUBLISHER_TYPE);

	if (_heartbeat > 0){
		_publisher = new UDPSessionPublisher(this, udpSendFd());
		addTimer(_heartbeat * 1000, TimerCallback::FOREVER, this);
		ret &= true;
	}
	return ret;
}

void
HeartbeatSession::onTimer(TimerId id)
{
	string format = string("%c");
	string curtime = Scheduler::getCurrentTime(format);
	long lcurtime = Scheduler::getTimeLong(curtime, format);
	char buf[20];
	sprintf(buf, "%d", lcurtime);
	MCMsgHeartbeat * msg = new MCMsgHeartbeat(buf); 
	msg->addKeyValue("INTERVAL", _interval.c_str());
	msg->buildSuffix(_p_name, _p_host, _pid);
	_publisher->publish(msg);
}

