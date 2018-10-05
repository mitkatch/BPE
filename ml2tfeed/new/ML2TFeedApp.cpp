
/************************************************************************
||
|| DATE:                $Date: 2006/04/07 19:31:50 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/ML2TFeedApp.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ML2TFeedApp.cpp,v 1.1 2006/04/07 19:31:50 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ML2TFeedApp.cpp,v $
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <signal.h>
#include "RMDSFeedSession.hpp"
//#include "rmdsPageHandler.hpp"

#include "DataCache.hpp"
#include "ML2TFeedApp.hpp"
#include "ML2TPageListener.hpp"
#include "ML2TFeedSession.hpp"


ML2TFeedApp *theML2TFeedApp = new ML2TFeedApp("ML2TFeedApp");

static void
sighandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedApp::Good bye cruel world! %d kiiled me. Exit...", signo);
	delete theML2TFeedApp;
	exit(1);
}

static void
ignoresignal(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedApp:: IgnoreSygnal %d ", signo);
}


static void
shutdownHandler(int signo){
        Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedApp::shutdownHandler %d ", signo);
	theML2TFeedApp->shutdown();
	exit(1);
}



ML2TFeedApp::ML2TFeedApp(const char* name):Application(name)
{
}

ML2TFeedApp::~ML2TFeedApp()
{
	this->shutdown();
}

void
ML2TFeedApp::shutdown()
{
	cout<<"About to delete mySession"<<endl;
	if (_mySession){
		delete _mySession;
		_mySession = NULL;
	}
	cout<<"About to delete rmdsPageHandler"<<endl;
	if (_rmdsPageHandler){
		delete _rmdsPageHandler;
		_rmdsPageHandler = NULL;
	}
	int i = 0;
	for(i; i < _vListeners.size(); i++){
		delete _vListeners[i];	
	}
	cout << "Deleted "<<i<<" Listeners"<<endl;
	cout<<"Done."<<endl;
}

void
ML2TFeedApp::onInit(const ConfigMap& configMap)
{
	buildNameSpace(configMap);

	_rmdsPageHandler = new rmdsPageHandler(configMap);
	if (_rmdsPageHandler->init() == -1 ){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TFeedApp::onInit rmdsPageHandler init failed. ");
		exit(3);
	}
	setDataFeed(_rmdsPageHandler);

	string session =  configMap.getValue("FEED_SESSION");
	if (session == "ML2T"){
		_mySession = new ML2TFeedSession(this, _name);
	} else if (session == "RMDS"){
		_mySession = new RMDSFeedSession(this, _name);
	} else {
                Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TFeedApp::onInit failed create session %s", session.c_str());
                exit(4);
	}

        if (_mySession->initSession() == false){
                Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TFeedApp::onInit failed _mySession->initSession");
                exit(4);
        }



	//subscribeSources(); from the session

	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGHUP, ignoresignal);
	signal(SIGALRM, shutdownHandler);

}

void
ML2TFeedApp::buildNameSpace(const ConfigMap& confMap)
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
				_my_namespace[new_ns] = URLNameSpace();
			}
		}
	}
	NSMap::iterator nsit = _my_namespace.begin();
	for (nsit; nsit != _my_namespace.end(); nsit++){
		URLNameSpace &ns = (*nsit).second;
		string getpage = string(URL)+(*nsit).first + string(URL_PAGE);
		string getsource = string(URL)+(*nsit).first + string(URL_SOURCE);
		string getslot = string(URL) + (*nsit).first + string(URL_SLOT);
		string getsecurity_id = string(URL) + (*nsit).first + string(URL_SECURITY_ID);

		ns.source = confMap.getValue(getsource.c_str());
		ns.page = confMap.getValue(getpage.c_str());
		ns.slot = confMap.getValue(getslot.c_str());
		ns.security_id = confMap.getValue(getsecurity_id.c_str());
		//Formatter::split(confMap.getValue(getparam.c_str()), ",", ns.parameters);

	}
}

void
ML2TFeedApp::subscribeSources()
{
	NSMap ns = this->getNameSpace();
	NSMap::iterator it = ns.begin();
	for (it; it != ns.end(); it++){
		string address = (*it).second.source + string(1, '.') +  (*it).second.page;
		DataCache::getCache()->subscribe(address, this, Datum::PAGE);
		Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TFeedApp::onInit subscribed for %s. ", address.c_str());

		//registerListeners after whole page recieved 
	}
}

void
ML2TFeedApp::notify(Datum *datum)
{
	DataPage *page = dynamic_cast<DataPage *>(datum);
	//cout<<"ML2TFeedApp notified from "<<page->address()<<endl;
	if (page){	
		NSMap ns = this->getNameSpace();
		NSMap::iterator it = ns.begin();
		for (it; it != ns.end(); it++){
			string address = (*it).second.source + string(1, '.') +  (*it).second.page;
			if (address == page->address()){
				ML2TPageListener *listener = new ML2TPageListener(_mySession, (*it).second);
				vector<DataPage::PageSlot> slots = listener->getMySlot();
				vector<DataPage::PageSlot>::iterator its = slots.begin();
				for (its; its != slots.end(); its++){

					page->addListener(listener, (*its));	
				}
				DataCache::getCache()->subscribe(address, listener, Datum::PAGE);
				_vListeners.push_back(listener);
			}
		}
		DataCache::getCache()->unsubscribe(page->address(), this);//to be done only once to activate listeners
		//cout<<"ML2TFeedApp unsubscribe from "<<page->address()<<endl;
	}
}
