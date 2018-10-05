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

// System includes
#include <assert.h>

// Application includes
#include "RMDSFeedSession.hpp"
#include "ML2TFeedApp.hpp"
#include "Logger.hpp"
#include "ML2TConverter.hpp"
#include "DataCache.hpp"

RMDSFeedSession::RMDSFeedSession(ML2TFeedApp* app, const string& name): rmdsPageProducer(name)
{
    assert(app != NULL);
    m_app = app;
}


RMDSFeedSession::~RMDSFeedSession()
{
}

bool
RMDSFeedSession::initSession()
{
	bool ret = true;
	if (this->init() == -1){
		ret = false;
		Logger::getLogger()->log(Logger::TYPE_ERROR, "RMDSFeedSession->init failed. Exit.");
	}

	return ret;
}

void 
RMDSFeedSession::onConnected()
{
    if ( connected() )
    {
	sendSvcStatus(rmdsManagedHandler::Up);

	// Notify app 
	m_app->subscribeSources();
    }
}


void
RMDSFeedSession::publishPageImage(DataPage* page, DataPage::PageSlot& slot, const std::string &sec_id)
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "RMDSFeedSession::publishPageImage for %s", sec_id.c_str());
	DataPage *newpage = (DataPage *)DataCache::getCache()->getDatum(sec_id);
	if (newpage == NULL){
		if (DataCache::getCache()->addDatum(sec_id, Datum::PAGE))
			newpage = (DataPage *)DataCache::getCache()->getDatum(sec_id);
	}

	if (newpage){
		ML2TConverter c(page, newpage, slot);
		this->updatePage(&c);
	}else {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "RMDSFeedSession::publishPageImage publish failed for %s", sec_id.c_str());

	}
}

void
RMDSFeedSession::publishPageUpdate(DataPage::Update& update, const std::string &sec_id)
{

	Logger::getLogger()->log(Logger::TYPE_INFO, "RMDSFeedSession::publishPageUpdate for %s", sec_id.c_str());
	DataPage *newpage = (DataPage *)DataCache::getCache()->getDatum(sec_id);
	if (newpage == NULL){
		if (DataCache::getCache()->addDatum(sec_id, Datum::PAGE))
			newpage = (DataPage *)DataCache::getCache()->getDatum(sec_id);
	}

	if (newpage){
		ML2TConverter c(newpage, update);
		this->updatePage(&c);
	}else {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "RMDSFeedSession::publishPageImage publish failed for %s", sec_id.c_str());

	}
}
