/************************************************************************
||
|| DATE:                $Date: 2006/03/28 20:11:41 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/subjectMgr.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: subjectMgr.cpp,v 1.1 2006/03/28 20:11:41 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: subjectMgr.cpp,v $
|| LOG:                 Revision 1.1  2006/03/28 20:11:41  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

// Application includes
#include "subjectMgr.hpp"
#include "Logger.hpp"
#include "DataGraph.hpp"
#include "ConfigMap.hpp"


TibrvNetTransport 	subjectMgr::s_transport;
TibrvDispatcher		subjectMgr::s_dispatcher;
pthread_mutex_t		subjectMgr::s_DataMutex;
bool			subjectMgr::s_initialized = false;
bool			subjectMgr::s_dispatched = false;


// Tibrv configuration names
#define TIBRV_SERVICE	"TIBRV_SERVICE"
#define TIBRV_NETWORK	"TIBRV_NETWORK"
#define TIBRV_DAEMON	"TIBRV_DAEMON"


subjectMgr::subjectMgr(const string& name): m_name(name)
{
}

subjectMgr::~subjectMgr()
{
}



/*****************************************************************************
 * init
 * Construct the PUBLISH and SUBSCRIBE lists as well as the request/reply
 * subject details.
 *****************************************************************************/
int
subjectMgr::init()
{
    if ( s_initialized )
	return(0);

    TibrvStatus status;
    
    ConfigMap* confMap = ConfigMap::getConfig();	
    
    string service = confMap->getValue(TIBRV_SERVICE);
    string network = confMap->getValue(TIBRV_NETWORK);
    string daemon = confMap->getValue(TIBRV_DAEMON);
    
    // open Tibrv
    status = Tibrv::open();
    if (status != TIBRV_OK)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Error: could not open TIB/RV, status%d, text=%s",
				 (int)status, status.getText());
	
	return(-1);
    }
    
    // Create network transport
    status = s_transport.create(service.c_str(), network.c_str(), daemon.c_str());
    if (status != TIBRV_OK)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Error: could not create transport, status%d, text=%s",
				 (int)status, status.getText());    
	
        Tibrv::close();
        return(-1);
    }
    s_transport.setDescription(m_name.c_str());

    s_initialized = true;

    return(0);

}

int
subjectMgr::processMsgs()
{
    pthread_mutex_lock(&s_DataMutex);

    if ( s_initialized && !s_dispatched )
    {
	// Launch thread and start dispatching Tibrv events
	s_dispatcher.create(Tibrv::defaultQueue());

	s_dispatched = true;
    }

    pthread_mutex_unlock(&s_DataMutex);

    return(0);
}


bool
subjectMgr::createListener(TibrvListener*& listener, string& subject, void* closure)
{
    if ( !subject.empty() )
    {
	// Create list listener
	listener = new TibrvListener();

	// Subscribe
	TibrvStatus status = listener->create(Tibrv::defaultQueue(), this, 
					      &s_transport, subject.c_str(), closure);
	if (status != TIBRV_OK)
	{
	    Logger::getLogger()->log(Logger::TYPE_ERROR, 
				     "Error: could not create listener on%s, status%d, text=%s",
				     subject.c_str(), (int)status, status.getText());

	    delete listener;
	    listener = NULL;

	    return(false);
	}
    }
    
    return(true);
}
