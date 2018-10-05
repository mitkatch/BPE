
/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:29:16 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxApp.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxApp.cpp,v 1.7 2006/06/08 15:29:16 nickz Exp $
|| REVISION:    $Revision: 1.7 $
|| LOG:                 $Log: CanpxApp.cpp,v $
|| LOG:                 Revision 1.7  2006/06/08 15:29:16  nickz
|| LOG:                 ported to managed publishing
|| LOG:
|| LOG:                 Revision 1.6  2006/06/05 13:18:35  nickz
|| LOG:                 Ported record publisher as managed
|| LOG:
|| LOG:                 Revision 1.5  2006/04/10 17:50:03  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.4  2006/04/01 12:11:51  nickz
|| LOG:                 Changed to make RMDS aware.
|| LOG:
|| LOG:                 Revision 1.3  2005/09/21 17:44:59  mikhailt
|| LOG:                 configurable built chain
|| LOG:
|| LOG:                 Revision 1.2  2005/09/12 21:56:39  mikhailt
|| LOG:                 builds ldap chain
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

// System includes
#include <signal.h>

// Application includes
#include "CanpxApp.hpp"
#include "DataCache.hpp"
#include "MCMsg.hpp"
#include "ControlSession.hpp"
#include "Formatter.hpp"
#include "StorageManager.hpp"


CanpxApp *theCanpxApp = new CanpxApp("CanpxApp");

static void
sighandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxApp::Signal %d caught.  Shutting down application...", signo);
	delete theCanpxApp;
	exit(1);
}


static void
shutdownHandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxApp::shutdownHandler %d ", signo);
	theCanpxApp->shutdown();
}



CanpxApp::CanpxApp(char* name):Application(name)
{
    _myServer	= NULL;
    _rmdsProducer = NULL;
    _subjSubMgr = NULL;
}

CanpxApp::~CanpxApp()
{
	this->shutdown();
}

void
CanpxApp::shutdown()
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxApp::shutdown myServer");
	StorageManager::turnOff();
	if (_myServer){
		delete _myServer;
		_myServer = NULL;
	}

	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxApp::shutdown rmdsDataProducer");
	if (_rmdsProducer)
	{
	    delete _rmdsProducer;
	    _rmdsProducer = NULL;
	}
	

	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxApp::shutdown rmdsHandler");
	if (_rmdsHandler)
	{
	    delete _rmdsHandler;
	    _rmdsHandler = NULL;
	}

	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxApp::shutdown CanpxSubjectSubMgr");
	if (_subjSubMgr)
	{
	    delete _subjSubMgr;
	    _subjSubMgr = NULL;
	}

	exit(1);
}

void
CanpxApp::onInit(const ConfigMap& configMap)
{
    _producer = configMap.getValue("PRODUCER");

    _myServer = new CanpxOpenServer(configMap);
    if (_myServer->initServer() == false){
	Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxApp::onInit failed _myServer->initServer");
	exit(4);
    }
    
    string persistent = configMap.getValue("PERSISTENT_CACHE");
    if (persistent == "TRUE")
	DataCache::getCache()->setPersistent(true);
    
    string storage = configMap.getValue("STORAGE_LOCATION");
    if (!storage.empty())
	StorageManager::setActive(storage.c_str());

    // Define, initializae and launch the RMDS record handler thread
    _rmdsHandler = new rmdsRecordHandler(configMap);
    setDataFeed(_rmdsHandler);

    if (_rmdsHandler->init() == -1 )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "CanpxApp::onInit rmdsHandler init failed. ");
	exit(3);
    }

    string wcard = configMap.getValue("CANPX_ENGINE.SUBSCRIBER");

    _rmdsProducer = new canpxRecordProducer(this, _producer);
    if (!wcard.empty())
    {
	DataCache::getCache()->setSubscriberByWCard(_rmdsProducer, wcard);
    }

    if (_rmdsProducer->init() == -1 )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "CanpxApp::onInit rmdsProducer init failed. ");
	exit(4);
    }

    // Subject Subscription Manager
    _subjSubMgr = new CanpxSubjectSubMgr(_producer, this);

    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGALRM, shutdownHandler); //for debug
}

void
CanpxApp::subscribeRecords()
{
    // Initialize and launch CanpxSubjectSubManager thread - request for subjects
    if (_subjSubMgr->init() == -1 )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Canpxapp::onInit CanpxSubjectSubMgr init failed. ");
	exit(3);
    }
}

void
CanpxApp::broadcastControlMsg(const char* type)
{
	MCMsg msg;
	msg.buildPrefix("CHAIN");
	msg.buildSuffix(_producer.c_str());
	msg.addKeyValue("TYPE", type);
	ControlSession *cs = NULL;
	if (cs = (ControlSession *)this->getControlSession()){
		cs->broadcast(msg);
	}	

}
