
/************************************************************************
||
|| DATE:                $Date: 2004/10/20 19:47:10 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/PageListener.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PageListener.hpp,v 1.1 2004/10/20 19:47:10 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: PageListener.hpp,v $
|| LOG:                 Revision 1.1  2004/10/20 19:47:10  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef PAGELISTENER_HPP
#define PAGELISTENER_HPP

#include "DataPage.hpp"

class PageListener
{
	public:
		PageListener(){};
		virtual ~PageListener(){};

		virtual void onData(DataPage *page, DataPage::PageSlot slot) = 0;

};


#endif //PAGELISTENER_HPP
