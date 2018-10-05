/************************************************************************
||
|| DATE:                $Date:$
|| SOURCE:              $Source:$
|| STATE:               $State:$
|| ID:                  $Id:$
|| REVISION:		$Revision:$
|| LOG:                 $Log:$
|| LOG:
************************************************************************/

#ifndef RMDSFEEDSESSION_HPP 
#define RMDSFEEDSESSION_HPP 

// System includes
#include <string>

// Application includes
#include "rmdsPageProducer.hpp"
#include "FeedSession.hpp"

class ML2TFeedApp;


class RMDSFeedSession: public rmdsPageProducer, public FeedSession
{
	public:	
    		RMDSFeedSession(ML2TFeedApp* app, const string& name);
    		virtual ~RMDSFeedSession();
		virtual bool initSession();

		virtual void publishPageImage(DataPage* page, DataPage::PageSlot& slot, const std::string &sec_id);
		virtual void publishPageUpdate(DataPage::Update& update, const std::string &sec_id);    
	public:
    		// Virtual requirement (Canpx Subscriber)	
    		virtual void onConnected();
    
	private:
    		ML2TFeedApp*	m_app;
};


#endif //  RMDSFEEDSESSION_HPP







