/************************************************************************
||
|| DATE:                $Date: 2006/05/02 15:10:15 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsHandler.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsHandler.hpp,v 1.6 2006/05/02 15:10:15 nickz Exp $
|| REVISION:			$Revision: 1.6 $
|| LOG:                 $Log: rmdsHandler.hpp,v $
|| LOG:                 Revision 1.6  2006/05/02 15:10:15  nickz
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.5  2006/04/07 16:03:51  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.4  2006/04/01 12:09:21  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.3  2006/03/18 13:23:20  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.2  2006/03/06 16:00:16  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.1  2006/02/28 21:04:25  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

#ifndef _RMDS_HANDLER_HPP_
#define _RMDS_HANDLER_HPP_

// System includes

// RFA includes
#include "Config/ConfigDatabase.h"
#include "SessionLayer/ConnectionEvent.h"
#include "SessionLayer/MarketDataSubscriber.h"
#include "SessionLayer/MarketDataDictEvent.h"
#include "SessionLayer/MarketDataItemEvent.h"
#include "SessionLayer/EntitlementsAuthenticationEvent.h"
#include "SessionLayer/MarketDataSvcEvent.h"
#include "SessionLayer/MarketDataDictSub.h"
#include "SessionLayer/MarketDataItemSub.h"
#include "SessionLayer/PubErrorEvent.h"

// Application includes
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "Runnable.hpp"
#include "DataFeed.hpp"

class rmdsHandler: public rfa::common::Client, public DataFeed, public Runnable
{
public:
    rmdsHandler();
    virtual ~rmdsHandler();
    
    virtual int init();
    
public:
    // Pure-virtual requirement (Runnable)
    virtual long Run();
    
    // Subscribe/cancel routines (Pure virtual requirement)
    virtual void setUpRequestAsync(const char* service, const char* item);
    virtual void cancelRequestAsync(const char* address);
    
protected:
    // Pure-virtual requirement (RFA event processing callback)
    void processEvent( const rfa::common::Event& event );
    
    // Specific RFA event routines
    virtual void processConnectionEvent( const rfa::sessionLayer::ConnectionEvent& event );
    virtual void processDictionaryEvent(const rfa::sessionLayer::MarketDataDictEvent& event);
    virtual void processEntitlementsEvent(const rfa::sessionLayer::EntitlementsAuthenticationEvent& event);
    virtual void processMarketDataSvcEvent(const rfa::sessionLayer::MarketDataSvcEvent& event);
    virtual void processMarketDataEvent(const rfa::sessionLayer::MarketDataItemEvent& event);
    virtual void processPubErrorEvent( const rfa::sessionLayer::PubErrorEvent& event);
    
    // Page and Record classes must override these virtuals to handle events
    virtual void processImage(const rfa::sessionLayer::MarketDataItemEvent& event) {}
    virtual void processUpdate(const rfa::sessionLayer::MarketDataItemEvent& event) {}
    virtual void processStatus(const rfa::sessionLayer::MarketDataItemEvent& event) {}

    // Other RFA event routine (usually managed within deriving classes)
    virtual void processRfaEvent(const rfa::common::Event& event);

    // Useful routines
    void getDictType(rfa::sessionLayer::DataDictInfo::DictType type, std::string& dictType);

    // Handle RFA exceptions
    void rfaException(rfa::common::Exception& e);

protected:
    // RFA details
    static rfa::sessionLayer::Session*		     	s_pSession;
    static rfa::sessionLayer::MarketDataSubscriber*  	s_pMarketDataSubscriber;
    static rfa::common::Handle*		     		s_pHandle;
    static rfa::sessionLayer::MarketDataDictSub*    	s_pMarketDataDictSub;
    static rfa::sessionLayer::MarketDataItemSub*    	s_pMarketDataItemSub;	
    static rfa::config::ConfigDatabase*	     		s_pDB;
    static rfa::config::StagingConfigDatabase*	     	s_pSDB;
    static rfa::common::EventQueue*		     	s_pEQueue;
    
    static pthread_mutex_t	s_DataMutex;

private:
    void subscribeToSystemEvents();
    void getDataDictionary();

private:
    static bool s_initialized;
    static bool s_requestedMFDataDict;
};

#endif // _RMDS_HANDLER_HPP_
