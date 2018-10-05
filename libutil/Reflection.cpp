
/************************************************************************
||
|| DATE:                $Date: 2005/06/29 18:55:58 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/Reflection.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Reflection.cpp,v 1.2 2005/06/29 18:55:58 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: Reflection.cpp,v $
|| LOG:                 Revision 1.2  2005/06/29 18:55:58  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.1  2005/03/29 16:09:36  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/
#include "Logger.hpp" 
#include "Reflection.hpp"
#include <algorithm>
Reflection * Reflection::_reflection = NULL;

Reflection*
Reflection::getReflection()
{
	if (_reflection == NULL){
		_reflection = new Reflection();
	}
	return _reflection;

}

set<StringCheck>
Reflection::getSet(const string& var)
{
	set<StringCheck> out;
	map<string, StringCheckMap>::iterator it1 = _rMap.find(var);
	if (it1 != _rMap.end()){
		StringCheckMap& scm = (*it1).second;
		map<StringCheck, vector<StringCheck> >::iterator it2 = scm.begin();
		for (it2; it2 != scm.end(); it2++){
			out.insert((*it2).first);
		} 
	}

	return out;
}

StringCheckMap
Reflection::getMap(const string& var)
{

	StringCheckMap out;
	map<string, StringCheckMap>::iterator it1 = _rMap.find(var);
	if (it1 != _rMap.end()){
		out = (*it1).second;
	}
	return out;

}

bool 
Reflection::addSetVar(const string& var, const StringCheck& sc)
{
	StringCheck dummy("", false);
	return addMapVar(var, sc, dummy); 
	
}

bool
Reflection::addMapVar(const string& var, const StringCheck& key, const StringCheck& val)
{
	map<string, StringCheckMap>::iterator it1 = _rMap.find(var);
	Logger::getLogger()->log(Logger::TYPE_INFO, "addMapVar for %s to size %i", var.c_str(), _rMap.size());
	if (it1 != _rMap.end()){
		StringCheckMap&  sct = (*it1).second;
		StringCheckMap::iterator it2 = sct.find(key);
		if (it2 != sct.end()){
			vector<StringCheck> v = (*it2).second;
			vector<StringCheck>::iterator found = std::find(v.begin(), v.end(), val);
			if (found != v.end()){
				v.erase(found);//if it' update, replace it
			}
			v.push_back(val);
			sct.erase(it2);
			sct[key] = v; //delete old if key has been changed
			Logger::getLogger()->log(Logger::TYPE_INFO, "addMapVar added to found [%s] value [%s]", key.str1.c_str(), val.str1.c_str());
		} else {
			vector<StringCheck> v;
			v.push_back(val);
			sct[key] = v; 
			Logger::getLogger()->log(Logger::TYPE_INFO, "addMapVar added to new [%s] value [%s] size %i ", key.str1.c_str(), val.str1.c_str(), sct.size());
		} 		
	}else {
		vector<StringCheck> v;
		v.push_back(val);
		StringCheckMap sct;
		sct[key] = v; 
		_rMap[var] = sct;

	}	
	return true;

}

