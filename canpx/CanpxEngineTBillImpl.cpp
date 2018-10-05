
/************************************************************************
||
|| DATE:                $Date: 2005/09/13 19:01:44 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxEngineTBillImpl.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxEngineTBillImpl.cpp,v 1.3 2005/09/13 19:01:44 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: CanpxEngineTBillImpl.cpp,v $
|| LOG:                 Revision 1.3  2005/09/13 19:01:44  mikhailt
|| LOG:                 chain fixed
|| LOG:
|| LOG:                 Revision 1.2  2005/09/12 21:56:39  mikhailt
|| LOG:                 builds ldap chain
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <strings.h>
#include <assert.h>
#include <functional>
#include "CanpxEngineTBillImpl.hpp"
#include "Scheduler.hpp"

#include "Logger.hpp"
#include "Reflection.hpp"


CanpxEngineTBillImpl::CanpxEngineTBillImpl()
{
	_monthMap["JAN"] = "1";
	_monthMap["FEB"] = "2";
	_monthMap["MAR"] = "3";
	_monthMap["APR"] = "4";
	_monthMap["MAY"] = "5";
	_monthMap["JUN"] = "6";
	_monthMap["JUL"] = "7";
	_monthMap["AUG"] = "8";
	_monthMap["SEP"] = "9";
	_monthMap["OCT"] = "O";
	_monthMap["NOV"] = "N";
	_monthMap["DEC"] = "D";
}

CanpxEngineTBillImpl::~CanpxEngineTBillImpl()
{
	//clean _canpxMap

}

bool
CanpxEngineTBillImpl::addImpl(CanpxInstrument *inst)
{
	string code = inst->getInstrumentCode();
	inst->setIDNLabel(buildIDNLabel(code));
	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngineTBillImpl::add [%s] to [%s]", (inst->realName()).c_str(), code.c_str());
	CanpxSMap::iterator it = _canpxMap.find(code);
	bool retA = false, retB = false;;
	if (it != _canpxMap.end()){
		CanpxBidSSet* bset = (*it).second.bid;
		BestBidTBill ba(inst);
		retB = (bset->insert(ba)).second;
		//cout << retB << " Added to bset size= "<<bset->size()<<endl;
		CanpxAskSSet* aset = (*it).second.ask;
		BestAskTBill aa(inst);
		retA = (aset->insert(aa)).second;
		//cout << retA<< " Added to aset size= "<<aset->size()<<endl;

	} else {
		CanpxBidSSet *bidset = new CanpxBidSSet;
		BestBidTBill ba(inst);
		bidset->insert(ba);

		CanpxAskSSet *askset = new CanpxAskSSet;
		BestAskTBill aa(inst);
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
CanpxEngineTBillImpl::updateImpl(CanpxInstrument *inst)
{
	bool ret = false;
	string code = inst->getInstrumentCode();
	Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngineTBillImpl::update %s to %s", inst->realName(), code.c_str());
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
		BestBidTBill ba(inst);
		retB = (bset->insert(ba)).second;

		CanpxAskSSet* aset = (*it).second.ask;
		CanpxAskSSet::iterator it3 = find_if(aset->begin(), 
						aset->end(), 
						FindCanpxInstrumentByRealName(inst));
		if (it3 != aset->end()){
			aset->erase(it3);
		}
		BestAskTBill aa(inst);
		retA = (aset->insert(aa)).second;
		
		ret = (retA && retB);
	} else {
		Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngineTBillImpl::update not found %s to %s", inst->realName(), code.c_str());
	}

	return ret;

}

bool
CanpxEngineTBillImpl::removeImpl(CanpxInstrument *inst)
{
	return false;

}

double
CanpxEngineTBillImpl::bestBid(const string& code)
{
	double d = 0;
	string s = this->bidString(code, 0);
	if (!s.empty())
		d = atof(s.c_str());
	return d;
}

string
CanpxEngineTBillImpl::bidString(const string& code, int level)
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
CanpxEngineTBillImpl::bestAsk(const string& code)
{
	double d = 0;
	string s = this->askString(code, 0);
	if (!s.empty())
		d = atof(s.c_str());
	return d;
}

string
CanpxEngineTBillImpl::askString(const string& code, int level)
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
CanpxEngineTBillImpl::askYield(const string& code, double bestPrice)
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
CanpxEngineTBillImpl::aggregatedAskSize(const string& code, double bestPrice)
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
CanpxEngineTBillImpl::bidYield(const string& code, double bestPrice)
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
CanpxEngineTBillImpl::aggregatedBidSize(const string& code, double bestPrice)
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
CanpxEngineTBillImpl::FindCanpxInstrumentByRealName::operator()(const BestBidTBill& bid)
{
	return _i->realName() == (bid.getInstrument())->realName();

}

bool
CanpxEngineTBillImpl::FindCanpxInstrumentByRealName::operator()(const BestAskTBill& ask)
{
	return _i->realName() == (ask.getInstrument())->realName();

}

double
CanpxEngineTBillImpl::toChain(const string& address)
{
	//address CANPX_TB.AUG20
	//address CANPX_TB.WI_3M
	/**** disable wi's ******
	if (address.find(".WI_3M") != string::npos){
		return 1L;
	} else if (address.find(".WI_6M") != string::npos){
		return 2L;
	}else if (address.find(".WI_1Y") != string::npos){
		return 3L;
	}
	*******/

	int label_ind = address.find_first_of('.');
	string label;
	if (label_ind != string::npos){
		label = address.substr(label_ind +1, address.size() - (label_ind +1));
	}
	string year_s = Scheduler::getCurrentTime("%Y");
	string today_s = Scheduler::getCurrentTime("%b%d");
	long today_l = Scheduler::getTimeLong(today_s, "%b%d");
	long label_l = Scheduler::getTimeLong(label + year_s, "%b%d%Y");
	return (double)label_l;
}

string
CanpxEngineTBillImpl::buildIDNLabel(const string& code)
{
	//code APR06, return "TB 406"
	//code OCT05, return "TB O05"
	string label;
	if (code.size() == 5){
		string mnth = code.substr(0, 3);
		string day = code.substr(3, 2);
		map<string, string>::iterator it = _monthMap.find(mnth);
		if (it != _monthMap.end()){
			label = "TB " + (*it).second + day;
		}
	}
	// return empty if not complied
	return label;
}
