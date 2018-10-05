
/************************************************************************
||
|| DATE:                $Date: 2005/11/17 21:57:58 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/espeedII/RCS/ESHttpClient.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESHttpClient.hpp,v 1.1 2005/11/17 21:57:58 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ESHttpClient.hpp,v $
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef	ESHTTPCLIENT_HPP 
#define ESHTTPCLIENT_HPP 

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

class ESOpenServer; 

class ESHttpClient: public IOCallback, 
		public MsgProcessor, 
		public TimerCallback
{
	public:
		ESHttpClient(ESOpenServer *server, int fd);
		virtual ~ESHttpClient();
		virtual int getFd()const{
			return _myfd;
		}
		virtual void defaultMsgType(string& msgType, MsgGraph& msg);

		//registerd handlers	
		static bool onListRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onRecordRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onResetAvol(MsgGraph &image, MsgProcessor *processor);
		static bool onRecordUpdate(MsgGraph &image, MsgProcessor *processor);
		//callbacks for handlers
		virtual ESOpenServer * myServer()const{
			return _myServer;
		}

		virtual void publish(const string& str);
		virtual string getParameter(const string& attr, MsgGraph &image);

		void virtual onListRequestImpl(MsgGraph &image);
		void virtual onRecordRequestImpl(MsgGraph &image);
		void virtual onRecordUpdateImpl(MsgGraph &image);
	protected:

		virtual void panic(const string& msg);

		ESOpenServer *_myServer;
		int _myfd;

		class HttpClientPublisher: public IOCallback
		{
			public:
				HttpClientPublisher (ESHttpClient *cl, int fd);
				virtual ~HttpClientPublisher(){};
				virtual void publish(const string& str);
			protected:
				virtual void onRead(int fd, InputId id){};
				virtual void onWrite(int fd, InputId id);
				virtual void onExcept(int fd, InputId id){};

				ESHttpClient * _myClient;
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
};
#endif //CANPXHTTPCLIENT_HPP
