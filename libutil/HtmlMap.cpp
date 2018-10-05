
/************************************************************************
||
|| DATE:                $Date: 2004/10/20 19:47:10 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/HtmlMap.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: HtmlMap.cpp,v 1.1 2004/10/20 19:47:10 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: HtmlMap.cpp,v $
|| LOG:                 Revision 1.1  2004/10/20 19:47:10  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "Logger.hpp"
#include "HtmlMap.hpp"

HtmlMap * HtmlMap::_htmlMap = new HtmlMap();

HtmlMap::HtmlMap()
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "Create HtmlMap::HtmlMap()");
	_html[' '] = "&nbsp;";
}

string
HtmlMap::toHtml(char ch)
{
	return _htmlMap->getHtmlCode(ch);

}

string
HtmlMap::getHtmlCode(char ch)
{
	string str;
	map<char, string>::iterator it = _html.find(ch);
	if (it != _html.end()){
		str = (*it).second;
	} else {
		str = string(1, ch);
	}

	return str;

}
