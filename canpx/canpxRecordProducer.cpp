/************************************************************************
||
|| DATE:                $Date: 2006/06/05 13:18:38 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/canpxRecordProducer.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: canpxRecordProducer.cpp,v 1.1 2006/06/05 13:18:38 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: canpxRecordProducer.cpp,v $
|| LOG:                 Revision 1.1  2006/06/05 13:18:38  nickz
|| LOG:                 Ported record publisher as managed
|| LOG:
|| LOG:
************************************************************************/

// System includes
#include <assert.h>

// Application includes
#include "canpxRecordProducer.hpp"
#include "CanpxApp.hpp"


canpxRecordProducer::canpxRecordProducer(CanpxApp* app, const string& name): rmdsRecordProducer(name)
{
    assert(app != NULL);
    m_app = app;
}


canpxRecordProducer::~canpxRecordProducer()
{
}

void 
canpxRecordProducer::onConnected()
{
    if ( connected() )
    {
	sendSvcStatus(rmdsManagedHandler::Up);

	// Notify app 
	m_app->subscribeRecords();
    }
}


