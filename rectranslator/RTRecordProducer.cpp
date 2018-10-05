
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
#include "RTRecordProducer.hpp"
#include "RTApp.hpp"


RTRecordProducer::RTRecordProducer(RTApp* app, const string& name): rmdsRecordProducer(name)
{
    assert(app != NULL);
    m_app = app;
}


RTRecordProducer::~RTRecordProducer()
{
}

void 
RTRecordProducer::onConnected()
{
    if ( connected() )
    {
	sendSvcStatus(rmdsManagedHandler::Up);

	// Notify app 
	m_app->subscribeRecords();
    }
}


