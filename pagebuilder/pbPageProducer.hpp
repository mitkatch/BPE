/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:29:51 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/pbPageProducer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: pbPageProducer.hpp,v 1.1 2006/06/08 15:29:51 nickz Exp $
|| REVISION:		$Revision: 1.1 $
|| LOG:                 $Log: pbPageProducer.hpp,v $
|| LOG:                 Revision 1.1  2006/06/08 15:29:51  nickz
|| LOG:                 ported to managed publishing
|| LOG:
************************************************************************/

#ifndef _PB_PAGE_PRODUCER_HPP_
#define _PB_PAGE_PRODUCER_HPP_

// System includes
#include <string>

// Application includes
#include "rmdsPageProducer.hpp"

class PBuilder;


class pbPageProducer: public rmdsPageProducer
{
public:	
    pbPageProducer(PBuilder* app, const string& name);
    virtual ~pbPageProducer();
    
public:
    // Virtual requirement (Canpx Subscriber)	
    virtual void onConnected();
    
private:
    PBuilder*	m_app;
};


#endif // _PB_PAGE_PRODUCER_HPP_







