
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
#include "RTApp.hpp"
#include "DataCache.hpp"
#include "StorageManager.hpp"

RTApp *theRTApp = new RTApp("RTApp");

static void
sighandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "RTApp::Good bye cruel world! %d kiiled me. Exit...", signo);
        delete theRTApp;
        exit(1);
}

static void
ignoresignal(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "RTApp:: IgnoreSygnal %d ", signo);
}


RTApp::RTApp(char *name):Application(name), 
				_pArrayConverter(NULL) ,
				_rmdsRecordProducer(NULL),
				_subjPubManager(NULL)
{

}

RTApp::~RTApp()
{
	StorageManager::turnOff();
	if (_pArrayConverter)
		delete _pArrayConverter;
	if (_rmdsRecordProducer)
		delete _rmdsRecordProducer;

	if (_subjPubManager)
		delete _subjPubManager;
	
}

RTRecordProducer* 
RTApp::getProducer()
{
	return _rmdsRecordProducer;

}

void
RTApp::onInit(const ConfigMap& configMap)
{
	_rmdsRecordHandler =  new RTrmdsRecordHandler(configMap);
	if (_rmdsRecordHandler ->init() == -1) {
		Logger::getLogger()->log(Logger::TYPE_ERROR, 
			"RTApp::onInit _rmdsRecordHandler init failed. ");
		exit(3);
	} else {
		setDataFeed(_rmdsRecordHandler);
	}
	// create record handler 
	// recordProducer
	// new converter
	_rmdsRecordProducer = new RTRecordProducer(this, _name);
	_subjPubManager = new subjectPubMgr(_name);
	if(_rmdsRecordProducer->init() == -1) {
		Logger::getLogger()->log(Logger::TYPE_ERROR, 
			"RTApp::onInit _rmdsRecordProducer init failed. ");
		exit(5);
	}
	if (_subjPubManager->init() == -1 ){
		Logger::getLogger()->log(Logger::TYPE_ERROR, 
			"RTApp::onInit _subjPubManager init failed. ");
		exit(6);
	}

	//RecordConverter::setRecordProducer(_rmdsRecordProducer);
	_pArrayConverter = new RecordArrayConverter(configMap.getValue("LDAP_URL")); 
	
	string storage = configMap.getValue("STORAGE_LOCATION");
	if (!storage.empty())
		StorageManager::setActive(storage.c_str());
       
	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGHUP, ignoresignal);
	//signal(SIGALRM, sighandler); //for debug

}

void
RTApp::subscribeRecords()
{
	ConfigMap* confMap = ConfigMap::getConfig();
	_pArrayConverter->load(confMap->getValue("LDAP_SEARCH_TRANSLATOR"));
	/*****
	string source = confMap->getValue("IDN_RIC_SUBSCRIBER");
	if (!source.empty()){
		DataCache::getCache()->setSubscriberByWCard(_subjPubManager, source);
	}else{
		Logger::getLogger()->log(Logger::TYPE_ERROR, 
			"RTApp::subscribeRecords _subjPubManager failed. No IDN_RIC_SUBSCRIBER found ");
	}
	****/
}


