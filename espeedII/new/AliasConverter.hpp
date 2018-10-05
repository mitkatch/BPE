
/************************************************************************
||
|| DATE:                $Date: 2005/11/17 21:57:58 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/AliasConverter.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: AliasConverter.hpp,v 1.1 2005/11/17 21:57:58 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: AliasConverter.hpp,v $
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ALIASCONVERTER_HPP
#define ALIASCONVERTER_HPP

#include <map>
#include <string>
#include <set>
#include "MsgGraph.hpp"

using std::map;
using std::set;
using std::string;

class AliasConverter
{
	public:
		static AliasConverter* getConverter();
		virtual ~AliasConverter();
		virtual void load(const string& url, const string& search);
		virtual string getAlias(const string& label);
	protected:
		static AliasConverter* _myConverter;
		AliasConverter();
		void processAliases(MsgGraph *rset);
		bool addAlias(const string& aliasUrl, const string& rec);
		map<string, string> _aliasMap;	

};

#endif //ALIASCONVERTER_HPP
