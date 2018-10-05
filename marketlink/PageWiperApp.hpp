
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

#ifndef PAGEWIPERAPP_HPP 
#define PAGEWIPERAPP_HPP 

#include <vector>


#include "Application.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "DataGraph.hpp"

#include "WiperFeedSession.hpp"

class PageWiperApp: public Application 
{
	public:

		PageWiperApp(const char *);
		virtual ~PageWiperApp();
		virtual void onInit(const ConfigMap& confMap);
		virtual const char *const className(){return "PageWiperApp";}
		void buildPage();

		virtual void shutdown();
	protected:
		virtual void onRead(int fd, InputId id){};
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};
		virtual void onTimer(TimerId id){};

	private:
		const ConfigMap *_configMap;
		bool            _init;
		string _security_id;
		DataGraph::DataMap _pageMap;
		map<string, string> _trMap;
		WiperFeedSession *_mySession;
};

extern PageWiperApp *thePageWiperApp;

#endif // 
