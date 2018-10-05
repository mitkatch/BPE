
/************************************************************************
||
|| DATE:                $Date: 2006/03/23 18:54:33 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/OpenServer.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: OpenServer.cpp,v 1.2 2006/03/23 18:54:33 nickz Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: OpenServer.cpp,v $
|| LOG:                 Revision 1.2  2006/03/23 18:54:33  nickz
|| LOG:                 Added vector for wildcard retrieval
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "OpenServer.hpp"
#include <string.h>
#include <errno.h>

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
	struct sockaddr    sock_addr;
	struct sockaddr_in serv_addr;
	struct hostent  *hp;
	struct in_addr *pIn;
	int reuseAddr = 1;
	bool ret = false;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(_port);
	Logger::getLogger()->log(Logger::TYPE_INFO, "Trying to open listener socket on port %i", _port);
	memcpy( (void *)&sock_addr, (void *)&serv_addr, sizeof(serv_addr) );

	if((_socketFd= socket(AF_INET, SOCK_STREAM, PF_UNSPEC)) < 0)
	{
		Logger::getLogger()->log(Logger::TYPE_ERROR, "Can't open socket on port %i (%s)", 
					 _port, strerror(errno));
	} else 
	{
		if ( setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseAddr, sizeof(int)) < 0 )
		{
		    Logger::getLogger()->log(Logger::TYPE_ERROR, "Can't setsockopt socket on port %i (%s)", 
					     _port, strerror(errno));
		    close(_socketFd);

		}
		if(bind(_socketFd, &sock_addr, sizeof(sock_addr)) < 0)
		{
			Logger::getLogger()->log(Logger::TYPE_ERROR, "Can't bind on port %i (%s)", 
						 _port, strerror(errno));
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


