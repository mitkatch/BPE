/************************************************************************
||
|| DATE:                $Date: 2006/04/06 17:57:47 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PBrmdsRecordHandler.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PBrmdsRecordHandler.cpp,v 1.1 2006/04/06 17:57:47 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: PBrmdsRecordHandler.cpp,v $
|| LOG:                 Revision 1.1  2006/04/06 17:57:47  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:
************************************************************************/


// System includes
#include <string>

// Application includes
#include "PBrmdsRecordHandler.hpp"
#include "DataGraph.hpp"
#include "DataCache.hpp"

// RFA includes
#include "TIBMsg/TibMsg.h"


// Namespace resolution
using namespace std;
using namespace rfa::sessionLayer;
using namespace rfa::common;


PBrmdsRecordHandler::PBrmdsRecordHandler(const ConfigMap &configMap) : rmdsRecordHandler(configMap)
{
}

PBrmdsRecordHandler::~PBrmdsRecordHandler()
{
}


void PBrmdsRecordHandler::processStatus(const MarketDataItemEvent& event)
{
    const string& service = event.getServiceName();
    const string& item = event.getItemName();

    string address = event.getServiceName() + "." + event.getItemName();
    
    DataGraph* data = (DataGraph *)DataCache::getCache()->getData(address, Datum::GRAPH);

    if ( data != NULL )
    {
	// Notify of change
	data->updated();
    }

    Logger::getLogger()->log(Logger::TYPE_INFO, "PB RMDS item status for service: %s, Item: %s [%s]",
			     service.c_str(), item.c_str(), 
			     event.getStatus().getStatusText().c_str());
}

