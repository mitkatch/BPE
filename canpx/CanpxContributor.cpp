/************************************************************************
||
|| DATE:                $Date: 2005/09/12 21:56:39 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxContributor.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxContributor.cpp,v 1.3 2005/09/12 21:56:39 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: CanpxContributor.cpp,v $
|| LOG:                 Revision 1.3  2005/09/12 21:56:39  mikhailt
|| LOG:                 builds ldap chain
|| LOG:
|| LOG:                 Revision 1.2  2005/07/29 18:10:28  mikhailt
|| LOG:                 handle failed attempt to getList
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "CanpxContributor.hpp"
#include "DataGraph.hpp"
#include "DataCache.hpp"
#include "Reflection.hpp"
#include "ConfigMap.hpp"

CanpxContributor::CanpxContributor(TrsRecordHandler *trsHandler, const string& name, const ContribNameSpace& ns):_trsHandler(trsHandler), _configName(name),  _ns(ns), _active(false), _counter(0)
{
	string list = _ns.request; 
	_dataAddress = _ns.source + "." + list;
	_name = _ns.source;
	buildAlias();
}

CanpxContributor::~CanpxContributor()
{
	map<string, CanpxInstVector >::iterator it = _mapInst.begin();
	for (it; it != _mapInst.end(); it++){
		CanpxInstVector &v = (*it).second;
		for (int i = 0; i < v.size(); i++){
			delete v[i];
		}
	}
	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxContributor %s Deleted %i", _name.c_str(), _mapInst.size());
	for (int i = 0; i < _vDuplicated.size(); i++){
		delete _vDuplicated[i];
	}
	_vDuplicated.clear();
}

void
CanpxContributor::getList()
{
	if (_counter++ <= 2){
		addWorker(&TrsRecordHandler::getInstBlocked, this);
	}

}

bool
CanpxContributor::descardDuplications(CanpxInstrument *i)
{
	string val = stripPageOf(i->realName()); 
	map<string,CanpxInstVector >::iterator it = _mapInst.find(val);
	if (it != _mapInst.end()){
		CanpxInstVector &v = (*it).second;
		if (v.size() > 1){
			CanpxInstVector::iterator it2 = v.begin();
			while (it2 != v.end()){
				if ((*it2)->realName() != i->realName()){
					_trsHandler->cancelRequestAsync(((*it2)->realName()).c_str());
					_vDuplicated.push_back((*it2));
					(*it2)->descarded(true);

					map<CanpxInstrument *, bool>::iterator chain_it = _chainMap.find((*it2));
					if (chain_it != _chainMap.end()){
						_chainMap.erase(chain_it);
					}
					it2 = v.erase(it2);
					Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxContributor::descardDuplications descarded %s", ((*it2)->realName()).c_str());
				} else{
					it2++;
				}			
			}
		}
	}else{
		Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxContributor::deleteDuplications Instrument not found %s", (i->realName()).c_str());
	} 
	return true;
}


void
CanpxContributor::onWorker(DataGraph *dgr)
{
	if (dgr != NULL)
		notify((Datum *)dgr);
	else
		getList();// attempt failed , try again
}

void
CanpxContributor::notify(Datum *datum)
{
	if (!_active){
		_active = true;
		StringCheck out(_name, _active);
		Reflection::getReflection()->addSetVar("Reflection::contribList", out); 
	}

	DataGraph *data = dynamic_cast<DataGraph *>(datum);
	if (data){
		//cout <<"I've been notified "<<datum->address()<<endl;

		set<string> fields = data->getFieldSet();
		set<string>::iterator it = fields.begin();
		for (it; it != fields.end(); it++){
			if ((*it).find("LINK_") != string::npos){
				string realName = data->getValue(*it);
				string alias = toAlias(realName);
				string val = stripPageOf(realName);
				CanpxInstrument *i = new CanpxInstrument(_trsHandler, realName, alias, this);
				_chainMap[i] = false;
				map<string, CanpxInstVector>::iterator it2 = _mapInst.find(val);
				if (it2 != _mapInst.end()){
					CanpxInstVector &v = (*it2).second;
					v.push_back(i);
					Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxContributor::notify duplicated added Instrument %s", realName.c_str());
				}else{
					CanpxInstVector v;
					v.push_back(i);
					_mapInst[val] = v;
					Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxContributor::notify unique added Instrument %s", realName.c_str());
				} 

				DataCache::getCache()->subscribe(_name+ "." +realName, i, Datum::GRAPH);
			}
		}
	}

}

//static
string
CanpxContributor::stripPageOf(const string& rec)
{
	//input like PREBON.17671.450_0907
	//output PREBON.450_0907
	string out = rec;;
	int dot1 = rec.find_first_of('.', 0);
	if (dot1 != string::npos){
		int dot2 = rec.find_first_of('.', dot1+1);
		if (dot2 != string::npos){
			out = rec.substr(0, dot1) + rec.substr(dot2, rec.size() - dot2);
		}	

	}
	return out;
}

string
CanpxContributor::toAlias(const string& realName)
{
	string alias = realName;
	int lastDot = alias.find_last_of('.');
	if (lastDot != string::npos && (lastDot != (alias.size() -1))){
		string inst = alias.substr(lastDot +1, alias.size() - (lastDot +1));
		map<string, string>::iterator it = _aliasMap.find(inst);
		if (it != _aliasMap.end()){

			alias = alias.replace(lastDot +1, inst.size(), (*it).second);
			cout << "toAlias "<<realName<<" --> "<<alias<<endl;
		}
	}
	return alias;
}

void
CanpxContributor::buildAlias()
{
	string pref = CONTRIBUTOR + _configName + ALIAS + string("."); 
	map<string, string>::const_iterator it = ConfigMap::getConfig()->begin();
	for (it; it != ConfigMap::getConfig()->end(); it++){
		string key = (*it).first;
		int prefFind = key.find(pref);
		if (prefFind != string::npos){
			string alias = key.substr(prefFind + pref.size(), key.size() - (prefFind + pref.size())); 
			string realName = ConfigMap::getConfig()->getValue(key.c_str());
			_aliasMap[realName] = alias;
			cout <<"BuildAlias realName=["<<realName<<"], alias=["<<alias<<"]"<<endl;
		} 

	}
}


void
CanpxContributor::onTimer(TimerId id)
{
	//disactivated
	/***
	**/
}

bool
CanpxContributor::toChain(CanpxInstrument *i)
{
	bool ret = false;
	if (!i->descarded()){
		map<CanpxInstrument *, bool >::iterator it = _chainMap.find(i);
		if ( it != _chainMap.end()){
			(*it).second = ret = true;
		} else {
			Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxContributor::toChain  [%p] %s not registered for chain", i, (i->realName()).c_str());

		}
	}
	return ret;
}

bool
CanpxContributor::chainLoaded()
{
	bool ret = true;
	int reg = 0, unreg = 0;
	map<CanpxInstrument *, bool> ::iterator it = _chainMap.begin();
	for (it; it != _chainMap.end(); it++){
		ret &= (*it).second; 
		if ((*it).second)
			reg++;
		else
			unreg++;
	}
	cout << "CanpxContributor::chainLoaded() registerd "<<reg<<", unregisterd "<<unreg<<" ["<<_configName<<"]"<<endl;

	return ret;
}

