
/************************************************************************
||
|| DATE:                $Date: 2004/12/10 16:47:24 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/ParserHttpClient.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ParserHttpClient.hpp,v 1.4 2004/12/10 16:47:24 mikhailt Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: ParserHttpClient.hpp,v $
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


#ifndef	PARSERHTTPCLIENT_HPP 
#define PARSERHTTPCLIENT_HPP 

#include <string>
#include <set>
#include "IOCallback.hpp"
#include "TimerCallback.hpp"
#include "MsgProcessor.hpp"
#include "MsgGraph.hpp"
#include "DataGraph.hpp"
#include "DataPage.hpp"
#include "Formatter.hpp"

using std::string;
using std::set;

class ParserOpenServer; 

class ParserHttpClient: public IOCallback, 
		public MsgProcessor, 
		public TimerCallback
		/* public Subscriber */
{
	public:
		ParserHttpClient(ParserOpenServer *server, int fd);
		virtual ~ParserHttpClient();
		virtual int getFd()const{
			return _myfd;
		}
		virtual void defaultMsgType(string& msgType, MsgGraph& msg);

		//registerd handlers	
		static bool onParserRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onListRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onRecordRequest(MsgGraph &image, MsgProcessor *processor);
		static bool onRecordUpdate(MsgGraph &image, MsgProcessor *processor);
		static bool onVideoUpdate(MsgGraph &image, MsgProcessor *processor);
		//callbacks for handlers
		virtual ParserOpenServer * myServer()const{
			return _myServer;
		}

		virtual void publish(const string& str);
		virtual string getParameter(const string& attr, MsgGraph &image);

		void virtual onParserRequestImpl(MsgGraph &image);
		void virtual onListRequestImpl(MsgGraph &image);
		void virtual onRecordRequestImpl(MsgGraph &image);
		void virtual onRecordUpdateImpl(MsgGraph &image);
		void virtual onVideoUpdateImpl(MsgGraph &image);
	protected:
		void buildHtmlAttrMap(int row, map<int, string>& htmlAttr, DataPage* page);

		virtual void panic(const string& msg);

		ParserOpenServer *_myServer;
		int _myfd;

		class HttpClientPublisher: public IOCallback
		{
			public:
				HttpClientPublisher (ParserHttpClient *cl, int fd);
				virtual ~HttpClientPublisher(){};
				virtual void publish(const string& str);
			protected:
				virtual void onRead(int fd, InputId id){};
				virtual void onWrite(int fd, InputId id);
				virtual void onExcept(int fd, InputId id){};

				ParserHttpClient * _myClient;
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
#endif //PARSERHTTPCLIENT_HPP
