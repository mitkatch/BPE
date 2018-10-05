/************************************************************************
||
|| DATE:                $Date: 2006/06/04 11:34:14 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsManagedHandler.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsManagedHandler.hpp,v 1.1 2006/06/04 11:34:14 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: rmdsManagedHandler.hpp,v $
|| LOG:                 Revision 1.1  2006/06/04 11:34:14  nickz
|| LOG:                 Changed record producer from Unmanaged to managed publisher
|| LOG:
|| LOG:
************************************************************************/

#ifndef _RMDS_MANAGED_HANDLER_HPP_
#define _RMDS_MANAGED_HANDLER_HPP_

// System includes
#include <string>

// RFA includes
#include "SessionLayer/MarketDataManagedPublisher.h"
#include "SessionLayer/MarketDataManagedItemPub.h"
#include "SessionLayer/MarketDataManagedSvcPub.h"
#include "TIBMsg/TibMsg.h"

// Application includes
#include "rmdsHandler.hpp"


class rmdsManagedHandler: public rmdsHandler
{
public:	
    rmdsManagedHandler(const string& name);
    virtual ~rmdsManagedHandler();
    
public:
     enum ServiceState
     {
	 Up	= 1,
	 Down	= 2
     };

public:
    virtual int init();

    // Does this managed publisher have a connected client?
    bool connected() { return( m_criteriaClient != NULL ); }

    // Name of the service produced by this managed pubisher
    std::string service() { return(m_serviceName); }

    // Notify RMDS the state of the publishing service (UP or DOWN)
    void sendSvcStatus(const ServiceState state);

public:
    // Notification that the Managed publisher is connected (available for publishing)
    virtual void onConnected();

    // Notification that the Managed publisher is disconnected (unavailable for publishing)
    virtual void onDisconnected();

protected:
    // Handle RFA managed events
    virtual void processRfaEvent(const rfa::common::Event& event);

    std::string		m_name;

    void processMarketDataManagedActiveClientSessionPubEvent(const 
				     rfa::sessionLayer::MarketDataManagedActiveClientSessionPubEvent& event);
    void processMarketDataManagedActiveItemsPubEvent( const 
						      rfa::sessionLayer::MarketDataManagedActiveItemsPubEvent& event );
    void processMarketDataManagedInactiveClientSessionPubEvent(const 
			             rfa::sessionLayer::MarketDataManagedInactiveClientSessionPubEvent& event);
    void replyCriteria( const rfa::sessionLayer::MarketDataManagedActiveItemsPubEvent& criteria, bool ack );

protected:
    rfa::sessionLayer::MarketDataManagedPublisher*	m_pMarketDataManagedPublisher;
    rfa::sessionLayer::MarketDataManagedItemPub		m_marketDataManagedItemPub;
    rfa::sessionLayer::MarketDataManagedSvcPub		m_marketDataManagedSvcPub;

    rfa::sessionLayer::Session*		     		m_pSession;
    rfa::common::Handle*				m_criteriaClient;

    // Publishing service name
    std::string			m_serviceName;
};


#endif // _RMDS_MANAGED_HANDLER_HPP_

