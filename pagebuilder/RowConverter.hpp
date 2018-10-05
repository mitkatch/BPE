/************************************************************************
||
|| DATE:                $Date: 2006/04/06 17:57:31 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/RowConverter.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: RowConverter.hpp,v 1.3 2006/04/06 17:57:31 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: RowConverter.hpp,v $
|| LOG:                 Revision 1.3  2006/04/06 17:57:31  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:                 Revision 1.2  2005/09/21 17:49:55  mikhailt
|| LOG:                 activated only when loaded()
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ROWCONVERTER_HPP
#define ROWCONVERTER_HPP

#include "Datum.hpp"
#include "Subscriber.hpp"
#include "CellConverter.hpp"

class PageConverter;

class RowConverter: public Subscriber
{
	public:
		typedef vector<CellConverter *> VCells;
		typedef map<string, VCells> MCells;

		RowConverter(const string & source, PageConverter* pc, const string& row);
		virtual ~RowConverter();
		virtual void notify(Datum *datum);
		virtual void addCell(CellConverter *cc);
		virtual string source(){
			return _source;
		}
		
		virtual PageConverter * myPage() const{
			return _myPage;
		} 

		virtual void rowNum(int r){
			_rowNum = r;
		}

		virtual int rowNum(){
			return _rowNum;
		}
		
		virtual bool loaded(){
			return _active;
		}

	protected:
		map<string, CellConverter *> _cells;
		MCells _fieldsMap;
		string _source;
		PageConverter *_myPage;
		bool _active;
		string _row;
		int  _rowNum; //for dynamically built rowNum
};
#endif //ROWCONVERTER_HPP
