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

#ifndef MKLFEEDAPP_HPP 
#define MKLFEEDAPP_HPP 

#include <vector>


//#include "MKLrmdsRecordHandler.hpp"
#include "rmdsHandler.hpp"
#include "Subscriber.hpp"
#include "Application.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "Datum.hpp"
#include "MsgGraph.hpp"
#include "FeedSession.hpp"
#include "NSMap.hpp"
#include "MKLRecordSubscriber.hpp"
#include "MKLPageSubscriber.hpp"
#include "MKLOpenServer.hpp"
#include "MKLSubjectSubMgr.hpp"

#define PRODUCER_ID	"PRODUCER.ID." 
#define CANPX_TIME_FORMAT	"%d/%m/%y %H:%M:%S"
#define CPX_REC_TIMESTAMP       "ROW64_12"


class MKLFeedApp: public Application, public Subscriber
{
	public:
		static enum {	NO_TYPE, 
				RECORD_TYPE, 
				PAGE_TYPE};

		typedef struct AddressPage{
			AddressPage():id(-1), 
				last_updt(NULL), 
				inst_type(NO_TYPE),
				subscriber(NULL){};
			int id;
			string inst;
			string mlip_inst;
			char *last_updt;
			int inst_type;
			void *subscriber;
			bool operator<(const AddressPage& a) const{
				return id < a.id;
			}

		}AddressPage;

		typedef set<AddressPage *> AddressChapter;
		typedef map<string, AddressChapter > AddressBook;

		// {"canpx2goc" : [<1, "CANPX_GOC.575_0633">, <2, "....">]}

		MKLFeedApp(const char *);
		virtual ~MKLFeedApp();
		virtual void onInit(const ConfigMap& configMap);
		virtual const char *const className(){return "MKLFeedApp";}

		virtual rmdsHandler* getrmdsHandler() const {
			return _rmdsHandler;
		}
		static AddressBook& getAddressBook(){
			return *_addressBook;
		}
		virtual MKLSubjectSubMgr * getSubMgr(){
			return _subjSubMgr;
		}

		virtual void subscribeSources();


		virtual void notify(Datum *datum);

		virtual void shutdown();
		virtual void setTimestamp(const string& address, const string& timestmp);
	protected:
		virtual void setSubscriber(const string& address, int type, void *s);
		virtual void subscribeRecords();
		virtual void subscribePages();
		virtual void notifyRec(DataGraph *rec);
		virtual void notifyPage(DataPage *page);
		virtual void buildRecordNameSpace(const ConfigMap& confMap);
		virtual void  buildPageNameSpace(const ConfigMap&) ;
		virtual void populateAddressBook(const map<string, string>& my_producers);
		virtual void collectInstruments(AddressChapter& v_inst, DataGraph *rec);
		virtual void collectPages();
		virtual void onRead(int fd, InputId id){};
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};
		virtual void onTimer(TimerId id){};

	private:
		FeedSession * _mySession;
		OpenServer * _myOpenServer;
		const ConfigMap *_configMap;
		bool            _init;
		rmdsHandler * _rmdsHandler;
		vector<MKLRecordSubscriber*> _vRecSubscribers;
		vector<MKLPageSubscriber*> _vPageSubscribers;
		static AddressBook* _addressBook;
		int _sessionType;
		NSMap _page_ns;
		MKLSubjectSubMgr* _subjSubMgr;
};

extern MKLFeedApp *theMKLFeedApp;

#endif // MKLFEEDAPP_HPP
