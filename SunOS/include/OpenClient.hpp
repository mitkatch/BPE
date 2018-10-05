/************************************************************************
||
|| DATE:                $Date:$
|| SOURCE:              $Source:$
|| STATE:               $State:$
|| ID:                  $Id:$
|| REVISION:            $Revision:$
|| LOG:                 $Log:$
|| LOG:
************************************************************************/

#ifndef OPENCLIENT_HPP
#define OPENCLIENT_HPP

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

class OpenClient: public IOCallback
{
	public:
		OpenClient();
		virtual ~OpenClient();

		virtual bool initClient();

		virtual void setHost(const string& host){
			_host = host;
		}
		virtual void setPort(const string& port){
			_port = port;
		}
		virtual int myfd() const{
			return _socketFd;
		}

	protected:
		int _socketFd;
		string _host, _port;
		InputId _in;

};
#endif//OPENCLIENT_HPP
