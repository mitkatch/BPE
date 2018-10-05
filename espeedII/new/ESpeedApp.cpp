
/************************************************************************
||
|| DATE:                $Date: 2006/06/07 14:29:11 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/ESpeedApp.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESpeedApp.cpp,v 1.6 2006/06/07 14:29:11 nickz Exp $
|| REVISION:    $Revision: 1.6 $
|| LOG:                 $Log: ESpeedApp.cpp,v $
|| LOG:                 Revision 1.6  2006/06/07 14:29:11  nickz
|| LOG:                 Ported to managed published
|| LOG:
|| LOG:                 Revision 1.5  2006/06/04 11:28:01  nickz
|| LOG:                 Make into Managed Publisher
|| LOG:
|| LOG:                 Revision 1.4  2006/04/19 01:13:47  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.3  2006/04/11 16:03:55  nickz
|| LOG:                 Ported to RMDS (not tested)
|| LOG:
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
			_rmdsDataProducer(NULL), _cfetiSession(NULL)	
{

}

ESpeedApp::~ESpeedApp()
{
    StorageManager::turnOff();
    if (_myServer)
	delete _myServer;
    
    if (_rmdsDataProducer)
	delete _rmdsDataProducer;
    
    if (_cfetiSession)
    {
	_cfetiSession->shutdown();
	delete _cfetiSession;
    }

    if (_subjPubManager)
	delete _subjPubManager;
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

    if (_myServer->initServer() == false)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "ESpeedFeed::onInit failed _myServer->initServer");
	exit(4);
    }

    string storage = configMap.getValue("STORAGE_LOCATION");
    if (!storage.empty())
	StorageManager::setActive(storage.c_str());
    
    _rmdsDataProducer = new esRecordProducer(this, _name);

    // Initialize and launch rmdsRecordProducer
    if(_rmdsDataProducer->init() == -1)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, "ESpeedFeed::loadDataProducer failed.");
	exit(5);
    }

    _subjPubManager = new subjectPubMgr(_name);

    // Initialize and launch the subjectManager thread
    if (_subjPubManager->init() == -1 )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, "ESpeedFeed::loadDataProducer failed.");
	exit(5);
    }

    //_tV.push_back(CreateThread(_cfetiSession));
}

void
ESpeedApp::loadDataProducer()
{
    Logger::getLogger()->log(Logger::TYPE_INFO, "ESpeedFeed::loadDataProducer Fetching Cantor data...");

    string ifeedSubscr = ConfigMap::getConfig()->getValue("ES_FEED.SUBSCRIBER");
    if (!ifeedSubscr.empty())
    {
	set<string> aset = DataCache::getCache()->getAddressSet();
	set<string>::iterator it = aset.begin();

	for (it; it != aset.end(); it++)
	{
	    if ((*it).find(ifeedSubscr) == 0)
	    {
		Datum *d = DataCache::getCache()->getDatum((*it));
		DataGraph *gr = dynamic_cast<DataGraph *>(d);
		if (gr)
		{
		    gr->registerSubscriber(_rmdsDataProducer, true);
		    gr->registerSubscriber(_subjPubManager, true);
		}
	    }
	}
    }
}

void
ESpeedApp::subscribeRecords()
{
    if (_cfetiSession == NULL)
    {
	string ldapurl = ConfigMap::getConfig()->getValue("LDAP_URL");
	string searchalias = ConfigMap::getConfig()->getValue("LDAP_SEARCH_ALIAS");
	if (ldapurl.size() && searchalias.size())
	{
	    AliasConverter::getConverter()->load(ldapurl, searchalias);
	}
	
	_cfetiSession = new ESCFETISession(*ConfigMap::getConfig());
	CreateThread(_cfetiSession);
    }

    addTimer(1*1000, 1, this);	
}

void
ESpeedApp::onTimer(TimerId id)
{
    if (_cfetiSession->activated())
	loadDataProducer();
    else
	addTimer(1*1000, 1, this);
}

void
ESpeedApp::sessionExpired(ESCFETISession *session)
{
	if (session == _cfetiSession)
		_cfetiSession = NULL;
}

