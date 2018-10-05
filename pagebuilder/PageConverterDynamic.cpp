
/************************************************************************
||
|| DATE:                $Date: 2005/09/21 17:49:55 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PageConverterDynamic.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PageConverterDynamic.cpp,v 1.1 2005/09/21 17:49:55 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: PageConverterDynamic.cpp,v $
|| LOG:                 Revision 1.1  2005/09/21 17:49:55  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "Logger.hpp"
#include "Formatter.hpp"
#include "DataCache.hpp"
#include "PageConverterDynamic.hpp"

PageConverterDynamic::PageConverterDynamic(PageArrayConverter *array, const string& ldapurl, MsgGraph* res):PageConverter(array, ldapurl)
{
	string pname;
	res->getAttributeValue("pageName@" + ldapurl, 0, pname);
	Formatter::split(pname, ",", _pagename);

}

PageConverterDynamic::~PageConverterDynamic()
{

}

void
PageConverterDynamic::load(const string &url)
{
	for (int i = 0; i < _pagename.size(); i++){
		string address = Formatter::trim(_pagename[i]);
		if (DataCache::getCache()->addDatum(address, Datum::PAGE)){
			DataPage *page = (DataPage *)DataCache::getCache()->getDatum(address);
			if (page){
				if((_myDataPageMap.insert(map<string, DataPage*>::value_type(address, page))).second ){
					Logger::getLogger()->log(Logger::TYPE_INFO, "PageConverterDynamic::load %s page added", address.c_str());	
					loadLdap(_connstr, url, PageConverter::loadPageConfig);
				} else {
					Logger::getLogger()->log(Logger::TYPE_ERROR, "PageConverterDynamic::load %s page failed", address.c_str());	
				}
			}
		}	


	}

}

