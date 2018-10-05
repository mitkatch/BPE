/************************************************************************
||
|| DATE:                $Date: 2006/06/04 11:34:16 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/rmdsRecordProducer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: rmdsRecordProducer.hpp,v 1.4 2006/06/04 11:34:16 nickz Exp $
|| REVISION:		$Revision: 1.4 $
|| LOG:                 $Log: rmdsRecordProducer.hpp,v $
|| LOG:                 Revision 1.4  2006/06/04 11:34:16  nickz
|| LOG:                 Changed record producer from Unmanaged to managed publisher
|| LOG:
|| LOG:                 Revision 1.3  2006/04/01 12:09:22  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.2  2006/03/18 13:23:23  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.1  2006/03/06 16:00:18  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

#ifndef _RMDS_RECORD_PRODUCER_HPP_
#define _RMDS_RECORD_PRODUCER_HPP_

// System includes
#include <string>

// RFA includes
#include "TIBMsg/TibMsg.h"

// Application includes
#include "rmdsManagedHandler.hpp"
#include "Datum.hpp"
#include "Subscriber.hpp"


class rmdsRecordProducer: public Subscriber, public rmdsManagedHandler
{
public:	
    rmdsRecordProducer(const string& name);
    virtual ~rmdsRecordProducer();
    
public:
    // Virtual requirement (Canpx Subscriber)	
    virtual void notify(Datum *datum);
    
private:
    void publish( TibMsg& msg, Datum* datum);
    
    bool tibMsg2MFbuffer( const std::string& item, TibMsg& msg, rfa::common::Buffer& buffer,
			  rfa::sessionLayer::MarketDataManagedItemPub::MarketDataMsgType type);
};


#endif // _RMDS_RECORD_PRODUCER_HPP_

