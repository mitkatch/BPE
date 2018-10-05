
/************************************************************************
||
|| DATE:                $Date: 2004/03/18 16:58:10 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbutil/DBRecordIterator.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBRecordIterator.hpp,v 1.1 2004/03/18 16:58:10 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: DBRecordIterator.hpp,v $
|| LOG:                 Revision 1.1  2004/03/18 16:58:10  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef DBRECORDITERATOR_HPP
#define DBRECORDITERATOR_HPP

#include "DBRecord.hpp"
#include "DBRecordSet.hpp"

class DBRecordIterator
{
	public:
		DBRecordIterator(DBRecordSet *rset);
		virtual ~DBRecordIterator();

		virtual DBRecord* getRecord();

		virtual void next(){ _current++;};
		virtual void first(){_current = 0;};
		virtual bool isDone() {
			return _current >= _rset->count();
		}

	private:
		DBRecordSet * _rset;
		long _current;

};

class DBRecordIteratorPtr
{
	public:
		DBRecordIteratorPtr(DBRecordIterator * i): _i(i) {};
		virtual ~DBRecordIteratorPtr(){delete _i;};
		DBRecordIterator* operator->() {return _i; }
		DBRecordIterator& operator*() {return * _i; }
	private:
		DBRecordIteratorPtr (const DBRecordIteratorPtr&);
		DBRecordIteratorPtr& operator=(const DBRecordIteratorPtr &);

	private:
		DBRecordIterator * _i;


};



#endif //DBRECORDITERATOR_HPP
