
/************************************************************************
||
|| DATE:                $Date: 2004/04/29 19:51:31 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbutil/DBRecordSet.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBRecordSet.hpp,v 1.2 2004/04/29 19:51:31 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: DBRecordSet.hpp,v $
|| LOG:                 Revision 1.2  2004/04/29 19:51:31  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/03/18 16:58:10  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef DBRECORDSET_HPP
#define DBRECORDSET_HPP

#include <vector>

using std::vector;

#include "DBRecord.hpp"

class DBRecordIterator;

class DBRecordSet
{
	public:
		DBRecordSet(){};
		virtual ~DBRecordSet();

		virtual bool add(DBRecord* rec);
		virtual void deleteAllRecords();
		virtual DBRecord* get(long counter);
		virtual long count();
		virtual DBRecordIterator* createIterator();
		virtual void setHeaders(vector<string>& vheaders){
			_vheaders = vheaders;
		}
		
		virtual int getFieldEnum(const char* header);
	protected:
		vector<DBRecord *> _vset;
		vector<string> _vheaders;

};

#endif //DBRECORDSET_HPP
