
/************************************************************************
||
|| DATE:                $Date: 2005/06/29 18:15:53 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbutil/DBConnector.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBConnector.cpp,v 1.3 2005/06/29 18:15:53 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: DBConnector.cpp,v $
|| LOG:                 Revision 1.3  2005/06/29 18:15:53  mikhailt
|| LOG:                 split orcl and ldap
|| LOG:
|| LOG:                 Revision 1.2  2005/06/21 15:35:04  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.1  2004/03/18 16:57:51  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "DBRecordSet.hpp"
#include "DBConnector.hpp"

DBConnector::DBConnector(string& userid, string& passwd, string& conn)
{

}

DBConnector::DBConnector(const string& userid, const string& passwd, const string& conn)
{

}

DBConnector::~DBConnector()
{

}

DBRecordSet *
DBConnector::execute()
{
	return NULL;	
}

bool
DBConnector::bind(int n, string val)
{
	return false;
}

bool
DBConnector::fetch(const string& sqlstmt)
{
	return false;
}
void
DBConnector::disconnect()
{

}

void
DBConnector::onInit()
{

}

void
DBConnector::onExit()
{

}

