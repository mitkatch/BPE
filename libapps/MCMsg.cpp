/************************************************************************
||
|| DATE:                $Date: 2005/03/29 16:05:55 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/MCMsg.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: MCMsg.cpp,v 1.3 2005/03/29 16:05:55 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: MCMsg.cpp,v $
|| LOG:                 Revision 1.3  2005/03/29 16:05:55  mikhailt
|| LOG:                 static ConfigMap
|| LOG:
|| LOG:                 Revision 1.2  2005/01/05 17:24:48  mikhailt
|| LOG:                 control mc msg sent
|| LOG:
|| LOG:                 Revision 1.1  2004/07/30 16:14:57  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <sys/utsname.h>
#include <unistd.h>

#include "MCMsg.hpp"

MCMsg::MCMsg(const MCMsg& msg)
{
	_suffix = msg._suffix;
	_prefix = msg._prefix;
	_stub = msg._stub;	
	_stabMap = msg._stabMap;

}

void
MCMsg::buildSuffix(const string& pname, const string& phost, const string& pid)
{
	string dot = string(1, '.');
	_suffix = string(1, '@') + pname + dot + phost + dot + pid; 
}

void
MCMsg::buildSuffix(const string& pname)
{
	string dot = string(1, '.');
	struct utsname name;
	uname(&name);
	string phost = name.nodename;
	char pid[10];
	sprintf(pid, "%d", getpid());
	string spid(pid);

	_suffix = string(1, '@') + pname + dot + phost + dot + spid; 
}

string
MCMsg::toString() const 
{
	string msg_header = string(1, '\001');
	string msg_footer = string(1, '\002');
	string nline = string(1, '\n');
	string msg = msg_header + nline 
			+ _prefix + _suffix +  _stub  + nline + msg_footer;
	return msg;

}

void
MCMsg::addKeyValue(const char* key, const char* val)
{
	if (_stabMap.insert(map<string, string>::value_type(key, val)).second){
		string nline = string(1, '\n');
		_stub += nline + string(key) + string(":") + string(val);
	}
}

bool
MCMsg::isKey(const char* key)
{
	bool res = false;
	map<string, string>::iterator it = _stabMap.find(key);
	if (it != _stabMap.end())
		res = true;

	return res;
}

MCMsgHeartbeat::MCMsgHeartbeat(const string& str)
{
	buildPrefix("HEARTBEAT");
	addKeyValue("TIMESTAMP", str.c_str());	
}
