
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

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <stropts.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ctype.h>


#include "FTManager.hpp"
#include "Formatter.hpp"

FTManager* FTManager::_myft = NULL;

FTManager::~FTManager()
{
	if (_publisher)
		delete _publisher;

	if (_client)
		delete _client;

}

//static
FTManager*
FTManager::getFTManager()
{
	if (_myft == NULL){
		_myft = new FTManager();
                Logger::getLogger()->log(Logger::TYPE_INFO, "FTManager created");
	}

	return _myft;
}

FTManager::FTManager()
{
	_host = ConfigMap::getConfig()->getValue("FT_SESSION_IP");
	_port = ConfigMap::getConfig()->getValue("FT_SESSION_PORT");
	_request = ConfigMap::getConfig()->getValue("FT_SESSION_REQUEST");
	string st = ConfigMap::getConfig()->getValue("FT_SESSION_TIMER");
	_timer = 1;
	status = FTManager::UNKNOWN;
	if (st.size())
		_timer = Formatter::ATOF(st.c_str());

	_client = new FTOpenClient();
	_publisher = new FTPublisher();
	_client->setPort(_port);
	_client->setHost(_host);
	addTimer(1*1000, 1, this);
}

FTManager::FT
FTManager::getFT()
{
	return _ftinst;
}

void
FTManager::onTimer(TimerId id)
{
	if (_client->initClient()){
		_myfd = _client->myfd();
		_publisher->setFD(_myfd);
		_publisher->publish(_request);
	}else{
		status = FTManager::PRIMARY;
		_ftinst.port = "";
		_ftinst.host = "";
		_ftinst.status = status;
	}

	addTimer(_timer * 1000, 1, this);
}

void
FTManager::FTOpenClient::onRead(int fd, InputId id)
{
	//cout << "FTManager::FTOpenClient::onRead"<<endl;
	if (fd == myfd()){
                int ntoread = 0, nread;
                int ntoread_new =0;
                char *toBuf = NULL;
                bool sucseed = false;
                if((ioctl(fd, I_NREAD, &ntoread) == -1) || (ntoread == 0)){
                        Logger::getLogger()->log(Logger::TYPE_INFO, "FTManager %i connection dropped", fd);
			close(fd);
                        removeIO(fd);
                        return;
                }
		char *szBuffer = new char[ntoread + 1];
		nread = read(fd, szBuffer, ntoread);
		if (nread <= 0){
                        Logger::getLogger()->log(Logger::TYPE_INFO, "FTManager %i closed connection %i read", fd, nread);
			close(fd);
                        removeIO(fd);
                        return;
		}
		szBuffer[nread] = '\0';
		_strBuffer += szBuffer;
		//cout<<"["<<_strBuffer<<"]"<<endl;
		int istatus = _strBuffer.find("FT_STATUS:PRIMARY");
		if (istatus != string::npos){
			_myft->status = FTManager::SECONDARY;
		}else {
			istatus = _strBuffer.find("FT_STATUS:SECONDARY");
				if (istatus != string::npos){
					_myft->status = FTManager::PRIMARY;
				}else{
					_myft->status = FTManager::UNKNOWN;

				}
		}
		_myft->_ftinst.port = _myft->_port;
		_myft->_ftinst.host = _myft->_host;
		_myft->_ftinst.status = _myft->status;
                Logger::getLogger()->log(Logger::TYPE_INFO, "FTManager %i closed connection, size[%i]", fd, nread);
		close(fd);
                removeIO(fd);
		_strBuffer.erase();

	}

}

void
FTManager::FTPublisher::publish(const string& str)
{
	_pubstr += string("GET ") + str + string(" HTTP/1.0") + string("\r\n\r\n") + _pubstr ;
	newIO(_pfd, IOCallback::IO_WRITE, this);
}

void
FTManager::FTPublisher::onWrite(int fd, InputId id)
{
	//cout << "FTManager::FTPublisher::onWrite " <<endl;
	if (fd == _pfd){
		int n = 0;
		n = send(fd, _pubstr.c_str(), _pubstr.size(), 0);
		_pubstr.erase();
		this->removeIO(fd);
		//TEST
		FTManager::_myft->_client->newIO(fd, IOCallback::IO_READ, FTManager::_myft->_client);
	}
}
