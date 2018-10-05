
/************************************************************************
||
|| DATE:                $Date:$
|| SOURCE:              $Source:$
|| STATE:               $State:$
|| ID:                  $Id:$
|| REVISION:            $Revision:$
|| LOG:                 $Log:$
|| LOG:
************************************************************************/

// System includes
#include <string>

// Application includes
#include "MKLrmdsRecordHandler.hpp"
#include "DataGraph.hpp"
#include "DataCache.hpp"

// RFA includes
#include "TIBMsg/TibMsg.h"


// Namespace resolution
using namespace std;
using namespace rfa::sessionLayer;
using namespace rfa::common;


MKLrmdsRecordHandler::MKLrmdsRecordHandler(const ConfigMap &configMap) : rmdsRecordHandler(configMap)
{
}

MKLrmdsRecordHandler::~MKLrmdsRecordHandler()
{
}


void MKLrmdsRecordHandler::processStatus(const MarketDataItemEvent& event)
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

    Logger::getLogger()->log(Logger::TYPE_INFO, "MKL RMDS item status for service: %s, Item: %s [%s]",
			     service.c_str(), item.c_str(), 
			     event.getStatus().getStatusText().c_str());
}

