
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

#ifndef	WIPERFEEDSESSION_HPP
#define WIPERFEEDSESSION_HPP

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

#define 	MSG_ID_STR       "-1"
#define 	MSG_TYPE_STR     "-2"

class PageWiperApp;

class WiperFeedSession: public FeedSession,
		public OpenClient, 
		//public MsgProcessor,
		public TimerCallback
{
	public:

		enum SFormat{
			STX = 2,
			ETX = 3,
			ESC = 27,
			FS  = 28,
			GS  = 29,
			RS  = 30,
			US  = 31
		};


		enum ConnStatus {
			CONN_CLOSE = 0,
			CONN_OPEN = 1,
			CONN_INIT = 2
		};
	public:
		WiperFeedSession(PageWiperApp* app, const string& name);
		virtual ~WiperFeedSession();


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

		virtual void panic(const string& msg);
		virtual void send_STATUS_REQUEST();
		virtual void sendResetMsg();


		virtual void parseMKLMsg(MKLMsg& msg, MsgGraph* msgGraph);

		bool _need_reset;
		int _myfd;
		int conn_status;
		class FeedPublisher /*: public TimerCallback */
		{
			public:
				FeedPublisher (WiperFeedSession *cl, int fd);
				virtual ~FeedPublisher();
				virtual void publish(const MKLMsg& str);
				virtual void setFd(int fd);
				static void printMsg(char *msg, int size);

			protected:
				static void* toWrite(void *arg);
				void onWrite();


				void addMsg(MKLMsg* msg);
				WiperFeedSession * _mySession;
				int _myfd;
				vector<MKLMsg * > _publishStr;

				
		} *_publisher;

		class RecoveryMgr : public TimerCallback 
		{
			public:
				RecoveryMgr(WiperFeedSession *session):_session(session), _status(WiperFeedSession::CONN_INIT){};
				~RecoveryMgr(){};
				int _status;
				virtual void onTimer(TimerId id);
				
			protected:
				WiperFeedSession *_session;
			
		} *_recoveryMgr;

		virtual void onRead(int fd, InputId id);
		virtual void onTimer(TimerId id);
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};

		InputId _in;
		unsigned long m_nMsgID;

		MKLMsg _strBuffer;
		string _publishStr;
		map<int, pair<string, string> > _rowID;

	private:
		PageWiperApp* _myApp;
		string _name;

};
#endif //WIPERFEEDSESSION_HPP
