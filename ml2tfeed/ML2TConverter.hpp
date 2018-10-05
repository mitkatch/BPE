/************************************************************************
||
|| DATE:                $Date:$
|| SOURCE:              $Source:$
|| STATE:               $State:$
|| ID:                  $Id:$
|| REVISION:            $Revision:$
|| LOG:                 $Log:$
|| LOG:
************************************************************************/

#ifndef ML2TCONVERTER
#define ML2TCONVERTER
#include "DataPage.hpp"
#include "Converter.hpp"

class ML2TConverter: public Converter
{
	public:
		ML2TConverter(DataPage *page, DataPage* newpage, DataPage::PageSlot& slot);
		ML2TConverter(DataPage *newpage, DataPage::Update& update);

		virtual ~ML2TConverter(){};

		virtual int row(){
			return _row;
		}
		virtual int rowCnt(){
			return _rowCnt;
		}
		virtual int col(){
			return _col;
		}

		virtual string value(){
			return _value;
		}
	protected:
		int _row, _col, _rowCnt;
		string _value;
};


#endif //ML2TCONVERTER
