
/************************************************************************
||
|| DATE:                $Date: 2005/06/29 18:15:53 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbutil/DBConnector.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBConnector.hpp,v 1.4 2005/06/29 18:15:53 mikhailt Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: DBConnector.hpp,v $
|| LOG:                 Revision 1.4  2005/06/29 18:15:53  mikhailt
|| LOG:                 split orcl and ldap
|| LOG:
|| LOG:                 Revision 1.3  2005/06/21 15:35:21  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.2  2004/04/29 19:51:31  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/03/18 16:58:10  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef DBCONNECTOR_HPP
#define DBCONNECTOR_HPP
#include <string>
#include <iostream.h>

using std::string;

#include "Logger.hpp"

class DBRecordSet;

class DBConnector 
{
	public:
		DBConnector(string& userid, string& passwd, string& conn); 
		DBConnector(const string& userid, const string& passwd, const string& conn); 
		virtual ~DBConnector();

		virtual bool fetch(const string& sqlstmt);
		virtual bool bind(int n, string val);
		virtual DBRecordSet *execute();
		virtual void disconnect(); 

	protected:
		virtual void onInit();
		virtual void onExit();
};
#endif //DBCONNECTOR_HPP
