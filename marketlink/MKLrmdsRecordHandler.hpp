
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

#ifndef _MKL_RMDS_RECORD_HANDLER_HPP_
#define _MKL_RMDS_RECORD_HANDLER_HPP_

// Application includes
#include "rmdsRecordHandler.hpp"

class MKLrmdsRecordHandler : public rmdsRecordHandler
{
	public:
    		MKLrmdsRecordHandler(const ConfigMap &);
    		virtual ~MKLrmdsRecordHandler();
    
	protected:
    		// Page handlers (Pure virtual requirement)
    		virtual void processStatus(const rfa::sessionLayer::MarketDataItemEvent& event);
};

#endif // _MKL_RMDS_RECORD_HANDLER_HPP_
