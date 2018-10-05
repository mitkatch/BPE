/************************************************************************
||
|| DATE:                $Date: 2006/03/30 21:49:07 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/Application.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Application.cpp,v 1.8 2006/03/30 21:49:07 tomcat Exp $
|| REVISION:    $Revision: 1.8 $
|| LOG:                 $Log: Application.cpp,v $
|| LOG:                 Revision 1.8  2006/03/30 21:49:07  tomcat
|| LOG:                 command line -V 4 for ERROR
|| LOG:
|| LOG:                 Revision 1.7  2006/03/28 21:51:36  mikhailt
|| LOG:                 const char in the constructor
|| LOG:
|| LOG:                 Revision 1.6  2005/03/29 16:05:55  mikhailt
|| LOG:                 static ConfigMap
|| LOG:
|| LOG:                 Revision 1.5  2005/01/10 16:12:24  mikhailt
|| LOG:                 send multicast
|| LOG:
|| LOG:                 Revision 1.4  2004/12/15 20:24:46  mikhailt
|| LOG:                 check for SYSLOG condition
|| LOG:
|| LOG:                 Revision 1.3  2004/07/30 16:14:57  mikhailt
|| LOG:                 sends correct heartbeats
|| LOG:
|| LOG:                 Revision 1.2  2004/04/28 16:22:45  mikhailt
|| LOG:                 using basename for name of app
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <libgen.h>

#include "Application.hpp"
#include <assert.h>
#include <stdlib.h>
#include "ConfigMap.hpp"
#include "Logger.hpp"
#include "HeartbeatSession.hpp"
#include "ControlSession.hpp"

static char * URL_DELIMITER = "://";
Application *theApplication = NULL;

ConfigMap * Application::configMap = NULL;

Application::Application ( const char *appClassName ) : 
                    BasicComponent( appClassName )
{
	// Set the global Application pointer
    
	theApplication = this;  
    
	// Initialize data members
    
	_appContext = NULL;
	_controlSession = NULL;
	_applicationClass = strdup ( appClassName );
	_commOpt = "V:Dc:"; 	//-V [_level_] Logger.hpp verbose
				//-D run as demon
				//-c file://<PATH_TO_THE_CONFIG_FILE>
				//-c http://<URL_TO_CONFIG_FILE> 
}

void Application::initialize ( int argcp, char **argv )
{
	XtToolkitInitialize();
	if (XtToolkitThreadInitialize())
		_appContext = XtCreateApplicationContext();
	else{
		Logger::getLogger()->log(Logger::TYPE_ERROR, "XtToolkitThreadInitialialize not initialized. Exit");
		exit(9);

	}
	if (_name) delete []_name;
	_name = strdup ( basename(argv[0]) );
	installDestroyHandler();
	_confmap = new ConfigMap(this);
	ConfigMap::setConfig(_confmap);
	Application::configMap = _confmap;
	_confmap->loadCommandLine(argcp, argv);
	string verbose = _confmap->getValue("V");
	if (verbose.size()){
		Logger::getLogger()->setLevel(atoi(verbose.c_str()));
	}

	string isDaemon = _confmap->getValue("D");
	if (isDaemon.size()){
		initDaemon();
	}

	string confstr = _confmap->getValue("c");
	if (confstr.size()){
		getConfig(confstr);
	} else{
		Logger::getLogger()->log(Logger::TYPE_ERROR, "No command options -c found");
	}
		
	//string control = _confmap->getValue("CONTROL_SESSION");
	if ( _confmap->getValue("CONTROL_SESSION") == "TRUE")
		initControlSession(*_confmap);
 
	//string hb = _confmap->getValue("HEARTBEAT_SESSION");
	if (_confmap->getValue("HEARTBEAT_SESSION") == "TRUE")
		initHeartbeatSession(*_confmap);
 
	string loggerTo = _confmap->getValue("LOGGER");
	if (loggerTo == "SYSLOG")
		Logger::getLogger()->setLogger(Logger::SYS_LOGGER, _name);
	else
		Logger::getLogger()->setLogger(Logger::LOG_CONSOLE, _name);

	onInit(*_confmap);    
}

void
Application::onInit(const ConfigMap& configMap)
{
	cout<<"onInit of "<<_name<<" : "<<_applicationClass<<endl;

}

bool
Application::initControlSession(const ConfigMap& configMap)
{
	_controlSession = new ControlSession(configMap);
	_controlSession->setPName(_name);
	return _controlSession->initSession();

}

bool
Application::initHeartbeatSession(const ConfigMap& configMap)
{
	_heartbeatSession = new HeartbeatSession(configMap);
	_heartbeatSession->setPName(_name);
	return _heartbeatSession->initSession();

}

/****
void
Application::onRead(int fd)
{
	cout << "Application::onIO for fd="<<fd<<endl;
}
***/

void
Application::regTimerCallback(XtPointer clientData, TimerId timer)
{

}

void
Application::appRemoveTimer ( TimerId timer)
{
	XtRemoveTimeOut(timer);
}

Application::~Application()
{
	XtDestroyApplicationContext(_appContext);
	delete []_applicationClass;
	if (_confmap){
		delete _confmap;
		Application::configMap = NULL;
	}
}

void 
Application::handleEvents()
{
	// Just loop forever
    
	XtAppMainLoop ( _appContext );
}

void 
Application::manage()
{
}

void 
Application::unmanage()
{
}

void
Application::getConfig(string& opt)
{
	// to process diff file://, http://, xml:// or anythig else
	unsigned int type = opt.find(URL_DELIMITER);
	if (type != string::npos){
		//type += strlen(URL_DELIMITER);
		string url = opt.substr(type + strlen(URL_DELIMITER), string::npos);
		cout<<"Found url "<<url<<endl;
		string url_type = opt.substr(0, type);
		cout<<"The type is "<<url_type<<endl;
		_confmap->loadUrl(url_type, url);
	} 
	onConfig(_confmap);
}

int
Application::initDaemon()
{
	pid_t pid;
	if ( (pid = fork()) < 0)
		return -1;
	else if (pid != 0)
		exit(0); //parent exits;

	//child continue;
	setsid();
	chdir("/");
	umask(0);
	return 0;
}

