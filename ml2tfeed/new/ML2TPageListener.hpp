
/************************************************************************
||
|| DATE:                $Date: 2006/04/07 19:31:50 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/ML2TPageListener.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ML2TPageListener.hpp,v 1.1 2006/04/07 19:31:50 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ML2TPageListener.hpp,v $
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ML2TPAGELISTENER_HPP
#define ML2TPAGELISTENER_HPP
#include "Subscriber.hpp"
#include "PageListener.hpp"
#include "Datum.hpp"
#include "DataPage.hpp"
#include "NSMap.hpp"
#include "FeedSession.hpp"

class ML2TPageListener: public Subscriber, public PageListener
{
	public:
		ML2TPageListener(FeedSession* s, const URLNameSpace& ns);
		virtual ~ML2TPageListener();

		virtual void notify(Datum *datum);
		virtual void onData(DataPage *page, DataPage::PageSlot slot);

		virtual void setMySlot(const string& slot, const string& id);
		virtual vector<DataPage::PageSlot> getMySlot()const;

	protected:
		vector<DataPage::PageSlot> _my_slot;
		map<DataPage::PageSlot*, string> _mySlotMap;
		string _address;
		string _security_id;
		FeedSession* _mySession;
		bool _activated;
};


#endif //ML2TPAGELISTENER_HPP
