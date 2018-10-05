
/************************************************************************
||
|| DATE:                $Date: 2005/11/17 21:57:58 $
|| SOURCE:              $Source: /export/home/tomcat/Build/espeedfeedII/espeedII/RCS/ESHttpClient.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESHttpClient.cpp,v 1.1 2005/11/17 21:57:58 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ESHttpClient.cpp,v $
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <stropts.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <libgen.h>

#include <set>

#include "Logger.hpp"
#include "ESHttpClient.hpp"
#include "ESOpenServer.hpp"
#include "MessageFactory.hpp"
#include "MsgParser.hpp"
#include "DataCache.hpp"
#include "Application.hpp"
#include "Datum.hpp"
#include "HttpDataMap.hpp"
#include "HtmlBeanProcessor.hpp"
#include "ConfigMap.hpp"

ESHttpClient::ESHttpClient(ESOpenServer *server, int fd):
			_myServer(server), 
			_myfd(fd), 
			_publisher(NULL)
{ 
	newIO(_myfd, IOCallback::IO_READ, this);
	
	//cout << "Register Clients for "<<(*it).second.url<<endl;
	string root = server->root();
	MessageFactory::getFactory()->registerMsgHandler(root+string("/list"), ESHttpClient::onListRequest);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/record"), ESHttpClient::onRecordRequest);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/recordUpdate"), ESHttpClient::onRecordUpdate);
	_publisher = new HttpClientPublisher(this, _myfd);
	addTimer(60*1000, 1, this);//timeout timer

};
ESHttpClient::~ESHttpClient()
{
	//cout<<"Client deleting..."<<endl;
	if (_publisher){
		delete _publisher;
		//cout<<"Publisher deleted..."<<endl;
	}
	removeIO(_myfd);
}


void
ESHttpClient::onRead(int fd, InputId id)
{
	if( fd == _myfd){
		int ntoread = 0, nread;
		int ntoread_new =0;
		char *toBuf = NULL;
		bool sucseed = false;
		// for LINUX it's FIONREAD, for SUN it's I_NREAD
		if((ioctl(_myfd, I_NREAD, &ntoread) == -1) || (ntoread == 0)){
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESHttpClient %i closed connection", _myfd);
			_myServer->removeClient(this);
			return;
		}
		char *szBuffer = new char[ntoread + 1];
		nread = read(_myfd, szBuffer, ntoread);
		if (nread <= 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "ESHttpClient %i read failed", _myfd);
			_myServer->removeClient(this);
			return ;
		}	
		szBuffer[nread] = '\0';
		_strBuffer += szBuffer;
		//cout<<"["<<_strBuffer<<"]"<<endl;
		int buffSize = _strBuffer.length();
		while(buffSize > 0){
			int start = _strBuffer.find("GET ");
			if (start == string::npos)
				start = _strBuffer.find("POST ");

			int end = _strBuffer.find("\r\n\r\n");
			if(start == string::npos){
				_strBuffer = "";
				//cout<<"Start not found"<<endl;
				break;
			}
			if(end == string::npos){
				//cout<<"End not found"<<endl;
				break; //to get rest of the msg
			}

			string strCur = _strBuffer.substr(start, end+4);
			//cout <<"Get Msg ["<<strCur<<"] from "<<_myfd<<endl;
			MsgGraph httpImage;
			MsgParser msgParser(httpImage);
			if (msgParser.httpParse(strCur)){
				string url, method ;
				if(httpImage.getAttributeValue("method@HTTP", 0, method)){
					if (method == "POST"){
						string length;
						if(httpImage.getAttributeValue("Content-Length@HTTP", 0, length)){
							if(_strBuffer.length() == (start + end + 5 + atoi(length.c_str())+1)){
								string datastr = _strBuffer.substr(end+4, atoi(length.c_str()));
								httpImage.addGraphNode("HTTP", 0, "data", datastr);
							} else {
								//cout << "POST message NOT get size="<<_strBuffer.length()<<" != "<<start + end + 5 + atoi(length.c_str()) +1<<endl;
								break;
							} 
						}
					}
					if (method == "GET" || method == "POST"){
						if(httpImage.getAttributeValue("url@HTTP", 0, url)){
							MessageFactory::getFactory()->processMsg(url, httpImage , this);
							}else{
								Logger::getLogger()->log(Logger::TYPE_ERROR, "%s url@HTTP corrupted in %s. Panic", method.c_str(), strCur.c_str());
								panic(string("url@HTTP corrupted in ") + strCur);
								return ;
							}
						}
					}
				}
				string strTemp = _strBuffer;
				_strBuffer = strTemp.substr(end+4, strTemp.length());	
			}
		if (szBuffer)
			delete [] szBuffer;	
	}		
}


void
ESHttpClient::defaultMsgType(string& msgType, MsgGraph& msg)
{
	Logger::getLogger()->log(Logger::TYPE_ERROR, "Unknown msg type %s for msg", msgType.c_str());
	panic(string("Unknown msg type ") + msgType);
}

bool
ESHttpClient::onRecordUpdate(MsgGraph &image, MsgProcessor *processor)
{
	cout << "ESHttpClient::onRecordUpdate"<<endl;
	ESHttpClient *client = (ESHttpClient *)processor;
	if (client){
		client->onRecordUpdateImpl(image);
	}
	return true;
}

bool
ESHttpClient::onRecordRequest(MsgGraph &image, MsgProcessor *processor)
{
	ESHttpClient *client = (ESHttpClient *)processor;
	if (client){
		client->onRecordRequestImpl(image);
	}
	return true;
}

bool
ESHttpClient::onListRequest(MsgGraph &image, MsgProcessor *processor)
{
	ESHttpClient *client = (ESHttpClient *)processor;
	if (client){
		client->onListRequestImpl(image);
	}
	return true;
}

void
ESHttpClient::publish(const string& str)
{
	_publisher->publish(str);

}

ESHttpClient::HttpClientPublisher::HttpClientPublisher (ESHttpClient *cl, int fd):_myClient(cl), _myfd(fd)
{


}

void
ESHttpClient::HttpClientPublisher::onWrite(int fd, InputId id)
{
	if (fd == _myfd){
		int n = 0;
		int len = (1500 >_publishStr.size()? _publishStr.size(): 1500);
		n = send(fd, _publishStr.c_str(), len /*_publishStr.size()*/, 0);
		//cout <<n <<" WRITE = "<<id <<" for ["<<_publishStr<<"]"<<endl;
		_publishStr.erase(0, n );

		if (_publishStr.empty()){
			this->removeIO(fd);
			_myClient->myServer()->removeClient(_myClient);
			//cout << ":onWrite removeClient, bytes sent "<<n<<endl;
		} else {
			//cout << "onWrite add newIO, to send "<<_publishStr.size()<<endl;
			newIO(_myfd, IOCallback::IO_WRITE, this);
		}
	}
}

void
ESHttpClient::HttpClientPublisher::publish(const string& str)
{
	_publishStr += str;
	string header = "HTTP/1.0 200 OK\r\nDate:\r\nContent-Type: text/html\r\nContent-Length: ";
	char buf[12];
	sprintf(buf,"%i", _publishStr.size());
	_publishStr = header + string(buf) + string("\r\n\r\n") + _publishStr;
	newIO(_myfd, IOCallback::IO_WRITE, this);

}

string
ESHttpClient::getParameter(const string& attr, MsgGraph &image)
{
	string req, val;
	bool found = false;
	image.getAttributeValue(string("request@HTTP"), 0, req);	
	if (!req.empty()){
		int start = req.find(attr, 0);
		if (start >= 0){
			start = start + attr.length() + 1; //length of attr + '='=
			int stop = req.find_first_of('&', start+1);
			if (stop == string::npos) 
				stop = req.find_first_of(' ', start+1);
			
			val = req.substr(start, stop - start);
			found = true;
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESHttpClient::getParameter found key [%s] = [%s] ", attr.c_str(), val.c_str());
		}
	}
	if (!found)
		Logger::getLogger()->log(Logger::TYPE_ERROR, "ESHttpClient::getParameter not found key [%s] = [%s] ", attr.c_str(), val.c_str());


	return val;

}


void
ESHttpClient::panic(const string& msg)
{
	string header = "<b>HTTP/1.0 404 Not Found</b><br>\r\nDate:\r\nContent-Type: text/plain\r\nContent-Length: ";
	char buf[12];
	sprintf(buf,"%i", msg.size());
	string publishStr = header + string(buf) + string("\r\n\r\n") + msg;
	_publisher->publish(publishStr);
}

void
ESHttpClient::onTimer(TimerId id)
{
	this->panic("Drop Connection onTimer");

}

void
ESHttpClient::onRecordRequestImpl(MsgGraph &image)
{
	cout<<"ESHttpClient::onRecordRequestImpl "<<endl;
	publish("ESHttpClient::onRecordRequestImpl ");
}


void
ESHttpClient::onListRequestImpl(MsgGraph &image)
{
	cout<<"ESHttpClient::onListRequestImpl"<<endl;
	publish("ESHttpClient::onListRequestImpl");
}

void
ESHttpClient::onRecordUpdateImpl(MsgGraph &image)
{
	cout << "ESHttpClient::onRecordUpdate"<<endl;
	publish("ESHttpClient::onRecordUpdateImpl");
}

