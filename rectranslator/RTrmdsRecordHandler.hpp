
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

#ifndef _RT_RMDS_RECORD_HANDLER_HPP_
#define _RT_RMDS_RECORD_HANDLER_HPP_

// Application includes
#include "rmdsRecordHandler.hpp"

class RTrmdsRecordHandler : public rmdsRecordHandler
{
public:
    RTrmdsRecordHandler(const ConfigMap &);
    virtual ~RTrmdsRecordHandler();
    
protected:
    // Page handlers (Pure virtual requirement)
    virtual void processStatus(const rfa::sessionLayer::MarketDataItemEvent& event);
};

#endif // _RT_RMDS_RECORD_HANDLER_HPP_
