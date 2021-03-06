
/************************************************************************
||
|| DATE:                $Date: 2004/01/08 20:51:46 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/OpenServer.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: OpenServer.cpp,v 1.1 2004/01/08 20:51:46 tomcat Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: OpenServer.cpp,v $
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "OpenServer.hpp"

OpenServer::OpenServer(const ConfigMap& confmap):
		_socketFd(-1), _numberOfConn(-1),
		_port(-1)
{

}

OpenServer::~OpenServer()
{
	if (_socketFd != -1)
		close(_socketFd);

	//removeIO(_in);
	removeIO(_socketFd);
}

bool
OpenServer::initServer()
{
	if (_port == -1 || _numberOfConn == -1){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "OpenServer::initServer port %i, numberOfConn %i", _port, _numberOfConn);
		return false;
	}

	struct sockaddr_in serv_addr;
	struct hostent  *hp;
	struct in_addr *pIn;
	int reuseAddr = 1;
	bool ret = false;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(_port);
	Logger::getLogger()->log(Logger::TYPE_INFO, "Try to open socket on port %i", _port);
	if((_socketFd= socket(AF_INET, SOCK_STREAM, 0)) < 0){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "Can't open socket on port %i", _port);
	} else {
		setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseAddr, sizeof(int));
		if(bind(_socketFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "Can't bind on port %i", _port);
			close(_socketFd);
		} else {
			listen (_socketFd, _numberOfConn);
			newIO(_socketFd, IOCallback::IO_READ, this);
			Logger::getLogger()->log(Logger::TYPE_INFO, "Success Open socket %i on port %i", _socketFd, _port);
			ret = true;
		}
	}

	return ret;
}


