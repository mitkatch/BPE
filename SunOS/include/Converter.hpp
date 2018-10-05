/************************************************************************
||
|| DATE:                $Date: 2006/04/06 18:00:35 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/Converter.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Converter.hpp,v 1.3 2006/04/06 18:00:35 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: Converter.hpp,v $
|| LOG:                 Revision 1.3  2006/04/06 18:00:35  nickz
|| LOG:                 Expanded Page interface
|| LOG:
|| LOG:                 Revision 1.2  2005/03/29 16:09:36  mikhailt
|| LOG:                 DataPage interface added
|| LOG:
|| LOG:                 Revision 1.1  2005/02/17 17:51:56  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CONVERTER_HPP
#define CONVERTER_HPP
#include <string>

#include "DataPage.hpp"

using std::string;

class Converter
{
public:
  
    // Represents the start row updated/affected within the PAGE
    virtual int row() = 0;

    // Represents the start column updated/affected within the PAGE
    virtual int col() = 0;

    // Represents the number of rows updated/affected within the PAGE
    virtual int rowCnt() = 0;

    virtual string value() = 0;

    virtual void setDataPage(DataPage *page)
	{
	    _myDataPage = page;
	}
    
    virtual DataPage * getDataPage() const
	{
	    return _myDataPage;
	}
    
protected:
    DataPage *_myDataPage;

};
#endif //CONVERTER_HPP
