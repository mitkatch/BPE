/************************************************************************
||
|| DATE:                $Date: 2006/06/07 14:29:12 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/esRecordProducer.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: esRecordProducer.cpp,v 1.1 2006/06/07 14:29:12 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: esRecordProducer.cpp,v $
|| LOG:                 Revision 1.1  2006/06/07 14:29:12  nickz
|| LOG:                 Ported to managed published
|| LOG:
************************************************************************/

// System includes
#include <assert.h>

// Application includes
#include "esRecordProducer.hpp"
#include "ESpeedApp.hpp"


esRecordProducer::esRecordProducer(ESpeedApp* app, const string& name): rmdsRecordProducer(name)
{
    assert(app != NULL);
    m_app = app;
}


esRecordProducer::~esRecordProducer()
{
}

void 
esRecordProducer::onConnected()
{
    if ( connected() )
    {
	sendSvcStatus(rmdsManagedHandler::Up);

	// Notify app 
	m_app->subscribeRecords();
    }
}


