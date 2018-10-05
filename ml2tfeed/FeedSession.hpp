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
#include "DataPage.hpp"

using namespace std;

//interface

class FeedSession
{
	public:
		FeedSession(){};
		virtual ~FeedSession(){};

		virtual bool initSession() = 0;

		virtual void publishPageImage(DataPage *page, DataPage::PageSlot& slot, const string& sec_id) = 0;
		virtual void publishPageUpdate(DataPage::Update& up, const string& sec_id) = 0;

};
#endif //FEEDSESSION_HPP
