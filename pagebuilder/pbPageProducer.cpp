/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:29:50 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/pbPageProducer.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: pbPageProducer.cpp,v 1.1 2006/06/08 15:29:50 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: pbPageProducer.cpp,v $
|| LOG:                 Revision 1.1  2006/06/08 15:29:50  nickz
|| LOG:                 ported to managed publishing
|| LOG:
************************************************************************/

// System includes
#include <assert.h>

// Application includes
#include "pbPageProducer.hpp"
#include "PBuilderApp.hpp"


pbPageProducer::pbPageProducer(PBuilder* app, const string& name): rmdsPageProducer(name)
{
    assert(app != NULL);
    m_app = app;
}


pbPageProducer::~pbPageProducer()
{
}

void 
pbPageProducer::onConnected()
{
    if ( connected() )
    {
	sendSvcStatus(rmdsManagedHandler::Up);

	// Notify app 
	m_app->subscribeRecords();
    }
}


