
/************************************************************************
||
|| DATE:                $Date: 2005/11/17 21:57:58 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/ESCFETISession.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESCFETISession.hpp,v 1.1 2005/11/17 21:57:58 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ESCFETISession.hpp,v $
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ESCFETISESSION_HPP
#define ESCFETISESSION_HPP 
#include <vector>
#include "Runnable.hpp"
#include "ConfigMap.hpp"
#include "ESCFETIProcessor.hpp"

using std::vector;

class ESCFETISession: public Runnable
{
	public:
		ESCFETISession(const ConfigMap& configMap);
		virtual ~ESCFETISession();
		virtual long Run();
		static void shutdown();

		static ESCFETISession *getMySession();
		static void SigHandler(int sig);
		static void activated(bool status);
		static bool activated();
	protected:


		static void freeInstArray();
	protected:
		friend ESCFETIProcessor;
		static ESCFETISession* _mySession;
		static char* currentSession;
		static unsigned int room;
		static unsigned int code;
		static bool isQueryDone;
		static char** instArray;
		static int numInstruments;
		static CFETI_TRADE_SESS_ID currentTradingSession;
		static bool _activated;
		static char*  _username;
		static char*  _password;
		static char*  _primary;
		static char*  _secondary;
};
#endif // ESCFETISESSION_HPP
