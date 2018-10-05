/************************************************************************
||
|| DATE:                $Date: 2005/03/29 16:09:36 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/Converter.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Converter.hpp,v 1.2 2005/03/29 16:09:36 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: Converter.hpp,v $
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
		virtual int row() = 0;
		virtual int col() = 0;
		virtual string value() = 0;
		virtual void setDataPage(DataPage *page){
			_myDataPage = page;
		}

		virtual DataPage * getDataPage() const{
			return _myDataPage;
		}

	protected:
		DataPage *_myDataPage;
};
#endif //CONVERTER_HPP
