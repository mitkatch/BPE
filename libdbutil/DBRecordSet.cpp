
/************************************************************************
||
|| DATE:                $Date: 2004/08/26 16:58:12 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbutil/DBRecordSet.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBRecordSet.cpp,v 1.2 2004/08/26 16:58:12 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: DBRecordSet.cpp,v $
|| LOG:                 Revision 1.2  2004/08/26 16:58:12  mikhailt
|| LOG:                 use non-block connection
|| LOG:
|| LOG:                 Revision 1.1  2004/03/18 16:57:51  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "DBRecordIterator.hpp"

#include "DBRecordSet.hpp"

DBRecordSet::~DBRecordSet()
{
	if (_vset.size()){
		cout <<" Delete DBRecordSet with records "<<_vset.size()<<endl;
		deleteAllRecords();
	}
}

void
DBRecordSet::deleteAllRecords()
{
	vector<DBRecord *>::iterator it = _vset.begin();
	for (it; it != _vset.end(); it++){
		DBRecord *rec = (DBRecord *)(*it);
		if (rec) delete rec;
	}
	_vset.clear();
}

bool
DBRecordSet::add(DBRecord *rec)
{
	_vset.push_back(rec);
	return true;
}

DBRecord *
DBRecordSet::get(long counter)
{
	DBRecord *rec = NULL;
	if (counter >= 0 && counter < _vset.size())
		rec = _vset[counter];

	return rec;
}

long
DBRecordSet::count()
{
	return _vset.size();
}

DBRecordIterator* 
DBRecordSet::createIterator()
{
	return new DBRecordIterator(this);
}

int
DBRecordSet::getFieldEnum(const char* header)
{
	vector<string>::iterator it;
	int count =0, ctemp =0;
	for(it = _vheaders.begin(); it!= _vheaders.end(); it++){
		if ((*it).compare(header) == 0){
			count = ctemp;
			break;
		}
		ctemp++;
	}
	return count;
}
