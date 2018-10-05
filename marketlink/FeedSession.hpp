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

#ifndef	FEEDSESSION_HPP
#define FEEDSESSION_HPP

#include <string>
#include "DataGraph.hpp"

using namespace std;

//interface

class FeedSession
{
	public:
		FeedSession(){};
		virtual ~FeedSession(){};

		virtual bool initSession() = 0;

		virtual void publishRecordImage(DataGraph *rec, const string& sec_id) = 0;
		virtual void publishRecordUpdate(DataGraph *rec, const string& sec_id) = 0;

};
#endif //FEEDSESSION_HPP
