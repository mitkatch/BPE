
/************************************************************************
||
|| DATE:                $Date: 2006/04/07 19:31:50 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/ML2TFeedSession.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ML2TFeedSession.cpp,v 1.1 2006/04/07 19:31:50 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ML2TFeedSession.cpp,v $
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
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
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <set>
#include <iomanip>
#include <strstream>


#include "ML2TFeedApp.hpp"
#include "Logger.hpp"
#include "ML2TFeedSession.hpp"

#include "MessageFactory.hpp"
#include "MsgParser.hpp"
#include "DataCache.hpp"
#include "Application.hpp"
#include "Scheduler.hpp"
#include "Datum.hpp"
#include "DataPage.hpp"
#include "AnsiPage/Page.h"
#include "AnsiPage/PageCell.h"

//using namespace rfa::AnsiPage;
//using namespace rfa::AnsiPage;

#define ML2T_STR	"ML2T"

const char ML2TFeedSession::FEED_PAGE_IMAGE[]  	= "25";
const char ML2TFeedSession::FEED_PAGE_UPDATE[]  	= "26";
const char ML2TFeedSession::FEED_RESET[]  	= "27";
const char ML2TFeedSession::FEED_HBNG[]  	= "28";
const char ML2TFeedSession::FEED_HB[]  		= "29";
const char ML2TFeedSession::SECURITY_ID_STR[]  		= "1";

ML2TFeedSession::ML2TFeedSession(ML2TFeedApp *app, const string& name):
			_myApp(app), _name(name),
			_publisher(NULL), m_nMsgID(0L), _need_reset(true)
{ 
	setRowID();

};
ML2TFeedSession::~ML2TFeedSession()
{
	//cout<<"Client deleting..."<<endl;
	if (_publisher){
		delete _publisher;
		//cout<<"Publisher deleted..."<<endl;
	}
	removeIO(_myfd);
}

bool
ML2TFeedSession::initSession()
{

	assert(_myApp);

	_myApp->subscribeSources();

	bool ret = false;
	string host = ConfigMap::getConfig()->getValue("GATEWAY_IP");
	string port = ConfigMap::getConfig()->getValue("GATEWAY_PORT");
	this->setPort(port);
	this->setHost(host);
	if (this->initClient()){
		conn_status = ML2TFeedSession::CONN_OPEN;
		_myfd = this->myfd();
		newIO(_myfd, IOCallback::IO_READ, this);
	
		ret = true;
		sendResetMsg(); 
		_need_reset = false;
		addTimer(1*1000, 1, this);//reset timer
	}
	return ret;
}

void
ML2TFeedSession::recoverConnection(int status)
{
	if (status == ML2TFeedSession::CONN_CLOSE){
		Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedSession::recoverConnection Client deleting...");
		if (_publisher){
			delete _publisher;
			Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedSession::recoverConnection Publisher deleted...");
		}
		removeIO(_myfd);

		this->initSession();
	}
}

void
ML2TFeedSession::onRead(int fd, InputId id)
{
	if( fd == _myfd){
		int ntoread = 0, nread;
		int ntoread_new =0;
		char *toBuf = NULL;
		bool sucseed = false;
		// for LINUX it's FIONREAD, for SUN it's I_NREAD
		if((ioctl(_myfd, I_NREAD, &ntoread) == -1) || (ntoread == 0)){
			Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedSession %i closed connection", _myfd);
			conn_status = ML2TFeedSession::CONN_CLOSE;
			recoverConnection(conn_status);
			return;
		}


		return; // we don't expect anything from the server 
		//////////////////////////////////////////////////
		/////////////////////////////////////////////////
		/**************************
		char *szBuffer = new char[ntoread + 1];
		nread = read(_myfd, szBuffer, ntoread);

		//cout << "onRead ["<<szBuffer<<"]"<<endl;

		if (nread <= 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TFeedSession %i read failed", _myfd);
			conn_status = ML2TFeedSession::CONN_CLOSE;
			return ;
		}	

		string tmpmsg(szBuffer, ntoread);
		_strBuffer += tmpmsg;
		//int buffSize = _strBuffer.length();
		int buffSize = _strBuffer.find_first_of(ML2TFeedSession::ETX);
		if (buffSize != string::npos && _strBuffer.size() > (buffSize +1)){
			buffSize += 1; //for CRC
		}

		while(buffSize > 0){
			string msg(_strBuffer, buffSize);
			MsgGraph msgGraph;
			parseML2TMsg(msg, &msgGraph);
			//MessageFactory::getFactory()->processMsg(url, httpImage , this);
			//string strTemp = _strBuffer;
			//_strBuffer = strTemp.substr(end+4, strTemp.length());	
			
		}
		if (szBuffer)
			delete [] szBuffer;	
		******************************/
	}		
	
}

void
ML2TFeedSession::parseML2TMsg(const string& msg, MsgGraph* msgGraph)
{

	string id, val;
	string::size_type posRS, posUS;

	string::size_type lastPos = 0;

	while ( (posRS = msg.find(char(ML2TFeedSession::RS), lastPos)) != string::npos ){
		posUS = msg.find(char(ML2TFeedSession::US), posRS);
		if (posUS == string::npos)
			break;

		//field id
		id = msg.substr(posRS + 1, posUS - posRS - 1);
		lastPos = string::npos;
		for( unsigned i = posUS;
			lastPos == string::npos && i < msg.size();
			i++){
				if( msg[i] == char(ML2TFeedSession::GS) ||
					msg[i] == char(ML2TFeedSession::RS) ||
					msg[i] == char(ML2TFeedSession::FS)){
						lastPos = i;
				}
		}	

		val = msg.substr(posUS+1, lastPos - posUS - 1);

		msgGraph->addGraphNode(ML2T_STR, 0, id, val);
	}

}

void
ML2TFeedSession::defaultMsgType(string& msgType, MsgGraph& msg)
{
	Logger::getLogger()->log(Logger::TYPE_ERROR, "Unknown msg type %s for msg", msgType.c_str());
	panic(string("Unknown msg type ") + msgType);
}

/*****
bool
ML2TFeedSession::onParserRequest(MsgGraph &image, MsgProcessor *processor)
{
	ML2TFeedSession *client = (ML2TFeedSession *)processor;
	if (client){
		client->onRequest(image);
	}
	return true;
}
*****/

void
ML2TFeedSession::sendHBMsg(const string& type)
{
	/***** NO HB messages
	string msg="";

	//// ACTUAL MSG BEGINS
	msg += char(ML2TFeedSession::US);
	msg += type;
	if (type == ML2TFeedSession::FEED_HBNG){
		msg += char(ML2TFeedSession::RS);
		msg += "8191";                          // interval
		msg += char(ML2TFeedSession::US);
		msg += "10";
		msg += char(ML2TFeedSession::RS);
		msg += "8190";
		msg += char(ML2TFeedSession::US);
		msg += "3";
	}
	publish( msg );
	*****/
}

void
ML2TFeedSession::sendResetMsg()
{
	ML2TMsg msg;

	m_nMsgID = 0;
	msg.push_back(char(ML2TFeedSession::US));
	msg.push_back(FEED_RESET);
	msg.push_back(char(ML2TFeedSession::GS));

	publish(msg);
}

void
ML2TFeedSession::publishPageImage(DataPage *page, DataPage::PageSlot& slot, const string& sec_id)
{

	ML2TMsg msg;
	msg.push_back(char(ML2TFeedSession::US));
	msg.push_back(ML2TFeedSession::FEED_PAGE_IMAGE);
	msg.push_back(char(ML2TFeedSession::GS));
	msg.push_back(char(ML2TFeedSession::RS));
	msg.push_back(ML2TFeedSession::SECURITY_ID_STR);
	msg.push_back(char(ML2TFeedSession::US));
	msg.push_back(sec_id);
	for (int i = slot.row1; i <= slot.row2; i++){
		msg.push_back(char(ML2TFeedSession::RS));
		pair<string, string> rowID = getRowID(i+ 1 +1); //first row 0 vs 1 and skip first row (Reuters reqirement)
		msg.push_back(rowID.first);
		msg.push_back(char(ML2TFeedSession::US));
		msg.push_back(page->rowAsPageRow(i)); //change to PageRow
		msg.push_back(char(ML2TFeedSession::RS));
		msg.push_back(rowID.second);
		msg.push_back(char(ML2TFeedSession::US));
		msg.push_back(buildVideo(page->rowAsPageRow(i), slot));//build and add video here
	}
	Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedSession::publishPageImage [%s] Image Publishing [%s]", sec_id.c_str(), (page->address()).c_str());

	this->publish(msg);

}

void 
ML2TFeedSession::publishPageUpdate(DataPage::Update& up, const string& sec_id)
{
	ML2TMsg msg;
	msg.push_back(char(ML2TFeedSession::US));
	msg.push_back(ML2TFeedSession::FEED_PAGE_UPDATE);
	msg.push_back(char(ML2TFeedSession::GS));
	msg.push_back(char(ML2TFeedSession::RS));
	msg.push_back(ML2TFeedSession::SECURITY_ID_STR);
	msg.push_back(char(ML2TFeedSession::US));
	msg.push_back(sec_id);
	msg.push_back(char(ML2TFeedSession::RS));
	pair<string, string> rowID = getRowID(up.row +1 +1); //skip first row
	msg.push_back(rowID.first);
	msg.push_back(char(ML2TFeedSession::US));
	//build and add ESC`,column data
	strstream str;
	str << char(ML2TFeedSession::ESC) << '`' <<up.column +1<<','<<up.data; 
	msg.push_back(str.str()); str.freeze(false);
	msg.push_back(char(ML2TFeedSession::RS));
	msg.push_back(rowID.second);
	msg.push_back(char(ML2TFeedSession::US));
	msg.push_back(buildVideo(up));//build and add video here
	Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedSession::publishPageUpdate [%s] Update Publishing [%s]", sec_id.c_str(), up.data.c_str());

	this->publish(msg);
}

void
ML2TFeedSession::publish(const ML2TMsg& msg)
{
	if (_publisher == NULL){
		_publisher = new FeedPublisher(this, _myfd);

	}
	_publisher->publish(msg);
	m_nMsgID++;

}


char
ML2TFeedSession::calculateCRC(const vector<char>& szMsg)
{
	char crc = 0;
	
	for (int i = 0; i < szMsg.size(); ++i) {
		crc ^= szMsg[i];
	}
	
	return crc;
}

unsigned char 
ML2TFeedSession::toML2TVAttr(unsigned char v)
{

	unsigned char attr = 0x0;

	if (v  == rfa::AnsiPage::PageCell::blink){
		attr |= 1 << 5; //0x20 
	}

	if (v  == rfa::AnsiPage::PageCell::plain){
		attr |= 1 << 2	; 0x4; 
	}

	if (v  == rfa::AnsiPage::PageCell::reverse){
		attr |= 1 << 6; //0x40; 
	}

	if (v  == rfa::AnsiPage::PageCell::underline){
		attr |= 1 << 4;//0x10; 
	}

	return attr;

}

unsigned char 
ML2TFeedSession::toML2TFg(unsigned char v)
{

	return 0x08; //green

}

unsigned char 
ML2TFeedSession::toML2TBg(unsigned char v)
{
	return 0x01; //black

}

ML2TMsg
ML2TFeedSession::buildVideo(DataPage::Update &up)
{
	ML2TMsg videoData;
	int size = up.data.size(); // *2;
	//wstring videoData = wstring(up.data.size() *2, '\0');

	unsigned char attr = toML2TVAttr(up.video);
	unsigned char color = toML2TFg(up.fg) | toML2TBg(up.bg) << 4;

	for (int i = 0; i < size; i++){
		videoData.push_back(color);
		videoData.push_back(attr);
	}

	return videoData;
}

ML2TMsg
ML2TFeedSession::buildVideo(DataPage::PageRow &row, DataPage::PageSlot& slot)
{
	ML2TMsg videoData;
	int size = row.size();// * 2;
	//wstring videoData = wstring(row.size() *2, '\0');
	vector<DataPage::PageGliph>::iterator it2 = row.begin();
	int i = 0;
	for(it2; it2!= row.end(); it2++, i++){
		unsigned char attr = toML2TVAttr((*it2).video);
		unsigned char color = toML2TFg((*it2).fg) | toML2TBg((*it2).bg) << 4;
		videoData.push_back(color);
		videoData.push_back(attr);
		
	}
	return videoData;
}

ML2TFeedSession::FeedPublisher::FeedPublisher (ML2TFeedSession *cl, int fd):_mySession(cl), _myfd(fd)
{


}

void
ML2TFeedSession::FeedPublisher::onWrite(int fd, InputId id)
{
	if (fd == _myfd){
		int n = 0;
		vector<ML2TMsg *>::iterator it = _publishStr.begin();
		while(it != _publishStr.end()){	
			ML2TMsg* msg = (*it);	
			//printMsg(msg->getMsg(), msg->size());
			n = send(fd, msg->getMsg(), msg->size(), 0/*MSG_DONTWAIT*/);
			if (n == -1){
				Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TFeedSession::onWrite %s size[%i] failed", strerror(errno), msg->size());
			} else{
				Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedSession::FeedPublisher::onWrite send %i ", n);
				it = _publishStr.erase(it);
				delete msg;
			}
		}


		#if 0 //just a hack, because I don't call it from the X	
		if (_publishStr.size() == 0)
		{
			this->removeIO(fd);
		} else {
			newIO(_myfd, IOCallback::IO_WRITE, this);
		}
		#endif
		
	}
}

void
ML2TFeedSession::FeedPublisher::printMsg(char *msg, int size)
{
	FILE *m_stream = NULL;
	if ((m_stream = fopen("logfile", "a+")) != NULL) {
		fprintf(m_stream, "====== BEGIN =====\n");
		for (int i = 0; i < size; i++){
			if (isprint(msg[i]))
				fprintf(m_stream, "%c", msg[i]);
			else
				fprintf(m_stream, "{%X}", msg[i]);
			if (!(i%80))
				fprintf(m_stream, "\n");

		}
		fprintf(m_stream, "\n====== END =====\n");
		fflush(m_stream);
		fclose(m_stream);

	}

}


void
ML2TFeedSession::FeedPublisher::publish(const ML2TMsg& pm)
{
	//str is the [US + MSG_TYPE value + GS + DATA]

	char strtmp[12];
	ML2TMsg msg;
	msg.push_back(char(ML2TFeedSession::FS));
	msg.push_back(char(ML2TFeedSession::RS));
	msg.push_back(MSG_ID_STR);
	msg.push_back(char(ML2TFeedSession::US));
	sprintf(strtmp, "%li", _mySession->msgID()); 
	msg.push_back(strtmp);
	msg.push_back(char(ML2TFeedSession::RS));
	msg.push_back(MSG_TYPE_STR);

	msg.push_back(pm);
	msg.push_back(char(ML2TFeedSession::FS));
	msg.push_back(char(ML2TFeedSession::ETX));
	char crc = ML2TFeedSession::calculateCRC(msg.getMsgAsVector());

	unsigned msgSize = msg.size()
					+ 1 /* CRC */ ;
	sprintf(strtmp, "%c%06d", char(ML2TFeedSession::STX), msgSize);
	
	ML2TMsg *msg1 = new ML2TMsg;
	//msg1->push_back(string(sstr.str()));
	msg1->push_back(strtmp);
	msg1->push_back(msg);

	
	msg1->push_back(crc);


	_publishStr.push_back(msg1);

	//Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedSession::FeedPublisher::publish ML2TMsg.size [%i], msgID [%li]", msg1->size(), _mySession->msgID());
	Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedSession::FeedPublisher::publish msgID [%li]", _mySession->msgID());
	//newIO(_myfd, IOCallback::IO_WRITE, this);//need to be fixed to be called from theX
	onWrite(_myfd, 0L);

}

void
ML2TFeedSession::panic(const string& msg)
{
	sendResetMsg();
}

void
ML2TFeedSession::onTimer(TimerId id)
{
	if (_need_reset){
		sendResetMsg();
	}
	Scheduler scheduler;
	if (scheduler.readCronLine(ConfigMap::getConfig()->getValue("SCHEDULER_RESET"))){
		long next = scheduler.getSecondsToNext();
		next = (next?next:86400); //24 hours
		Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TFeedSession::onTimer reset in [%li] sec", next);

		addTimer(next*1000, 1, this);
		_need_reset = true;
	}

}

pair<string, string>
ML2TFeedSession::getRowID(int r)
{
	pair<string, string> id;
	if (r > 0 && r < 30){

		id = _rowID[r];
	} 
	return id;

}


void
ML2TFeedSession::setRowID()
{
	//pair<text_id, video_id>

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				1, 
				pair<string, string>("1001", "2001"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				2, 
				pair<string, string>("1002", "2002"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				3, 
				pair<string, string>("1003", "2003"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				4, 
				pair<string, string>("1004", "2004"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				5, 
				pair<string, string>("1005", "2005"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				6, 
				pair<string, string>("1006", "2006"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				7, 
				pair<string, string>("1007", "2007"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				8, 
				pair<string, string>("1008", "2008"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				9, 
				pair<string, string>("1009", "2009"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				10, 
				pair<string, string>("1010", "2010"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				11, 
				pair<string, string>("1011", "2011"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				12, 
				pair<string, string>("1012", "2012"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				13, 
				pair<string, string>("1013", "2013"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				14, 
				pair<string, string>("1014", "2014"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				15, 
				pair<string, string>("1015", "2015"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				16, 
				pair<string, string>("1016", "2016"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				17, 
				pair<string, string>("1017", "2017"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				18, 
				pair<string, string>("1018", "2018"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				19, 
				pair<string, string>("1002", "2002"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				20, 
				pair<string, string>("1003", "2003"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				21, 
				pair<string, string>("1004", "2004"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				22, 
				pair<string, string>("1005", "2005"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				23, 
				pair<string, string>("1006", "2006"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				24, 
				pair<string, string>("1007", "2007"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				25, 
				pair<string, string>("1008", "2008"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				26, 
				pair<string, string>("1009", "2009"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				27, 
				pair<string, string>("1010", "2010"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				28, 
				pair<string, string>("1011", "2011"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				29, 
				pair<string, string>("1012", "2012"))); 

	_rowID.insert(
		map<int, pair<string, string> >::value_type(
				30, 
				pair<string, string>("1013", "2013"))); 


}


