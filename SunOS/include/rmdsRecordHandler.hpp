/************************************************************************
||
|| DATE:                $Date: 2006/03/18 13:23:22 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsRecordHandler.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsRecordHandler.hpp,v 1.3 2006/03/18 13:23:22 nickz Exp $
|| REVISION:			$Revision: 1.3 $
|| LOG:                 $Log: rmdsRecordHandler.hpp,v $
|| LOG:                 Revision 1.3  2006/03/18 13:23:22  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.2  2006/03/06 16:00:18  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.1  2006/02/28 21:04:27  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

#ifndef _RMDS_RECORD_HANDLER_HPP_
#define _RMDS_RECORD_HANDLER_HPP_

// Application includes
#include "rmdsHandler.hpp"
#include "ConfigMap.hpp"

class rmdsRecordHandler : public rmdsHandler
{
	public:
		rmdsRecordHandler(const ConfigMap &);
		virtual ~rmdsRecordHandler();

	protected:
		// Page handlers (Pure virtual requirement)
		virtual void processImage(const rfa::sessionLayer::MarketDataItemEvent& event);
		virtual void processUpdate(const rfa::sessionLayer::MarketDataItemEvent& event);
		virtual void processStatus(const rfa::sessionLayer::MarketDataItemEvent& event);
	
	private:
		void processData(const rfa::sessionLayer::MarketDataItemEvent& event);
		void updateCache(const rfa::sessionLayer::MarketDataItemEvent& event, const rfa::common::Buffer& buffer);
};

#endif // _RMDS_RECORD_HANDLER_HPP_
