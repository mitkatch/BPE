
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

#ifndef	MKLFEEDSESSION_HPP
#define MKLFEEDSESSION_HPP

#include <string>
#include <set>
#include <utility>
#include "FeedSession.hpp"
#include "OpenClient.hpp"
#include "TimerCallback.hpp"
#include "MsgGraph.hpp"
#include "DataGraph.hpp"
#include "MsgProcessor.hpp"

#include "MKLMsg.hpp"

using namespace std;

#define 	THROTTLE_WARNING	"415"
#define 	THROTTLE_SEVERE_WARNING	"416"
#define 	THROTTLE_EXCEEDED	"417"
#define		TTL_EXCEEDED		"247"

class MKLFeedApp;

class MKLFeedSession: public FeedSession,
		public OpenClient, 
		//public MsgProcessor,
		public TimerCallback
{
	public:

		enum SFormat{
			STX = 0x02,
			ETX = 0x03,
			ESC = 0x1B,
			FS  = 0x1C,
			GS  = 0x1D,
			RS  = 0x1E,
			US  = 0x1F
		};


		enum ConnStatus {
			CONN_CLOSE = 0,
			CONN_OPEN = 1,
			CONN_INIT = 2,
			CONN_RECOVER = 3
		};
	public:
		MKLFeedSession(MKLFeedApp* app, const string& name);
		virtual ~MKLFeedSession();


		virtual bool initSession();

		virtual int getFd()const{
			return _myfd;
		}

		virtual unsigned long msgID() const{
			return m_nMsgID;
		}

		static char calculateCRC(const vector<char>& msg);
		static unsigned short calculateLength(const MKLMsg& msg);

		virtual void defaultMsgType(string& msgType, MsgGraph& msg);
		//registerd handlers	

		virtual void publish(const MKLMsg& msg);
		virtual void publishInsertRequest(const DataGraph::DataMap& rec ,  const string& sec_id);
		virtual void publishRecordImage(DataGraph*, const std::string &){}; 
		virtual void publishRecordUpdate(DataGraph*, const std::string &){};

		virtual void recoverConnection(int status);
		static const char* getUTime();

	protected:

		virtual bool initSessionFT();

		virtual void panic(const string& msg);
		virtual void send_STATUS_REQUEST();
		virtual void sendResetMsg();


		virtual void parseMKLMsg(MKLMsg& msg, MsgGraph* msgGraph);
		virtual string parseMKLMsgStr(const vector<char>& v);

		bool _need_reset;
		int _myfd;
		int conn_status;
		int _counter;
		class FeedPublisher /*: public TimerCallback */
		{
			public:
				FeedPublisher (MKLFeedSession *cl, int fd);
				virtual ~FeedPublisher();
				virtual void publish(const MKLMsg& str);
				virtual void setFd(int fd);
				static void printMsg(char *msg, int size);

			protected:
				static void* toWrite(void *arg);
				void onWrite();


				void addMsg(MKLMsg* msg);
				MKLFeedSession * _mySession;
				int _myfd;
				vector<MKLMsg * > _publishStr;

				
		} *_publisher;

		class RecoveryMgr : public TimerCallback 
		{
			public:
				RecoveryMgr(MKLFeedSession *session):_session(session), _status(MKLFeedSession::CONN_INIT){};
				~RecoveryMgr(){};
				int _status;
				virtual void onTimer(TimerId id);
				
			protected:
				MKLFeedSession *_session;
			
		} *_recoveryMgr;

		virtual void onRead(int fd, InputId id);
		virtual void onTimer(TimerId id);
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};

		InputId _in;
		unsigned long m_nMsgID;
		bool _active;
		MKLMsg _strBuffer;
		string _publishStr;
		map<int, pair<string, string> > _rowID;

	private:
		MKLFeedApp* _myApp;
		string _name;
		string _host, _port, _host1, _port1, _host2, _port2;

};
#endif //MKLFEEDSESSION_HPP
