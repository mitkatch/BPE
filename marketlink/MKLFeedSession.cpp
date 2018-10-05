
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
#include <set>
#include <iomanip>
#include <strstream>
#include <algorithm>

#include "MKLFeedApp.hpp"
#include "ConfigMap.hpp"
#include "Logger.hpp"
#include "MKLFeedSession.hpp"
#include "MKLTag.hpp"
#include "MessageFactory.hpp"
#include "MsgParser.hpp"
#include "DataCache.hpp"
#include "Application.hpp"
#include "Scheduler.hpp"
#include "Datum.hpp"
#include "DataPage.hpp"
#include "Formatter.hpp"
#include "FTManager.hpp"
#include "MKLThrottleHandler.hpp"

#define MKL_STR	"MKL"

pthread_mutex_t _mutexLock;
pthread_cond_t _waitCond;

MKLFeedSession::MKLFeedSession(MKLFeedApp *app, const string& name):
			 _myApp(app), _name(name),
			_publisher(NULL), 
			m_nMsgID(0L), 
			_need_reset(true),
			_recoveryMgr(NULL),
			_active(false),
			_counter(0)
{ 
	_recoveryMgr = new MKLFeedSession::RecoveryMgr(this);
	_host1 = ConfigMap::getConfig()->getValue("GATEWAY_IP_1");
	_port1 = ConfigMap::getConfig()->getValue("GATEWAY_PORT_1");
	_host2 = ConfigMap::getConfig()->getValue("GATEWAY_IP_2");
	_port2 = ConfigMap::getConfig()->getValue("GATEWAY_PORT_2");

};
MKLFeedSession::~MKLFeedSession()
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
MKLFeedSession::initSession()
{
	FTManager::FT ft = FTManager::getFTManager()->getFT();
	if(ft.status == FTManager::PRIMARY){
		_active = initSessionFT();

	}else{
		addTimer(1*1000, 1, this);
	}

	return true;
}

bool
MKLFeedSession::initSessionFT()
{
	if (_recoveryMgr->_status == MKLFeedSession::CONN_INIT){
		assert(_myApp);
		_myApp->subscribeSources();
		_port = _port1;
		_host = _host1;	
	}
	//if RECOVER it's set in the recovery
	this->setPort(_port);
	this->setHost(_host);

	bool ret = true;

	conn_status = MKLFeedSession::CONN_CLOSE;
	if (this->initClient()){
		conn_status = MKLFeedSession::CONN_OPEN;
		_myfd = this->myfd();
		if (_publisher)
			_publisher->setFd(_myfd);

		newIO(_myfd, IOCallback::IO_READ, this);
	
		ret = true;
		send_STATUS_REQUEST(); 
		_need_reset = false;
		addTimer(5*1000, 1, this);
	} else {
		_recoveryMgr->_status = conn_status;
		_recoveryMgr->addTimer(5*1000, 1, _recoveryMgr);
	}
	return ret;
}

void
MKLFeedSession::recoverConnection(int status)
{
	if (_counter >= 5){
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::recoverConnection Client attempts exceeded %i. Exit", _counter);
		_exit(55);
	}
	if (status == MKLFeedSession::CONN_CLOSE){
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::recoverConnection Client recovering %i ...", _counter++);
		if (_publisher){
			//delete _publisher;
			Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::recoverConnection Publisher recovering...");
		}

		if (_port == _port1 && _host == _host1){
			_port = _port2;
			_host = _host2;

		}else if (_port == _port2 && _host == _host2){
			_port = _port1;
			_host = _host1;
		}else{
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedSession::recoverConnection failed for _host %s, _port %s", _host.c_str(), _port.c_str());
		}
		_recoveryMgr->_status = MKLFeedSession::CONN_RECOVER; 
		if (this->initSessionFT()==false){
			_recoveryMgr->_status = conn_status;
			_recoveryMgr->addTimer(5*1000, 1, _recoveryMgr);
		}
	}
}

void
MKLFeedSession::onRead(int fd, InputId id)
{
	if( fd == _myfd){
		int ntoread = 0, nread;
		int ntoread_new =0;
		char *toBuf = NULL;
		bool sucseed = false;
		// for LINUX it's FIONREAD, for SUN it's I_NREAD
		if((ioctl(_myfd, I_NREAD, &ntoread) == -1) || (ntoread == 0)){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedSession %i closed connection", _myfd);
			conn_status = MKLFeedSession::CONN_CLOSE;
			_recoveryMgr->_status = conn_status;
			_recoveryMgr->addTimer(5*1000, 1, _recoveryMgr);
			removeIO(_myfd);
			return;
		}


		char *szBuffer = new char[ntoread + 1];
		nread = read(_myfd, szBuffer, ntoread);

		//cout << "onRead size ["<<ntoread<<"] bytes"<<endl;
		MKLFeedSession::FeedPublisher::printMsg(szBuffer, nread);
		if (nread <= 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedSession %i read failed", _myfd);
			conn_status = MKLFeedSession::CONN_CLOSE;
			_recoveryMgr->_status = conn_status;
			_recoveryMgr->addTimer(5*1000, 1, _recoveryMgr);
			removeIO(_myfd);
			return;
		}	

		//string tmpmsg(szBuffer, ntoread);
		MKLMsg tmpmsg(szBuffer, ntoread);

		_strBuffer.push_back(tmpmsg);

		//_strBuffer.printMsg();
		//int buffSize = _strBuffer.length();
		bool b_processed = false;
		do {
			vector<char>::iterator fs1, fs2, rs1;
			vector<char> v = _strBuffer.getMsgAsVector();
			b_processed = false;

			//cout << "onRead strBuffer.size()=["<<_strBuffer.size()<<"] "<<endl;
			fs1 = std::find(v.begin(), v.end(),(char)MKLFeedSession::FS);
			if (fs1 != v.end() ){
				fs2 = std::find(fs1 + 1, v.end(), (char)MKLFeedSession::FS);
				// potential problem forwarding of the iterator 
				rs1 = std::find(fs1 + 1, v.end(), (char)MKLFeedSession::RS);
				if ( fs2 != v.end() && rs1 != v.end() && fs2 > rs1){
					MKLMsg msg(vector<char>(fs1, fs2+1));
					MsgGraph msgGraph;
					parseMKLMsg(msg, &msgGraph);
					//string strTemp = _strBuffer;
					vector<char> tv(fs2+1, v.end());	
					MKLMsg mtv(tv);
					_strBuffer = mtv;
					b_processed = true;
					//cout << "onRead processed strBuffer.size()=["<<_strBuffer.size()<<"] "<<endl;
				}
			}
		} while (b_processed);
		if (szBuffer)
			delete [] szBuffer;	
	}		
	//cout << "onRead end strBuffer.size()=["<<_strBuffer.size()<<"] "<<endl;
	
}

void
MKLFeedSession::parseMKLMsg(MKLMsg& msg, MsgGraph* msgGraph)
{
	//Response 	<FS>407 (<US>TAG) <GS>S_CODE <RS>RIC <FS>
	//	<FS>407<US>bM<GS>415<RS>CANPXTEST02<US>701<US>1399<US>20<US>3500<FS>

	Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::parseMKLMsg ");
	vector<char> v = msg.getMsgAsVector();
	vector<char>::iterator fs1 = v.end(); 
	vector<char>::iterator us1 = v.end(); 
	vector<char>::iterator us2 = v.end(); 
	vector<char>::iterator us3 = v.end(); 
	vector<char>::iterator us4 = v.end(); 
	vector<char>::iterator us5 = v.end(); 
	vector<char>::iterator gs1 = v.end(); 
	vector<char>::iterator rs1 = v.end(); 
	vector<char>::iterator fs2 = v.end();

	vector<char> v_tmp;
	string msgType, s_code, ric, tag_str;
	int tag = -1;  
	fs1 = std::find(v.begin(), v.end(),(char)MKLFeedSession::FS);
	if (fs1 != v.end() ){
		us1 = std::find(fs1 + 1, v.end(), (char)MKLFeedSession::US);
		if (us1 != v.end())
			gs1 = std::find(us1 + 1, v.end(), (char)MKLFeedSession::GS);
		if (gs1 != v.end())
			rs1 = std::find(gs1 + 1, v.end(), (char)MKLFeedSession::RS);
		if (rs1 != v.end())
			us2 = std::find(rs1 + 1, v.end(), (char)MKLFeedSession::US);
		if (us2 != v.end())
			us3 = std::find(us2 + 1, v.end(), (char)MKLFeedSession::US);
		if (rs1 != v.end())
			fs2 = std::find(rs1 + 1, v.end(),(char)MKLFeedSession::FS);
		if ( gs1 != v.end() && us1 != v.end() && gs1 > us1){
			v_tmp = vector<char>(fs1 +1, us1);
			msgType = parseMKLMsgStr(v_tmp);	
			if (msgType != "407"){
				Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::parseMKLMsg msgType %s", msgType.c_str());
			}
		}
		if ( gs1 != v.end() && rs1 != v.end() && rs1 > gs1){
			v_tmp = vector<char>(gs1 +1, rs1);
			s_code = parseMKLMsgStr(v_tmp);	
		}
		if ( us1 != v.end() && gs1 != v.end() && gs1 > us1){
			v_tmp = vector<char>(us1 +1, gs1);
			tag_str = parseMKLMsgStr(v_tmp);
			MKLTag::TAG t(v_tmp);
			tag = MKLTag::tagVal(t);	

		}
		if (s_code == THROTTLE_WARNING 
			|| s_code == THROTTLE_SEVERE_WARNING
			|| s_code == THROTTLE_EXCEEDED){
			int sec = 0;
			int rate = 0;
			int credit = 0;
			int ceiling = 0;
			if (us3 != v.end())
				us4 = std::find(us3 + 1, v.end(), (char)MKLFeedSession::US);
			if (us4 != v.end())
				us5 = std::find(us4 + 1, v.end(), (char)MKLFeedSession::US);
			if ( us2 != v.end() && rs1 != v.end() && us2 > rs1){
				v_tmp = vector<char>(rs1 +1, us2);
				ric = parseMKLMsgStr(v_tmp);	
			}
			if ( us2 != v.end() && us3 != v.end() && us3 > us2){
				v_tmp = vector<char>(us2 +1, us3);
				string s_sec = parseMKLMsgStr(v_tmp);	
				sec = Formatter::ATOF(s_sec.c_str());
			}
			if ( us3 != v.end() && us4 != v.end() && us4 > us3){
				v_tmp = vector<char>(us3 +1, us4);
				string s_credit = parseMKLMsgStr(v_tmp);	
				credit = Formatter::ATOF(s_credit.c_str());
				MKLThrottleHandler::setCredit(ric, credit);
			}
			if ( us4 != v.end() && us5 != v.end() && us5 > us4){
				v_tmp = vector<char>(us4 +1, us5);
				string s_rate = parseMKLMsgStr(v_tmp);	
				rate = Formatter::ATOF(s_rate.c_str());
				MKLThrottleHandler::setRate(ric, rate);
			}
			if ( us5 != v.end() && fs2 != v.end() && fs2 > us5){
				v_tmp = vector<char>(us5 +1, fs2);
				string s_ceiling = parseMKLMsgStr(v_tmp);	
				ceiling = Formatter::ATOF(s_ceiling.c_str());
				MKLThrottleHandler::setCeiling(ric, ceiling);
			}
			if (ric.size() && sec && s_code == THROTTLE_WARNING){
				MKLThrottleHandler::addWaitTime(ric, sec);
			}

			Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::parseMKLMsg Throttle s_code [%s], ric [%s], tag [%i], sec [%i]", s_code.c_str(), ric.c_str(), tag, sec);
		} else if (s_code == TTL_EXCEEDED){
			if ( fs2 != v.end() && rs1 != v.end() && fs2 > rs1){
				v_tmp = vector<char>(rs1 +1, fs2);
				ric = parseMKLMsgStr(v_tmp);	
			}
			if (ric.size()){
				MKLThrottleHandler::addRefresh(ric);
			}

		} else if (s_code != "0"){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedSession::parseMKLMsg not handled s_code [%s], ric [%s], tag [%i], tag_str [%s]", s_code.c_str(), ric.c_str(), tag, tag_str.c_str());


		}
	}
}

string
MKLFeedSession::parseMKLMsgStr(const vector<char>& v)
{
	string type;
	for (int i = 0; i < v.size(); i++){
		//if (isalpha(v[i]))
			type.append(1, v[i]);
	}

	return type;

}

void
MKLFeedSession::defaultMsgType(string& msgType, MsgGraph& msg)
{
	Logger::getLogger()->log(Logger::TYPE_ERROR, "Unknown msg type %s for msg", msgType.c_str());
	panic(string("Unknown msg type ") + msgType);
}

/*****
bool
MKLFeedSession::onParserRequest(MsgGraph &image, MsgProcessor *processor)
{
	MKLFeedSession *client = (MKLFeedSession *)processor;
	if (client){
		client->onRequest(image);
	}
	return true;
}
*****/

void
MKLFeedSession::send_STATUS_REQUEST()
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
MKLFeedSession::sendResetMsg()
{
	/**********

	MKLMsg msg;
	m_nMsgID = 0;
	msg.push_back(char(MKLFeedSession::US));
	msg.push_back(FEED_RESET);
	msg.push_back(char(MKLFeedSession::GS));
	publish(msg);

	*********/
}

void 
MKLFeedSession::publishInsertRequest(const DataGraph::DataMap& rec, const string& sec_id)
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
	tmpmsg.push_back((char)US);
	MKLTag::TAG t = MKLTag::getTag()->tag();
	tmpmsg.push_back(t[0]);
	tmpmsg.push_back(t[1]);
	tmpmsg.push_back((char)GS);
	tmpmsg.push_back(sec_id);
	tmpmsg.push_back(fields);
	tmpmsg.push_back((char)FS);
	unsigned short length = calculateLength(tmpmsg);
	MKLMsg mklmsg;
	mklmsg.push_back(length);
	mklmsg.push_back(tmpmsg);
	Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::MKLFeedSession::publishInsertRequest publishing TAG %i, sec_id %s", MKLTag::tagVal(t), sec_id.c_str());
	publish(mklmsg);	
}

void
MKLFeedSession::publish(const MKLMsg& msg)
{
	if (_publisher == NULL){
		_publisher = new FeedPublisher(this, _myfd);

	}
	//cout << "MKLFeedSession::publish"<<endl;
	//msg.printMsg();
	_publisher->publish(msg);

}

unsigned short 
MKLFeedSession::calculateLength(const MKLMsg& msg)
{
	unsigned short s = msg.size();
	return htons(s);

}

char
MKLFeedSession::calculateCRC(const vector<char>& szMsg)
{
	char crc = 0;
	
	for (int i = 0; i < szMsg.size(); ++i) {
		crc ^= szMsg[i];
	}
	
	return crc;
}

MKLFeedSession::FeedPublisher::FeedPublisher (MKLFeedSession *cl, int fd):_mySession(cl)
{
	pthread_mutex_init (&_mutexLock, NULL);
	pthread_cond_init (&_waitCond, NULL);
	setFd(fd);
	pthread_t tid;
	pthread_create(&tid, NULL, MKLFeedSession::FeedPublisher::toWrite, this);
}

void
MKLFeedSession::FeedPublisher::setFd(int fd)
{
	//set off non-blocking
	_myfd = fd;
	
	int flags = fcntl(_myfd, F_GETFL, 0);
	flags &= ~O_NONBLOCK;
	//flags &= ~O_NDELAY;
	if (fcntl(_myfd, F_SETFL, flags) < 0){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedSession::FeedPublisher F_SETFL  failed");
	}
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	setsockopt(_myfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
		

}

MKLFeedSession::FeedPublisher::~FeedPublisher()
{
	pthread_mutex_destroy(&_mutexLock);
	pthread_cond_destroy(&_waitCond);
}

void
MKLFeedSession::FeedPublisher::onWrite()
{
	//pthread_mutex_lock(&_mutexLock); already locked
	int n = 0 , j = 0;
	vector<MKLMsg *>::iterator it = _publishStr.begin();
	while(it != _publishStr.end()){	
		MKLMsg* msg = (*it);	
		printMsg(msg->getMsg(), msg->size());
		char *chMsg = msg->getMsg();
		int msgSize = msg->size();
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::FeedPublisher::onWrite sending msgID [%li], @[%s]", msg->msgID(), MKLFeedSession::getUTime());
		//int flags = fcntl(_myfd, F_GETFL, 0);
		//if ((flags & O_NONBLOCK) == O_NONBLOCK)
		//	cout << "it is O_NONBLOCK"<<endl;
		n = send(_myfd, chMsg, msgSize, 0);
		j++;
		if (n  < 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedSession::onWrite %s size[%i] failed", strerror(errno), msg->size());
			break;
		} else{
			Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::FeedPublisher::onWrite sent size [%i] msgID [%li], @[%s]", n, msg->msgID(), MKLFeedSession::getUTime());
			if ( n != msgSize )	
				Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedSession::FeedPublisher::onWrite chunk sent %i ", n);
			it = _publishStr.erase(it);
			delete msg;
		}
	}
	//pthread_mutex_unlock(&_mutexLock);
	Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedSession::FeedPublisher::onWrite sent %i msgs", j);
}

void
MKLFeedSession::FeedPublisher::printMsg(char *msg, int size)
{
	string filename = ConfigMap::getConfig()->getValue("CAPTURE_MSG");
	if (filename.empty())
		return;

	FILE *m_stream = NULL;
	string utime = MKLFeedSession::getUTime();
	if ((m_stream = fopen(filename.c_str(), "a+")) != NULL) {
		string begin = string("@") + utime + string("->");
		fprintf(m_stream, begin.c_str());
		for (int i = 0; i < size; i++){
			if (isprint(msg[i]))
				fprintf(m_stream, "%c", msg[i]);
			else
				fprintf(m_stream, "{%X}", msg[i]);
			if ((i != 0) && !(i%80)){
				fprintf(m_stream, "\n");
				fprintf(m_stream, "\n@%s->", utime.c_str());
			}

		}
		fprintf(m_stream, "<-\n");
		fflush(m_stream);
		fclose(m_stream);

	}
}


void
MKLFeedSession::FeedPublisher::publish(const MKLMsg& pm)
{
	MKLMsg *mklmsg = new MKLMsg(pm);
	// will be deleted in onWrite
	addMsg(mklmsg);

}

void
MKLFeedSession::FeedPublisher::addMsg(MKLMsg* msg)
{
	pthread_mutex_lock(&_mutexLock);
	_publishStr.push_back(msg);
	pthread_cond_signal(&_waitCond);
	pthread_mutex_unlock(&_mutexLock);

}

//static
void*
MKLFeedSession::FeedPublisher::toWrite(void *arg)
{
	//MKLFeedSession::FeedPublisher *publisher = dynamic_cast<MKLFeedSession::FeedPublisher *>(arg);	
	MKLFeedSession::FeedPublisher *publisher = (MKLFeedSession::FeedPublisher *)arg;	
	if (publisher){
		for(;;){
			pthread_cond_wait(&_waitCond, &_mutexLock);
			publisher->onWrite();
		}
	}
	return arg;
}

void
MKLFeedSession::panic(const string& msg)
{
	sendResetMsg();
}

void
MKLFeedSession::RecoveryMgr::onTimer(TimerId id)
{
	if (_status == MKLFeedSession::CONN_CLOSE)
		_session->recoverConnection(_status);


}

void
MKLFeedSession::onTimer(TimerId id)
{
	FTManager::FT ft = FTManager::getFTManager()->getFT();
	if(ft.status == FTManager::PRIMARY){
		if (!_active) 
			_active = initSessionFT();
		send_STATUS_REQUEST();
		addTimer(5*1000, 1, this);
	}else{
		addTimer(1*1000, 1, this);
	}
}

const char*
MKLFeedSession::getUTime()
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
