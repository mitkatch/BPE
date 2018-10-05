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

#ifndef RTAPP_HPP 
#define RTAPP_HPP 

// Application includes
#include "RTrmdsRecordHandler.hpp"
#include "RTRecordProducer.hpp"
#include "Application.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "RecordArrayConverter.hpp"
#include "subjectPubMgr.hpp"


class RTApp: public Application
{
	public:
		RTApp(char*);
		virtual ~RTApp();
		virtual void onInit(const ConfigMap& configMap);
		virtual const char *const className(){return "RTApp";}
                virtual void subscribeRecords();
		virtual RTRecordProducer* getProducer();
		virtual Subscriber* getSubMgr(){
			return _subjPubManager;
		}
			
	protected:
		virtual void onRead(int fd, InputId id){};
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};

	private:
		const ConfigMap *_configMap;
		bool		_init;
		RTRecordProducer* _rmdsRecordProducer;
		RTrmdsRecordHandler * _rmdsRecordHandler;
		RecordArrayConverter *_pArrayConverter;
		subjectPubMgr*	_subjPubManager;
};

extern RTApp *theRTApp;

#endif // RTAPP_HPP
