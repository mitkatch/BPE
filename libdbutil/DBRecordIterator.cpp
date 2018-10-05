
/************************************************************************
||
|| DATE:                $Date: 2004/03/18 16:57:51 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbutil/DBRecordIterator.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBRecordIterator.cpp,v 1.1 2004/03/18 16:57:51 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: DBRecordIterator.cpp,v $
|| LOG:                 Revision 1.1  2004/03/18 16:57:51  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "DBRecordIterator.hpp"

DBRecordIterator::DBRecordIterator(DBRecordSet *rset):_rset(rset), _current(0)
{
	//cout<<"Create DBRecordIterator"<<endl;
}

DBRecordIterator::~DBRecordIterator()
{
	//cout<<"Delete DBRecordIterator"<<endl;
}

DBRecord*
DBRecordIterator::getRecord() 
{
	DBRecord * currentRec = NULL;
	if (!isDone()){
		currentRec = _rset->get(_current);
	}

	return currentRec;
}

DBRecordIteratorPtr::DBRecordIteratorPtr(const DBRecordIteratorPtr& it)
{
	_i = it._i;
}

DBRecordIteratorPtr& 
DBRecordIteratorPtr::operator=(const DBRecordIteratorPtr &it)
{
	_i = it._i;
	return *this; 
}

