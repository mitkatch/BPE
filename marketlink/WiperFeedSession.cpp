
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

#include <set>
#include <iomanip>
#include <strstream>
#include <algorithm>

#include "PageWiperApp.hpp"
#include "ConfigMap.hpp"
#include "Logger.hpp"
#include "WiperFeedSession.hpp"
#include "MessageFactory.hpp"
#include "MsgParser.hpp"
#include "DataCache.hpp"
#include "Application.hpp"
#include "Scheduler.hpp"
#include "Datum.hpp"
#include "DataPage.hpp"


pthread_mutex_t _mutexLock;
pthread_cond_t _waitCond;

WiperFeedSession::WiperFeedSession(PageWiperApp *app, const string& name):
			 _myApp(app), _name(name),
			_publisher(NULL), 
			m_nMsgID(0L), 
			_need_reset(true),
			_recoveryMgr(NULL)
{ 
	_recoveryMgr = new WiperFeedSession::RecoveryMgr(this);

};
WiperFeedSession::~WiperFeedSession()
{
	//cout<<"Client deleting..."<<endl;
	if (_publisher){
		delete _publisher;
		//cout<<"Publisher deleted..."<<endl;
	}
	if (_recoveryMgr)
		delete _recoveryMgr;

	removeIO(_myfd);
}

bool
WiperFeedSession::initSession()
{

	if (_recoveryMgr->_status == WiperFeedSession::CONN_INIT){
		assert(_myApp);
		string host = ConfigMap::getConfig()->getValue("GATEWAY_IP");
		string port = ConfigMap::getConfig()->getValue("GATEWAY_PORT");
		this->setPort(port);
		this->setHost(host);
	}

	bool ret = false;

	conn_status = WiperFeedSession::CONN_CLOSE;
	if (this->initClient()){
		conn_status = WiperFeedSession::CONN_OPEN;
		_myfd = this->myfd();
		if (_publisher)
			_publisher->setFd(_myfd);

		newIO(_myfd, IOCallback::IO_READ, this);
	
		ret = true;
		send_STATUS_REQUEST(); 
		_need_reset = false;
		addTimer(5*1000, 1, this);
	}

	if (ret)
		_myApp->buildPage();
	return ret;
}

void
WiperFeedSession::recoverConnection(int status)
{
	if (status == WiperFeedSession::CONN_CLOSE){
		Logger::getLogger()->log(Logger::TYPE_INFO, "WiperFeedSession::recoverConnection Client recovering...");
		if (_publisher){
			//delete _publisher;
			Logger::getLogger()->log(Logger::TYPE_INFO, "WiperFeedSession::recoverConnection Publisher recovering...");
		}


		if (this->initSession()==false){
			_recoveryMgr->_status = conn_status;
			_recoveryMgr->addTimer(5*1000, 1, _recoveryMgr);
		}
	}
}

void
WiperFeedSession::onRead(int fd, InputId id)
{
	if( fd == _myfd){
		int ntoread = 0, nread;
		int ntoread_new =0;
		char *toBuf = NULL;
		bool sucseed = false;
		// for LINUX it's FIONREAD, for SUN it's I_NREAD
		if((ioctl(_myfd, I_NREAD, &ntoread) == -1) || (ntoread == 0)){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "WiperFeedSession %i closed connection", _myfd);
			conn_status = WiperFeedSession::CONN_CLOSE;
			_recoveryMgr->_status = conn_status;
			_recoveryMgr->addTimer(5*1000, 1, _recoveryMgr);
			removeIO(_myfd);
			return;
		}


		char *szBuffer = new char[ntoread + 1];
		nread = read(_myfd, szBuffer, ntoread);

		cout << "onRead ["<<ntoread<<"] bytes"<<endl;
		WiperFeedSession::FeedPublisher::printMsg(szBuffer, nread);
		if (nread <= 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "WiperFeedSession %i read failed", _myfd);
			conn_status = WiperFeedSession::CONN_CLOSE;
			_recoveryMgr->_status = conn_status;
			_recoveryMgr->addTimer(5*1000, 1, _recoveryMgr);
			removeIO(_myfd);
			return;
		}	

		//string tmpmsg(szBuffer, ntoread);
		MKLMsg tmpmsg(szBuffer, ntoread);

		_strBuffer.push_back(tmpmsg);
		_strBuffer.printMsg();
		//int buffSize = _strBuffer.length();
		vector<char>::iterator fs1, fs2, rs1;
		vector<char> v = _strBuffer.getMsgAsVector();
		fs1 = std::find(v.begin(), v.end(),(char)WiperFeedSession::FS);
		if (fs1 != v.end() ){
			fs2 = std::find(fs1 + 1, v.end(), (char)WiperFeedSession::FS);
			// potential problem forwarding of the iterator 
			rs1 = std::find(fs1 + 1, v.end(), (char)WiperFeedSession::RS);
			if ( fs2 != v.end() && rs1 != v.end() && fs2 > rs1){
				MKLMsg msg(vector<char>(fs1, fs2+1));
				MsgGraph msgGraph;
				parseMKLMsg(msg, &msgGraph);
				//string strTemp = _strBuffer;
				vector<char> tv(fs2+1, v.end());	
				cout << "Found MSG to process!!"<<endl;
				MKLMsg mtv(tv);
				_strBuffer = mtv;
			}
		}

		if (szBuffer)
			delete [] szBuffer;	
	}		
	
}

void
WiperFeedSession::parseMKLMsg(MKLMsg& msg, MsgGraph* msgGraph)
{
	cout << "WiperFeedSession::parseMKLMsg "<<endl;
	msg.printMsg();
}

void
WiperFeedSession::defaultMsgType(string& msgType, MsgGraph& msg)
{
	Logger::getLogger()->log(Logger::TYPE_ERROR, "Unknown msg type %s for msg", msgType.c_str());
	panic(string("Unknown msg type ") + msgType);
}

/*****
*****/

void
WiperFeedSession::send_STATUS_REQUEST()
{
	// <FS>409{<US>TAG}<GS>S_CODE<FS>
	//S_CODE="0"
	string status_request = "409";
	string s_code = string(1, '0');
	MKLMsg msg;
	msg.push_back(char(FS));
	msg.push_back(status_request);
	msg.push_back(char(GS));
	msg.push_back(s_code);
	msg.push_back(char(FS));
	unsigned short length = calculateLength(msg);
	MKLMsg mklmsg;
	mklmsg.push_back(length);
	mklmsg.push_back(msg);
	publish(mklmsg);	
}

void
WiperFeedSession::sendResetMsg()
{
	/**********

	*********/
}

void 
WiperFeedSession::publishInsertRequest(const DataGraph::DataMap& rec, const string& sec_id)
{
	//<FS>5 (<US>TAG) <GS>RIC 1{<RS>FID <US>FDAT }n <FS>
	string msg_id = "5";
	MKLMsg fields;

	DataGraph::DataMap::const_iterator it = rec.begin();
	for (it; it != rec.end(); it++){
		fields.push_back((char)RS);	
		fields.push_back((*it).first);
		fields.push_back((char)US);	
		fields.push_back(DataGraph::serializeVariantValue((*it).second));	
	}

	MKLMsg tmpmsg;
	tmpmsg.push_back((char)FS);
	tmpmsg.push_back(msg_id);
	//tmpmsg.push_back((char)US);
	//tmpmsg.push_back((TAG));
	tmpmsg.push_back((char)GS);
	tmpmsg.push_back(sec_id);
	tmpmsg.push_back(fields);
	tmpmsg.push_back((char)FS);
	unsigned short length = calculateLength(tmpmsg);
	MKLMsg mklmsg;
	mklmsg.push_back(length);
	mklmsg.push_back(tmpmsg);
	publish(mklmsg);	
}

void
WiperFeedSession::publish(const MKLMsg& msg)
{
	if (_publisher == NULL){
		_publisher = new FeedPublisher(this, _myfd);

	}
	cout << "WiperFeedSession::publish"<<endl;
	msg.printMsg();
	_publisher->publish(msg);

}

unsigned short 
WiperFeedSession::calculateLength(const MKLMsg& msg)
{
	unsigned short s = msg.size();
	return htons(s);

}

char
WiperFeedSession::calculateCRC(const vector<char>& szMsg)
{
	char crc = 0;
	
	for (int i = 0; i < szMsg.size(); ++i) {
		crc ^= szMsg[i];
	}
	
	return crc;
}

WiperFeedSession::FeedPublisher::FeedPublisher (WiperFeedSession *cl, int fd):_mySession(cl)
{
	pthread_mutex_init (&_mutexLock, NULL);
	pthread_cond_init (&_waitCond, NULL);
	setFd(fd);
	pthread_t tid;
	pthread_create(&tid, NULL, WiperFeedSession::FeedPublisher::toWrite, this);
}

void
WiperFeedSession::FeedPublisher::setFd(int fd)
{
	//set off non-blocking
	_myfd = fd;
	
	int flags = fcntl(_myfd, F_GETFL, 0);
	flags &= ~O_NONBLOCK;
	//flags &= ~O_NDELAY;
	if (fcntl(_myfd, F_SETFL, flags) < 0){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "WiperFeedSession::FeedPublisher F_SETFL  failed");
	}
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	setsockopt(_myfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
		

}

WiperFeedSession::FeedPublisher::~FeedPublisher()
{
	pthread_mutex_destroy(&_mutexLock);
	pthread_cond_destroy(&_waitCond);
}

void
WiperFeedSession::FeedPublisher::onWrite()
{
	//pthread_mutex_lock(&_mutexLock); already locked
	int n = 0 , j = 0;
	vector<MKLMsg *>::iterator it = _publishStr.begin();
	while(it != _publishStr.end()){	
		MKLMsg* msg = (*it);	
		//printMsg(msg->getMsg(), msg->size());
		char *chMsg = msg->getMsg();
		int msgSize = msg->size();
		Logger::getLogger()->log(Logger::TYPE_INFO, "WiperFeedSession::FeedPublisher::onWrite sending msgID [%li], @[%s]", msg->msgID(), WiperFeedSession::getUTime());
		//int flags = fcntl(_myfd, F_GETFL, 0);
		//if ((flags & O_NONBLOCK) == O_NONBLOCK)
		//	cout << "it is O_NONBLOCK"<<endl;
		n = send(_myfd, chMsg, msgSize, 0);
		j++;
		if (n  < 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "WiperFeedSession::onWrite %s size[%i] failed", strerror(errno), msg->size());
			break;
		} else{
			Logger::getLogger()->log(Logger::TYPE_INFO, "WiperFeedSession::FeedPublisher::onWrite sent size [%i] msgID [%li], @[%s]", n, msg->msgID(), WiperFeedSession::getUTime());
			if ( n != msgSize )	
				Logger::getLogger()->log(Logger::TYPE_ERROR, "WiperFeedSession::FeedPublisher::onWrite chunk sent %i ", n);
			it = _publishStr.erase(it);
			delete msg;
		}
	}
	//pthread_mutex_unlock(&_mutexLock);
	Logger::getLogger()->log(Logger::TYPE_INFO, "WiperFeedSession::FeedPublisher::onWrite sent %i msgs", j);
}

void
WiperFeedSession::FeedPublisher::printMsg(char *msg, int size)
{
	string filename = ConfigMap::getConfig()->getValue("CAPTURE_MSG");
	if (filename.empty())
		return;

	FILE *m_stream = NULL;
	if ((m_stream = fopen(filename.c_str(), "a+")) != NULL) {
		string begin = string("== BEGIN ==@ ") + string(WiperFeedSession::getUTime()) + string("\n");
		fprintf(m_stream, begin.c_str());
		for (int i = 0; i < size; i++){
			if (isprint(msg[i]))
				fprintf(m_stream, "%c", msg[i]);
			else
				fprintf(m_stream, "{%X}", msg[i]);
			if ((i != 0) && !(i%80))
				fprintf(m_stream, "\n");

		}
		fprintf(m_stream, "\n== END ==\n");
		fflush(m_stream);
		fclose(m_stream);

	}
}


void
WiperFeedSession::FeedPublisher::publish(const MKLMsg& pm)
{
	MKLMsg *mklmsg = new MKLMsg(pm);
	// will be deleted in onWrite
	addMsg(mklmsg);

}

void
WiperFeedSession::FeedPublisher::addMsg(MKLMsg* msg)
{
	pthread_mutex_lock(&_mutexLock);
	_publishStr.push_back(msg);
	pthread_cond_signal(&_waitCond);
	pthread_mutex_unlock(&_mutexLock);

}

//static
void*
WiperFeedSession::FeedPublisher::toWrite(void *arg)
{
	//WiperFeedSession::FeedPublisher *publisher = dynamic_cast<WiperFeedSession::FeedPublisher *>(arg);	
	WiperFeedSession::FeedPublisher *publisher = (WiperFeedSession::FeedPublisher *)arg;	
	if (publisher){
		for(;;){
			pthread_cond_wait(&_waitCond, &_mutexLock);
			publisher->onWrite();
		}
	}
	return arg;
}

void
WiperFeedSession::panic(const string& msg)
{
	sendResetMsg();
}

void
WiperFeedSession::RecoveryMgr::onTimer(TimerId id)
{
	if (_status == WiperFeedSession::CONN_CLOSE)
		_session->recoverConnection(_status);


}

void
WiperFeedSession::onTimer(TimerId id)
{
	cout << "onTimer @"<< getUTime()<<endl;
	send_STATUS_REQUEST();
	addTimer(5*1000, 1, this);
}

const char*
WiperFeedSession::getUTime()
{
	char t[56];
	struct timeval tv;
	struct timezone tz;
	struct tm *tm;
	gettimeofday(&tv, &tz);
	tm = localtime(&tv.tv_sec);
	sprintf(t, "%d:%02d:%02d.%d", tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec);
	return t;
}
