
/************************************************************************
||
|| DATE:                $Date: 2005/11/17 21:57:58 $
|| SOURCE:              $Source: /export/home/tomcat/Build/espeedfeedII/espeedII/RCS/ESpeedApp.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESpeedApp.hpp,v 1.1 2005/11/17 21:57:58 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ESpeedApp.hpp,v $
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ESPEEDAPP_HPP
#define ESPEEDAPP_HPP

#include <vector>
#include <string>

#include "Application.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "ESOpenServer.hpp"
#include "ESCFETISession.hpp"
#include "TrsRecordProducer.hpp"
#include "Datum.hpp"

using std::vector;
using std::string;

class ESpeedApp: public Application
{
	public:
		ESpeedApp(char *);
		virtual ~ESpeedApp();
		virtual void onInit(const ConfigMap& configMap);
		virtual const char *const className(){return "ESpeedApp";}

		virtual void shutdown();
		virtual void loadDataProducer();
		virtual void sessionExpired(ESCFETISession *session);
	protected:
		virtual void onRead(int fd, InputId id){};
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};
		virtual void onTimer(TimerId id);

	private:
		TrsRecordProducer* _trsDataProducer;
		ESOpenServer * _myServer;
		ESCFETISession * _cfetiSession;
		vector<long> _tV;
};
extern ESpeedApp *theESpeedApp;
#endif // ESPEEDAPP_HPP
