
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

#include "MKLFeedApp.hpp"
#include "Logger.hpp"
#include "MKLHttpClient.hpp"
#include "MKLOpenServer.hpp"
#include "MKLPageSubscriber.hpp"
#include "Formatter.hpp"
#include "MessageFactory.hpp"
#include "MsgParser.hpp"
#include "DataCache.hpp"
#include "Application.hpp"
#include "Datum.hpp"
#include "HttpDataMap.hpp"
#include "ConfigMap.hpp"
#include "FTManager.hpp"

MKLHttpClient::MKLHttpClient(MKLOpenServer *server, int fd):
			_myServer(server), 
			_myfd(fd), 
			_publisher(NULL)
{ 
	newIO(_myfd, IOCallback::IO_READ, this);
	
	string root = server->root();
	MessageFactory::getFactory()->registerMsgHandler(root, MKLHttpClient::onMKLRequest);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/list"), MKLHttpClient::onListRequest);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/refresh"), MKLHttpClient::onRefreshRequest);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/record"), MKLHttpClient::onRecordRequest);
	MessageFactory::getFactory()->registerMsgHandler(root+string("/ft"), MKLHttpClient::onFTRequest);
	_htmlHome = ConfigMap::getConfig()->getValue("HTML_HOME");
	_publisher = new HttpClientPublisher(this, _myfd);
	addTimer(60*1000, 1, this);//timeout timer

};
MKLHttpClient::~MKLHttpClient()
{
	//cout<<"Client deleting..."<<endl;
	if (_publisher){
		delete _publisher;
		//cout<<"Publisher deleted..."<<endl;
	}
	removeIO(_myfd);
}


void
MKLHttpClient::onRead(int fd, InputId id)
{
	if( fd == _myfd){
		int ntoread = 0, nread;
		int ntoread_new =0;
		char *toBuf = NULL;
		bool sucseed = false;
		// for LINUX it's FIONREAD, for SUN it's I_NREAD
		if((ioctl(_myfd, I_NREAD, &ntoread) == -1) || (ntoread == 0)){
			Logger::getLogger()->log(Logger::TYPE_INFO, "MKLHttpClient %i closed connection", _myfd);
			_myServer->removeClient(this);
			return;
		}
		char *szBuffer = new char[ntoread + 1];
		nread = read(_myfd, szBuffer, ntoread);
		if (nread <= 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLHttpClient %i read failed", _myfd);
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
MKLHttpClient::defaultMsgType(string& msgType, MsgGraph& msg)
{
	Logger::getLogger()->log(Logger::TYPE_ERROR, "Unknown msg type %s for msg", msgType.c_str());
	panic(string("Unknown msg type ") + msgType);
}

bool
MKLHttpClient::onFTRequest(MsgGraph &image, MsgProcessor *processor)
{
	MKLHttpClient *client = (MKLHttpClient *)processor;
	if (client){
		client->onFTRequestImpl(image);
	}
	return true;
}


bool
MKLHttpClient::onRecordRequest(MsgGraph &image, MsgProcessor *processor)
{
	MKLHttpClient *client = (MKLHttpClient *)processor;
	if (client){
		client->onRecordRequestImpl(image);
	}
	return true;
}

bool
MKLHttpClient::onMKLRequest(MsgGraph &image, MsgProcessor *processor)
{
	MKLHttpClient *client = (MKLHttpClient *)processor;
	if (client){
		client->onMKLRequestImpl(image);
	}
	return true;
}

bool
MKLHttpClient::onListRequest(MsgGraph &image, MsgProcessor *processor)
{
	MKLHttpClient *client = (MKLHttpClient *)processor;
	if (client){
		client->onListRequestImpl(image);
	}
	return true;
}

bool
MKLHttpClient::onRefreshRequest(MsgGraph &image, MsgProcessor *processor)
{
	MKLHttpClient *client = (MKLHttpClient *)processor;
	if (client){
		client->onRefreshRequestImpl(image);
	}
	return true;
}

void
MKLHttpClient::publish(const string& str)
{
	_publisher->publish(str);

}

MKLHttpClient::HttpClientPublisher::HttpClientPublisher (MKLHttpClient *cl, int fd):_myClient(cl), _myfd(fd)
{


}

void
MKLHttpClient::HttpClientPublisher::onWrite(int fd, InputId id)
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
MKLHttpClient::HttpClientPublisher::publish(const string& str)
{
	_publishStr += str;
	string header = "HTTP/1.0 200 OK\r\nDate:\r\nContent-Type: text/html\r\nContent-Length: ";
	char buf[12];
	sprintf(buf,"%i", _publishStr.size());
	_publishStr = header + string(buf) + string("\r\n\r\n") + _publishStr;
	newIO(_myfd, IOCallback::IO_WRITE, this);

}

string
MKLHttpClient::getParameter(const string& attr, MsgGraph &image)
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
			Logger::getLogger()->log(Logger::TYPE_INFO, "MKLHttpClient::getParameter found key [%s] = [%s] ", attr.c_str(), val.c_str());
		}
	}
	if (!found)
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLHttpClient::getParameter not found key [%s] = [%s] ", attr.c_str(), val.c_str());


	return val;

}


void
MKLHttpClient::panic(const string& msg)
{
	string header = "<b>HTTP/1.0 404 Not Found</b><br>\r\nDate:\r\nContent-Type: text/plain\r\nContent-Length: ";
	char buf[12];
	sprintf(buf,"%i", msg.size());
	string publishStr = header + string(buf) + string("\r\n\r\n") + msg;
	_publisher->publish(publishStr);
}

void
MKLHttpClient::onTimer(TimerId id)
{
	this->panic("Drop Connection onTimer");

}

void
MKLHttpClient::onRecordRequestImpl(MsgGraph &image)
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
MKLHttpClient::onRefreshRequestImpl(MsgGraph &image)
{
	//http://10.1.4.143:6520/mkl/refresh?mlip_inst=CANPXTEST01
	string my_req, my_url, mlip_inst, pubstr = "Nothing's done";
	string delim = "mlip_inst=";
	bool found = false;
	image.getAttributeValue("request@HTTP", 0, my_req);
	if (!my_req.empty()){
		int s = my_req.find(delim);
		if ( s != string::npos){
			mlip_inst = my_req.substr(s + delim.size(),  my_req.size() -delim.size());
		}
	}
	if (mlip_inst.size()){
		MKLFeedApp::AddressBook& addressBook = theMKLFeedApp->getAddressBook();
		MKLFeedApp::AddressBook::iterator it = addressBook.begin();
		for (it; it != addressBook.end(); it++){
			MKLFeedApp::AddressChapter &ach = (*it).second;
			MKLFeedApp::AddressChapter::const_iterator ait = ach.begin();
			for (ait; ait != ach.end(); ait++){
				MKLFeedApp::AddressPage *ap = (*ait);
				if (ap->mlip_inst == mlip_inst){

					if (ap->inst_type == MKLFeedApp::PAGE_TYPE){
						MKLPageSubscriber *s = (MKLPageSubscriber*)ap->subscriber;
						if (s){
							s->publishRefresh();
							pubstr = "Refresh's sent:" + ap->inst; 
						}
					} else if (ap->inst_type == MKLFeedApp::RECORD_TYPE){
						pubstr = "Refresh's not implemented for RECORD_TYPE:" + ap->inst; 

					} else {
						pubstr = "Refresh's not implemented for NO_TYPE:" + ap->inst; 

					}

					found = true;
					break;
				}

			}
			if (found)
				break;

		}
	}
	publish(pubstr);

}

void
MKLHttpClient::onListRequestImpl(MsgGraph &image)
{
	//http://10.1.4.143:6520/mkl/list?contributor=canpx2goc

	string my_req, my_url, contr, pubstr;
	string delim = "contributor=";
	image.getAttributeValue("url@HTTP", 0, my_url);
	my_url = dirname((char *)my_url.c_str()); // returns only /mkl	
	image.getAttributeValue("request@HTTP", 0, my_req);
	if (!my_req.empty()){
		int s = my_req.find(delim);
		if ( s != string::npos){
			contr = my_req.substr(s + delim.size(),  my_req.size() -delim.size());
		}
	}

	MKLFeedApp::AddressBook& addressBook = theMKLFeedApp->getAddressBook();
	MKLFeedApp::AddressBook::iterator it = addressBook.find(Formatter::trim(contr));
	pubstr = "<html><body><b><H2>" + contr + "</H2></b><BR>";
	pubstr += "<table border=1>";
	if (it != addressBook.end()){
		MKLFeedApp::AddressChapter& c_v = (*it).second;
		MKLFeedApp::AddressChapter::const_iterator i = c_v.begin();
		for (i; i !=  c_v.end(); i++){
			MKLFeedApp::AddressPage *p = (*i);
			char buf[16];
			pubstr += "<tr>";
			sprintf(buf,"<td>%i</td>", p->id);
			pubstr += buf;
			pubstr += "<td>" + p->inst + "</td>";
			pubstr += "<td>" + p->mlip_inst + "</td>";
			pubstr += "<td>" + string(p->last_updt) + "</td>";

			string forward = my_url + "/refresh?mlip_inst=";
			string href = "<a href=\"" + forward +  p->mlip_inst + "\">";
			string c = "<td>" + href + "Refresh" + "</a></td>";
			pubstr += c;

			pubstr +="</tr>";
		}
	}	
	pubstr += "</table></body></html>";
	publish(pubstr);
}

void
MKLHttpClient::onMKLRequestImpl(MsgGraph &image)
{
	assert(theMKLFeedApp);
	vector <string> v_c;	
	string my_url, my_url2;

	image.getAttributeValue("url@HTTP", 0, my_url);
	string forward = my_url; //dirname((char *)my_url.c_str());
	forward += "/list?contributor=";
	MKLFeedApp::AddressBook& addressBook = theMKLFeedApp->getAddressBook();
	MKLFeedApp::AddressBook::iterator it = addressBook.begin();
	for (it; it != addressBook.end(); it++){
		string href = "<a href=\"" + forward +  (*it).first + "\">";
		string c = "<td>" + href + (*it).first + "</a></td>";
		int sz = ((*it).second).size();
		char buf[16];
		sprintf(buf,"<td>%i</td>", sz);
		c += buf;
		v_c.push_back(c);	
	} 	
	string pubstr = "<html><body>" + getFTStatus();
	pubstr += "<BR><b><H2>CONTRIBUTORS</H2></b><BR>";
	pubstr += "<table border=1>";
	for (int i = 0; i < v_c.size(); i++){
		string contr = "<tr>" + v_c[i] + "</tr>";
		pubstr += contr;
	}
	pubstr += "</table></body></html>";
	publish(pubstr);

}

void
MKLHttpClient::onFTRequestImpl(MsgGraph &image)
{
	assert(theMKLFeedApp);

	publish(getFTStatus());
}

string
MKLHttpClient::getFTStatus()
{
	FTManager::FT ft = FTManager::getFTManager()->getFT();
	string status = "FT_STATUS:";
	string second;
	if (ft.status == FTManager::PRIMARY){
		status += "PRIMARY";
		second = "SECONDARY ";
	}else if (ft.status == FTManager::SECONDARY){
		status += "SECONDARY";
		second = "PRIMARY ";
	}else{
		status += "UNKNOWN";
	}
	string pubstr = status + string("<br>") ;
	pubstr += second + string("HOST:") + ft.host + string("<br>");
	pubstr += second + string("PORT:") + ft.port + string("<br>");
	return pubstr;

}
