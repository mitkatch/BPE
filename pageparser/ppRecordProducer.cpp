/************************************************************************
||
|| DATE:                $Date: 2006/06/04 11:27:07 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/ppRecordProducer.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ppRecordProducer.cpp,v 1.1 2006/06/04 11:27:07 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: ppRecordProducer.cpp,v $
|| LOG:                 Revision 1.1  2006/06/04 11:27:07  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

// System includes
#include <assert.h>

// Application includes
#include "ppRecordProducer.hpp"
#include "PageParserApp.hpp"


ppRecordProducer::ppRecordProducer(PageParserApp* app, const string& name): rmdsRecordProducer(name)
{
    assert(app != NULL);
    m_app = app;
}


ppRecordProducer::~ppRecordProducer()
{
}

void 
ppRecordProducer::onConnected()
{
    if ( connected() )
    {
	sendSvcStatus(rmdsManagedHandler::Up);

	// Notify app 
	m_app->subscribePages();
    }
}


