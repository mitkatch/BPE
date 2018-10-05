
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


#ifndef	MKLHTTPCLIENT_HPP 
#define MKLHTTPCLIENT_HPP 

#include <string>
#include <set>
#include "IOCallback.hpp"
#include "TimerCallback.hpp"
#include "MsgProcessor.hpp"
#include "MsgGraph.hpp"
#include "DataGraph.hpp"
#include "Formatter.hpp"

using std::string;
using std::set;

class MKLOpenServer; 

class MKLHttpClient: public IOCallback, 
		public MsgProcessor, 
		public TimerCallback
{
	public:
		MKLHttpClient(MKLOpenServer *server, int fd);
		virtual ~MKLHttpClient();
		virtual int getFd()const{
			return _myfd;
		}
		virtual void defaultMsgType(string& msgType, MsgGraph& msg);

		//registerd handlers	
		static bool onFTRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onMKLRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onListRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onRefreshRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onRecordRequest(MsgGraph &image, MsgProcessor *processor);
		//callbacks for handlers
		virtual MKLOpenServer * myServer()const{
			return _myServer;
		}

		virtual void publish(const string& str);
		virtual string getParameter(const string& attr, MsgGraph &image);

		void virtual onFTRequestImpl(MsgGraph &image);
		void virtual onMKLRequestImpl(MsgGraph &image);
		void virtual onListRequestImpl(MsgGraph &image);
		void virtual onRefreshRequestImpl(MsgGraph &image);
		void virtual onRecordRequestImpl(MsgGraph &image);
	protected:

		virtual void panic(const string& msg);

		MKLOpenServer *_myServer;
		int _myfd;
		virtual string getFTStatus();

		class HttpClientPublisher: public IOCallback
		{
			public:
				HttpClientPublisher (MKLHttpClient *cl, int fd);
				virtual ~HttpClientPublisher(){};
				virtual void publish(const string& str);
			protected:
				virtual void onRead(int fd, InputId id){};
				virtual void onWrite(int fd, InputId id);
				virtual void onExcept(int fd, InputId id){};

				MKLHttpClient * _myClient;
				int _myfd;
				InputId _out;
				string _publishStr;
				
		} *_publisher;
		virtual void onRead(int fd, InputId id);
		virtual void onTimer(TimerId id);
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};
		
		InputId _in;

		string _strBuffer;
		string _publishStr;
		string _htmlHome;
};
#endif //MKLHTTPCLIENT_HPP
