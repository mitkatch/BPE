/************************************************************************
||
|| DATE:                $Date: 2006/06/04 11:27:06 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/PageParserApp.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PageParserApp.cpp,v 1.11 2006/06/04 11:27:06 nickz Exp $
|| REVISION:		$Revision: 1.11 $
|| LOG:                 $Log: PageParserApp.cpp,v $
|| LOG:                 Revision 1.11  2006/06/04 11:27:06  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.10  2006/04/10 17:50:04  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.9  2006/04/01 12:10:09  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.8  2006/03/23 19:30:16  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.7  2006/03/18 13:19:57  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.6  2006/03/06 16:01:46  nickz
|| LOG:                 Updating interface to RMDS
|| LOG:
|| LOG:
************************************************************************/

// System includes
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Application includes
#include "PageParserApp.hpp"
#include "PyParser.hpp"
#include "DataCache.hpp"
#include "MCMsg.hpp"
#include "ControlSession.hpp"

PageParserApp *thePageParserApp = new PageParserApp((char *)"PageParserApp");

static void
sighandler(int signo)
{
    pid_t pid;
    int status;
    Logger::getLogger()->log(Logger::TYPE_INFO, 
			     "PageParserApp::Signal %d caught.  Shutting down application...", signo);
    delete thePageParserApp;
    exit(1);
}

static void
sigchildhandler(int sigid)
{
    pid_t pid;
    int status;
    Logger::getLogger()->log(Logger::TYPE_INFO, "PageParserApp::sigchildhandler SIGCHLD");
    
    if ( (pid = wait(&status)) < 0)
    {
       	Logger::getLogger()->log(Logger::TYPE_INFO, "PageParserApp::sigchildhandler SIGCHLD");
    }
    Logger::getLogger()->log(Logger::TYPE_INFO, "PageParserApp::sigchildhandler SIGCHLD pid=%i");
    signal(SIGCHLD, sigchildhandler);
    return;
}


static void
shutdownHandler(int signo)
{
    Logger::getLogger()->log(Logger::TYPE_INFO, "PageParserApp::shutdownHandler %d ", signo);
    thePageParserApp->shutdown();
}

static void
ignoresignal(int signo)
{
    Logger::getLogger()->log(Logger::TYPE_INFO, "PageParserApp:: ignoresignal %d " , signo);
}


PageParserApp::PageParserApp(char* name):Application(name)
{
}

PageParserApp::~PageParserApp()
{
    this->shutdown();
}

void
PageParserApp::shutdown()
{
    cout<<"About to shutdown  myServer"<<endl;
    if (_myServer)
	delete _myServer;
    
    cout<<"About to shutdown rmdsDataProducer"<<endl;
    if (_rmdsDataProducer)
	delete _rmdsDataProducer;

    if (_subjPubManager)
	delete _subjPubManager;
    
    cout<<"About to shutdown rmdsHandler"<<endl;
    if (_rmdsHandler)
	delete _rmdsHandler;
    
    int i = 0;
    for(i; i < _vListeners.size(); i++){
	delete _vListeners[i];	
    }
    
    cout << "Deleted "<<i--<<" Listeners"<<endl;
    cout<<"Done."<<endl;
}

void
PageParserApp::onInit(const ConfigMap& configMap)
{
    buildNameSpace(configMap);
    
    _myServer = new ParserOpenServer(configMap);
    if (_myServer->initServer() == false){
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "PageParserApp::onInit failed _myServer->initServer");
	exit(4);
    }
    _myServer->setNameSpace(_my_namespace);
    
    string persistent = configMap.getValue("PERSISTENT_CACHE");
    if (persistent == "TRUE")
	DataCache::getCache()->setPersistent(true);

    // Define, initializae and launch the RMDS record handler thread
    _rmdsHandler = new rmdsPageHandler(configMap);
    setDataFeed(_rmdsHandler);

    // Initialize and launch RMDS Handler thread
    if (_rmdsHandler->init() == -1 )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "PageParserApp::onInit rmdsHandler init failed. ");
	exit(3);
    }
    
    // Set the subscribers upon update within the data cache
    _rmdsDataProducer = new ppRecordProducer(this, _name);
    _subjPubManager = new subjectPubMgr(_name);
    
    if(_rmdsDataProducer->init() == -1)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "PageParserApp::onInit _rmdsDataProducer init failed. ");
	exit(4);
    }

    // Initialize and launch the subjectManager thread
    if (_subjPubManager->init() == -1 )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "PageParserApp::onInit subjectPubMgr init failed. ");
	exit(3);
    }

    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGHUP, ignoresignal);
    //signal(SIGCHLD, sigchildhandler);
    //signal(SIGALRM, shutdownHandler); //for debug
    
}

void 
PageParserApp::subscribePages()
{
    static bool subscribed = false;

    if ( !subscribed )
    {
	NSMap ns = this->getNameSpace();
	NSMap::iterator it = ns.begin();
	
	for (it; it != ns.end(); it++)
	{
	    string address = (*it).second.source + string(1, '.') +  (*it).second.page;
	    DataCache::getCache()->subscribe(address, this, Datum::PAGE);
	    DataCache::getCache()->setSubscriberByWCard(_rmdsDataProducer, (*it).second.subscriber);
	    DataCache::getCache()->setSubscriberByWCard(_subjPubManager, (*it).second.subscriber);
	}
	subscribed = true;
    }
}


void
PageParserApp::buildNameSpace(const ConfigMap& confMap)
{
    string url_id = URL;
    string  root_ns = confMap.getValue("NAME_SPACE");
    map<string, string>::const_iterator it = confMap.begin();
    for (it ; it != confMap.end(); it++){
	int urlind = (*it).first.find(url_id);
	if (urlind == 0){
	    int name_id = (*it).first.find(URL_NAME);
	    if (name_id != string::npos){
		int i = (*it).first.length();
		string new_ns = (*it).first.substr(url_id.length(), 
						   i - strlen(URL_NAME) - url_id.length());
		cout << "NameSpace is "<<new_ns<<endl;
		_my_namespace[new_ns] = URLNameSpace();
	    }
	}
    }
    NSMap::iterator nsit = _my_namespace.begin();
    for (nsit; nsit != _my_namespace.end(); nsit++){
	URLNameSpace &ns = (*nsit).second;
	string getname = string(URL)+(*nsit).first + string(URL_NAME);
	string getsource = string(URL)+(*nsit).first + string(URL_SOURCE);
	string getpage = string(URL) +(*nsit).first + string(URL_PAGE);
	string getparser_file = string(URL) + (*nsit).first + string(URL_PARSER);
	string getparser_map = string(URL) + (*nsit).first + string(URL_PARSER_MAP);
	string getsubscriber = string(URL) + (*nsit).first + string(URL_SUBSCRIBER);
	ns.url = root_ns + confMap.getValue(getname.c_str());
	ns.source = confMap.getValue(getsource.c_str());
	ns.page = confMap.getValue(getpage.c_str());
	ns.parser = confMap.getValue(Formatter::trim(getparser_file).c_str());
	ns.parser_map = confMap.getValue(Formatter::trim(getparser_map).c_str());
	ns.subscriber = confMap.getValue(Formatter::trim(getsubscriber).c_str());
	//Formatter::split(confMap.getValue(getparam.c_str()), ",", ns.parameters);
	
    }
}

void
PageParserApp::notify(Datum *datum)
{
    DataPage *page = dynamic_cast<DataPage *>(datum);
    cout<<"PageParserApp notified from "<<page->address()<<endl;
    if (page){	
	NSMap ns = this->getNameSpace();
	NSMap::iterator it = ns.begin();
	for (it; it != ns.end(); it++){
	    string address = (*it).second.source + string(1, '.') +  (*it).second.page;
	    if (address == page->address()){
		PyParser *listener = new PyParser((*it).second);
		vector<DataPage::PageSlot> slots = listener->getMySlot();
		vector<DataPage::PageSlot>::iterator its = slots.begin();
		for (its; its != slots.end(); its++){
		    
		    page->addListener(listener, (*its));	
		}
		DataCache::getCache()->subscribe(address, listener, Datum::PAGE);
		_vListeners.push_back(listener);
	    }
	}

        //to be done only once to activate listeners
	DataCache::getCache()->unsubscribe(page->address(), this);
	cout << "PageParserApp unregister IMAGE listener from "<< page->address() << endl;
    }
    /** (TEST) **/
    if (_vListeners.size() == _my_namespace.size())
    { 
	//run only once when all pages processed 
	broadcastControlMsg("LOADED");
    }
}

void
PageParserApp::broadcastControlMsg(const char* type)
{
    MCMsg msg;
    msg.buildPrefix("LIST");
    msg.buildSuffix(_name);
    msg.addKeyValue("TYPE", type);
    ControlSession *cs = NULL;
    if (cs = (ControlSession *)this->getControlSession()){
	cs->broadcast(msg);
    }	
    
}

