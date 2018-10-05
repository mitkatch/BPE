/************************************************************************
||
|| DATE:                $Date: 2006/06/07 14:29:13 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/esRecordProducer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: esRecordProducer.hpp,v 1.1 2006/06/07 14:29:13 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: esRecordProducer.hpp,v $
|| LOG:                 Revision 1.1  2006/06/07 14:29:13  nickz
|| LOG:                 Ported to managed published
|| LOG:
************************************************************************/

#ifndef _ES_RECORD_PRODUCER_HPP_
#define _ES_RECORD_PRODUCER_HPP_

// System includes
#include <string>

// Application includes
#include "rmdsRecordProducer.hpp"

class ESpeedApp;


class esRecordProducer: public rmdsRecordProducer
{
public:	
    esRecordProducer(ESpeedApp* app, const string& name);
    virtual ~esRecordProducer();
    
public:
    // Virtual requirement (Canpx Subscriber)	
    virtual void onConnected();
    
private:
    ESpeedApp*	m_app;
};


#endif // _ES_RECORD_PRODUCER_HPP_

