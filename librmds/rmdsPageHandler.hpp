/************************************************************************
||
|| DATE:                $Date: 2006/03/18 13:23:21 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsPageHandler.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsPageHandler.hpp,v 1.3 2006/03/18 13:23:21 nickz Exp $
|| REVISION:		$Revision: 1.3 $
|| LOG:                 $Log: rmdsPageHandler.hpp,v $
|| LOG:                 Revision 1.3  2006/03/18 13:23:21  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.2  2006/03/06 16:00:17  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.1  2006/02/28 21:04:26  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

#ifndef _RMDS_PAGE_HANDLER_HPP_
#define _RMDS_PAGE_HANDLER_HPP_

// RFA includes
#include "AnsiPage/Page.h"

// Application includes
#include "rmdsHandler.hpp"
#include "ConfigMap.hpp"
#include "DataPage.hpp"

class rmdsPageHandler: public rmdsHandler
{
public:
    rmdsPageHandler(const ConfigMap &);
    virtual ~rmdsPageHandler();
    
protected:
    // Page handlers (Pure virtual requirement)
    virtual void processImage(const rfa::sessionLayer::MarketDataItemEvent& event);
    virtual void processUpdate(const rfa::sessionLayer::MarketDataItemEvent& event);
    virtual void processStatus(const rfa::sessionLayer::MarketDataItemEvent& event);
    
private:
    void updateCache(const rfa::sessionLayer::MarketDataItemEvent& event, const rfa::common::Buffer& buffer);
    void updateCacheANSI(const rfa::sessionLayer::MarketDataItemEvent& event, const rfa::common::Buffer& buffer);
    void updateCacheMarketfeed(const rfa::sessionLayer::MarketDataItemEvent& event, const rfa::common::Buffer& buffer);
    void updatePageRow(DataPage *page, const string& rowNum, int offset, const string& buff);
    void getUpdateData(rfa::AnsiPage::Page& page, int row, int startCol, int endCol, std::string& result);
};

#endif // _RMDS_PAGE_HANDLER_HPP_
