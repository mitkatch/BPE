
/************************************************************************
||
|| DATE:                $Date: 2005/11/21 16:54:05 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/espeedII/RCS/ESpeedApp.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESpeedApp.cpp,v 1.2 2005/11/21 16:54:05 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: ESpeedApp.cpp,v $
|| LOG:                 Revision 1.2  2005/11/21 16:54:05  mikhailt
|| LOG:                 TIME formatted
|| LOG:
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "ESpeedApp.hpp"
#include "CreateThread.hpp"
#include "StorageManager.hpp"
#include "AliasConverter.hpp"
#include "DataCache.hpp"
 
ESpeedApp *theESpeedApp = new ESpeedApp("ESpeedApp");

void emptyhandler(int sig)
{


}

ESpeedApp::ESpeedApp(char *name):Application(name),_myServer(NULL),
			_trsDataProducer(NULL), _cfetiSession(NULL)	
{

}

ESpeedApp::~ESpeedApp()
{
	StorageManager::turnOff();
	if (_myServer)
		delete _myServer;

	if (_trsDataProducer)
		delete _trsDataProducer;

	if (_cfetiSession){
		_cfetiSession->shutdown();
		delete _cfetiSession;
	}
}

void
ESpeedApp::shutdown()
{


}

void
ESpeedApp::onInit(const ConfigMap& configMap)
{
	signal(SIGHUP, emptyhandler); //run in the background
	_myServer = new ESOpenServer(configMap);
	if (_myServer->initServer() == false){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "ESpeedFeed::onInit failed _myServer->initServer");
		exit(4);
	}
	string storage = configMap.getValue("STORAGE_LOCATION");
	if (!storage.empty())
		StorageManager::setActive(storage.c_str());


	addTimer(1* 1000, TimerCallback::FOREVER, this);	
	//_tV.push_back(CreateThread(_cfetiSession));
	
}

void
ESpeedApp::loadDataProducer()
{
	_trsDataProducer = new TrsRecordProducer(_name);
	string ifeedSubscr = ConfigMap::getConfig()->getValue("ES_FEED.SUBSCRIBER");
	if (!ifeedSubscr.empty()){
		set<string> aset = DataCache::getCache()->getAddressSet();
		set<string>::iterator it = aset.begin();
		for (it; it != aset.end(); it++){
			if ((*it).find(ifeedSubscr) == 0){
				Datum *d = DataCache::getCache()->getDatum((*it));
				DataGraph *gr = dynamic_cast<DataGraph *>(d);
				if (gr){
					gr->registerSubscriber(_trsDataProducer, false);
				}
			}
		}
	}

	if(_trsDataProducer->init() == -1){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "ESpeedFeed::loadDataProducer failed");
		exit(5);
	}

}

void
ESpeedApp::onTimer(TimerId id)
{
	//cout << "ESpeedApp::onTimer "<<_cfetiSession<<endl;
	if (_cfetiSession == NULL){
		string ldapurl = ConfigMap::getConfig()->getValue("LDAP_URL");
		string searchalias = ConfigMap::getConfig()->getValue("LDAP_SEARCH_ALIAS");
		if (ldapurl.size() && searchalias.size()){
			AliasConverter::getConverter()->load(ldapurl, searchalias);
		}

		_cfetiSession = new ESCFETISession(*ConfigMap::getConfig());
		CreateThread(_cfetiSession);
	}else{
		if (_cfetiSession->activated() && _trsDataProducer == NULL)
			loadDataProducer();
	}
}

void
ESpeedApp::sessionExpired(ESCFETISession *session)
{
	if (session == _cfetiSession)
		_cfetiSession = NULL;
}

