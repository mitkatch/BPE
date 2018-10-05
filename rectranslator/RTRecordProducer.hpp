
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

#ifndef _RT_RECORD_PRODUCER_HPP_
#define _RT_RECORD_PRODUCER_HPP_

// System includes
#include <string>

// Application includes
#include "rmdsRecordProducer.hpp"

class RTApp;


class RTRecordProducer: public rmdsRecordProducer
{
public:	
    RTRecordProducer(RTApp* app, const string& name);
    virtual ~RTRecordProducer();
    
public:
    // Virtual requirement (RT Subscriber)	
    virtual void onConnected();
    
private:
    RTApp*	m_app;
};


#endif // _RT_RECORD_PRODUCER_HPP_

