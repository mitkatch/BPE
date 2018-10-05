
/************************************************************************
||
|| DATE:                $Date: 2004/01/08 20:51:46 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/OpenServer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: OpenServer.hpp,v 1.1 2004/01/08 20:51:46 tomcat Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: OpenServer.hpp,v $
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef OPENSERVER_HPP
#define OPENSERVER_HPP

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//#include <sys/conf.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>

#include "IOCallback.hpp"
#include "ConfigMap.hpp"
#include "Logger.hpp"

class OpenServer: public IOCallback
{
	public:
		OpenServer(const ConfigMap &confmap);
		virtual ~OpenServer();

		virtual bool initServer();
	protected:
		int _socketFd, _numberOfConn, _port;
		InputId _in;

};
#endif//OPENSERVER_HPP
