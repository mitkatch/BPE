/************************************************************************
||
|| DATE:                $Date: 2005/07/29 18:10:28 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxHttpClient.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxHttpClient.hpp,v 1.2 2005/07/29 18:10:28 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: CanpxHttpClient.hpp,v $
|| LOG:                 Revision 1.2  2005/07/29 18:10:28  mikhailt
|| LOG:                 handle failed attempt to getList
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef	CANPXHTTPCLIENT_HPP 
#define CANPXHTTPCLIENT_HPP 

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

class CanpxOpenServer; 

class CanpxHttpClient: public IOCallback, 
		public MsgProcessor, 
		public TimerCallback
		/* public Subscriber */
{
	public:
		CanpxHttpClient(CanpxOpenServer *server, int fd);
		virtual ~CanpxHttpClient();
		virtual int getFd()const{
			return _myfd;
		}
		virtual void defaultMsgType(string& msgType, MsgGraph& msg);

		//registerd handlers	
		static bool onCanpxRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onListRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onRecordRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onResetAvol(MsgGraph &image, MsgProcessor *processor);
		static bool onRecordUpdate(MsgGraph &image, MsgProcessor *processor);
		//callbacks for handlers
		virtual CanpxOpenServer * myServer()const{
			return _myServer;
		}

		virtual void publish(const string& str);
		virtual string getParameter(const string& attr, MsgGraph &image);

		void virtual onCanpxRequestImpl(MsgGraph &image);
		void virtual onListRequestImpl(MsgGraph &image);
		void virtual onRecordRequestImpl(MsgGraph &image);
		void virtual onResetAvolImpl(MsgGraph &image);
		void virtual onRecordUpdateImpl(MsgGraph &image);
	protected:

		virtual void panic(const string& msg);

		CanpxOpenServer *_myServer;
		int _myfd;

		class HttpClientPublisher: public IOCallback
		{
			public:
				HttpClientPublisher (CanpxHttpClient *cl, int fd);
				virtual ~HttpClientPublisher(){};
				virtual void publish(const string& str);
			protected:
				virtual void onRead(int fd, InputId id){};
				virtual void onWrite(int fd, InputId id);
				virtual void onExcept(int fd, InputId id){};

				CanpxHttpClient * _myClient;
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
#endif //CANPXHTTPCLIENT_HPP
