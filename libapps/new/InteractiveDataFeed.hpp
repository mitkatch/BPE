
/************************************************************************
||
|| DATE:                $Date: 2004/10/20 19:48:24 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/InteractiveDataFeed.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: InteractiveDataFeed.hpp,v 1.2 2004/10/20 19:48:24 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: InteractiveDataFeed.hpp,v $
|| LOG:                 Revision 1.2  2004/10/20 19:48:24  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef INTERACTIVEDATAFEED_HPP
#define INTERACTIVEDATAFEED_HPP

#include "DataFeed.hpp"

class InteractiveDataFeed :public DataFeed
{
	public:
		InteractiveDataFeed(){};
		virtual ~InteractiveDataFeed(){};
		
		virtual void setUpRequestAsync(const char * source, const char * instr){};
		virtual void cancelRequestAsync(const char* address){};

	protected:

};

#endif //INTERACTIVEDATAFEED_HPP
