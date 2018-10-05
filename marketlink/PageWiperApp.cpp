
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

#include "PageWiperApp.hpp"
#include <signal.h>


PageWiperApp *thePageWiperApp = new PageWiperApp("PageWiperApp");

static void
sighandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "PageWiperApp::Good bye cruel world! %d kiiled me. Exit...", signo);
	delete thePageWiperApp;
	exit(1);
}

static void
ignoresignal(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "PageWiperApp:: IgnoreSygnal %d ", signo);
}


static void
shutdownHandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "PageWiperApp::shutdownHandler %d ", signo);
	thePageWiperApp->shutdown();
	exit(1);
}


PageWiperApp::PageWiperApp(const char* name):Application(name)
{
	_trMap["ROW80_2"]=    "316";
	_trMap["ROW80_3"]=    "317";
	_trMap["ROW80_4"]=    "318";
	_trMap["ROW80_5"]=    "319";
	_trMap["ROW80_6"]=    "320";
	_trMap["ROW80_7"]=    "321";
	_trMap["ROW80_8"]=    "322";
	_trMap["ROW80_9"]=    "323";
	_trMap["ROW80_10"]=   "324";
	_trMap["ROW80_11"]=   "325";
	_trMap["ROW80_12"]=   "326";
	_trMap["ROW80_13"]=   "327";
	_trMap["ROW80_14"]=   "328";
	_trMap["ROW80_15"]=   "329";
	_trMap["ROW80_16"]=   "330";
	_trMap["ROW80_17"]=   "331";
	_trMap["ROW80_18"]=   "332";
	_trMap["ROW80_19"]=   "333";
	_trMap["ROW80_20"]=   "334";
	_trMap["ROW80_21"]=   "335";
	_trMap["ROW80_22"]=   "336";
	_trMap["ROW80_23"]=   "337";
	_trMap["ROW80_24"]=   "338";
	_trMap["ROW80_25"]=   "339";
	

}

PageWiperApp::~PageWiperApp()
{
	this->shutdown();
}

void
PageWiperApp::shutdown()
{
	
}

void
PageWiperApp::onInit(const ConfigMap& confMap)
{


	_mySession = new WiperFeedSession(this, _name);
        if (_mySession->initSession() == false){
               	Logger::getLogger()->log(Logger::TYPE_ERROR, "PageWiperApp::onInit failed _mySession->initSession");
               	exit(5);
        }



	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGHUP, ignoresignal);
	signal(SIGALRM, shutdownHandler);


}

void
PageWiperApp::buildPage()
{
	_security_id = ConfigMap::getConfig()->getValue("SECURITY_ID");
	char buf[10];
	for (int i=1 ; i <= 25; i++){
		sprintf(buf, "ROW80_%i", i);
		map<string, string>::iterator it = _trMap.find(buf);
		if (it != _trMap.end()){
			string rowID = (*it).second;
			string row = ConfigMap::getConfig()->getValue(buf);
			DataGraph::Variant v(DataGraph::SVAL, row.c_str());
			if ((_pageMap.insert(map<string, DataGraph::Variant>::value_type(rowID, v))).second == false)	
               			Logger::getLogger()->log(Logger::TYPE_ERROR, "PageWiperApp::failed insert for %s ", buf);
		}else{
               		Logger::getLogger()->log(Logger::TYPE_ERROR, "PageWiperApp::failed translate  %s ", buf);

		}
	}
	_mySession->publishInsertRequest(_pageMap, _security_id);
}

