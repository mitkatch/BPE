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

#include "ML2TConverter.hpp"
#include "Logger.hpp"

ML2TConverter::ML2TConverter(DataPage *page, DataPage* newpage, DataPage::PageSlot& slot)
{
	//copy slot from page -> newpage

	_row = slot.row1;
	_col = slot.column1;
	_rowCnt = slot.row2 - slot.row1 +1;
	for (int i = 0; i < _rowCnt; i++){
		DataPage::PageRow row = page->rowAsPageRow(_row+i);
		DataPage::PageRow &newrow = newpage->rowAsPageRow(_row+i);
		for(int j = slot.column1; (row.size() > slot.column1 + j) && j <= slot.column2; j++ ){
			newrow[j].data 	= row[j].data;
			newrow[j].bg	= row[j].bg;
			newrow[j].fg	= row[j].fg;
			newrow[j].font	= row[j].font;
			newrow[j].video	= row[j].video;
		}
		if (!newpage->addPageRow(newrow, _row+i)){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "ML2TConverter::ML2TConverter failed converted [%s] from [%s] to [%s], row %d", _value.c_str(), page->address().c_str(), newpage->address().c_str(), _row+i);	

		}
		_value = newpage->rowAsString(_row+i);		
		Logger::getLogger()->log(Logger::TYPE_INFO, "ML2TConverter::ML2TConverter converted [%s] from [%s] to [%s], row %d", _value.c_str(), page->address().c_str(), newpage->address().c_str(), _row+i);	
	}

	setDataPage(newpage);

}


ML2TConverter::ML2TConverter(DataPage *newpage, DataPage::Update& u)
{
	_row = u.row;
	_col = u.column;
	_rowCnt = 1;
	_value = u.data;
	newpage->update(u.row, u.column, u.data, u.video, u.bg, u.fg, u.font);
	//newpage->updated();
	setDataPage(newpage);

}


