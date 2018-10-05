
/************************************************************************
||
|| DATE:                $Date: 2004/10/20 19:48:24 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/DataFeed.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DataFeed.hpp,v 1.3 2004/10/20 19:48:24 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: DataFeed.hpp,v $
|| LOG:                 Revision 1.3  2004/10/20 19:48:24  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.2  2004/04/29 19:52:56  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef DATAFEED_HPP
#define DATAFEED_HPP

#include <iostream.h>
#include <string>

using std::string;

class DataFeed
{
	public:
		DataFeed(){};

		virtual ~DataFeed(){};
		virtual void setUpRequestAsync(const char * source, const char *instr) = 0;
		virtual void cancelRequestAsync(const char *address)= 0;

};
#endif //DATAFEED_HPP
