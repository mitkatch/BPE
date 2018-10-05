
/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:29:48 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PBuilderApp.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PBuilderApp.cpp,v 1.7 2006/06/08 15:29:48 nickz Exp $
|| REVISION:    $Revision: 1.7 $
|| LOG:                 $Log: PBuilderApp.cpp,v $
|| LOG:                 Revision 1.7  2006/06/08 15:29:48  nickz
|| LOG:                 ported to managed publishing
|| LOG:
|| LOG:                 Revision 1.6  2006/05/02 15:12:09  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.5  2006/04/06 17:57:29  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:                 Revision 1.4  2006/04/04 13:19:32  nickz
|| LOG:                 ported to rmds
|| LOG:
|| LOG:                 Revision 1.3  2005/09/21 17:49:55  mikhailt
|| LOG:                 activated only when loaded()
|| LOG:
|| LOG:                 Revision 1.2  2005/07/29 19:22:02  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <signal.h>
#include "PBuilderApp.hpp"
#include "DataCache.hpp"
#include "StorageManager.hpp"

PBuilder *thePBuilder = new PBuilder("PBuilder");

static void
sighandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "PBuilder::Good bye cruel world! %d kiiled me. Exit...", signo);
        delete thePBuilder;
        exit(1);
}

static void
ignoresignal(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "PBuilder:: IgnoreSygnal %d ", signo);
}


PBuilder::PBuilder(char *name):Application(name), 
				_pArrayConverter(NULL),
				_rmdsPageProducer(NULL)
{

}

PBuilder::~PBuilder()
{
	StorageManager::turnOff();
	if (_pArrayConverter)
		delete _pArrayConverter;
	if (_rmdsPageProducer)
		delete _rmdsPageProducer;

	
}

void
PBuilder::onInit(const ConfigMap& configMap)
{
    _rmdsRecordHandler =  new PBrmdsRecordHandler(configMap);
    if (_rmdsRecordHandler ->init() == -1)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "PBuilder::onInit _rmdsRecordHandler init failed. ");
	exit(3);
    }
    else
    {
	setDataFeed(_rmdsRecordHandler);
    }
    // create record handler 
    // pageProducer
    // new converter
    
    _rmdsPageProducer = new pbPageProducer(this, _name);
    if(_rmdsPageProducer->init() == -1)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "PBuilder::onInit _rmdsPageProducer init failed. ");
	exit(5);
    }

    PageConverter::setPageProducer(_rmdsPageProducer);
    
    _pArrayConverter = new PageArrayConverter(configMap.getValue("LDAP_URL")); 

    string storage = configMap.getValue("STORAGE_LOCATION");
    if (!storage.empty())
	StorageManager::setActive(storage.c_str());
       
    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGHUP, ignoresignal);
    signal(SIGALRM, sighandler); //for debug
}

void
PBuilder::subscribeRecords()
{
    ConfigMap* confMap = ConfigMap::getConfig();

   _pArrayConverter->load(configMap->getValue("LDAP_SEARCH_PAGE"));
}


