
/************************************************************************
||
|| DATE:                $Date: 2004/12/10 16:47:24 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/ParserHttpClient.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ParserHttpClient.cpp,v 1.4 2004/12/10 16:47:24 mikhailt Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: ParserHttpClient.cpp,v $
|| LOG:                 Revision 1.4  2004/12/10 16:47:24  mikhailt
|| LOG:                 add onVideoUpdate
|| LOG:
|| LOG:                 Revision 1.3  2004/12/03 18:45:08  mikhailt
|| LOG:                 handles HTTP POST
|| LOG:
|| LOG:                 Revision 1.2  2004/11/15 17:40:09  mikhailt
|| LOG:                 more-less stable
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:44:25  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:42:08  mikhailt
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
#include "ParserHttpClient.hpp"
#include "ParserOpenServer.hpp"
#include "MessageFactory.hpp"
#include "MsgParser.hpp"
#include "DataCache.hpp"
#include "Application.hpp"
#include "Datum.hpp"
#include "DataPage.hpp"
#include "PyParser.hpp"
#include "HttpDataMap.hpp"

ParserHttpClient::ParserHttpClient(ParserOpenServer *server, int fd):
			_myServer(server), 
			_myfd(fd), 
			_publisher(NULL)
{ 
	newIO(_myfd, IOCallback::IO_READ, this);
	
	NSMap urlns = _myServer->getNameSpace();
	NSMap::iterator it = urlns.begin();
	for (it; it != urlns.end(); it++){
		//cout << "Register Clients for "<<(*it).second.url<<endl;
		MessageFactory::getFactory()->registerMsgHandler((*it).second.url, ParserHttpClient::onParserRequest);
		MessageFactory::getFactory()->registerMsgHandler((*it).second.url+string("/list"), ParserHttpClient::onListRequest);
		MessageFactory::getFactory()->registerMsgHandler((*it).second.url+string("/record"), ParserHttpClient::onRecordRequest);
		MessageFactory::getFactory()->registerMsgHandler((*it).second.url+string("/recordUpdate"), ParserHttpClient::onRecordUpdate);
		MessageFactory::getFactory()->registerMsgHandler((*it).second.url+string("/videoUpdate"), ParserHttpClient::onVideoUpdate);
	}

	_publisher = new HttpClientPublisher(this, _myfd);
	addTimer(60*1000, 1, this);//timeout timer

};
ParserHttpClient::~ParserHttpClient()
{
	//cout<<"Client deleting..."<<endl;
	if (_publisher){
		delete _publisher;
		//cout<<"Publisher deleted..."<<endl;
	}
	removeIO(_myfd);
}


void
ParserHttpClient::onRead(int fd, InputId id)
{
	if( fd == _myfd){
		int ntoread = 0, nread;
		int ntoread_new =0;
		char *toBuf = NULL;
		bool sucseed = false;
		// for LINUX it's FIONREAD, for SUN it's I_NREAD
		if((ioctl(_myfd, I_NREAD, &ntoread) == -1) || (ntoread == 0)){
			Logger::getLogger()->log(Logger::TYPE_INFO, "ParserHttpClient %i closed connection", _myfd);
			_myServer->removeClient(this);
			return;
		}
		char *szBuffer = new char[ntoread + 1];
		nread = read(_myfd, szBuffer, ntoread);
		if (nread <= 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "ParserHttpClient %i read failed", _myfd);
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
ParserHttpClient::defaultMsgType(string& msgType, MsgGraph& msg)
{
	Logger::getLogger()->log(Logger::TYPE_ERROR, "Unknown msg type %s for msg", msgType.c_str());
	panic(string("Unknown msg type ") + msgType);
}

bool
ParserHttpClient::onRecordUpdate(MsgGraph &image, MsgProcessor *processor)
{
	cout << "ParserHttpClient::onRecordUpdate"<<endl;
	ParserHttpClient *client = (ParserHttpClient *)processor;
	if (client){
		client->onRecordUpdateImpl(image);
	}
	return true;
}

bool
ParserHttpClient::onVideoUpdate(MsgGraph &image, MsgProcessor *processor)
{
	ParserHttpClient *client = (ParserHttpClient *)processor;
	if (client){
		client->onVideoUpdateImpl(image);
	}
	return true;
}

bool
ParserHttpClient::onRecordRequest(MsgGraph &image, MsgProcessor *processor)
{
	ParserHttpClient *client = (ParserHttpClient *)processor;
	if (client){
		client->onRecordRequestImpl(image);
	}
	return true;
}

bool
ParserHttpClient::onParserRequest(MsgGraph &image, MsgProcessor *processor)
{
	ParserHttpClient *client = (ParserHttpClient *)processor;
	if (client){
		client->onParserRequestImpl(image);
	}
	return true;
}

bool
ParserHttpClient::onListRequest(MsgGraph &image, MsgProcessor *processor)
{
	ParserHttpClient *client = (ParserHttpClient *)processor;
	if (client){
		client->onListRequestImpl(image);
	}
	return true;
}

void
ParserHttpClient::publish(const string& str)
{
	_publisher->publish(str);

}

ParserHttpClient::HttpClientPublisher::HttpClientPublisher (ParserHttpClient *cl, int fd):_myClient(cl), _myfd(fd)
{


}

void
ParserHttpClient::HttpClientPublisher::onWrite(int fd, InputId id)
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
ParserHttpClient::HttpClientPublisher::publish(const string& str)
{
	_publishStr += str;
	string header = "HTTP/1.0 200 OK\r\nDate:\r\nContent-Type: text/html\r\nContent-Length: ";
	char buf[12];
	sprintf(buf,"%i", _publishStr.size());
	_publishStr = header + string(buf) + string("\r\n\r\n") + _publishStr;
	newIO(_myfd, IOCallback::IO_WRITE, this);

}

string
ParserHttpClient::getParameter(const string& attr, MsgGraph &image)
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
			Logger::getLogger()->log(Logger::TYPE_INFO, "ParserHttpClient::getParameter found key [%s] = [%s] ", attr.c_str(), val.c_str());
		}
	}
	if (!found)
		Logger::getLogger()->log(Logger::TYPE_ERROR, "ParserHttpClient::getParameter not found key [%s] = [%s] ", attr.c_str(), val.c_str());


	return val;

}


void
ParserHttpClient::panic(const string& msg)
{
	string header = "<b>HTTP/1.0 404 Not Found</b><br>\r\nDate:\r\nContent-Type: text/plain\r\nContent-Length: ";
	char buf[12];
	sprintf(buf,"%i", msg.size());
	string publishStr = header + string(buf) + string("\r\n\r\n") + msg;
	_publisher->publish(publishStr);
}

void
ParserHttpClient::onTimer(TimerId id)
{
	this->panic("Drop Connection onTimer");

}

void
ParserHttpClient::onRecordRequestImpl(MsgGraph &image)
{
	//http://192.168.5.104:5511/parser/shorcan/17646/record?id=SHORCAN.17646.425_0908
	string my_req, my_url, inst;
	string delim = "id=";
	DataCache *cache = DataCache::getCache();
	if (!cache){
		this->panic("No DataCache found");
		return;
	}
	
	string pubstr = "<html><body><b><H2>KEY:VALUES OF ";

	image.getAttributeValue("request@HTTP", 0, my_req);
	if (!my_req.empty()){
		int s = my_req.find(delim);
		if ( s != string::npos){
			inst = my_req.substr(s + delim.size(),  my_req.size() - delim.size()); 
		}
	}
	pubstr += inst + "</H2<</b><BR>";
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
ParserHttpClient::onListRequestImpl(MsgGraph &image)
{
	string my_url, my_url2, page;
	DataCache *cache = DataCache::getCache();
	if (!cache){
		this->panic("No DataCache found");
		return;
	}
	
	vector<string> inst;
	string pubstr = "<H2>LIST OF INSTRUMENTS MAPPED TO</H2><BR>";
	pubstr += "<OL>";
	image.getAttributeValue("url@HTTP", 0, my_url);
	if (!my_url.empty()){
		my_url2.append(my_url);
		page =basename(dirname((char *)my_url.c_str()));
	}
	set<string> instset = DataCache::getCache()->getAddressSet();
	set<string>::iterator it = instset.begin();
	for (it; it != instset.end(); it++){
		Datum *datum = cache->getDatum((*it));  
		if (datum){
			DataGraph *data = dynamic_cast<DataGraph *>(datum);
			if (data){
				if (data->getValue("ROW66_23") == page){
					inst.push_back(data->address());
				}
			}
		}
	}
	string forward = dirname((char *)my_url2.c_str());
	forward += "/record?id=";
	for (int i = 0; i < inst.size(); i++){
		string href = "<a href=\""+forward +inst[i]+"\">" + inst[i] +"</a>";
		pubstr += "<LI>" + href + "</LI>";	

	}
	pubstr += "</OL>";
	publish(pubstr);
}

void
ParserHttpClient::onParserRequestImpl(MsgGraph &image)
{
	string my_url;
	string address;

	image.getAttributeValue("url@HTTP", 0, my_url);
	
	NSMap urlns = _myServer->getNameSpace();
	NSMap::iterator it = urlns.begin();
	for (it; it != urlns.end(); it++){
		if ((*it).second.url == my_url){
			address = (*it).second.source + string(1, '.') + (*it).second.page;
			break;
		}
	}

	string str("<html><head></head><body>");
	str.append( "<form name=\"videoUpdate\" action=\"" + my_url +"/videoUpdate\" method=\"POST\">"); 
	str.append("R1: <input type=\"text\" name=\"r1\" /> C1: <input type=\"text\" name=\"c1\" /> ");
	str.append("Data: <input type=\"text\" name=\"data\" />" );
	str.append("Video: <input type=\"text\" name=\"video\" />" );
	str.append("<input type=\"hidden\" name=\"page\" value=\"" + address + "\"/>");
	str.append("<input type=\"submit\" value=\"VideoUpdate\"></form>");
	str.append("<table border = '1'>");

	DataPage *page = (DataPage *)DataCache::getCache()->getData(address, Datum::PAGE);
	
	if (page){
		char buf[40];

		bool ones = true;
		for (int i = 0; i < page->height(); i++){
			if (ones){
				for (int j = -1; j < page->width(); j++){
					sprintf(buf, "<td bgcolor=#009807>%d</td>", j);
					str += string(buf);
				}
				ones = false;
			}
			map<int, string> htmlAttr;
			buildHtmlAttrMap(i, htmlAttr, page);
			sprintf(buf, "<tr><td bgcolor=#009807>%d</td>", i);
			str += string(buf) + page->rowAsHtmlRow(i, htmlAttr) + string("</tr>\r\n");
		}

	}
	str.append("</table></body></html>");
	publish(str);
}

void
ParserHttpClient::onVideoUpdateImpl(MsgGraph &image)
{
	cout << "ParserHttpClient::onRecordUpdate"<<endl;
	string my_req, my_url, inst;
	DataCache *cache = DataCache::getCache();
	if (!cache){
		this->panic("No DataCache found");
		return;
	}
	
	string datastr, pubstr("onVideUpdateImpl::done ");
	vector<string> words;
	string page, data;
	int r1 = -1, c1 = -1, video = -1;
	//map<string, string> postdata;
	if (image.getAttributeValue("data@HTTP", 0, datastr)){
		Formatter::split(datastr, "&", words);
		for(int i =0; i < words.size(); i++){
			string tmp = HttpDataMap::toString(words[i]);
			int eq = tmp.find("=");
			if (eq != string::npos){
				string field = tmp.substr(0, eq);
				string val = tmp.substr(eq +1, tmp.size() - (eq +1));
				//cout<< "["<<field<<"] = ["<<val<<"]"<<endl;
				if (field == "r1")
					r1 = atoi(val.c_str());
				else if (field == "c1")
					c1 = atoi(val.c_str());
				else if (field == "video")
					video = atoi(val.c_str());
				else if (field == "data")
					data = val;
				else if (field == "page")
					page = val;
			}

		}
	}
	DataPage *datapage = (DataPage *)DataCache::getCache()->getData(page, Datum::PAGE);
	if (datapage){
		if(!datapage->update(r1, c1, data, video)){
			pubstr.append("Failed to setVideo");
		}

	}else {
		pubstr.append("Page not found ");

	}
	publish(pubstr);
}


void
ParserHttpClient::onRecordUpdateImpl(MsgGraph &image)
{
	cout << "ParserHttpClient::onRecordUpdate"<<endl;
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
			//cout <<"["<<i<<"]=["<<words[i]<<"]"<<endl;
			postdata.push_back(HttpDataMap::toString(words[i]));

		}
		DataGraph newdata("UNKNOWN");
		newdata.buildImageFromFlat(postdata, "=");
		DataGraph *data = (DataGraph *)cache->getDatum(newdata.getValue("ML_GRAPH_ADDRESS"));
		if (data){
			data->updateFromFlat(newdata);
			pubstr += "DataGraph Updated";	
		} else {
			pubstr += "Error DataGraph not found";	
		}

	}
	//cout << "onRecordUpdateImpl ["<<datastr<<"]"<<endl;
	publish(pubstr);	
}

void
ParserHttpClient::buildHtmlAttrMap(int row, map<int, string>& htmlAttr, DataPage* page)
{
	if (page){
		DataPage::PageSlot slot;
		slot.row1 = row;
		slot.row2 = row;
		slot.column1 = DataPage::MIN_COLUMN;
		slot.column2 = DataPage::MAX_COLUMN; 
		set<PageListener *> pageListeners = page->getListenersSet(slot);
		set<PageListener *>::iterator it = pageListeners.begin();
		for (it; it != pageListeners.end(); it++){
			PageListener * pl = (*it);
			PyParser *parser = dynamic_cast<PyParser *>(pl);
			if (parser){
				parser->buildHtmlAttr(htmlAttr, slot);

			}


		}
	}

}
