
/************************************************************************
||
|| DATE:                $Date: 2006/06/07 14:29:11 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/ESCFETISession.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESCFETISession.cpp,v 1.3 2006/06/07 14:29:11 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: ESCFETISession.cpp,v $
|| LOG:                 Revision 1.3  2006/06/07 14:29:11  nickz
|| LOG:                 Ported to managed published
|| LOG:
|| LOG:                 Revision 1.2  2006/04/11 16:03:55  nickz
|| LOG:                 Ported to RMDS (not tested)
|| LOG:
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <pthread.h>
#include <signal.h>
#include "Formatter.hpp"
#include "ESCFETISession.hpp"
#include "Logger.hpp"
#include "StorageManager.hpp"
#include "ESpeedApp.hpp"

ESCFETISession* ESCFETISession::_mySession = NULL;
char *ESCFETISession::currentSession = NULL;
unsigned int ESCFETISession::room = 0; 
bool ESCFETISession::isQueryDone = false;
char **ESCFETISession::instArray = NULL;
int ESCFETISession::numInstruments = 0;
unsigned int ESCFETISession::code = 0;
CFETI_TRADE_SESS_ID ESCFETISession::currentTradingSession = 0;
char* ESCFETISession:: _username = NULL;
char*  ESCFETISession::_password = NULL;
char*  ESCFETISession::_primary = NULL;
char*  ESCFETISession::_secondary = NULL;
bool  ESCFETISession::_activated = false;

pthread_mutex_t myMutex;

ESCFETISession::ESCFETISession(const ConfigMap& configMap)
{
	_username = strdup((configMap.getValue("CFETI.USER")).c_str());
	_password = strdup((configMap.getValue("CFETI.PASSWORD")).c_str());
	_primary = strdup((configMap.getValue("CFETI.ADDRESS1")).c_str());
	_secondary = strdup((configMap.getValue("CFETI.ADDRESS2")).c_str());
	room = Formatter::ATOF((configMap.getValue("CFETI.ROOM")).c_str());
	
}

ESCFETISession::~ESCFETISession()
{
	pthread_mutex_lock(&myMutex);
	theESpeedApp->sessionExpired(this);
	pthread_mutex_unlock(&myMutex);
}

long
ESCFETISession::Run()
{
	//make here thread for MessageLoop
	pthread_t tid;
	bool run_forever = true;
	pthread_create(&tid, NULL, ESCFETIProcessor::MessageLoop, NULL);
	signal (SIGINT, ESCFETISession::SigHandler);
	signal (SIGTERM, ESCFETISession::SigHandler);
	Logger::getLogger()->log(Logger::TYPE_INFO, "Run from %d thread", pthread_self());

	CFETI_IDENTIFICATION_DESC Ident;
	Ident.Company = const_cast<char*>("Moneyline");
	Ident.Application = const_cast<char*>("eSpeedDLP");
	Ident.Version = const_cast<char*>("2.0.0");
	int result = CFETIOpenSession(_primary, _secondary, &Ident);
	switch(result) {
		case CFETI_SUCCESS:
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETISession::Run() CFETIOpenSession succeeded");
			break;
		default:
			Logger::getLogger()->log(Logger::TYPE_ERROR, "ESCFETISession::Run() failed CFETIOpenSession  %d (%s)", result,CFETIGetLastError());
			//exit(1);
			shutdown();
			pthread_cancel(tid);
			return 0L;
	}
	
	result = CFETILogin(_username, _password, 0,
				ESCFETIProcessor::SystemCallback,
				(void *)this);
	switch(result) {
		case CFETI_SUCCESS:
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETISession::Run() CFETILogin succeeded");
			break;
		default:
			Logger::getLogger()->log(Logger::TYPE_ERROR, "ESCFETISession::Run() failed CFETILogin  %d (%s)", result,CFETIGetLastError());
			//exit(1);
			shutdown();
			pthread_cancel(tid);
			run_forever = false;
	}

	while (run_forever){
		if (!isQueryDone){
			sleep(5);
			continue;
		}

		for (int i = 0; i < numInstruments; i++) {
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETISession::Run() subscribe  %d %s", i, instArray[i]);
			//take a break on 100
			if ((i % 100) == 0) {
				sleep(2);
			}

			result = CFETIPostMessage(currentSession, currentTradingSession, CFETC_SUBSCRIBE, instArray[i], 0);
			if (result != CFETI_SUCCESS) {
				Logger::getLogger()->log(Logger::TYPE_ERROR, "ESCFETISession::Run() CFETIPostMessage CFETC_SUBSCRIBE failed   %d %s", i, instArray[i]);
				shutdown();
				pthread_cancel(tid);
				run_forever = false;
				break;
			}

		}
		isQueryDone = false;
	}

	Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETISession::Run() expired ... %d", pthread_self());
	return 0L;

}


void
ESCFETISession::shutdown()
{
    int result;

    if (currentTradingSession != 0) 
    {
	Logger::getLogger()->log(Logger::TYPE_INFO, 
				 "Initialing Disconnect from Trading System: %d , thread %d", 
				 room, pthread_self());

	result = CFETIDisconnect(currentSession, currentTradingSession, 0);

	if (result != CFETI_SUCCESS) 
	{
	    Logger::getLogger()->log(Logger::TYPE_ERROR, 
				     "CFETIDisconnect Failed : Error %d (%s)", 
				     result, CFETIGetLastError());
	}
    }
    
    if ( currentSession != NULL )
    {
	Logger::getLogger()->log(Logger::TYPE_INFO, "Initialing a Logout ...%s", 
				 currentSession);
	result = CFETILogout(currentSession, 0);

	if (result != CFETI_SUCCESS) 
	{
	    Logger::getLogger()->log(Logger::TYPE_ERROR, 
				     "CFETILogout Failed : Error %d (%s)", 
				     result, CFETIGetLastError());
	}
    }

    sleep(2);
    CFETICloseSession();
    freeInstArray();
}

void
ESCFETISession::freeInstArray()
{
	if (numInstruments == 0) return;

	for (int i = 0; i < numInstruments; i++) {
		delete instArray[i];
	}

	delete [] instArray;
	instArray = NULL;
	numInstruments = 0;
	_mySession = NULL;
	delete [] currentSession;
	currentSession = NULL;
	room = 0; 
	isQueryDone = false;

	code = 0;
	currentTradingSession = 0;
	delete [] _username;
	_username = NULL;
	delete [] _password;
	_password = NULL;
	delete [] _primary;
	_primary = NULL;
	delete [] _secondary;
	_secondary = NULL;
}

void
ESCFETISession::SigHandler(int sig)
{
	cout<<"Shutting down"<<endl;
	StorageManager::turnOff();
	shutdown();
	cout<<"Shuted down, Exiting..."<<endl;
	_exit(0);
	cout<<"Exited..."<<endl;
}

ESCFETISession *
ESCFETISession::getMySession()
{
	return _mySession;
}

void
ESCFETISession::activated(bool status)
{
	pthread_mutex_lock(&myMutex);
	_activated = status;	
	pthread_mutex_unlock(&myMutex);
}

bool
ESCFETISession::activated()
{
	bool status = false;
	pthread_mutex_lock(&myMutex);
	status = _activated;	
	pthread_mutex_unlock(&myMutex);
	return status;
}

