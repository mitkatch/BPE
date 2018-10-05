/************************************************************************
||
|| DATE:                $Date: 2006/06/04 11:27:07 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/ppRecordProducer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ppRecordProducer.hpp,v 1.1 2006/06/04 11:27:07 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: ppRecordProducer.hpp,v $
|| LOG:                 Revision 1.1  2006/06/04 11:27:07  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

#ifndef _PP_RECORD_PRODUCER_HPP_
#define _PP_RECORD_PRODUCER_HPP_

// System includes
#include <string>

// Application includes
#include "rmdsRecordProducer.hpp"

class PageParserApp;


class ppRecordProducer: public rmdsRecordProducer
{
public:	
    ppRecordProducer(PageParserApp* app, const string& name);
    virtual ~ppRecordProducer();
    
public:
    // Virtual requirement (Canpx Subscriber)	
    virtual void onConnected();
    
private:
    PageParserApp*	m_app;
};


#endif // _PP_RECORD_PRODUCER_HPP_

