/************************************************************************
||
|| DATE:                $Date: 2006/03/28 21:15:07 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/Application.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Application.hpp,v 1.6 2006/03/28 21:15:07 mikhailt Exp $
|| REVISION:    $Revision: 1.6 $
|| LOG:                 $Log: Application.hpp,v $
|| LOG:                 Revision 1.6  2006/03/28 21:15:07  mikhailt
|| LOG:                 const char
|| LOG:
|| LOG:                 Revision 1.5  2005/01/10 16:12:24  mikhailt
|| LOG:                 send multicast
|| LOG:
|| LOG:                 Revision 1.4  2004/07/30 16:16:06  mikhailt
|| LOG:                 sends correct heartbeats
|| LOG:
|| LOG:                 Revision 1.3  2004/04/29 19:52:56  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.2  2004/01/09 21:33:58  tomcat
|| LOG:                 add TimerCallback
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H
#include <iostream.h>
#include <stdexcept>
#include <string>

using std::string;

#include "BasicComponent.hpp"
#include "IOCallback.hpp"
#include "TimerCallback.hpp"
#include "UDPSession.hpp"

//typedef XtIntervalId    TimerId;

class ConfigMap;
class DataFeed;
class UDPSession;

class Application : public IOCallback, public TimerCallback, public BasicComponent{
    
	// Allow main and to access protected member functions

	friend int main ( int, char ** );

	// private functions for registration

	private:    
		ConfigMap *_confmap;    
		DataFeed *_myDataFeed;
		UDPSession *_controlSession;	
		UDPSession *_heartbeatSession;	
	protected:
    
		// Support commonly needed data structures as a convenience
    
		XtAppContext _appContext;
    
		// Functions to handle Xt interface
		virtual void initialize ( int , char ** );  
		virtual void onInit(const ConfigMap& configMap);
		virtual void getConfig(string&);
		virtual void onConfig(ConfigMap *) {};
		virtual void handleEvents();
		virtual bool initControlSession(const ConfigMap& configMap);
		virtual bool initHeartbeatSession(const ConfigMap& configMap);
		virtual int initDaemon();

		//Calbacks
                // callback on IO event
                //virtual void onRead(int fid){ };
		//virtual void onWrite(int fid) { };
		//virtual void onExcept(int fid){ };

		virtual void appRemoveTimer ( TimerId timer);
		static void regTimerCallback(XtPointer clientData, TimerId timer);

		char   *_applicationClass;    // Class name of this application
		string _commOpt; //command line options, see getopt()
    
	public:
    
		Application ( const char * );
		virtual ~Application();     
    
		virtual void manage();
		virtual void unmanage();
    
		// Convenient access functions
    
		XtAppContext  appContext()  { return _appContext; }
		const char   *applicationClass()  { return _applicationClass; }
    
		virtual const char *const className() { return "Application"; }
		virtual string getCommOpt() const { return _commOpt;}
		virtual void setDataFeed(DataFeed * feed){
			_myDataFeed = feed;
		}

		virtual DataFeed * getDataFeed(){
			return _myDataFeed;
		}
	
		virtual ConfigMap * getMyConfig(){
			return _confmap;
		}
		
		virtual UDPSession * getControlSession(){
			return _controlSession;
		}

		static ConfigMap * configMap;
};

// Pointer to single global instance

extern Application *theApplication; 

#endif
