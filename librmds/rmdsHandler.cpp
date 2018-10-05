//
//|---------------------------------------------------------------
//|            Reuters Canada - Toronto, Ontario                --
//| All rights reserved. Duplication or distribution prohibited --
//|---------------------------------------------------------------
//
//||
//|| DATE:		$Date: 2006/05/02 15:10:14 $
//|| SOURCE:		$Source: /repository/projects/Canpx/librmds/rmdsHandler.cpp,v $
//|| STATE		$State: Exp $
//|| ID:		$Id: rmdsHandler.cpp,v 1.6 2006/05/02 15:10:14 nickz Exp $
//|| REVISION:		$Revision: 1.6 $
//|| LOG:		$Log: rmdsHandler.cpp,v $
//|| LOG:		Revision 1.6  2006/05/02 15:10:14  nickz
//|| LOG:		testing
//|| LOG:		
//|| LOG:		Revision 1.5  2006/04/07 16:03:51  nickz
//|| LOG:		testing...
//|| LOG:		
//|| LOG:		Revision 1.4  2006/04/01 12:09:21  nickz
//|| LOG:		continue building and testing...
//|| LOG:		
//|| LOG:		Revision 1.3  2006/03/23 18:55:46  nickz
//|| LOG:		testing...
//|| LOG:		
//|| LOG:		Revision 1.2  2006/03/18 13:23:20  nickz
//|| LOG:		Added RMDS-based functionality
//|| LOG:		
//|| LOG:		Revision 1.1  2006/02/28 21:04:25  nickz
//|| LOG:		.
//|| LOG:
//

// System includes
#include <assert.h>
#include <iostream>
#include <sstream>

// RFA
#include "Common/Context.h"
#include "Common/Exception.h"
#include "Config/ConfigTree.h"
#include "Config/ConfigString.h"
#include "Config/StagingConfigDatabase.h"
#include "SessionLayer/Session.h"
#include "SessionLayer/MarketDataSubscriberInterestSpec.h"
#include "SessionLayer/DataDictInfo.h"
#include "SessionLayer/MarketDataUnmanagedItemPub.h"
#include "TIBMsg/TibMsg.h"

// Application
#include "rmdsHandler.hpp"
#include "ConfigMap.hpp"
#include "CreateThread.hpp"

// Namespace resolution
using namespace std;
using namespace rfa::sessionLayer;
using namespace rfa::common;
using namespace rfa::config;


// Static initialization
Session*		rmdsHandler::s_pSession = NULL;
MarketDataSubscriber*  	rmdsHandler::s_pMarketDataSubscriber = NULL;
Handle*		     	rmdsHandler::s_pHandle = NULL;
MarketDataDictSub*    	rmdsHandler::s_pMarketDataDictSub = NULL;
MarketDataItemSub*    	rmdsHandler::s_pMarketDataItemSub = NULL;
ConfigDatabase*	     	rmdsHandler::s_pDB = NULL;
StagingConfigDatabase*	rmdsHandler::s_pSDB = NULL;
EventQueue*		rmdsHandler::s_pEQueue = NULL;
bool 			rmdsHandler::s_initialized = false;
bool 			rmdsHandler::s_requestedMFDataDict = false;
pthread_mutex_t		rmdsHandler::s_DataMutex;


rmdsHandler::rmdsHandler()
{
}

rmdsHandler::~rmdsHandler()
{
	if (s_pSession)
	    s_pSession->release();
}

// In the constructor, create the transport. Only do this once, however.
int rmdsHandler::init()					  
{
    if ( s_initialized )
	return(0);

    s_pMarketDataDictSub = new MarketDataDictSub();
    assert(s_pMarketDataDictSub);	

    ConfigMap* config = ConfigMap::getConfig();
    
    string configFile = config->getValue("RFA_CONFIG_FILE");
    string sessionName = config->getValue("RFA_UNMANAGED_SESSION");
    
    //***************************************************************
    //	RFA Initialization
    //***************************************************************
    try
    {
	// Initialize the RFA context
	if (!rfa::common::Context::initialize()) 
	{	
	    Logger::getLogger()->log(Logger::TYPE_ERROR, "Can not initialize RFA Context ");
	    return(-1);
	}
	
	// Populate the ConfigDatabase
	s_pDB = rfa::config::ConfigDatabase::acquire("RFA");
	assert(s_pDB);
	
	s_pSDB = rfa::config::StagingConfigDatabase::create();
	assert(s_pSDB);
	
	s_pSDB->load(rfa::config::flatFile, configFile.data());
	s_pDB->merge(*s_pSDB);
	
	// create event queue which will be shared to receive
	// all events across this application
	s_pEQueue = rfa::common::EventQueue::create("Queue1");
	assert(s_pEQueue);
	
	// ***************** Session *******************************
	
	// Acquire the session, first determine if we must entitle
	string userid = config->getValue("RMDS_DACS_USER_ID");
	
	if ( !userid.empty() )
	{
	    StandardPrincipalIdentity identity;
	    
	    identity.setName(userid.c_str());
	    identity.setAppName("256");
	    identity.setPosition("0/0");
	    
	    s_pSession = Session::acquire(sessionName.data(), &identity);
	}
	else
	{
	    s_pSession = Session::acquire(sessionName.data());
	}
	
	if (s_pSession == NULL)
	{
	    Logger::getLogger()->log(Logger::TYPE_ERROR, 
				     "Error Acquiring RFA session.  Refer to RFA logs ");
	    return(-1);				       		       
	}
    }
    
    catch (rfa::common::Exception& e)
    {
	if ( s_pSDB != NULL )
	    s_pSDB->destroy();
	
	if ( s_pDB != NULL )
	    s_pDB->release();
	
	if ( s_pEQueue != NULL )
	    s_pEQueue->destroy();
	
	// Format and log the RFA exception
	rfaException(e);
	
	return(-1);
    }
    
    // Subscriptions
    subscribeToSystemEvents();
    
    return(0);
}

void rmdsHandler::subscribeToSystemEvents()
{
    if ( s_initialized )
	return;

    // Our first step is to create a subscriber for our system events
    s_pMarketDataSubscriber = s_pSession->createMarketDataSubscriber("RFAsystemEvents" );
    assert( s_pMarketDataSubscriber );
    
    // Next, we need to decide what types of events we are interested in.
    // In this case, we are interested in all system-related events.
    MarketDataSubscriberInterestSpec marketDataSubscriberInterestSpec(true);
    
    // To complete system subscription, we must register with our subscriber
    // 1.  What queue the events will sit on, 
    // 2.  What events we are interested in 
    // 3.  Where and how we want to process these events
    s_pHandle = s_pMarketDataSubscriber->registerClient( *s_pEQueue, 
							 marketDataSubscriberInterestSpec,
							 *this, NULL );
    assert( s_pHandle );
    
    s_pMarketDataItemSub = new MarketDataItemSub();
    assert( s_pMarketDataItemSub );
    
    // Launch thread
    CreateThread(this);	

    s_initialized = true;
}

void
rmdsHandler::setUpRequestAsync(const char* service, const char* item)
{
    if (service == NULL || item == NULL)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "rmdsHandler::setUpRequestAsync (service || instr) == NULL");
	return;
    }
    
    // Subscribe to service/item
    s_pMarketDataItemSub->setServiceName(service);
    s_pMarketDataItemSub->setItemName(item);
    
    s_pMarketDataSubscriber->subscribe( *s_pEQueue, *s_pMarketDataItemSub, 
					*this, NULL );

    Logger::getLogger()->log(Logger::TYPE_INFO, "Subscribed to RMDS item: %s (%s)",
			     item, service);
}

void
rmdsHandler::cancelRequestAsync(const char *address)
{
    // Cancel request
    // TBD - currently not a requirement
}

//*******************************************************************************
// run
//
// Main RFA processing loop.
// All RFA events are dispatched here.  
//*******************************************************************************
long rmdsHandler::Run()
{
    long timeoutInMilliSecs = 5000;
    
    do
    {
	// Dispatch (returns # of dispatched events if present)
	long dispatchReturn;
	
	switch(dispatchReturn = s_pEQueue->dispatch(timeoutInMilliSecs))
	{
	case Dispatchable::NothingDispatched:
	    // Nothing dispatched. Timeout occurred.
	    break;
	    
	case Dispatchable::NothingDispatchedInActive:
	    // Nothing dispatched - Inactive. The EventQueue has been deactivated.
	    break;
	    
	case Dispatchable::NothingDispatchedPartOfGroup:
	    // Nothing dispatched - PartOfGroup. The EventQueue is part of an 
	    // EventQueueGroup and should not be used directly.
	    break;
	    
	case Dispatchable::NothingDispatchedNoActiveEventStreams:
	    // Nothing dispatched - NoActiveEventStreams. The EventQueue does 
	    // not have any open Event Streams.
	    break;
	    
	default:
	    // Event dispatched. # of pending Events: dispatchReturn
	    break;
	}
    } while ( true );
    
    return(0);
}


//*******************************************************************************
// ProcessEvent
//
// Main RFA callback.
// All RFA registered events will be processed here.  
//*******************************************************************************
void rmdsHandler::processEvent(const Event& event )
{
    switch (event.getType())
    {
    case ConnectionEventEnum:
	// Connection event.
	processConnectionEvent(static_cast<const ConnectionEvent&>(event));
	break;
	
    case MarketDataDictEventEnum:
	// Dictionary event.
	processDictionaryEvent(static_cast<const MarketDataDictEvent&>(event));
	break;
	
    case rfa::sessionLayer::MarketDataItemEventEnum:
	// Market data event.
	processMarketDataEvent(static_cast<const MarketDataItemEvent&>(event));
	break;
	
    case EntitlementsAuthenticationEventEnum:
	// Entitlements event.
	processEntitlementsEvent(static_cast<const EntitlementsAuthenticationEvent&>(event));
	break;
	
    case MarketDataSvcEventEnum:
	// Market Data Service event.
	processMarketDataSvcEvent( static_cast<const MarketDataSvcEvent&>(event));
	break;

    case PubErrorEventEnum:
	processPubErrorEvent( static_cast<const PubErrorEvent&>(event));
	break;
	
    default:
	processRfaEvent(event);
	break;
    }
}

void
rmdsHandler::processRfaEvent(const Event& event)
{
    Logger::getLogger()->log(Logger::TYPE_WARNING, 
			     "rmdsHander::processEvent: unhandled event: %d",
			     event.getType());
}


//
// processConnectionEvent
//
void 
rmdsHandler::processConnectionEvent( const ConnectionEvent& event )
{
    ostringstream stream;
    stream << "Received Connection event for: " << event.getConnectionName().c_str()
	   << ". EventSource: " << event.getEventSource().getName().c_str()
	   << ".  State: ";

    switch (event.getStatus().getState())
    {
    case ConnectionStatus::Up:
	stream << "UP";
    
	// RMDS connectivity, fetch the data dictionaries required for message
	// extraction.
	getDataDictionary();
	break;
	
    case ConnectionStatus::Down:
	stream << "DOWN";
	break;
	
    default:
	stream << "UNKNOWN (" << event.getStatus().getState() << ")";
	break;
    }
    
    stream << ".  Status: ";
    
    switch (event.getStatus().getStatusCode())
    {
    case ConnectionStatus::None:
	break;
	
    case ConnectionStatus::AcessDenied:
	stream << "ACCESS DENIED ";
	break;
	
    case ConnectionStatus::IntermittentProblems:
	stream << "INTERMITTENT PROBLEMS ";
	break;
	
    default:
	stream << "UNKNOWN (" << event.getStatus().getStatusCode() << ")";
	break;
    }

    stream << "(" << event.getStatus().getStatusText() << ")" << endl << ends;

    Logger::getLogger()->log(Logger::TYPE_INFO, "%s", stream.str().c_str());
}


void rmdsHandler::processDictionaryEvent(const MarketDataDictEvent& event)
{
    string	msg;
    string	dictType;

    // Retrieve the Type of the Dictionary
    getDictType(event.getDictInfo().getDictType(), dictType);
    
    switch ( event.getStatus().getState() )
    {
    case MarketDataDictStatus::Failure:
	Logger::getLogger()->log(Logger::TYPE_INFO, 
				 "The RMDS (%s) data dictionary not available. %s",
				 dictType.data(), event.getStatus().getStatusText());
	
	getDataDictionary();
	return;
	
    case MarketDataDictStatus::NoChange:
	// Timeout
	return;
	
    default:
	// Ok
	Logger::getLogger()->log(Logger::TYPE_INFO, 
				 "Successfully received RMDS (%s) data dictionary from server.",
				 dictType.data());
	break;
    }
    
    TibErr err;

    assert( event.getBuffer().c_buf() != NULL );
    
    TibMsg tibmsg;
    err = tibmsg.UnPack( ( char *)event.getBuffer().c_buf() );
    if (err.code != TIBMSG_OK)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, "Tib error unpacking data dictionary: %s", 
				 err.Print());
    }
    else 
    {
	err = TibMsg::UnPackMfeedDictionary( &tibmsg );
	
	if (err.code && err.code != 13)
	{
	    Logger::getLogger()->log(Logger::TYPE_ERROR, 
				     "Tib error unpacking MFeed data dictionary: %s",
				     err.Print());
	}	
    }
}




void rmdsHandler::processEntitlementsEvent(const EntitlementsAuthenticationEvent& event)
{
    StandardPrincipalIdentity& identity = (StandardPrincipalIdentity &)event.getPrincipalIdentity();
    
    Logger::getLogger()->log(Logger::TYPE_INFO, 
			     "Entitlements Authentication Event Status Text: %s. Identity: %s+%s+%s",
			     event.getStatus().getStatusText().data(),
			     identity.getName().data(),
			     identity.getAppName().data(),
			     identity.getPosition().data());
}

void rmdsHandler::processMarketDataSvcEvent(const MarketDataSvcEvent& event)
{
    string state;

    switch (event.getStatus().getState())
    {
    case MarketDataSvcStatus::Up:
	state = "Up";
	break;
	
    case MarketDataSvcStatus::Down:
	state = "Down";
	break;
	
    case MarketDataSvcStatus::NoChange:
	state = "No Change";
	break;
	
    default:
	state = "Unknown";
	break;
    }
    
    string dictionaries;
    string dictType;

    vector<DataDictInfo> dict = event.getDictInfo();
    vector<DataDictInfo>::iterator itr;
    for (itr=dict.begin(); itr != dict.end(); itr++)
    {
	getDictType( (*itr).getDictType(), dictType );
	dictionaries += " " + dictType;
    }
    
    Logger::getLogger()->log(Logger::TYPE_INFO, 
			     "RMDS Service Event.  ServiceName: [%s], Dictionary: [%s], Status: [%s: %s]",
			     event.getServiceName().data(),
			     dictionaries.c_str(),
			     state.data(),
			     event.getStatus().getStatusText().data());
}

void rmdsHandler::processMarketDataEvent(const MarketDataItemEvent& event)
{
    // Determine type of event
    switch (event.getMarketDataMsgType())
    {
    case MarketDataItemEvent::Update:
    case MarketDataItemEvent::Correction:
    case MarketDataItemEvent::ClosingRun:		
	processUpdate(event);
	break;
	
    case MarketDataItemEvent::Image:
    case MarketDataItemEvent::UnsolicitedImage:
	processImage(event);
	break;
	
    default:
	processStatus(event);
	break;
    }	
    
    return;
}


void
rmdsHandler::processPubErrorEvent( const PubErrorEvent& event )
{
    MarketDataUnmanagedItemPub pub = (MarketDataUnmanagedItemPub &)event.getPublication();

    Logger::getLogger()->log(Logger::TYPE_WARNING,
			     "rmdsHandler::PubErrorEvent: Id: %d for subject: %s (%s)",
			     event.getPubID(), pub.getSubject().c_str(),
			     event.getStatus().getStatusText().c_str());
}


void rmdsHandler::getDataDictionary()
{
    if ( !s_requestedMFDataDict )
    {
	DataDictInfo ddi;
	ddi.setDictType( DataDictInfo::Marketfeed );
	
	s_pMarketDataDictSub->setDictInfo( ddi );
	s_pMarketDataSubscriber->subscribe( *s_pEQueue, *s_pMarketDataDictSub, *this, NULL);
	s_requestedMFDataDict = true;
	
	Logger::getLogger()->log(Logger::TYPE_INFO, 
				 "Requesting for RMDS MarketFeed data dictionary.");
    }
}

void
rmdsHandler::getDictType(DataDictInfo::DictType type, string& dictType)
{
    switch ( type )
    {
    case DataDictInfo::Marketfeed:
	dictType = "MarketFeed";
	break;
	
    case DataDictInfo::Sass:
	dictType = "QForm (Sass)";
	break;
	
    case DataDictInfo::IForm:
	dictType = "IForm";
	break;
	
    default:
	dictType = "Unknown";
	break;
    }
}


void rmdsHandler::rfaException(rfa::common::Exception& e)
{
    // Determine exception Severity
    int exceptionSeverity = e.getServerity();
    string excpSeverityStr;
    switch(exceptionSeverity)
    {
    case rfa::common::Exception::Error:
	excpSeverityStr = "Error";
	break;
    case rfa::common::Exception::Warning:
	excpSeverityStr = "Warning";
	break;
    case rfa::common::Exception::Information:
	excpSeverityStr = "Information";
	break;
    default:
	excpSeverityStr = "UNKNOWN Severity";
	break;
    }
    
    // Determine exception Classification
    int exceptionClassification = e.getClassification();
    string excpClassificationStr;
    switch(exceptionClassification)
    {
    case rfa::common::Exception::Anticipated:
	excpClassificationStr = "Anticipated";
	break;
    case rfa::common::Exception::Internal:
	excpClassificationStr = "Internal";
	break;
    case rfa::common::Exception::External:
	excpClassificationStr = "External";
	break;
    case rfa::common::Exception::IncorrectAPIUsage:
	excpClassificationStr = "IncorrectAPIUsage";
	break;
    case rfa::common::Exception::ConfigurationError:
	excpClassificationStr = "ConfigurationError";
	break;
    default:
	excpClassificationStr = "UNKNOWN Classification";
	break;
    }
    
    // Determine exception Type
    //rfa::common::Exception::CommonErrorType exceptionType;
    int exceptionType = e.getErrorType();
    string excpTypeStr;
    string excpStatusText;
    string excpDetails;
    
    switch(exceptionType)
    {
    case rfa::common::Exception::InvalidUsageException:
    {
	excpTypeStr = "InvalidUsageException";
	InvalidUsageException& actualException = static_cast<InvalidUsageException&>(e);
	excpStatusText = actualException.getStatus().getStatusText();
	break;
    }
    case rfa::common::Exception::InvalidConfigurationException:
    {
	excpTypeStr = "InvalidConfigurationException";
	InvalidConfigurationException& actualException = static_cast<InvalidConfigurationException&>(e);
	excpStatusText = actualException.getStatus().getStatusText();
	excpDetails = actualException.getParameterName() + " " + actualException.getParameterValue();
	break;
    }
#ifdef WIN32
    case rfa::common::Exception::SystemException:
    {
	excpTypeStr = "SystemException";
	SystemException& actualException = static_cast<SystemException&>(e);
	excpStatusText = actualException.getStatus().getStatusText();
	break;
    }
#endif
    default:
    {
	excpStatusText = "Unknown Exception Type!";
	break;
    }
    }
    
    // output exception information
    string tmpstr;
    
    tmpstr.append("\n\tException Type: ");
    tmpstr.append(excpTypeStr);
    tmpstr.append("\n\t");
    tmpstr.append("Exception Severity: ");
    tmpstr.append(excpSeverityStr);
    tmpstr.append("\n\t");
    tmpstr.append("Exception Classification: ");
    tmpstr.append(excpClassificationStr);
    tmpstr.append("\n\t");
    tmpstr.append("Exception Status Text: ");
    tmpstr.append(excpStatusText);
    tmpstr.append("\n\t");
    if (!excpDetails.empty())
    {
	tmpstr.append(excpDetails);
	tmpstr.append("\n");
    }
    
    Logger::getLogger()->log(Logger::TYPE_ERROR, tmpstr.c_str());
}

