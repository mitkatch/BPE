/************************************************************************
||
|| DATE:                $Date: 2006/04/06 17:57:48 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PBrmdsRecordHandler.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PBrmdsRecordHandler.hpp,v 1.1 2006/04/06 17:57:48 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: PBrmdsRecordHandler.hpp,v $
|| LOG:                 Revision 1.1  2006/04/06 17:57:48  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:
************************************************************************/

#ifndef _PB_RMDS_RECORD_HANDLER_HPP_
#define _PB_RMDS_RECORD_HANDLER_HPP_

// Application includes
#include "rmdsRecordHandler.hpp"

class PBrmdsRecordHandler : public rmdsRecordHandler
{
public:
    PBrmdsRecordHandler(const ConfigMap &);
    virtual ~PBrmdsRecordHandler();
    
protected:
    // Page handlers (Pure virtual requirement)
    virtual void processStatus(const rfa::sessionLayer::MarketDataItemEvent& event);
};

#endif // _PB_RMDS_RECORD_HANDLER_HPP_
