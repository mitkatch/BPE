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

#include <signal.h>
#include "rmdsRecordHandler.hpp"
#include "rmdsPageHandler.hpp"
#include "Scheduler.hpp"
#include "DBConnectorLdap.hpp"
#include "DataCache.hpp"
#include "MKLFeedApp.hpp"
#include "MKLRecordSubscriber.hpp"
#include "MKLFeedSession.hpp"


MKLFeedApp *theMKLFeedApp = new MKLFeedApp("MKLFeedApp");
MKLFeedApp::AddressBook* MKLFeedApp::_addressBook = new AddressBook();

static void
sighandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedApp::Good bye cruel world! %d kiiled me. Exit...", signo);
	delete theMKLFeedApp;
	exit(1);
}

static void
ignoresignal(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedApp:: IgnoreSygnal %d ", signo);
}


static void
shutdownHandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedApp::shutdownHandler %d ", signo);
	theMKLFeedApp->shutdown();
	exit(1);
}


MKLFeedApp::MKLFeedApp(const char* name):Application(name),
			_myOpenServer(NULL),
			_mySession(NULL),
			_rmdsHandler(NULL),
			_subjSubMgr(NULL)
{
}

MKLFeedApp::~MKLFeedApp()
{
	this->shutdown();
}

void
MKLFeedApp::shutdown()
{
	Logger::getLogger()->log(Logger::TYPE_INFO,"About to delete mySession");
	if (_mySession){
		delete _mySession;
		_mySession = NULL;
	}
	Logger::getLogger()->log(Logger::TYPE_INFO,"About to delete rmdsPageHandler");
	if (_rmdsHandler){
		delete _rmdsHandler;
		_rmdsHandler = NULL;
	}
	int i = 0;
	for(i; i < _vRecSubscribers.size(); i++){
		delete _vRecSubscribers[i];	
	}
	Logger::getLogger()->log(Logger::TYPE_INFO, "Deleted %i RecSubscribers", i);
	
	int j = 0;
	for(j; j < _vPageSubscribers.size(); j++){
		delete _vPageSubscribers[j];	
	}
	Logger::getLogger()->log(Logger::TYPE_INFO, "Deleted %i PageSubscribers", j);
	
	AddressBook::iterator it = _addressBook->begin();
	for (it; it != _addressBook->end(); it++){
		AddressChapter::iterator itc = ((*it).second).begin();
		for (itc; itc != ((*it).second).end(); itc++){
			if ((*itc)->last_updt != NULL)
				delete [] (*itc)->last_updt;
			delete (*itc);
		}
	}
	
	if (_subjSubMgr){
		delete _subjSubMgr;
		_subjSubMgr = NULL;
	}
}

void
MKLFeedApp::onInit(const ConfigMap& configMap)
{
	string sessionType = configMap.getValue("SESSION_TYPE");
	if (sessionType == "RECORD_TYPE"){

		_rmdsHandler = new rmdsRecordHandler(configMap);
		_sessionType = MKLFeedApp::RECORD_TYPE;
		_subjSubMgr = new MKLSubjectSubMgr(_name, this);

	} else if (sessionType == "PAGE_TYPE"){

		_rmdsHandler = new rmdsPageHandler(configMap);
		_sessionType = MKLFeedApp::PAGE_TYPE;
		buildPageNameSpace(configMap);

	} else {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedApp::onInit  init failed. SESSION_TYPE=RECORD_TYPE | PAGE_TYPE , found [%s] ", sessionType.c_str());
			exit(2);

	}
	if (_rmdsHandler){
		if (_rmdsHandler->init() == -1 ){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedApp::onInit rmdsHandler init failed. ");
			exit(3);
		}
		//buildRecordNameSpace(configMap);
		setDataFeed(_rmdsHandler);
		_myOpenServer = new MKLOpenServer(configMap);
		if (_myOpenServer->initServer() == false){
                	Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedApp::onInit failed _myOpenServer->initServer");
                	exit(4);

		}

		_mySession = new MKLFeedSession(this, _name);
        	if (_mySession->initSession() == false){
                	Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedApp::onInit failed _mySession->initSession");
                	exit(5);
        	}



		//subscribeSources(); called from MKLFeedSession 

	}


	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGHUP, ignoresignal);
	signal(SIGALRM, shutdownHandler);


}

void
MKLFeedApp::buildRecordNameSpace(const ConfigMap& confMap)
{
/****************
	string prod_id = PRODUCER_ID;
	map<string, string> my_producers;
	map<string, string>::const_iterator it = confMap.begin();
	for (it ; it != confMap.end(); it++){
		int prodind = (*it).first.find(prod_id);
		if (prodind == 0){
			int i = (*it).first.length();
			string new_ns = (*it).first.substr(prod_id.length(), i - prod_id.length());
			my_producers[new_ns] = (*it).second;
			cout << "NameSpace ["<<new_ns<<"]="<<(*it).second<<endl;
		}
	}


	populateAddressBook(my_producers);
**************/
}

void
MKLFeedApp::populateAddressBook(const map<string, string>& my_producers)
{
/***************
	string host, port, connstr;
	string producer = "/producer=";
	string hosturl = ConfigMap::getConfig()->getValue("LDAP_CHAIN_URL");
	DBConnectorLdap::getConnPar(hosturl, host, port, connstr);
	string s = ConfigMap::getConfig()->getValue("LDAP_SEARCH_CHAIN_ITEM");

	map<string, string>::const_iterator its = my_producers.begin();
	for (its; its != my_producers.end(); its++){
		AddressChapter v_inst;
		DBConnectorLdap *conn = new DBConnectorLdap(host, port);
		//dynamicly build "producer=canpx2goc, ou=Chain, o=moneyline.ca"
		string my_connstr = connstr;
		my_connstr.replace(0, 1, producer + (*its).second + string(", "));
		if (conn->connect(my_connstr)){
			if (conn->fetch(s)){
				MsgGraph * rset = conn->search();
				if (rset){
					collectInstruments(v_inst, rset);
				}
			}
		}
		_addressBook->insert(AddressBook::value_type((*its).second, v_inst));
		cout << "Add "<< v_inst.size() <<" to AddressBook "<<(*its).second<<endl;
		conn->disconnect();
		delete conn;
	}
******************/
}

void
MKLFeedApp::collectInstruments(AddressChapter& v_inst, DataGraph *rec)
{
	static int num_i = 1;
	AddressPage *p = new AddressPage();
	p->id = num_i++;
	p->inst = rec->getValue("DSPLY_NAME");
	p->mlip_inst = rec->address();
	p->last_updt = new char[24];
	memset(p->last_updt, 0x0, 25 );
	strcpy(p->last_updt, "");
	v_inst.insert(p);

	cout << "Collected Instruments "<<rec->address()<<endl;
}

void
MKLFeedApp::subscribeSources()
{
		switch (_sessionType){
			case MKLFeedApp::RECORD_TYPE:
				subscribeRecords();
				break;
			case MKLFeedApp::PAGE_TYPE:
				subscribePages();
				break;
			default:
                		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedApp::subscribeSources failed unknown _sessionType");
		}	

}


void
MKLFeedApp::subscribeRecords()
{
	if (_subjSubMgr->init() == -1 ){
		Logger::getLogger()->log(Logger::TYPE_ERROR,
			"MKLFeedApp::subscribeRecords() init failed. Exit");
		exit(7);
	}
/**************
	AddressBook::iterator it = _addressBook->begin();
	for (it; it != _addressBook->end(); it++){
		AddressChapter::iterator itc = ((*it).second).begin();
		for (itc; itc != ((*it).second).end(); itc++){
			string address = (*itc)->inst;
			DataCache::getCache()->subscribe(address, this, Datum::GRAPH);
			Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedApp::subscribeRecords subscribed for %s. ", address.c_str());
		}
	}
*************/	
}

void
MKLFeedApp::notify(Datum *datum)
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedApp notified from %s", (datum->address()).c_str());
	DataGraph *rec = dynamic_cast<DataGraph *>(datum);
	DataPage *page = dynamic_cast<DataPage *> (datum);
	if (rec){
		notifyRec(rec);	
	} else if (page){
		notifyPage(page);
	}else {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLFeedApp notified failed from %s. not a REC, not a PAGE", (datum->address()).c_str());
	}
}

void
MKLFeedApp::notifyRec(DataGraph *rec)
{
	if (rec){	
		string address = rec->address();
		string last = rec->getValue(CPX_REC_TIMESTAMP);
		setTimestamp(address, last);
		MKLRecordSubscriber *subscriber = new MKLRecordSubscriber(_mySession, address);
		rec->registerSubscriber(subscriber);	
				
		DataCache::getCache()->subscribe(address, subscriber, Datum::GRAPH);
		_vRecSubscribers.push_back(subscriber);

		DataCache::getCache()->unsubscribe(rec->address(), this);//to be done only once to activate subscribers 
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedApp unsubscribe from %s", (rec->address()).c_str());
		AddressChapter v_inst;
		collectInstruments(v_inst, rec);
		_addressBook->insert(AddressBook::value_type(_subjSubMgr->contributor(rec->address()), v_inst));
		setSubscriber(address, RECORD_TYPE, (void *)subscriber);

	}
	
}

void
MKLFeedApp::notifyPage(DataPage *page)
{
	if (page){	
		NSMap::iterator it = _page_ns.begin();
		for (it; it != _page_ns.end(); it++){
			string address = (*it).second.source + string(1, '.') +  (*it).second.page;
			if (address == page->address()){
				string last = Scheduler::getCurrentTime(CANPX_TIME_FORMAT);
				setTimestamp(address, last);
				MKLPageSubscriber *listener = new MKLPageSubscriber(_mySession, (*it).second);

				vector<DataPage::PageSlot> slots = listener->getMySlot();
				vector<DataPage::PageSlot>::iterator its = slots.begin();
				for (its; its != slots.end(); its++){

					page->addListener(listener, (*its));	
				}


				
				DataCache::getCache()->subscribe(address, listener, Datum::PAGE);
				setSubscriber(address, PAGE_TYPE, (void *)listener);
				_vPageSubscribers.push_back(listener);
				DataCache::getCache()->unsubscribe(page->address(), this);//to be done only once to activate subscribers 
				Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedApp unsubscribe from %s", (page->address()).c_str());
			}
		}	
	}
	
}


void
MKLFeedApp::setTimestamp(const string& a, const string& tmstmp)
{

	AddressBook::iterator it = _addressBook->begin();
	for (it; it != _addressBook->end(); it++){
		AddressChapter::iterator itc = ((*it).second).begin();
		for (itc; itc != ((*it).second).end(); itc++){
			string address = (*itc)->inst;
			if (a == address){
				char *last = (*itc)->last_updt;
				if (last != NULL);
					strncpy(last ,tmstmp.c_str(), 23);
				return;
			}
		}
	}


}

void
MKLFeedApp::setSubscriber(const string& a, int type, void *s)
{

	AddressBook::iterator it = _addressBook->begin();
	for (it; it != _addressBook->end(); it++){
		AddressChapter::iterator itc = ((*it).second).begin();
		for (itc; itc != ((*it).second).end(); itc++){
			string address = (*itc)->inst;
			AddressPage *p = (*itc);
			if (a == address){
				p->inst_type = type;
				p->subscriber = s;
				return;
			}
		}
	}


}

//////////////////////////////////////////////////////////////

void
MKLFeedApp::buildPageNameSpace(const ConfigMap& confMap)
{
	string url_id = URL;
	map<string, string>::const_iterator it = confMap.begin();
	for (it ; it != confMap.end(); it++){
		int urlind = (*it).first.find(url_id);
		if (urlind == 0){
			int name_id = (*it).first.find(URL_SECURITY_ID);
			if (name_id != string::npos){
				int i = (*it).first.length();
				string new_ns = (*it).first.substr(url_id.length(), i - strlen(URL_SECURITY_ID) - url_id.length());
				//cout << "NameSpace is "<<new_ns<<endl;
				_page_ns[new_ns] = URLNameSpace();
			}
		}
	}
	NSMap::iterator nsit = _page_ns.begin();
	for (nsit; nsit != _page_ns.end(); nsit++){
		URLNameSpace &ns = (*nsit).second;
		string getpage = string(URL)+(*nsit).first + string(URL_PAGE);
		string getsource = string(URL)+(*nsit).first + string(URL_SOURCE);
		string getslot = string(URL) + (*nsit).first + string(URL_SLOT);
		string getsecurity_id = string(URL) + (*nsit).first + string(URL_SECURITY_ID);
		string getrowtmpl = string(URL) + (*nsit).first + string(URL_ROW_TMPL);
		string delay =  string(URL) + (*nsit).first + string(URL_THROTTLE_DELAY);
		ns.source = confMap.getValue(getsource.c_str());
		ns.page = confMap.getValue(getpage.c_str());
		ns.slot = confMap.getValue(getslot.c_str());
		ns.security_id = confMap.getValue(getsecurity_id.c_str());
		ns.row_tmpl = confMap.getValue(getrowtmpl.c_str());
		ns.throttle_delay = confMap.getValue(delay.c_str());

		//Formatter::split(confMap.getValue(getparam.c_str()), ",", ns.parameters);
	}

	collectPages();
}

void
MKLFeedApp::collectPages()
{
	set<string> sources;
	NSMap::iterator nsit = _page_ns.begin();
	for (nsit; nsit != _page_ns.end(); nsit++){
		URLNameSpace &ns = (*nsit).second;
		sources.insert(ns.source);
	}

	set<string>::iterator its = sources.begin();
	for (its; its != sources.end(); its++){
		AddressChapter v_inst;
		int counter = 1;
		NSMap::iterator nsit = _page_ns.begin();
		for (nsit; nsit != _page_ns.end(); nsit++){
			URLNameSpace &ns = (*nsit).second;
			if ((*its) == ns.source){
				AddressPage *p = new AddressPage();
				p->id = counter++;
				p->inst = ns.source + "." + ns.page;
				p->mlip_inst = ns.security_id;
				p->last_updt = new char[24];
				memset(p->last_updt, 0x0, 25 );
				strcpy(p->last_updt, "");
				v_inst.insert(p);
			}	
		}
		_addressBook->insert(AddressBook::value_type((*its), v_inst));
	}
}

void
MKLFeedApp::subscribePages()
{
	NSMap::iterator it = _page_ns.begin();
	for (it; it != _page_ns.end(); it++){
		string address = (*it).second.source + string(1, '.') +  (*it).second.page;
		DataCache::getCache()->subscribe(address, this, Datum::PAGE);
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLFeedApp::subscribePages subscribed for %s. ", address.c_str());

		//registerListeners after whole page recieved 
	}
}

