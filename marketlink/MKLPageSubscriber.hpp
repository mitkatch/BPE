
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

#ifndef MKLPAGESUBSCRIBER_HPP
#define MKLPAGESUBSCRIBER_HPP
#include "NSMap.hpp"
#include "Subscriber.hpp"
#include "PageListener.hpp"
#include "Datum.hpp"
#include "DataPage.hpp"
#include "DataGraph.hpp"
#include "TimerCallback.hpp"
#include "FeedSession.hpp"
#include "MKLThrottled.hpp"



#define ROW80_1    "315"  
#define ROW80_2    "316"  
#define ROW80_3    "317"  
#define ROW80_4    "318"  
#define ROW80_5    "319"  
#define ROW80_6    "320"  
#define ROW80_7    "321" 
#define ROW80_8    "322"  
#define ROW80_9    "323"  
#define ROW80_10   "324"  
#define ROW80_11   "325"  
#define ROW80_12   "326"  
#define ROW80_13   "327"  
#define ROW80_14   "328"  
#define ROW80_15   "329"  
#define ROW80_16   "330"  
#define ROW80_17   "331"  
#define ROW80_18   "332"  
#define ROW80_19   "333"  
#define ROW80_20   "334"  
#define ROW80_21   "335"  
#define ROW80_22   "336"  
#define ROW80_23   "337"  
#define ROW80_24   "338"  
#define ROW80_25   "339"  


typedef map<string, DataPage::PageRow> 	MKLPage;
typedef vector<DataGraph::DataMap> 	MKLUpdates;

class MKLPageSubscriber: 	public Subscriber, 
				public PageListener, 
				//public TimerCallback,
				public MKLThrottled
{
	public:


	public:
		MKLPageSubscriber(FeedSession* s, const URLNameSpace& ns);
		virtual ~MKLPageSubscriber();

		virtual void setMySlot(const string& slot, const string& id);
		virtual vector<DataPage::PageSlot> getMySlot()const;
		virtual void notify(Datum *datum);
		virtual void onData(DataPage *page, DataPage::PageSlot slot);
		virtual void publishRefresh();		
		virtual void addWaitTime(int sec);
		virtual void addRefresh();
		//virtual void onTimer(TimerId id);
		class PageTimer;
		friend class PageTimer;
		class PageTimer: public TimerCallback
		{
			public:
				PageTimer(MKLPageSubscriber *sub, int credit,
						int rate, int ceiling);
				virtual ~PageTimer(){};
				virtual void initTimer();
				virtual void onTimer(TimerId id);
			protected:
				MKLPageSubscriber * _ps;
				int _t_rate;
				int _t_credit;
				int _t_ceiling;
		};
	protected:
		virtual void setDefaultRowMap();
		virtual bool setRowTmpl();
		virtual bool clearMKLImage(const string& rowID);
		virtual void buildPageImage();
		virtual void buildImageUpdate(DataPage::Update &up);

		virtual void compareRows(const DataPage::PageRow& mklrow, const DataPage::PageRow& prow, MKLUpdates* ups, const string& rowID);

		virtual void publishPageImage(DataPage *page, DataPage::PageSlot& slot);
		virtual void publishPageUpdate(DataPage::Update& up);
		virtual DataGraph::DataMap pageRowAsDataMap(const string& rowID, const DataPage::PageRow& pr);
		virtual DataGraph::DataMap rowAsDataMap(const string& rowID, const DataPage::PageRow& pagerow);

		virtual MKLUpdates buildUpdates(const string& rowID, const DataPage::PageRow& prow);


		vector<MKLPageSubscriber::PageTimer *> _timers;
		vector<MKLPageSubscriber::PageTimer *> _timersDone;
		vector<DataPage::PageSlot> _my_slot;
		map<DataPage::PageSlot*, string> _mySlotMap;
		string _address;
		string _security_id;
		string _row_tmpl_str;
		int _throttle_delay;
		int _watermark;
		FeedSession* _mySession;
		bool _activated;
		map<int, string> _rowMap;
		DataPage *_myPage;
		vector<string> _row_tmpl;
		MKLPage *_pageImage;// buffer page
		MKLPage *_mklImage; //page sent down;
		bool _locked;
		bool _isRefreshRequired; //indicator of missed refresh
};


#endif //MKLPAGESUBSCRIBER_HPP
