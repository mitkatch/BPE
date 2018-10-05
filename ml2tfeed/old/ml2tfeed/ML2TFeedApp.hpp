/************************************************************************
||
|| DATE:                $Date: 2006/04/07 19:31:50 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/ML2TFeedApp.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ML2TFeedApp.hpp,v 1.1 2006/04/07 19:31:50 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ML2TFeedApp.hpp,v $
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ML2TFEEDAPP_HPP 
#define ML2TFEEDAPP_HPP
#include <vector>
#include "Application.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "ML2TFeedSession.hpp"
#include "TrsPageHandler.hpp"
#include "TrsRecordHandler.hpp"
#include "NSMap.hpp"
#include "Datum.hpp"
#include "PageListener.hpp"

#define URL "URL."
#define URL_SOURCE ".SOURCE"
#define URL_PAGE ".PAGE"
#define URL_SLOT ".SLOT"
#define URL_SECURITY_ID ".SECURITY_ID"


class ML2TFeedApp: public Application, public Subscriber
{
	public:
		ML2TFeedApp(const char *);
		virtual ~ML2TFeedApp();
		virtual void onInit(const ConfigMap& configMap);
		virtual const char *const className(){return "ML2TFeedApp";}

		virtual TrsPageHandler* getTrsPageHandler() const {
			return _trsPageHandler;
		}

		virtual TrsRecordHandler* getTrsRecordHandler() const {
			return _trsRecordHandler;
		}

		virtual NSMap getNameSpace() const {
			return _my_namespace;
                }

		virtual void notify(Datum *datum);

		virtual void shutdown();
	protected:
		virtual void onRead(int fd, InputId id){};
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};
		virtual void onTimer(TimerId id){};

		virtual void buildNameSpace(const ConfigMap &confMap);
		Logger *_myLog;
		NSMap _my_namespace;

	private:
		const ConfigMap *_configMap;
		bool            _init;
		TrsPageHandler * _trsPageHandler;
		TrsRecordHandler * _trsRecordHandler;
		ML2TFeedSession	* _mySession;
		vector<PageListener *> _vListeners;
};

extern ML2TFeedApp *theML2TFeedApp;

#endif //ML2TFEEDAPP_HPP
