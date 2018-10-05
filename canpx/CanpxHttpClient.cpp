/************************************************************************
||
|| DATE:                $Date: 2005/07/29 18:10:28 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxHttpClient.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxHttpClient.cpp,v 1.2 2005/07/29 18:10:28 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: CanpxHttpClient.cpp,v $
|| LOG:                 Revision 1.2  2005/07/29 18:10:28  mikhailt
|| LOG:                 handle failed attempt to getList
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
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
#include "CanpxHttpClient.hpp"
#include "CanpxOpenServer.hpp"
#include "CanpxEngine.hpp"
#include "MessageFactory.hpp"
#include "MsgParser.hpp"
#include "DataCache.hpp"
#include "Application.hpp"
#include "Datum.hpp"
#include "HttpDataMap.hpp"
#include "HtmlBeanProcessor.hpp"
#include "ConfigMap.hpp"

CanpxHttpClient::CanpxHttpClient(CanpxOpenServer *server, int fd):
			_myServer(server), 
			_myfd(fd), 
			_publisher(NULL)
{ 
	newIO(_myfd, IOCallback::IO_READ, this);
	
	//cout << "Register Clients for "<<(*it).second.url<<endl;
	string root = server->root();
	MessageFactory::getFactory()->registerMsgHandler(root, CanpxHttpClient::onCanpxRequest);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/list"), CanpxHttpClient::onListRequest);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/record"), CanpxHttpClient::onRecordRequest);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/recordUpdate"), CanpxHttpClient::onRecordUpdate);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/reset_avol"), CanpxHttpClient::onResetAvol);
	_htmlHome = ConfigMap::getConfig()->getValue("HTML_HOME");
	_publisher = new HttpClientPublisher(this, _myfd);
	addTimer(60*1000, 1, this);//timeout timer

};
CanpxHttpClient::~CanpxHttpClient()
{
	//cout<<"Client deleting..."<<endl;
	if (_publisher){
		delete _publisher;
		//cout<<"Publisher deleted..."<<endl;
	}
	removeIO(_myfd);
}


void
CanpxHttpClient::onRead(int fd, InputId id)
{
	if( fd == _myfd){
		int ntoread = 0, nread;
		int ntoread_new =0;
		char *toBuf = NULL;
		bool sucseed = false;
		// for LINUX it's FIONREAD, for SUN it's I_NREAD
		if((ioctl(_myfd, I_NREAD, &ntoread) == -1) || (ntoread == 0)){
			Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxHttpClient %i closed connection", _myfd);
			_myServer->removeClient(this);
			return;
		}
		char *szBuffer = new char[ntoread + 1];
		nread = read(_myfd, szBuffer, ntoread);
		if (nread <= 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxHttpClient %i read failed", _myfd);
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
CanpxHttpClient::defaultMsgType(string& msgType, MsgGraph& msg)
{
	Logger::getLogger()->log(Logger::TYPE_ERROR, "Unknown msg type %s for msg", msgType.c_str());
	panic(string("Unknown msg type ") + msgType);
}

bool
CanpxHttpClient::onRecordUpdate(MsgGraph &image, MsgProcessor *processor)
{
	cout << "CanpxHttpClient::onRecordUpdate"<<endl;
	CanpxHttpClient *client = (CanpxHttpClient *)processor;
	if (client){
		client->onRecordUpdateImpl(image);
	}
	return true;
}

bool
CanpxHttpClient::onResetAvol(MsgGraph &image, MsgProcessor *processor)
{
	CanpxHttpClient *client = (CanpxHttpClient *)processor;
	if (client){
		client->onResetAvolImpl(image);
	}
	return true;
}

bool
CanpxHttpClient::onRecordRequest(MsgGraph &image, MsgProcessor *processor)
{
	CanpxHttpClient *client = (CanpxHttpClient *)processor;
	if (client){
		client->onRecordRequestImpl(image);
	}
	return true;
}

bool
CanpxHttpClient::onCanpxRequest(MsgGraph &image, MsgProcessor *processor)
{
	CanpxHttpClient *client = (CanpxHttpClient *)processor;
	if (client){
		client->onCanpxRequestImpl(image);
	}
	return true;
}

bool
CanpxHttpClient::onListRequest(MsgGraph &image, MsgProcessor *processor)
{
	CanpxHttpClient *client = (CanpxHttpClient *)processor;
	if (client){
		client->onListRequestImpl(image);
	}
	return true;
}

void
CanpxHttpClient::publish(const string& str)
{
	_publisher->publish(str);

}

CanpxHttpClient::HttpClientPublisher::HttpClientPublisher (CanpxHttpClient *cl, int fd):_myClient(cl), _myfd(fd)
{


}

void
CanpxHttpClient::HttpClientPublisher::onWrite(int fd, InputId id)
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
CanpxHttpClient::HttpClientPublisher::publish(const string& str)
{
	_publishStr += str;
	string header = "HTTP/1.0 200 OK\r\nDate:\r\nContent-Type: text/html\r\nContent-Length: ";
	char buf[12];
	sprintf(buf,"%i", _publishStr.size());
	_publishStr = header + string(buf) + string("\r\n\r\n") + _publishStr;
	newIO(_myfd, IOCallback::IO_WRITE, this);

}

string
CanpxHttpClient::getParameter(const string& attr, MsgGraph &image)
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
			Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxHttpClient::getParameter found key [%s] = [%s] ", attr.c_str(), val.c_str());
		}
	}
	if (!found)
		Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxHttpClient::getParameter not found key [%s] = [%s] ", attr.c_str(), val.c_str());


	return val;

}


void
CanpxHttpClient::panic(const string& msg)
{
	string header = "<b>HTTP/1.0 404 Not Found</b><br>\r\nDate:\r\nContent-Type: text/plain\r\nContent-Length: ";
	char buf[12];
	sprintf(buf,"%i", msg.size());
	string publishStr = header + string(buf) + string("\r\n\r\n") + msg;
	_publisher->publish(publishStr);
}

void
CanpxHttpClient::onTimer(TimerId id)
{
	this->panic("Drop Connection onTimer");

}

void
CanpxHttpClient::onRecordRequestImpl(MsgGraph &image)
{
	// record?id=CANPX_GOC.575_603
	
	string delim = "id=";
	string my_req, my_url, inst;
	DataCache *cache = DataCache::getCache();
	if (!cache){
		this->panic("No DataCache found");
		return;
	}
	image.getAttributeValue("request@HTTP", 0, my_req);
	if (!my_req.empty()){
		int s = my_req.find(delim);
		if ( s != string::npos){
			inst = my_req.substr(s + delim.size(),  my_req.size() -
delim.size());
		}
	}
	string pubstr = "<html><body><b><H2>KEY:VALUES OF " + inst + "</H2></b><BR>";
	pubstr += "<form name=\"update\" action=\"recordUpdate\" method=\"POST\">";
	string keyValTable;
	Datum *datum = cache->getDatum(Formatter::trim(inst));
	if (datum){
		DataGraph *data = dynamic_cast<DataGraph *>(datum);
		if (data){
			keyValTable = data->getKeyValHtmlTable(true, true, "border=1");
		}
	}
	pubstr += keyValTable + "<BR>";
	pubstr += "<input type=\"submit\" value=\"Update\">";
	pubstr += "</form></body></html>";
	publish(pubstr);
}

void
CanpxHttpClient::onResetAvolImpl(MsgGraph &image)
{
	int count = CanpxEngine::getEngine()->resetAvol();	
	char buf[218];
	sprintf(buf, "CanpxHttpClient::onResetAvolImpl updated %d entries", count);
	publish(buf);
		
}

void
CanpxHttpClient::onListRequestImpl(MsgGraph &image)
{
	cout<<"CanpxHttpClient::onListRequestImpl"<<endl;
	publish("CanpxHttpClient::onListRequestImpl");
}

void
CanpxHttpClient::onCanpxRequestImpl(MsgGraph &image)
{
	// load html with Reflection to show
	// active and non-active Contributors
	// table with all Canpx Instrument and source Instruments 
	string file = _htmlHome + "/canpx.html";
	HtmlBeanProcessor::getProcessor()->loadFile(file);
	string str = HtmlBeanProcessor::getProcessor()->process(file);
	if (str.empty()){
		publish("CanpxHttpClient::onCanpxRequestImpl HtmlBeenProcessor failed");
	} else {
		publish(str);
	}
	
}

void
CanpxHttpClient::onRecordUpdateImpl(MsgGraph &image)
{
	string my_req, my_url, inst;
	DataCache *cache = DataCache::getCache();
	if (!cache){
		this->panic("No DataCache found");
		return;
	}

	string datastr, pubstr;
	vector<string> words, postdata;
	if (image.getAttributeValue("data@HTTP", 0, datastr)){
		Formatter::split(datastr, "&", words);
		for(int i =0; i < words.size(); i++){
			postdata.push_back(HttpDataMap::toString(words[i]));
		}
		DataGraph newdata("UNKNOWN");
		newdata.buildImageFromFlat(postdata, "=");
		string newaddress = newdata.getValue("ML_GRAPH_ADDRESS");
	
		DataGraph *data = (DataGraph *)cache->getDatum(newaddress);
		if (data){
			string err = CanpxEngine::getEngine()->updateCompiledInstrument(newdata);
			pubstr += newaddress + " DataGraph Updated " + err;
		}else{
			pubstr += newaddress + " Error DataGraph not found";
		}
	}else{
		pubstr = "Error processing data@HTTP";
	}

	publish(pubstr);
}

