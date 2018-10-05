/************************************************************************
||
|| DATE:                $Date: 2006/03/23 18:55:46 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsPageHandler.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsPageHandler.cpp,v 1.4 2006/03/23 18:55:46 nickz Exp $
|| REVISION:			$Revision: 1.4 $
|| LOG:                 $Log: rmdsPageHandler.cpp,v $
|| LOG:                 Revision 1.4  2006/03/23 18:55:46  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.3  2006/03/18 13:23:21  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
************************************************************************/

// System includes
#include <iostream.h>
#include <stdexcept>

// Application includes
#include "rmdsPageHandler.hpp"
#include "DataCache.hpp"
#include "Formatter.hpp"
#include "TIBMsg/TibMsg.h"

// Namespace resolution
using namespace std;
using namespace rfa::sessionLayer;
using namespace rfa::AnsiPage;
using namespace rfa::common;


rmdsPageHandler::rmdsPageHandler(const ConfigMap &) 
{
}

rmdsPageHandler::~rmdsPageHandler()
{
    Logger::getLogger()->log(Logger::TYPE_INFO, "rmdsPageHandler Shutdown completed");
}



void rmdsPageHandler::processImage(const MarketDataItemEvent& event)
{
    // Ensure we have a PAGE Image
    if ( (event.getDataFormat() !=  MarketDataEnums::ANSI_Page) && 
		(event.getDataFormat() !=  MarketDataEnums::Marketfeed))
    {
	Logger::getLogger()->log(Logger::TYPE_WARNING, 
				 "Expecting RMDS page IMAGE for item %s (%s).  Ignoring",
				 event.getItemName().c_str(), event.getServiceName().c_str());
	return;
    }
    
    string address = event.getServiceName() + "." + event.getItemName();
    
    DataPage* data = (DataPage *)DataCache::getCache()->getData(address, Datum::PAGE);

    // Our application should contain data
    if ( data != NULL )
    {
	// Process PAGE IMAGE, get data contained in the event
	const Buffer& buffer = event.getBuffer();
	
	if ( !buffer.isEmpty() )
	{
	    // Update data cache
	    updateCache(event, buffer);
	    
	    Logger::getLogger()->log(Logger::TYPE_INFO, "rmdsPageHandler Page Name IMAGE: %s (%s)", 
				     event.getItemName().c_str(), event.getServiceName().c_str() );
	    
	    // Notify publisher of change
	    data->updated();
	}
    }
}

void rmdsPageHandler::processUpdate(const MarketDataItemEvent& event)
{
    // Ensure we have a PAGE Image
    if ( (event.getDataFormat() !=  MarketDataEnums::ANSI_Page) && 
		(event.getDataFormat() !=  MarketDataEnums::Marketfeed))
    {
	Logger::getLogger()->log(Logger::TYPE_WARNING, 
				 "Expecting RMDS page UPDATE for item %s (%s).  Ignoring",
				 event.getItemName().c_str(), event.getServiceName().c_str());
	return;
    }
    
    // Process PAGE IMAGE, get data contained in the event
    const Buffer& buffer = event.getBuffer();
    
    if ( !buffer.isEmpty() )
    {
	// Update data cache
	updateCache(event, buffer);
	
	Logger::getLogger()->log(Logger::TYPE_INFO, "rmdsPageHandler Page Name UPDATE: %s (%s)", 
				 event.getItemName().c_str(), event.getServiceName().c_str() );
    }
}

void rmdsPageHandler::processStatus(const MarketDataItemEvent& event)
{
    const string& service = event.getServiceName();
    const string& item = event.getItemName();
    
    Logger::getLogger()->log(Logger::TYPE_INFO, "RMDS item status for service: %s, Item: %s [%s]",
			     service.c_str(), item.c_str(), event.getStatus().getStatusText().c_str());
}


//
// updateCache
// Updates the main DataCache().
//
void rmdsPageHandler::updateCache(const MarketDataItemEvent& event, const Buffer& buffer)
{

    int format = event.getDataFormat(); 

    switch(format){
	case MarketDataEnums::ANSI_Page:
		updateCacheANSI(event, buffer);
		break;
	case MarketDataEnums::Marketfeed:
		updateCacheMarketfeed(event, buffer);
		break;
	default:
    	    	Logger::getLogger()->log(Logger::TYPE_ERROR, "rmdsPageHandler::updateCache failed. Don't know how to process format %l", format );
    }
}

void rmdsPageHandler::updateCacheANSI(const MarketDataItemEvent& event, const Buffer& buffer)
{    
    const string& service = event.getServiceName();
    const string& item = event.getItemName();
    Page page;
    
    const long MAX_UPDATES = 1000;
    
    PageUpdate updateList[MAX_UPDATES];
    
    long numUpdates = 0;
    
    //Initialize page using the ANSI "ESC c" sequence
    const unsigned char *pDataBuffer = buffer.c_buf();

    page.decode( (char *)pDataBuffer,
		 buffer.size(),
		 updateList,
		 MAX_UPDATES,
		 &numUpdates );
    
    string address = service + "." + item;
	//cout << "rmdsPageHandler::updateCache "<< address <<endl;
    DataPage* data = (DataPage *)DataCache::getCache()->getData(address, Datum::PAGE);
    
    if ( data != NULL )
    {
	// Iterate through the number of Updates
	for (int i=0; i < numUpdates; i++)
	{
	    int row = updateList[i].getRow();
	    int col = updateList[i].getBeginningColumn();
	    
	    PageCell cell = page.getPageCell( row, col );
	    
	    string result;
	    getUpdateData(page, row, col, updateList[i].getEndingColumn()-1, result);

	    // Update data cache.
	    // Note:  The ANSI Page interface is coordinate 1-based where the Canpx data cache
	    // is zero-based.  Adjust accordingly.
	    data->update( row-1, col-1, result, page );
    	    Logger::getLogger()->log(Logger::TYPE_INFO, "rmdsPageHandler::updateCache updated %s, R:%d, C:%d Result [%s]", address.c_str(), row -1, col -1, result.c_str());
		//cout << "updated "<<address<<"R:"<< row -1 <<" C:"<<col-1<<" ["<<result<<"]"<<endl;
	}
    }
}

void rmdsPageHandler::updateCacheMarketfeed(const MarketDataItemEvent& event, const Buffer& buffer)
{    
    	const string& service = event.getServiceName();
    	const string& item = event.getItemName();
    	string address = service + "." + item;
    	DataPage* page = (DataPage *)DataCache::getCache()->getData(address, Datum::PAGE);
    	if ( page != NULL ){
		TibMsg msg;
		char buff[80];
		memset( buff, 0, sizeof( buff ) );
		TibErr err = msg.UnPack((char*)buffer.c_buf(), buffer.size());
		if (err.code == TIBMSG_OK){
			TibField current;
			for (current.First(&msg); current.status == TIBMSG_OK; current.Next()){
				switch (current.Type()){
					case TIBMSG_PARTIAL:
					case TIBMSG_STRING:
						err = current.Convert(buff, sizeof(buff));
						if (err.code == TIBMSG_OK){
							updatePageRow(page, current.Name(), current.Offset(), buff);
						}else{
    	    						Logger::getLogger()->log(Logger::TYPE_ERROR, "rmdsPageHandler::updateCacheMarketfeed update failed %s ", current.Name());
						}
						memset( buff, 0, sizeof(buff));
						break;
					default:
    	    					Logger::getLogger()->log(Logger::TYPE_INFO, "rmdsPageHandler::updateCacheMarketfeed no handler implemented %s Type: %d", current.Name(), current.Type());
				}
			}
		}
	}

}

void
rmdsPageHandler::updatePageRow(DataPage *page, const string& rowNum, int offset, const string& buff)
{
	if (rowNum.find("ROW") == 0){
		int rowInd = rowNum.find_first_of('_');
		if (rowInd != string::npos){
			string srow = rowNum.substr(rowInd +1, rowNum.size() - (rowInd +1));
			int row = Formatter::ATOF(srow.c_str());
    	    		Logger::getLogger()->log(Logger::TYPE_INFO, "rmdsPageHandler::updatePageRow update R:%d, C:%d [%s]", row-1, offset, buff.c_str() );
			page->update(row -1, offset, buff);
		}
	} 

}


void rmdsPageHandler::getUpdateData(Page& page, int row, int startCol, int endCol, string& result)
{
    result.clear();
    
    for (int i=startCol; i <= endCol; i++)
    {
	result += page.getPageCell( row, i ).getChar();
    }
}
