
/************************************************************************
||
|| DATE:                $Date: 2004/08/26 16:58:12 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbutil/DBRecord.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBRecord.hpp,v 1.3 2004/08/26 16:58:12 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: DBRecord.hpp,v $
|| LOG:                 Revision 1.3  2004/08/26 16:58:12  mikhailt
|| LOG:                 use non-block connection
|| LOG:
|| LOG:                 Revision 1.2  2004/04/29 19:51:31  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/03/18 16:58:10  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef DBRECORD_HPP
#define DBRECORD_HPP

#include <iostream.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

class DBRecordSet;

class DBRecord
{
	public:
		DBRecord(DBRecordSet *rset);
		DBRecord(const DBRecord& rec);
		virtual ~DBRecord();
		virtual bool addVal(string& val);
		virtual string toString();
		virtual string toString(const string &delim);
		virtual void toVector(vector<string> &vec);

		virtual const char* getField(const char * header)const;

	protected:
		vector<string> _vrec;
		DBRecordSet *_myrset;
};
#endif //DBRECORD_HPP
