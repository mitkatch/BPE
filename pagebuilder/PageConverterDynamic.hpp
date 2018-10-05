
/************************************************************************
||
|| DATE:                $Date: 2005/09/21 17:49:55 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PageConverterDynamic.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PageConverterDynamic.hpp,v 1.1 2005/09/21 17:49:55 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: PageConverterDynamic.hpp,v $
|| LOG:                 Revision 1.1  2005/09/21 17:49:55  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef PAGECONVERTERDYNAMIC_HPP
#define PAGECONVERTERDYNAMIC_HPP

#include <vector>
#include <map>
#include "MsgGraph.hpp"
#include "DataPage.hpp"
#include "PageConverter.hpp"

using std::vector;
using std::map;

class PageConverterDynamic: public PageConverter
{
	public:
		PageConverterDynamic(PageArrayConverter *array, const string& ldapurl, MsgGraph* res);
		virtual ~PageConverterDynamic();
		virtual void load(const string& url);



	protected:

	private:

		vector<string> _pagename;
		map<string, DataPage *> _myDataPageMap;
};

#endif //PAGECONVERTERDYNAMIC_HPP
