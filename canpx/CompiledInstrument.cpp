
/************************************************************************
||
|| DATE:                $Date: 2006/06/19 21:32:57 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CompiledInstrument.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CompiledInstrument.cpp,v 1.9 2006/06/19 21:32:57 mikhail Exp $
|| REVISION:    $Revision: 1.9 $
|| LOG:                 $Log: CompiledInstrument.cpp,v $
|| LOG:                 Revision 1.9  2006/06/19 21:32:57  mikhail
|| LOG:                 cannot read the set of fileds from the parser
|| LOG:
|| LOG:                 Revision 1.8  2006/04/01 12:11:54  nickz
|| LOG:                 Changed to make RMDS aware.
|| LOG:
|| LOG:                 Revision 1.7  2005/10/12 14:35:40  mikhailt
|| LOG:                 fixed typo and if no bid chg also isn't published
|| LOG:
|| LOG:                 Revision 1.6  2005/10/05 17:40:59  mikhailt
|| LOG:                 load LAS_BID from storage
|| LOG:
|| LOG:                 Revision 1.5  2005/10/04 16:51:28  mikhailt
|| LOG:                 prov added
|| LOG:
|| LOG:                 Revision 1.4  2005/09/13 19:01:44  mikhailt
|| LOG:                 chain fixed
|| LOG:
|| LOG:                 Revision 1.3  2005/07/29 18:10:28  mikhailt
|| LOG:                 handle failed attempt to getList
|| LOG:
|| LOG:                 Revision 1.2  2005/06/29 18:33:06  mikhailt
|| LOG:                 MASTER_TRADE and array of TRADE
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <algorithm>
#include <strstream>
#include <sstream>

#include "CanpxApp.hpp"
#include "Logger.hpp"
#include "CompiledInstrument.hpp"
#include "CanpxTrade.hpp"
#include "Reflection.hpp"
#include "Scheduler.hpp"
#include "StorageManager.hpp"
#include "Formatter.hpp"


bool
TradeByInstrument::operator()(const CanpxTrade *t1)
{
	return _inst->realName() == t1->tradeInst();

}


CompiledInstrument::CompiledInstrument(CanpxInstrument *inst):_myDataGraph(NULL), _avol(0)
{
	_code = inst->getInstrumentCode();
	_idnLabel = inst->getIDNLabel();
	_bmark = inst->getFieldStr(_BENCHMARK_);
	_bid = inst->bid();
	if( _bid){
		_bidTime = Scheduler::getCurrentTime(TIME_FORMAT);
		_bidSource = inst->realName();
		_last_bid = _bid;
		_last_bidTime = _bidTime;
	}else{
		_bidTime = "";
		_bidSource = "";
		_last_bid = 0;
		_last_bidTime = "";
	}

	_ask = inst->ask();
	if (_ask){
		_askTime = Scheduler::getCurrentTime(TIME_FORMAT);
		_askSource = inst->realName();
	}else{
		_askTime = "";
		_askSource = "";

	}
	_askSize = inst->askSize();
	_bidSize = inst->bidSize();
	_askYield = inst->askYield();
	_bidYield = inst->bidYield();
	if( _bidYield){
		_last_bidYield = _bidYield;
	}else{
		_last_bidYield = 0;
	}
	_prefix = ConfigMap::getConfig()->getValue("CANPX_ENGINE.SUBSCRIBER");	
	_subject = _prefix + string(".") + _code;
	_lastTradePrice = 0;
	_lastTradeSize = 0; 
	_lastTradeYield = 0;
	_lastTradeTimeStart = ""; 
	_lastTradeTimeEnd =  "";
	_lastTradeTone = ""; 
	_lastTradeSource = ""; 
	_avol = 0; 
	_close_bid = 0;
	_close_bidYield = 0;
	_close_bidTime = "";
	initDataGraph();
}

CompiledInstrument::~CompiledInstrument()
{

}

void
CompiledInstrument::setBestBid(CanpxInstrument *i, double bestBid)
{
	if (_bid != bestBid ){
		_bidTime = Scheduler::getCurrentTime(TIME_FORMAT);

		_bidSource = i->realName();
		_bid = bestBid;
		if( _bid){
			_last_bid = _bid;
			_last_bidTime = _bidTime;
		}
	}
}

void
CompiledInstrument::setBestAsk(CanpxInstrument *i, double bestAsk)
{
	if (_ask != bestAsk ){
		_askTime = Scheduler::getCurrentTime(TIME_FORMAT);
		_askSource = i->realName();
		_ask = bestAsk;
	}

}

void
CompiledInstrument::setAskSize(CanpxInstrument *i, double size)
{
	if (_askSize != size ){
		_askSize = size;
	}
}

void
CompiledInstrument::setBidSize(CanpxInstrument *i, double size)
{
	if (_bidSize != size ){
		_bidSize = size;
	}
}

void
CompiledInstrument::setBidYield(CanpxInstrument *i, double y)
{
	if (_bidYield != y){
		_bidYield = y;
		if( _bidYield){
			_last_bidYield = _bidYield;
		}
	}
}

void
CompiledInstrument::setAskYield(CanpxInstrument *i, double y)
{
	if (_askYield != y){
		_askYield = y;
	}
}

double
CompiledInstrument::bidChg()
{
	double d = 0;
	if (_close_bid && _last_bid && _bid){
		d = _last_bid - _close_bid ;
	}
	return d;

}

double
CompiledInstrument::bidYieldChg()
{
	double d = 0;
	if (_close_bidYield && _last_bidYield && _bidYield){
		d = _last_bidYield - _close_bidYield ;
	}
	return d;

}



void
CompiledInstrument::initDataGraph()
{
	Datum *datum = DataCache::getCache()->getDatum(_subject);
	if (!datum){
        	bool done = DataCache::getCache()->addDatum(_subject, Datum::GRAPH);
        	if (done){
                	datum = DataCache::getCache()->getDatum(_subject);
		}
	}
	DataGraph *data = dynamic_cast<DataGraph *>(datum);
	if (data){
		_myDataGraph = data;
		setCloseFields();
		setLastTradeFields();
		setStaticFields();

		updateDataGraph();
	}
}

void
CompiledInstrument::setCloseFields()
{
	if (StorageManager::activated()){
		string last = StorageManager::getStorage(_subject)->asYesterday();
		if (last.empty())
			return ;

		vector<string> v;
		Formatter::split(last, "\n", v);
		_myDataGraph->buildImageFromFlat(v, DATAGRAPH_FLAT_DELIMITER);
		_close_bid = Formatter::ATOF((_myDataGraph->getValue(_LAST_BID_)).c_str());
		_close_bidYield = Formatter::ATOF((_myDataGraph->getValue(_LAST_BID_YIELD_)).c_str()); 
		_close_bidTime = _myDataGraph->getValue(_LAST_BID_TIME_); 
		if (_close_bidTime == "FIELD_NOT_FOUND")
			_close_bidTime = "";
	}
} 

void
CompiledInstrument::setLastTradeFields()
{
	if (StorageManager::activated()){
		string last = StorageManager::getStorage(_subject)->asLast();
		if (last.empty())
			return ;

		vector<string> v;
		Formatter::split(last, "\n", v);
		_myDataGraph->buildImageFromFlat(v, DATAGRAPH_FLAT_DELIMITER);
		_lastTradePrice = Formatter::ATOF((_myDataGraph->getValue(_LAST_TRADE_PRICE_)).c_str());
		_lastTradeSize = Formatter::ATOF((_myDataGraph->getValue(_LAST_TRADE_SIZE_)).c_str()); 
		_lastTradeYield = Formatter::ATOF((_myDataGraph->getValue(_LAST_TRADE_YIELD_)).c_str());
		_lastTradeTimeStart = _myDataGraph->getValue(_LAST_TRADE_TIME_ST_); 
		if (_lastTradeTimeStart == "FIELD_NOT_FOUND")
			_lastTradeTimeStart = "";
		_lastTradeTimeEnd =  _myDataGraph->getValue(_LAST_TRADE_TIME_END_);
		if (_lastTradeTimeEnd == "FIELD_NOT_FOUND")
			_lastTradeTimeEnd = "";
		_lastTradeTone = _myDataGraph->getValue(_LAST_TRADE_TONE_); 
		if (_lastTradeTone == "FIELD_NOT_FOUND")
			_lastTradeTone = "";
		_lastTradeSource = _myDataGraph->getValue(_LAST_TRADE_SOURCE_); 
		if (_lastTradeSource == "FIELD_NOT_FOUND")
			_lastTradeSource = "";
	
		//if lastTradeStart is today, so pick up AVOL
		if (!_lastTradeTimeStart.empty()){
			string today = Scheduler::getCurrentTime("%d/%m/%y");
			if (_lastTradeTimeStart.find(today) != string::npos){
				_avol = Formatter::ATOF((_myDataGraph->getValue(_AVOL_)).c_str()); 
			}
		}
		_last_bid = Formatter::ATOF((_myDataGraph->getValue(_LAST_BID_)).c_str()); 
		_last_bidYield = Formatter::ATOF((_myDataGraph->getValue(_LAST_BID_YIELD_)).c_str());
		_last_bidTime = _myDataGraph->getValue(_LAST_BID_TIME_); 
		if (_last_bidTime == "FIELD_NOT_FOUND")
			_last_bidTime = "";
	}
}
 
void
CompiledInstrument::setStaticFields()
{
	if(_myDataGraph){
		string sbj = _prefix + string(".") + _code;
		_myDataGraph->updateField(_INST_CODE_, DataGraph::Variant(DataGraph::SVAL, sbj.c_str()));
		_myDataGraph->updateField(_SYMBOL_, DataGraph::Variant(DataGraph::SVAL, sbj.c_str()));
		_myDataGraph->updateField(_SOURCE_, DataGraph::Variant(DataGraph::SVAL, _prefix.c_str()));
		_myDataGraph->updateField(_DSPLY_NAME_, DataGraph::Variant(DataGraph::SVAL, (_idnLabel.empty()?_code.c_str():_idnLabel.c_str())));
		_myDataGraph->updateField(_BENCHMARK_, DataGraph::Variant(DataGraph::SVAL, _bmark.c_str()));

	}

}

void
CompiledInstrument::updateDataGraph()
{
	if (_myDataGraph){
		_myDataGraph->updateField(_BID_, DataGraph::Variant(DataGraph::DVAL, _bid)); 

		_myDataGraph->updateField(_BIDSIZE_, DataGraph::Variant(DataGraph::DVAL, _bidSize));
		_myDataGraph->updateField(_BID_YIELD_, DataGraph::Variant(DataGraph::DVAL, _bidYield));
		_myDataGraph->updateField(_BID_TIME_, DataGraph::Variant(DataGraph::SVAL, _bidTime.c_str()));
		_myDataGraph->updateField(_LAST_BID_, DataGraph::Variant(DataGraph::DVAL, _last_bid)); 
		_myDataGraph->updateField(_LAST_BID_YIELD_, DataGraph::Variant(DataGraph::DVAL, _last_bidYield));
		_myDataGraph->updateField(_LAST_BID_TIME_, DataGraph::Variant(DataGraph::SVAL, _last_bidTime.c_str()));
		_myDataGraph->updateField(_CLOSE_BID_, DataGraph::Variant(DataGraph::DVAL, _close_bid)); 
		_myDataGraph->updateField(_CLOSE_BID_YIELD_, DataGraph::Variant(DataGraph::DVAL, _close_bidYield));
		_myDataGraph->updateField(_CLOSE_BID_TIME_, DataGraph::Variant(DataGraph::SVAL, _close_bidTime.c_str()));
		_myDataGraph->updateField(_BID_CHG_, DataGraph::Variant(DataGraph::DVAL, bidChg())); 
		_myDataGraph->updateField(_BID_YIELD_CHG_, DataGraph::Variant(DataGraph::DVAL, bidYieldChg()));
		_myDataGraph->updateField(_ASK_, DataGraph::Variant(DataGraph::DVAL, _ask)); 
		_myDataGraph->updateField(_ASKSIZE_, DataGraph::Variant(DataGraph::DVAL, _askSize));
		_myDataGraph->updateField(_ASK_YIELD_, DataGraph::Variant(DataGraph::DVAL, _askYield));
		_myDataGraph->updateField(_ASK_TIME_, DataGraph::Variant(DataGraph::SVAL, _askTime.c_str()));
		_myDataGraph->updateField(_MASTER_TRADE_PRICE_, DataGraph::Variant(DataGraph::DVAL, masterTradePrice())); 
		_myDataGraph->updateField(_MASTER_TRADE_SIZE_, DataGraph::Variant(DataGraph::DVAL, masterTradeSize()));
		_myDataGraph->updateField(_MASTER_TRADE_YIELD_, DataGraph::Variant(DataGraph::DVAL, masterTradeYield()));
		_myDataGraph->updateField(_MASTER_TRADE_TIME_ST_, DataGraph::Variant(DataGraph::SVAL, (masterTradeTimeStart()).c_str()));
		_myDataGraph->updateField(_MASTER_TRADE_TIME_END_, DataGraph::Variant(DataGraph::SVAL, (masterTradeTimeEnd()).c_str()));
		_myDataGraph->updateField(_MASTER_TRADE_TONE_, DataGraph::Variant(DataGraph::SVAL, (masterTradeTone()).c_str()));
		_myDataGraph->updateField(_MASTER_TRADE_SOURCE_, DataGraph::Variant(DataGraph::SVAL, (masterTradeSource()).c_str()));
		_myDataGraph->updateField(_TRADE_PRICE_, DataGraph::Variant(DataGraph::SVAL, (tradePrice()).c_str())); 
		_myDataGraph->updateField(_TRADE_SIZE_, DataGraph::Variant(DataGraph::SVAL, (tradeSize()).c_str()));
		_myDataGraph->updateField(_TRADE_YIELD_, DataGraph::Variant(DataGraph::SVAL, (tradeYield()).c_str()));
		_myDataGraph->updateField(_TRADE_TIME_ST_, DataGraph::Variant(DataGraph::SVAL, (tradeTimeStart()).c_str()));
		_myDataGraph->updateField(_TRADE_TIME_END_, DataGraph::Variant(DataGraph::SVAL, (tradeTimeEnd()).c_str()));
		_myDataGraph->updateField(_TRADE_TONE_, DataGraph::Variant(DataGraph::SVAL, (tradeTone()).c_str()));
		_myDataGraph->updateField(_TRADE_SOURCE_, DataGraph::Variant(DataGraph::SVAL, (tradeSource()).c_str()));
		
		_myDataGraph->updateField(_LAST_TRADE_PRICE_, DataGraph::Variant(DataGraph::DVAL, _lastTradePrice)); 
		_myDataGraph->updateField(_LAST_TRADE_SIZE_, DataGraph::Variant(DataGraph::DVAL, _lastTradeSize));
		_myDataGraph->updateField(_LAST_TRADE_YIELD_, DataGraph::Variant(DataGraph::DVAL, _lastTradeYield));
		_myDataGraph->updateField(_LAST_TRADE_TIME_ST_, DataGraph::Variant(DataGraph::SVAL, _lastTradeTimeEnd.c_str()));
		_myDataGraph->updateField(_LAST_TRADE_TIME_END_, DataGraph::Variant(DataGraph::SVAL, _lastTradeTimeEnd.c_str()));
		_myDataGraph->updateField(_LAST_TRADE_TONE_, DataGraph::Variant(DataGraph::SVAL, _lastTradeTone.c_str()));
		_myDataGraph->updateField(_LAST_TRADE_SOURCE_, DataGraph::Variant(DataGraph::SVAL, _lastTradeSource.c_str()));
		_myDataGraph->updateField(_AVOL_, DataGraph::Variant(DataGraph::DVAL, _avol));
		
		string timestmp = Scheduler::getCurrentTime(TIME_FORMAT);
		_myDataGraph->updateField(_TIMESTAMP_, DataGraph::Variant(DataGraph::SVAL, timestmp.c_str()));
		string raw = string("BID:") + _bidSource + string("; ASK:") + _askSource;
		_myDataGraph->updateField(_ML_RAW_DATA_, DataGraph::Variant(DataGraph::SVAL, raw.c_str()));
	}

}

void
CompiledInstrument::updateReflection(CanpxInstrument *inst)
{
	StringCheck key(asHREF(_code), str(_bid), str(_ask), str(_bidSize), str(_askSize),  true);
	StringCheck val(inst->realName(), inst->bidString(), 
		inst->askString(),inst->bidSizeString(), inst->askSizeString(), inst->active());
	Reflection::getReflection()->addMapVar("Reflection::canpxInstMap", key, val);
}

void
CompiledInstrument::update(CanpxInstrument *inst)
{
	updateReflection(inst);
	
	if (_myDataGraph){
		updateDataGraph();

	}

}

void
CompiledInstrument::handleTrade(CanpxInstrument *inst)
{
	inst->printTable();
	int tradeTone = inst->tradeTone();
	vector<CanpxTrade *>::iterator it = std::find_if(_tradeMap.begin(), _tradeMap.end(), TradeByInstrument(inst));
	if (it != _tradeMap.end()){
		//if trade_tone is set and trade found, it just is going on
		//if trade_tone is not set, it means trade has complited, "commit", clean and delete it
		CanpxTrade *trade = (*it);
		if (tradeTone){
			trade->update(inst);
		}else{
			trade->commit(inst);
			setLastTrade(trade);
			_tradeMap.erase(it);
			delete trade;
		}

	}else{
		//if trade_tone is set it means trade has started, add new Trade
		//if trade_tone is not set, it just quote update, do nothing
		if (tradeTone){
			CanpxTrade *trade = new CanpxTrade(this);
			trade->init(inst);
			_tradeMap.push_back(trade);
		}
	}
}

void
CompiledInstrument::setLastTrade(CanpxTrade *trade)
{
	_lastTradePrice = trade->tradePrice();
	_lastTradeSize = trade->tradeSize();
	_lastTradeYield = trade->tradeYield();
	_lastTradeTimeStart = trade->tradeTimeStart();
	_lastTradeTimeEnd = trade->tradeTimeEnd();
	_lastTradeTone = trade->tradeToneString();
	_lastTradeSource = trade->tradeInst();
	_avol += _lastTradeSize;

}

double
CompiledInstrument::masterTradePrice()
{
	double d = 0;
	CanpxTrade *ct = currentTrade();
	if (ct){
		d = ct->tradePrice();
	}

	return d;
}

double
CompiledInstrument::masterTradeSize()
{
	double d = 0;
	CanpxTrade *ct = currentTrade();
	if (ct){
		double pr = ct->tradePrice();
		int itt = ct->tradeTone(); 
		vector<CanpxTrade *>::iterator it = _tradeMap.begin();
		for (it; it != _tradeMap.end(); it++){
			if ((*it)->tradeTone() == itt &&
				(*it)->tradePrice() == pr ){
					d += (*it)->tradeSize();
					//cout << (*it)->tradeInst() <<" TradeSize == "<<d<<" TradeTone == "<<(*it)->tradeTone()<<endl;
				}
		}
	}

	return d;

}

double
CompiledInstrument::masterTradeYield()
{
	double d = 0;
	CanpxTrade *ct = currentTrade();
	if (ct){
		d = ct->tradeYield();
	}

	return d;

}

string
CompiledInstrument::masterTradeTone()
{
	string tt;
	int itt =  CanpxInstrument::CLEAR;
	CanpxTrade *ct = currentTrade();
	if (ct){
		itt = ct->tradeTone();
	}
	if (itt & CanpxInstrument::HIT)
		tt.append("H");
	if (itt & CanpxInstrument::TAKE)
		tt.append("T");

	return tt;
}

string
CompiledInstrument::masterTradeTimeStart()
{
	string tt;
	CanpxTrade *ct = currentTrade();
	if (ct){
		tt = ct->tradeTimeStart();
	}
	return tt;
}

string
CompiledInstrument::masterTradeTimeEnd()
{
	string tt;
	CanpxTrade *ct = currentTrade();
	if (ct){
		tt = ct->tradeTimeEnd();
	}
	return tt;
}

string
CompiledInstrument::masterTradeSource()
{
	string tt;
	int itt;
	CanpxTrade *ct = currentTrade();
	if (ct){
		double pr = ct->tradePrice();
		int itt = ct->tradeTone(); 
		vector<CanpxTrade *>::iterator it = _tradeMap.begin();
		for (it; it != _tradeMap.end(); it++){
			if ((*it)->tradeTone() == itt &&
				(*it)->tradePrice() == pr ){
					tt.append((*it)->tradeInst());
					tt.append(" ");
				}
		}
	}

	return tt;
}

string
CompiledInstrument::tradeSize()
{
	std::ostringstream ostr;
	ostr << "[";
	int s = _tradeMap.size();
	int i = 0;
	while(i != s){
		ostr << str(_tradeMap[i++]->tradeSize());
		if (i != s)	
			 ostr<< "],[";
	} 
	ostr << "]";
	return ostr.str();

}


string
CompiledInstrument::tradePrice()
{
	std::ostringstream ostr;
	ostr << "[";
	int s = _tradeMap.size();
	int i = 0;
	while(i != s){
		ostr << str(_tradeMap[i++]->tradePrice());
		if (i != s)	
			 ostr<< "],[";
	} 
	ostr << "]";
	return ostr.str();


}


string
CompiledInstrument::tradeYield()
{
	std::ostringstream ostr;
	ostr << "[";
	int s = _tradeMap.size();
	int i = 0;
	while(i != s){
		ostr << str(_tradeMap[i++]->tradeYield());
		if (i != s)	
			 ostr<< "],[";
	} 
	ostr << "]";
	return ostr.str();


}

string
CompiledInstrument::tradeTimeStart()
{
	std::ostringstream ostr;
	ostr << "[";
	int s = _tradeMap.size();
	int i = 0;
	while(i != s){
		ostr << _tradeMap[i++]->tradeTimeStart();
		if (i != s)	
			 ostr<< "],[";
	} 
	ostr << "]";
	return ostr.str();


}

string
CompiledInstrument::tradeTimeEnd()
{
	std::ostringstream ostr;
	ostr << "[";
	int s = _tradeMap.size();
	int i = 0;
	while(i != s){
		ostr << _tradeMap[i++]->tradeTimeEnd();
		if (i != s)	
			 ostr<< "],[";
	} 
	ostr << "]";
	return ostr.str();


}

string
CompiledInstrument::tradeTone()
{
	std::ostringstream ostr;
	ostr << "[";
	int s = _tradeMap.size();
	int i = 0;
	while(i != s){
		ostr << _tradeMap[i++]->tradeToneString();
		if (i != s)	
			 ostr<< "],[";
	} 
	ostr << "]";
	return ostr.str();


}

string
CompiledInstrument::tradeSource()
{
	std::ostringstream ostr;
	ostr << "[";
	int s = _tradeMap.size();
	int i = 0;
	while(i != s){
		ostr << _tradeMap[i++]->tradeInst();
		if (i != s)	
			 ostr<< "],[";
	} 
	ostr << "]";
	return ostr.str();

}

CanpxTrade*
CompiledInstrument::currentTrade()
{
	CanpxTrade *ct = NULL;
	if (_tradeMap.size()){
		ct = _tradeMap.front();
	}	

	return ct;

}

void
CompiledInstrument::refresh()
{
	this->updateDataGraph();
	if (_myDataGraph) 
		_myDataGraph->updated();

}

string
CompiledInstrument::str(double d)
{
	char buf[12];
	string s;
	if (d){
		sprintf(buf, "%.3f", d);
		s = string(buf);
	}
	return s;

}

string
CompiledInstrument::asHREF(const string& code)
{
	string ns =ConfigMap::getConfig()->getValue("NAME_SPACE");
	string pr =ConfigMap::getConfig()->getValue("CANPX_ENGINE.SUBSCRIBER");

	string href = "<a href=." + ns + "/record?id=" + pr + "." +code + ">" + code + "</a>";
	return href;
}

void
CompiledInstrument::updateFromFlat(DataGraph& newdata)
{
	if (_myDataGraph){
		_bid = Formatter::ATOF(newdata.getValue(_BID_).c_str());
		_bidSize = Formatter::ATOF(newdata.getValue(_BIDSIZE_).c_str());
		_bidYield = Formatter::ATOF(newdata.getValue(_BID_YIELD_).c_str());
		_ask = Formatter::ATOF(newdata.getValue(_ASK_).c_str());
		_askSize = Formatter::ATOF(newdata.getValue(_ASKSIZE_).c_str());
		_askYield = Formatter::ATOF(newdata.getValue(_ASK_YIELD_).c_str());
		_avol = Formatter::ATOF(newdata.getValue(_AVOL_).c_str());
	}
	this->updateDataGraph();

}
