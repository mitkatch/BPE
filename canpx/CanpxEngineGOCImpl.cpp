
/************************************************************************
||
|| DATE:                $Date: 2005/09/13 20:10:55 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxEngineGOCImpl.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxEngineGOCImpl.cpp,v 1.5 2005/09/13 20:10:55 mikhailt Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: CanpxEngineGOCImpl.cpp,v $
|| LOG:                 Revision 1.5  2005/09/13 20:10:55  mikhailt
|| LOG:                 filter out expired bonds
|| LOG:
|| LOG:                 Revision 1.4  2005/09/13 19:01:44  mikhailt
|| LOG:                 chain fixed
|| LOG:
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


#include <strings.h>
#include <assert.h>
#include <functional>
#include "CanpxEngineGOCImpl.hpp"
#include "Logger.hpp"
#include "Reflection.hpp"
#include "Formatter.hpp"
#include "Scheduler.hpp"

CanpxEngineGOCImpl::CanpxEngineGOCImpl()
{

}

CanpxEngineGOCImpl::~CanpxEngineGOCImpl()
{
	//clean _canpxMap

}

bool
CanpxEngineGOCImpl::addImpl(CanpxInstrument *inst)
{
	string code = inst->getInstrumentCode();
	inst->setIDNLabel(buildIDNLabel(code));
	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngineGOCImpl::add [%s] to [%s]", (inst->realName()).c_str(), code.c_str());
	CanpxSMap::iterator it = _canpxMap.find(code);
	bool retA = false, retB = false;;
	if (it != _canpxMap.end()){
		CanpxBidSSet* bset = (*it).second.bid;
		BestBidGOC ba(inst);
		retB = (bset->insert(ba)).second;
		//cout << retB << " Added to bset size= "<<bset->size()<<endl;
		CanpxAskSSet* aset = (*it).second.ask;
		BestAskGOC aa(inst);
		retA = (aset->insert(aa)).second;
		//cout << retA<< " Added to aset size= "<<aset->size()<<endl;

	} else {
		CanpxBidSSet *bidset = new CanpxBidSSet;
		BestBidGOC ba(inst);
		bidset->insert(ba);

		CanpxAskSSet *askset = new CanpxAskSSet;
		BestAskGOC aa(inst);
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
CanpxEngineGOCImpl::updateImpl(CanpxInstrument *inst)
{
	bool ret = false;
	string code = inst->getInstrumentCode();
	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngineGOCImpl::update %s to %s", (inst->realName()).c_str(), code.c_str());
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
		BestBidGOC ba(inst);
		retB = (bset->insert(ba)).second;

		CanpxAskSSet* aset = (*it).second.ask;
		CanpxAskSSet::iterator it3 = find_if(aset->begin(), 
						aset->end(), 
						FindCanpxInstrumentByRealName(inst));
		if (it3 != aset->end()){
			aset->erase(it3);
		}
		BestAskGOC aa(inst);
		retA = (aset->insert(aa)).second;
		
		ret = (retA && retB);
	} else {
		Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngineGOCImpl::update not found %s to %s", inst->realName(), code.c_str());
	}

	return ret;

}

bool
CanpxEngineGOCImpl::removeImpl(CanpxInstrument *inst)
{
	return false;

}

double
CanpxEngineGOCImpl::bestBid(const string& code)
{
	double d = 0;
	string s = this->bidString(code, 0);
	if (!s.empty())
		d = atof(s.c_str());
	return d;
}

string
CanpxEngineGOCImpl::bidString(const string& code, int level)
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
CanpxEngineGOCImpl::bestAsk(const string& code)
{
	double d = 0;
	string s = this->askString(code, 0);
	if (!s.empty())
		d = atof(s.c_str());
	return d;
}

string
CanpxEngineGOCImpl::askString(const string& code, int level)
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
CanpxEngineGOCImpl::askYield(const string& code, double bestPrice)
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
CanpxEngineGOCImpl::aggregatedAskSize(const string& code, double bestPrice)
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
CanpxEngineGOCImpl::bidYield(const string& code, double bestPrice)
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
CanpxEngineGOCImpl::aggregatedBidSize(const string& code, double bestPrice)
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
CanpxEngineGOCImpl::FindCanpxInstrumentByRealName::operator()(const BestBidGOC& bid)
{
	return _i->realName() == (bid.getInstrument())->realName();

}

bool
CanpxEngineGOCImpl::FindCanpxInstrumentByRealName::operator()(const BestAskGOC& ask)
{
	return _i->realName() == (ask.getInstrument())->realName();

}

double
CanpxEngineGOCImpl::toChain(const string& address)
{
        //address CANPX_GOC.300_0606
	double ret = -1;
        int label_ind = address.find_first_of('.');
        string coupon;
	string mat;
        if (label_ind != string::npos){
                string tmp = address.substr(label_ind +1, address.size() - (label_ind +1));
		int und_ind = tmp.find_first_of('_');
		if (und_ind != string::npos){
			coupon = tmp.substr(0, und_ind);
			mat = tmp.substr(und_ind +1, tmp.size() - (und_ind +1));
		}
        }
        string today_s = Scheduler::getCurrentTime("%d%m%y");
        long today_l = Scheduler::getTimeLong(Scheduler::getCurrentTime("%d%m%y"), "%d%m%y");
        if (Formatter::ATOF(coupon.c_str())){
		if (coupon.size() == 3){ // 425 -> 0425
			coupon = string("0") + coupon;
		}
		if (mat.size() == 3){ // 308 -> 0308
			mat = string("0") + mat;
		}
		mat = string("1") + mat;
		string mat_date = string("0") + mat;
        	long label_l = Scheduler::getTimeLong(mat_date, "%d%m%y");
		if (label_l > 0 && label_l >= today_l){
			char tmp[25];
			mat = Scheduler::getTimeAs(label_l, "%Y%m%d");
			sprintf(tmp, "%s.", mat.c_str());
			strcat(tmp, coupon.c_str());
        		ret = Formatter::ATOF(tmp);
			// attach coupon to the mat date and convert back to long
		}
	}
	return ret;
}

string
CanpxEngineGOCImpl::buildIDNLabel(const string& code)
{
	// code 275_1207 return "0275 D07"
	// code 425_0908 return "0425 908"
	// code 1175_309 return "1175 309"
	// code 1025_1214 return 1025 D14"

	string label;

	int delim = code.find_first_of("_");
	if (delim != string::npos){
		string idn_cpn, idn_mat;
		string cpn = code.substr(0, delim);
		string mat = code.substr(delim +1, code.size() - (delim + 1));
		switch(cpn.size()){
			case 3:
				idn_cpn = "0" + cpn;
				break;
			case 4:
				idn_cpn = cpn;
				break;
			default:
				Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxEngineGOCImpl::buildIDNLabel failed build coupon %s of %s", cpn.c_str(), code.c_str() );
		}
		switch(mat.size()){
			case 3:
				idn_mat = mat;
				break;
			case 4:
			{
				string yr = mat.substr(2,2);
				string mn = mat.substr(0,2);
				string idn_mn;
				if (mn[0] == '0'){
					idn_mn = string(1, mn[1]);
				}else{
					if (mn == "10"){
						idn_mn = "O";
					} else if (mn == "11"){
						idn_mn = "N";
					} else if (mn == "12"){
						idn_mn = "D";
					} else {
						Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxEngineGOCImpl::buildIDNLabel failed build mat mn %s of %s", mn.c_str(), code.c_str() );
					}

				}
				idn_mat = idn_mn + yr;
	
			}
				break;
			default:
				Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxEngineGOCImpl::buildIDNLabel failed build mat %s of %s", mat.c_str(), code.c_str() );
		}
		if (!idn_cpn.empty() && !idn_mat.empty())
			label = idn_cpn + " " + idn_mat;
	}
	return label;
}



