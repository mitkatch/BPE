
/************************************************************************
||
|| DATE:                $Date: 2004/10/20 19:47:10 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/HtmlMap.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: HtmlMap.hpp,v 1.1 2004/10/20 19:47:10 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: HtmlMap.hpp,v $
|| LOG:                 Revision 1.1  2004/10/20 19:47:10  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef HTMLMAP_HPP
#define HTMLMAP_HPP
#include <map>
#include <string>

using std::map;
using std::string;

class HtmlMap
{
	public:
		virtual ~HtmlMap(){};

		virtual string getHtmlCode(char ch);
	
		//static
		static string toHtml(char ch);

	protected:
		static HtmlMap* _htmlMap;
		HtmlMap();
		map<char, string> _html;
};
		

#endif//HTMLMAP_HPP
