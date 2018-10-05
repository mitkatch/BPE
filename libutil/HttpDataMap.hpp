/************************************************************************
||
|| DATE:                $Date: 2004/12/03 18:42:56 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/HttpDataMap.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: HttpDataMap.hpp,v 1.1 2004/12/03 18:42:56 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: HttpDataMap.hpp,v $
|| LOG:                 Revision 1.1  2004/12/03 18:42:56  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef HTTPDATAMAP_HPP 
#define HTTPDATAMAP_HPP 
#include <map>
#include <string>

using std::map;
using std::string;

class HttpDataMap 
{
	public:
		virtual ~HttpDataMap(){};
	
		//static
		//static string toHttpString(char ch);
		static string toString(const string& httpStr);

		//virtual string getHttpDataCode(char ch);

		map<string, string> _httpData;
	protected:
		static HttpDataMap* _httpMap;
		HttpDataMap();

		//map <httpDataString, String> like <"%26", "&">
};
		

#endif//HTMLMAP_HPP
