/************************************************************************
||
|| DATE:                $Date: 2006/06/04 11:34:14 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsManagedHandler.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsManagedHandler.cpp,v 1.1 2006/06/04 11:34:14 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: rmdsManagedHandler.cpp,v $
|| LOG:                 Revision 1.1  2006/06/04 11:34:14  nickz
|| LOG:                 Changed record producer from Unmanaged to managed publisher
|| LOG:
|| LOG:
************************************************************************/

// System includes
#include <assert.h>

// RFA includes
#include "SessionLayer/MarketDataManagedPublisher.h"
#include "SessionLayer/MarketDataManagedPublisherClientSessionListenerInterestSpec.h"
#include "SessionLayer/MarketDataManagedPublisherConnectionInterestSpec.h"
#include "SessionLayer/MarketDataManagedPublisherEntitlementsAuthenticationInterestSpec.h"
#include "SessionLayer/MarketDataManagedPublisherErrorInterestSpec.h"
#include "SessionLayer/MarketDataManagedItemsReplyPub.h"
#include "SessionLayer/MarketDataManagedActiveClientSessionPubEvent.h"
#include "SessionLayer/MarketDataManagedActiveItemsPubEvent.h"
#include "SessionLayer/MarketDataManagedInactiveItemsPubEvent.h"
#include "SessionLayer/MarketDataManagedInactiveClientSessionPubEvent.h"
#include "SessionLayer/MarketDataManagedActiveItemsPubEvent.h"
#include "SessionLayer/MarketDataManagedSvcPub.h"

// Application includes
#include "rmdsManagedHandler.hpp"
#include "Logger.hpp"

using namespace rfa::sessionLayer;
using namespace rfa::common;


rmdsManagedHandler::rmdsManagedHandler(const string& name): m_name(name)
{
    m_pMarketDataManagedPublisher = NULL;
    m_criteriaClient = NULL;
}

rmdsManagedHandler::~rmdsManagedHandler()
{
}

int 
rmdsManagedHandler::init()
{
    if ( rmdsHandler::init() != -1 )
    {
	assert(s_pDB != NULL);

	// Create our Managed session
	ConfigMap* config = ConfigMap::getConfig();

	string sessionName = config->getValue("RFA_MANAGED_SESSION");

	try
	{
	    m_pSession = Session::acquire(sessionName.data());
	
	    if (m_pSession == NULL)
	    {
		Logger::getLogger()->log(Logger::TYPE_ERROR, 
					 "Error Acquiring Managed RFA session.  Refer to RFA logs");
		return(-1);				       		       
	    }

	    // Our first step is to create our publisher to receive system events
	    m_pMarketDataManagedPublisher = m_pSession->createMarketDataManagedPublisher(m_name);
	    assert( m_pMarketDataManagedPublisher );
	
	    // Register to accept new client session requests
	    MarketDataManagedPublisherClientSessionListenerInterestSpec listenSpec;
	
	    // Registration defines the following:
	    // 1.  What queue the events will sit on, 
	    // 2.  What events we are interested in 
	    // 3.  Where and how we want to process these events
	    // 4.  What closure, if any, to receive (we're usually not interested)
	    Handle* pHandle = m_pMarketDataManagedPublisher->registerClient(*s_pEQueue, 
									    listenSpec,
									    *this, NULL);
	    assert(pHandle);
	
	    // Register for ManagedPublisher Connection Interest Specification
	    MarketDataManagedPublisherConnectionInterestSpec connectSpec;
	    
	    pHandle = m_pMarketDataManagedPublisher->registerClient(*s_pEQueue,
								    connectSpec, 
								    *this, NULL);
	    assert(pHandle);
	    
	    // Register for ManagedPublisher Error Interest Specification
	    MarketDataManagedPublisherErrorInterestSpec errSpec;
	    
	    pHandle = m_pMarketDataManagedPublisher->registerClient(*s_pEQueue,
								    errSpec, 
								    *this, NULL);
	    assert(pHandle);
	    
	    // Define the service
	    m_serviceName = config->getValue("RFA_MANAGED_SERVICE");
	    
	    // Initialize Item Pub object
	    MarketDataItemStatus status;
	    
	    status.setState( MarketDataItemStatus::Ok );
	    status.setStatusCode( MarketDataItemStatus::None);
	    status.setStatusText( "None" );
	    
	    // Initialize Item Publishing Object
	    m_marketDataManagedItemPub.setStatus( status );
	    m_marketDataManagedItemPub.setServiceName(m_serviceName);

	    // Initialize Svc Status Object
	    m_marketDataManagedSvcPub.setClientSessionHandle(NULL);	// Send to all clients
	    m_marketDataManagedSvcPub.setServiceName(m_serviceName);
	}
	catch (rfa::common::Exception& e)
	{
	    // Format and log the RFA exception
	    rfaException(e);
	    
	    return(-1);
	}

    }

    return(0);
}

void
rmdsManagedHandler::processRfaEvent(const Event& event)
{
    switch( event.getType() )
    {
    case MarketDataManagedActiveClientSessionPubEventEnum:
	// Client session is connecting
	processMarketDataManagedActiveClientSessionPubEvent(static_cast<const MarketDataManagedActiveClientSessionPubEvent&>(event));
	break;

    case MarketDataManagedActiveItemsPubEventEnum:
	// Client session is requesting for data.  Interface assumes a criteria-based request.
	processMarketDataManagedActiveItemsPubEvent(static_cast<const MarketDataManagedActiveItemsPubEvent&>(event));
	break;

    case MarketDataManagedInactiveItemsPubEventEnum:
	// Client session is closing data channel.  Not available for criteria-based publishers.
	break;

    case MarketDataManagedPubMirroringModeEventEnum:
	// Hot standby status.  Just informational, ignore.
	break;

    case MarketDataManagedItemContReqPubEventEnum:
	// Contribution request.  Currently not supported.
	break;

    case MarketDataManagedInactiveClientSessionPubEventEnum:
	// Client session is disconnecting
	processMarketDataManagedInactiveClientSessionPubEvent(static_cast<const 
							      MarketDataManagedInactiveClientSessionPubEvent&>(event));
	break;

    default:
	cout << "Unknown event." << endl;
	break;
    }
}

// *********************************************************************************
// processMarketDataManagedActiveClientSessionPubEvent
//
// This event occurs when a client application makes an attempt to connect to this
// managed publisher.  To accept a new connection request, the following general
// steps must occur:
//
// 1.  Accept the connection by registering the 
//     MarketDataManagedPublisherClientSessionInterestSpec
// 2.  Publish out the service information provided by this publisher
//
// See inline comments below
// ********************************************************************************
void
rmdsManagedHandler::processMarketDataManagedActiveClientSessionPubEvent( const MarketDataManagedActiveClientSessionPubEvent& event )
{
    // Retrieve the Managed Publisher associated with this request
    MarketDataManagedPublisher& publisher = (MarketDataManagedPublisher &)event.getEventSource();
    
    // Acquire the session handle used for registration
    const Handle* pCliSessionHandle = event.getClientSessionHandle();
    
    // Create MarketDataManagedPublisherClientSessionInterestSpec instance
    MarketDataManagedPublisherClientSessionInterestSpec interestSpec;
    interestSpec.setClientSessionHandle(pCliSessionHandle);
    
    // Accept the session; the Handle defines the client session
    Handle* tmpHandle = publisher.registerClient(event.getEventQueue(),
						 interestSpec,
						 *this, 0);
    
    // Define the service details
    MarketDataManagedSvcPub mDManagedSvcPub;
    MarketDataSvcStatus mMDSvcStatus;
    
    // Set the status as DOWN for now.  Application will control state.
    mMDSvcStatus.setState( MarketDataSvcStatus::NoChange );
    mMDSvcStatus.setStatusCode(MarketDataSvcStatus::None);
    mDManagedSvcPub.setStatus( mMDSvcStatus );
    
    mDManagedSvcPub.setClientSessionHandle( tmpHandle );
    mDManagedSvcPub.setServiceName( m_serviceName.c_str() );
    
    // Publish Svc status msg
    // Note, the publish ID is ignored for the exbxample application
    publisher.publish(mDManagedSvcPub);

    Logger::getLogger()->log(Logger::TYPE_INFO,
			     "rmdsManagedHandler: %s: %s, service: %s with handle: 0x%x",
			     "Accepting client connection for publisher",
			     publisher.getName().c_str(), m_serviceName.c_str(), tmpHandle);
}

//***************************************************************************
// processCriteriaRequest
// RFA supports only 1 criteria request and it has to be the first one it 
// encounters.
//***************************************************************************/
void
rmdsManagedHandler::processMarketDataManagedActiveItemsPubEvent( const MarketDataManagedActiveItemsPubEvent& event )
{
    const string& criteria = event.getCriteria();
    
    // Determine type of request
    if ( criteria.empty() )
    {
	Logger::getLogger()->log(Logger::TYPE_WARNING,
				 "rmdsManagedHandler: Received item request not criteria request.  Ignoring.");
	return;
    }

    Logger::getLogger()->log(Logger::TYPE_INFO,
			     "rmdsManagedHandler: Received RMDS request for criteria: %s, service: %s",
			     criteria.c_str(), event.getServiceName().c_str());

    // Record and reply to the criteria request
    replyCriteria(event, (m_criteriaClient == NULL));

    // Event notification the publisher is connected and available for publishing
    onConnected();
}


// Reply to the criteria request
void 
rmdsManagedHandler::replyCriteria( const MarketDataManagedActiveItemsPubEvent& criteria,
				   bool ack )
{
    MarketDataItemsReplyStatus status;
    MarketDataManagedItemsReplyPub criteriaReplyPub;
	
    criteriaReplyPub.setRequestToken(criteria.getRequestToken());
    
    // Send ACK or NAK
    if ( ack )
    {
	// Set the our publisher the criteria handle
	m_marketDataManagedItemPub.setRequestToken(criteria.getRequestToken());

	Logger::getLogger()->log(Logger::TYPE_INFO,
				 "rmdsManagedHandler::replyCriteria  Set criteria: 0x%x within our publisher",
				 criteria.getRequestToken());

	status.setState( MarketDataItemsReplyStatus::Ok );
    }
    else
    {
	status.setState( MarketDataItemsReplyStatus::Closed );
    }
    status.setStatusCode( MarketDataItemsReplyStatus::None );
    
    criteriaReplyPub.setStatus(status);
    
    // Publish criteria reply
    assert(m_pMarketDataManagedPublisher != NULL);
    m_pMarketDataManagedPublisher->publish(criteriaReplyPub);

    // Record the client handle associated with this criteria
    m_criteriaClient = criteria.getHandle();

    Logger::getLogger()->log(Logger::TYPE_INFO, "Sent CRITERIA %s for: %s", 
			     (ack ? "ack" : "nack"), criteria.getCriteria().c_str());
}


// *********************************************************************************
// processMarketDataManagedInactiveClientSessionPubEvent
//
// This event occurs when a client application drops its connection to this
// managed publisher.  To properly drop the connection, the general
// steps must occur:
//
// 1.  Unregister client
// 2.  Event notification of disconnect
//
// See inline comments below
// ********************************************************************************
void
rmdsManagedHandler::processMarketDataManagedInactiveClientSessionPubEvent( const 
									   MarketDataManagedInactiveClientSessionPubEvent& event )
{
    // Get client session handle
    Handle* clientHandle = event.getHandle();

    // Retrieve the Managed Publisher associated with this request
    MarketDataManagedPublisher& publisher = (MarketDataManagedPublisher &)event.getEventSource();
    
    // Make sure it is the client associated with the criteria before we act
    if ( clientHandle == m_criteriaClient )
    {
	m_criteriaClient = NULL;

	Logger::getLogger()->log(Logger::TYPE_INFO,
				 "rmdsManagedHandler: %s: %s (%x). Publishing DISABLED.",
				 "Request to release client connection for publisher: ",
				 publisher.getName(), clientHandle);
    }
    else
    {
	Logger::getLogger()->log(Logger::TYPE_INFO,
				 "rmdsManagedHandler: %s: %s (%x)",
				 "Request to release client connection for publisher: ",
				 publisher.getName(), clientHandle);
    }
    
    // Then, unregister client handle
    publisher.unregisterClient(*clientHandle);

    // Event notification of client disconnect (publisher unavailable for publishing)
    onDisconnected();
}

void 
rmdsManagedHandler::onConnected()
{
    if ( connected() )
	sendSvcStatus(rmdsManagedHandler::Up);
}

void
rmdsManagedHandler::onDisconnected()
{
    if ( !connected() )
	sendSvcStatus(rmdsManagedHandler::Down);
}


void 
rmdsManagedHandler::sendSvcStatus( const ServiceState state )
{
    MarketDataSvcStatus::State rmdsState;

    if ( state == rmdsManagedHandler::Up )
	rmdsState = MarketDataSvcStatus::Up;
    else
	rmdsState = MarketDataSvcStatus::Down;

    // Send service status tests
    MarketDataSvcStatus		svcStatus;
    
    // Group Status Stale
    svcStatus.setState(rmdsState);
    svcStatus.setStatusCode(MarketDataSvcStatus::None);
    m_marketDataManagedSvcPub.setStatus(svcStatus);
    
    // Publish status
    m_pMarketDataManagedPublisher->publish(m_marketDataManagedSvcPub);

    Logger::getLogger()->log(Logger::TYPE_INFO,
			     "rmdsManagedHandler::sendSvcStatus state sent: %s",
			     (state == rmdsManagedHandler::Up ? "UP" : "DOWN"));
}
