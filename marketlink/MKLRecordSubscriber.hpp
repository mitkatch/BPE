
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

#ifndef MKLRECORDSUBSCRIBER_HPP
#define MKLRECORDSUBSCRIBER_HPP
#include "Subscriber.hpp"
#include "Datum.hpp"
#include "DataGraph.hpp"
#include "FeedSession.hpp"
#include "MKLThrottled.hpp"

class MKLRecordSubscriber: public Subscriber, public MKLThrottled
{
	public:
		MKLRecordSubscriber(FeedSession* s, const string& address);
		virtual ~MKLRecordSubscriber();

		virtual void notify(Datum *datum);
		virtual void onData(DataGraph *rec);
		
		virtual void addWaitTime(int sec){};
		virtual void addRefresh(){};
	protected:
		void buildDifData(DataGraph::DataMap& data);
		void setFields();
		string _address;
		string _idn_address;
		string _security_id;
		string _tmstmpField;
		FeedSession* _mySession;
		bool _activated;
		DataGraph::DataMap _diffData;
		DataGraph::DataMap _cacheData;
};


#endif //MKLRECORDSUBSCRIBER_HPP
