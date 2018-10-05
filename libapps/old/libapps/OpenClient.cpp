
/************************************************************************
||
|| DATE:                $Date: 2006/05/12 16:00:25 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/OpenClient.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: OpenClient.cpp,v 1.2 2006/05/12 16:00:25 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: OpenClient.cpp,v $
|| LOG:                 Revision 1.2  2006/05/12 16:00:25  mikhailt
|| LOG:                 NONBLOCK flag is used
|| LOG:
|| LOG:                 Revision 1.1  2006/03/28 21:15:39  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "OpenClient.hpp"
#include <errno.h>

OpenClient::OpenClient():
		_socketFd(-1), 
		_port(""), _host("")
{
	_port = ConfigMap::getConfig()->getValue("OPENCLIENT_PORT");
	_host = ConfigMap::getConfig()->getValue("OPENCLIENT_HOST");
}

OpenClient::~OpenClient()
{
	if (_socketFd != -1)
		close(_socketFd);

	//removeIO(_in);
}

bool
OpenClient::initClient()
{
	bool ret = false;
	if (_port.empty() == -1 || _host.empty()  ){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "OpenClient::initClient port %s, _host [%s]", _port.c_str(), _host.c_str());
		return false;
	}

	int n, flags;
	struct addrinfo hints, *res = NULL, *ressave = NULL;
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags =AI_CANONNAME;
	hints.ai_family = AF_INET; //AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ( (n = getaddrinfo(_host.c_str(), _port.c_str(), &hints, &res)) != 0){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "OpenClient::initClient port %s, host [%s] getaddrinfo failed %s", _port.c_str(), _host.c_str(), gai_strerror(n));
		return false;
	}

	ressave = res;

	do {
		_socketFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (_socketFd < 0)	
			continue;  //ignore
		
		// the following is the BLOCKED connect , 
		//needs to be changed to NON_BLOCKED
		if (connect(_socketFd, res->ai_addr, res->ai_addrlen) == 0){
			flags = fcntl(_socketFd, F_GETFL, 0);
			if(fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK) == -1){
				Logger::getLogger()->log(Logger::TYPE_ERROR, "Failed OpenClient O_NDELAY socket %i on port %s host %s [%s]", _socketFd, _port.c_str(), _host.c_str(), strerror(errno));
			}
			//newIO(_socketFd, IOCallback::IO_READ, this);
			Logger::getLogger()->log(Logger::TYPE_INFO, "Success OpenClient socket %i on port %s host %s", _socketFd, _port.c_str(), _host.c_str());
			ret = true;
			break; //success
		}else{
			Logger::getLogger()->log(Logger::TYPE_ERROR, "Failed OpenClient socket %i on port %s host %s [%s]", _socketFd, _port.c_str(), _host.c_str(), strerror(errno));


		}

		close(_socketFd); //ignore
	} while ( (res = res->ai_next) != NULL);


	if (res == NULL){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "OpenClient::initClient port %s, _host [%s] connect error", _port.c_str(), _host.c_str());
		return false;
	}

	freeaddrinfo(ressave);
	return ret;
}


