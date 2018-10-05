
/************************************************************************
||
|| DATE:                $Date: 2006/05/15 19:37:01 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/ML2TFeedSession.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ML2TFeedSession.hpp,v 1.3 2006/05/15 19:37:01 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: ML2TFeedSession.hpp,v $
|| LOG:                 Revision 1.3  2006/05/15 19:37:01  mikhailt
|| LOG:                 RecoveryMgr added
|| LOG:
|| LOG:                 Revision 1.2  2006/05/12 15:58:17  mikhailt
|| LOG:                 switch to BLOCKing mode
|| LOG:
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef	ML2TFEEDSESSION_HPP
#define ML2TFEEDSESSION_HPP

#include <string>
#include <set>
#include <utility>
#include "FeedSession.hpp"
#include "OpenClient.hpp"
#include "TimerCallback.hpp"
#include "MsgGraph.hpp"
#include "DataGraph.hpp"
#include "DataPage.hpp"
#include "MsgProcessor.hpp"

#include "ML2TMsg.hpp"

using namespace std;

#define 	MSG_ID_STR       "-1"
#define 	MSG_TYPE_STR     "-2"

class ML2TFeedApp;

class ML2TFeedSession: public FeedSession,
		public OpenClient, 
		//public MsgProcessor,
		public TimerCallback
{
	public:

		enum SFromat{
			STX = 2,
			ETX = 3,
			ESC = 27,
			FS  = 28,
			GS  = 29,
			RS  = 30,
			US  = 31
		};

		enum Header {
			MSG_ID       = -1,
			MSG_TYPE     = -2,
			MSG_VERSION  = -3,
			MSG_TO_ID    = -4,
			SESSION_ID   = -5,
			SYS_TIME         = -6,
			MSG_ID_START = -11
		};

		enum RowBase {
			SECURITY_ID        = 1,
			ROW_BASE_TEXT  = 1000,
			ROW_BASE_VIDEO = 2000
		};

		enum ConnStatus {
			CONN_CLOSE = 0,
			CONN_OPEN = 1
		};
	public:
		ML2TFeedSession(ML2TFeedApp* app, const string& name);
		virtual ~ML2TFeedSession();


		virtual bool initSession();

		virtual int getFd()const{
			return _myfd;
		}

		virtual unsigned long msgID() const{
			return m_nMsgID;
		}

		static char calculateCRC(const vector<char>& msg);

		virtual void defaultMsgType(string& msgType, MsgGraph& msg);
		//registerd handlers	
		//static bool onParserRequest(MsgGraph &image, MsgProcessor *processor);
		//callbacks for handlers

		virtual void publish(const ML2TMsg& msg);
		virtual void publishPageImage(DataPage *page, DataPage::PageSlot& slot, const string& sec_id);
		virtual void publishPageUpdate(DataPage::Update& up, const string& sec_id);
		//void virtual onRequest(MsgGraph &image);
		static const char FEED_PAGE_IMAGE[];
		static const char FEED_PAGE_UPDATE[];
		static const char FEED_RESET[];
		static const char FEED_HBNG[];
		static const char FEED_HB[];
		static const char SECURITY_ID_STR[];
		static const char* getUTime();
		virtual void recoverConnection(int status);

	protected:

		virtual void panic(const string& msg);
		virtual void sendHBMsg(const string& type);
		virtual void sendResetMsg();

		virtual void parseML2TMsg(const string& msg, MsgGraph* msgGraph);
		virtual ML2TMsg buildVideo(DataPage::Update &up);
		virtual ML2TMsg buildVideo(DataPage::PageRow& row, DataPage::PageSlot& slot);
		//virtual string buildVideo(DataPage::Update &up);
		//virtual string buildVideo(DataPage::PageRow& row, DataPage::PageSlot& slot);
		unsigned char toML2TBg(unsigned char v);
		unsigned char toML2TFg(unsigned char v);
		unsigned char toML2TVAttr(unsigned char v);

		void setRowID();
		pair<string, string> getRowID(int r);
		bool _need_reset;
		int _myfd;
		int conn_status;
		class FeedPublisher /*: public TimerCallback */
		{
			public:
				FeedPublisher (ML2TFeedSession *cl, int fd);
				virtual ~FeedPublisher();
				virtual void publish(const ML2TMsg& str);
				virtual void setFd(int fd);

			protected:
				static void* toWrite(void *arg);
				void onWrite();

				static void printMsg(char *msg, int size);
				void addMsg(ML2TMsg* msg);
				ML2TFeedSession * _mySession;
				int _myfd;
				vector<ML2TMsg * > _publishStr;

				
		} *_publisher;

		class RecoveryMgr : public TimerCallback 
		{
			public:
				RecoveryMgr(ML2TFeedSession *session):_session(session), _status(ML2TFeedSession::CONN_CLOSE){};
				~RecoveryMgr(){};
				int _status;
				virtual void onTimer(TimerId id);
				
			protected:
				ML2TFeedSession *_session;
			
		} *_recoveryMgr;

		virtual void onRead(int fd, InputId id);
		virtual void onTimer(TimerId id);
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};

		InputId _in;
		unsigned long m_nMsgID;

		string _strBuffer;
		string _publishStr;
		map<int, pair<string, string> > _rowID;

	private:
		ML2TFeedApp* _myApp;
		string _name;

};
#endif //ML2TFEEDSESSION_HPP
