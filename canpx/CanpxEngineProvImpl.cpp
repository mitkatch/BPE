/************************************************************************
||
|| DATE:                $Date: 2006/04/01 12:11:52 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxEngineProvImpl.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxEngineProvImpl.cpp,v 1.2 2006/04/01 12:11:52 nickz Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: CanpxEngineProvImpl.cpp,v $
|| LOG:                 Revision 1.2  2006/04/01 12:11:52  nickz
|| LOG:                 Changed to make RMDS aware.
|| LOG:
|| LOG:                 Revision 1.1  2005/10/04 16:51:28  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include <strings.h>
#include <assert.h>
#include <functional>
#include "CanpxEngineProvImpl.hpp"
#include "Logger.hpp"
#include "Reflection.hpp"
#include "Formatter.hpp"
#include "Scheduler.hpp"
#include "ConfigMap.hpp"

CanpxEngineProvImpl::CanpxEngineProvImpl()
{
	buildConverterMap();
}

CanpxEngineProvImpl::~CanpxEngineProvImpl()
{
	//clean _canpxMap

}

bool
CanpxEngineProvImpl::addImpl(CanpxInstrument *inst)
{
	string code = inst->getInstrumentCode();
	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngineProvImpl::add [%s] to [%s]", (inst->realName()).c_str(), code.c_str());
	CanpxSMap::iterator it = _canpxMap.find(code);
	bool retA = false, retB = false;;
	if (it != _canpxMap.end()){
		CanpxBidSSet* bset = (*it).second.bid;
		BestBidProv ba(inst);
		retB = (bset->insert(ba)).second;
		//cout << retB << " Added to bset size= "<<bset->size()<<endl;
		CanpxAskSSet* aset = (*it).second.ask;
		BestAskProv aa(inst);
		retA = (aset->insert(aa)).second;
		//cout << retA<< " Added to aset size= "<<aset->size()<<endl;

	} else {
		CanpxBidSSet *bidset = new CanpxBidSSet;
		BestBidProv ba(inst);
		bidset->insert(ba);

		CanpxAskSSet *askset = new CanpxAskSSet;
		BestAskProv aa(inst);
		askset->insert(aa);

		CBestPrice bprice;
		bprice.bid = bidset;
		bprice.ask = askset;
		_canpxMap[code] = bprice;;
		retA = retB = true;
	}
	return retA && retB;
}

bool
CanpxEngineProvImpl::updateImpl(CanpxInstrument *inst)
{
	bool ret = false;
	string code = inst->getInstrumentCode();
	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngineProvImpl::update %s to %s", (inst->realName()).c_str(), code.c_str());
	CanpxSMap::iterator it = _canpxMap.find(code);
	bool retA = false, retB = false;;
	if (it != _canpxMap.end()){
		CanpxBidSSet* bset = (*it).second.bid;
		CanpxBidSSet::iterator it2 = find_if(bset->begin(), 
						bset->end(), 
						FindCanpxInstrumentByRealName(inst));
		if (it2 != bset->end()){
			bset->erase(it2);
		}
		BestBidProv ba(inst);
		retB = (bset->insert(ba)).second;

		CanpxAskSSet* aset = (*it).second.ask;
		CanpxAskSSet::iterator it3 = find_if(aset->begin(), 
						aset->end(), 
						FindCanpxInstrumentByRealName(inst));
		if (it3 != aset->end()){
			aset->erase(it3);
		}
		BestAskProv aa(inst);
		retA = (aset->insert(aa)).second;
		
		ret = (retA && retB);
	} else {
		Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngineProvImpl::update not found %s to %s", inst->realName(), code.c_str());
	}

	return ret;

}

bool
CanpxEngineProvImpl::removeImpl(CanpxInstrument *inst)
{
	return false;

}

double
CanpxEngineProvImpl::bestBid(const string& code)
{
	double d = 0;
	string s = this->bidString(code, 0);
	if (!s.empty())
		d = atof(s.c_str());
	return d;
}

string
CanpxEngineProvImpl::bidString(const string& code, int level)
{
	string bid1;
	CanpxSMap::iterator it1 = _canpxMap.find(code);
	if (it1 != _canpxMap.end()){
		CanpxBidSSet* bset = (*it1).second.bid; //bid
		CanpxBidSSet::iterator it2 = bset->begin();
		int j = 0;;
		for (j, it2; it2 != bset->end(); it2++, j++){
			if (j == level){
				CanpxInstrument *i = (*it2).getInstrument();
				bid1 = i->bidString();
				if (bid1.empty()) {
					level++;
					continue;
				} else {
					Logger::getLogger()->log(Logger::TYPE_INFO, "BestBid for %s [%s] = %s", code.c_str(), (i->realName()).c_str(), bid1.c_str());
				}
			}
		}
	}

	return bid1 ;
}

double
CanpxEngineProvImpl::bestAsk(const string& code)
{
	double d = 0;
	string s = this->askString(code, 0);
	if (!s.empty())
		d = atof(s.c_str());
	return d;
}

string
CanpxEngineProvImpl::askString(const string& code, int level)
{
	string ask1;
	CanpxSMap::iterator it1 = _canpxMap.find(code);
	if (it1 != _canpxMap.end()){
		CanpxAskSSet* aset = (*it1).second.ask; //ask
		CanpxAskSSet::iterator it2 = aset->begin();
		int j = 0;;
		for (j, it2; it2 != aset->end(); it2++, j++){
			if (j == level){
				CanpxInstrument *i = (*it2).getInstrument();
				ask1 = i->askString();
				if (ask1.empty()){
					level++;
					continue;
				}else{
					Logger::getLogger()->log(Logger::TYPE_INFO, "BestAsk for %s [%s] = %s", code.c_str(), (i->realName()).c_str(), ask1.c_str());
				}
			}
		}
	}

	return ask1;
}

double
CanpxEngineProvImpl::askYield(const string& code, double bestPrice)
{
	double ask1;
	double ayield = 0;
	CanpxSMap::iterator it1 = _canpxMap.find(code);
	if (it1 != _canpxMap.end()){
		CanpxAskSSet* aset = (*it1).second.ask; //ask
		CanpxAskSSet::iterator it2 = aset->begin();
		int j = 0;;
		for (j, it2; it2 != aset->end(); it2++, j++){
			CanpxInstrument *i = (*it2).getInstrument();
			ask1 = i->ask();
			if (ask1 == bestPrice){
				ayield = i->askYield();
				break;
			}
		}
	}
	return ayield;
}

double
CanpxEngineProvImpl::aggregatedAskSize(const string& code, double bestPrice)
{
	double ask1;
	double asize = 0;
	char buf[10];
	string ssize;
	CanpxSMap::iterator it1 = _canpxMap.find(code);
	if (it1 != _canpxMap.end()){
		CanpxAskSSet* aset = (*it1).second.ask; //ask
		CanpxAskSSet::iterator it2 = aset->begin();
		int j = 0;;
		for (j, it2; it2 != aset->end(); it2++, j++){
			CanpxInstrument *i = (*it2).getInstrument();
			ask1 = i->ask();
			if (ask1 == bestPrice){
				asize += i->askSize();
			}
		}
	}
	return asize;
}

double
CanpxEngineProvImpl::bidYield(const string& code, double bestPrice)
{
	double bid1;
	double byield = 0;
	CanpxSMap::iterator it1 = _canpxMap.find(code);
	if (it1 != _canpxMap.end()){
		CanpxBidSSet* bset = (*it1).second.bid; //bid
		CanpxBidSSet::iterator it2 = bset->begin();
		int j = 0;;
		for (j, it2; it2 != bset->end(); it2++, j++){
			CanpxInstrument *i = (*it2).getInstrument();
			bid1 = i->bid();
			if (bid1 == bestPrice){
				byield = i->bidYield();
			}
		}
	}
	return byield;
}

double
CanpxEngineProvImpl::aggregatedBidSize(const string& code, double bestPrice)
{
	double bid1;
	double bsize = 0;
	char buf[10];
	string ssize;
	CanpxSMap::iterator it1 = _canpxMap.find(code);
	if (it1 != _canpxMap.end()){
		CanpxBidSSet* bset = (*it1).second.bid; //bid
		CanpxBidSSet::iterator it2 = bset->begin();
		int j = 0;;
		for (j, it2; it2 != bset->end(); it2++, j++){
			CanpxInstrument *i = (*it2).getInstrument();
			bid1 = i->bid();
			if (bid1 == bestPrice){
				bsize += i->bidSize();
			}
		}
	}
	return bsize;
}

bool
CanpxEngineProvImpl::FindCanpxInstrumentByRealName::operator()(const BestBidProv& bid)
{
	return _i->realName() == (bid.getInstrument())->realName();

}

bool
CanpxEngineProvImpl::FindCanpxInstrumentByRealName::operator()(const BestAskProv& ask)
{
	return _i->realName() == (ask.getInstrument())->realName();

}

double
CanpxEngineProvImpl::toChain(const string& address)
{
        //address CANPX_PROV.ON_07300_06
	// convert to 06_ON_07300 to order by mat
	double ret = -1;
	char ch[25];
	bzero(ch, sizeof(ch));
	char ch_tmp[127];
	bzero(ch_tmp, sizeof(ch_tmp));
        int label_ind = address.find_first_of('.');
	
        if (label_ind != string::npos){
                string tmp = address.substr(label_ind +1, address.size() - (label_ind +1));
		vector<string> v;
		string prov, cpn, mat;
		Formatter::split(tmp, "_", v);
		for (int j = 0; j < v.size(); j++){
			switch(j){
				case 0: //provincial
					prov = getConvertedAlias(v[j]);
					break;
				case 1: //coupon
					cpn = v[j];
					break;
				case 2:
					mat = v[j];
					break;
				default:
					Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxEngineProvImpl::toChain failed to split %s", address.c_str());
			}
		}
		tmp = mat + cpn + prov; //this the order !!!
		for (int j = 0; j < tmp.size(); j++){
			if (tmp[j] != '_'){
				sprintf(ch, "%d", (int)tmp[j]);
				strcat(ch_tmp, ch);
			}
		}
        }
        ret = Formatter::ATOF(ch_tmp);
	return ret;
}

void
CanpxEngineProvImpl::buildConverterMap()
{

	string pref = "PROV_NAME_CONVERTER.ALIAS.";
	map<string, string>::const_iterator it = ConfigMap::getConfig()->begin();
	for (it; it != ConfigMap::getConfig()->end(); it++){
		string key = (*it).first;
		int prefFind = key.find(pref);
		if (prefFind != string::npos){
			string alias = key.substr(prefFind + pref.size(), key.size() - (prefFind + pref.size()));
			string realNames = ConfigMap::getConfig()->getValue(key.c_str());
			vector<string> v;
			Formatter::split(realNames, ",", v);
			_provConvMap[alias] = v;
		}
	}
}

string
CanpxEngineProvImpl::getConvertedAlias(const string& prov)
{
	// read from config file and 
	// if prov size < 3, make it equal 3
	string alias = prov;
	ProvConverterMap::iterator it1 = _provConvMap.begin();
	for (it1; it1 != _provConvMap.end(); it1++){
		string a = (*it1).first;
		vector<string> &v = (*it1).second;
		for (int i = 0; i < v.size(); i++){
			if (prov == Formatter::trim(v[i])){
				alias = a;
				break;
			}
		}
	}	

	string alias3 = "000";
	for( int j = 0; j < alias.size() && j < alias3.size(); j++){
		alias3[j] = alias[j];
	} 
	return alias3;
}

