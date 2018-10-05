
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

#include "Logger.hpp"

#include "MKLOpenServer.hpp"

MKLOpenServer::MKLOpenServer(const ConfigMap& confmap):OpenServer(confmap)
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "MKLOpenServer::MKLOpenServer");
	if (confmap.getValue("OPENSERVER_PORT").size())
		_port = atoi((confmap.getValue("OPENSERVER_PORT")).c_str());
	if (confmap.getValue("NUMBER_CONNECTIONS").size())
		_numberOfConn = atoi((confmap.getValue("NUMBER_CONNECTIONS")).c_str());

	_root_ns = confmap.getValue("NAME_SPACE");

}

MKLOpenServer::~MKLOpenServer()
{
}

void
MKLOpenServer::onRead(int fd, InputId id)
{
	if (_socketFd == fd){
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLOpenServer::onIO connection accepted from %i", fd);
		struct sockaddr_in cli_addr;
		socklen_t client_len = (socklen_t)sizeof(cli_addr);
		int new_fd = accept(fd, (struct sockaddr *) &cli_addr, &client_len);
		if(new_fd < 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLOpenServer::onIO connection is not accepted from %i", fd);
			return ;
		}
		int sendbuf = BUFSIZ;
		setsockopt(new_fd, SOL_SOCKET, SO_SNDBUF, (void *)&sendbuf, sizeof(int));
		if(fcntl(new_fd, F_SETFL, O_NDELAY) == -1){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLOpenServer::onIO set socket to non-blocking mode  %i", new_fd);
		}
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLOpenServer::onIO accepted from %s : %l", inet_ntoa(cli_addr.sin_addr), ntohl(cli_addr.sin_port) );
		cout << "Accepted from "<<inet_ntoa(cli_addr.sin_addr)<<"::"<<ntohl(cli_addr.sin_port)<<endl;

		if ( _mapClients.size() >= _numberOfConn){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLOpenServer::onIO couldn't accept more then  %i connections", _numberOfConn);
			return;
		}
		MKLHttpClient *client = new MKLHttpClient(this, new_fd);
		_mapClients[new_fd] = client;
		
				
	}
}

void 
MKLOpenServer::removeClient(MKLHttpClient *cl)
{
	map<int, MKLHttpClient *>::iterator it = _mapClients.find(cl->getFd());
	if (it != _mapClients.end()){
		_mapClients.erase(it);
		close(cl->getFd());
		delete cl;
	}
}

