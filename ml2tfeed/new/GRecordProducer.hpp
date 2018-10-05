
/************************************************************************
||
|| DATE:                $Date: 2006/04/07 19:31:50 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/GRecordProducer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: GRecordProducer.hpp,v 1.1 2006/04/07 19:31:50 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: GRecordProducer.hpp,v $
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/
#ifndef GRECORDPRODUCER_HPP
#define GRECORDPRODUCER_HPP

#include <string>
#include "ConfigMap.hpp"
#include "IOCallback.hpp"
#include "TimerCallback.hpp"
#include "MsgProcessor.hpp"
#include "MsgGraph.hpp"
#include "DataGraph.hpp"
#include "Subscriber.hpp"

#define SA struct sockaddr

class GRecordProducer: public Subscriber,
			public TimerCallback
{
	public:
		GRecordProducer(const ConfigMap &);
		virtual ~GRecordProducer();
		virtual void notify(Datum *datum);
		virtual int init();

	protected:
		class GPublisher: public IOCallback
		{
			public:	
				GPublisher(GRecordProducer *producer, int fd);
				virtual ~GPublisher();
				virtual void publish(const string& str);
			protected:
				virtual void onRead(int fd, InputId id){};
				virtual void onWrite(int fd, InputId id);
				virtual void onExcept(int fd, InputId id){};
				InputId _out;
				string _publishStr;
				GRecordProducer *_producer;
				int _fd;
		} *_publisher; 

		virtual void onRead(int fd, InputId id){};
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};
		virtual void onTimer(TimerId id);
		virtual void publish(const string& str);
		int _fd;
		int _port;
		string _ip;

};


#endif //GRECORDPRODUCER_HPP
