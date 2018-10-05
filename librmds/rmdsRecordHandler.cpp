/************************************************************************
||
|| DATE:                $Date: 2006/03/18 13:23:22 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsRecordHandler.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsRecordHandler.cpp,v 1.3 2006/03/18 13:23:22 nickz Exp $
|| REVISION:		$Revision: 1.3 $
|| LOG:                 $Log: rmdsRecordHandler.cpp,v $
|| LOG:                 Revision 1.3  2006/03/18 13:23:22  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.2  2006/03/06 16:00:18  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.1  2006/02/28 21:04:26  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/


// System includes
#include <string>

// Application includes
#include "rmdsRecordHandler.hpp"
#include "DataGraph.hpp"
#include "DataCache.hpp"

// RFA includes
#include "TIBMsg/TibMsg.h"


// Namespace resolution
using namespace std;
using namespace rfa::sessionLayer;
using namespace rfa::common;


rmdsRecordHandler::rmdsRecordHandler(const ConfigMap &) 
{
}

rmdsRecordHandler::~rmdsRecordHandler()
{
    Logger::getLogger()->log(Logger::TYPE_INFO, "rmdsRecordHandler Shutdown completed");
}


void rmdsRecordHandler::processImage(const MarketDataItemEvent& event)
{
    processData(event);
}

void rmdsRecordHandler::processUpdate(const MarketDataItemEvent& event)
{
    processData(event);
}

void rmdsRecordHandler::processStatus(const MarketDataItemEvent& event)
{
    const string& service = event.getServiceName();
    const string& item = event.getItemName();
    
    Logger::getLogger()->log(Logger::TYPE_INFO, "RMDS item status for service: %s, Item: %s [%s]",
			     service.c_str(), item.c_str(), 
			     event.getStatus().getStatusText().c_str());
}


void rmdsRecordHandler::processData(const MarketDataItemEvent& event)
{
    // Ensure we have a PAGE Image
    if ( event.getDataFormat() !=  MarketDataEnums::Marketfeed)
    {
	Logger::getLogger()->log(Logger::TYPE_WARNING, 
				 "Expecting RMDS record-based data for item %s (%s).  Ignoring",
				 event.getItemName().c_str(), event.getServiceName().c_str());

	return;
    }
    
    // Process Record IMAGE, get data contained in the event
    const Buffer& buffer = event.getBuffer();
    
    if ( !buffer.isEmpty() )
    {
	// Update data cache
	updateCache(event, buffer);
	
	Logger::getLogger()->log(Logger::TYPE_INFO, "rmdsRecordHandler Record Name:%s (%s)", 
				 event.getItemName().c_str(), event.getServiceName().c_str() );
    }
}



//
// updateCache
// Updates the main DataCache().
//
void rmdsRecordHandler::updateCache(const MarketDataItemEvent& event, const Buffer& buffer)
{
    string address = event.getServiceName() + "." + event.getItemName();
    
    DataGraph* data = (DataGraph *)DataCache::getCache()->getData(address, Datum::GRAPH);
    
    if ( data != NULL )
    {
	TibMsg msg;
	TibErr err = msg.UnPack((char*)buffer.c_buf(), buffer.size());
	if (err.code != TIBMSG_OK )
	{
	    Logger::getLogger()->log(Logger::TYPE_ERROR, 
				     "Error processing RMDS update for service [%s], Item [%s] %s",
				     event.getServiceName().c_str(),
				     event.getItemName().c_str(),
				     err.Print());
	    
	    return;
	}
	
	// Iterate through message
	TibField     current;
	
	for (current.First(&msg); current.status == TIBMSG_OK; current.Next())
	{
	    DataGraph::Variant value;
	    
	    switch (current.Type())
	    {
	       case TIBMSG_STRING:
	       {
		   value.type = DataGraph::SVAL;
		   value.data.sval = strdup( (char *)current.Data() );
	       }
	       break;
	       
	       case TIBMSG_INT:
	       case TIBMSG_UINT:
	       case TIBMSG_BOOLEAN:
	       case TIBMSG_IPDATA:
	       {
		   value.type = DataGraph::LVAL;
		   current.Convert(value.data.lval);
	       }
	       break;
	    
	       case TIBMSG_REAL:
	       {
		   value.type = DataGraph::DVAL;
		   current.Convert(value.data.dval);
	       }
	       break;
	       
	       case TIBMSG_OPAQUE:
	       {
		   value.type = DataGraph::SVAL;
		   current.Convert(value.data.sval, current.Size());
	       }
	       break;
	       
	       default:
		   value.type = DataGraph::NVAL;
		   break;
	    }
	    
	    // Update field
	    data->updateField(current.Name(), value);			
	}
	
	// Notify of change
	data->updated();
    }
}
