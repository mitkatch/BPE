
/************************************************************************
||
|| DATE:                $Date: 2006/04/07 19:31:50 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/ml2tfeed/RCS/ActiveDataPage.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ActiveDataPage.hpp,v 1.1 2006/04/07 19:31:50 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ActiveDataPage.hpp,v $
|| LOG:                 Revision 1.1  2006/04/07 19:31:50  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:44:25  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:42:08  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef ACTIVEDATAPAGE_HPP
#define ACTIVEDATAPAGE_HPP

#include "DataPage.hpp"

class ActiveDataPage: public DataPage
{
	public:
		ActiveDataPage(const string& address):DataPage(address){};
		virtual ~ActiveDataPage(){};

		

};


#endif //ACTIVEDATAPAGE_HPP
