/************************************************************************
||
|| DATE:                $Date: 2006/06/05 13:18:39 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/canpxRecordProducer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: canpxRecordProducer.hpp,v 1.1 2006/06/05 13:18:39 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: canpxRecordProducer.hpp,v $
|| LOG:                 Revision 1.1  2006/06/05 13:18:39  nickz
|| LOG:                 Ported record publisher as managed
|| LOG:
|| LOG:
************************************************************************/

#ifndef _CANPX_RECORD_PRODUCER_HPP_
#define _CANPX_RECORD_PRODUCER_HPP_

// System includes
#include <string>

// Application includes
#include "rmdsRecordProducer.hpp"

class CanpxApp;


class canpxRecordProducer: public rmdsRecordProducer
{
public:	
    canpxRecordProducer(CanpxApp* app, const string& name);
    virtual ~canpxRecordProducer();
    
public:
    // Virtual requirement (Canpx Subscriber)	
    virtual void onConnected();
    
private:
    CanpxApp*	m_app;
};


#endif // _CANPX_RECORD_PRODUCER_HPP_

