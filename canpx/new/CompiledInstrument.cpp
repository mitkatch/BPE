
/************************************************************************
||
|| DATE:                $Date: 2006/04/26 18:27:20 $
|| SOURCE:              $Source: /export/home/tomcat/Build/canpx2/canpx2/RCS/CompiledInstrument.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CompiledInstrument.cpp,v 1.10 2006/04/26 18:27:20 mikhailt Exp $
|| REVISION:    $Revision: 1.10 $
|| LOG:                 $Log: CompiledInstrument.cpp,v $
|| LOG:                 Revision 1.10  2006/04/26 18:27:20  mikhailt
|| LOG:                 POST editor added
|| LOG:
|| LOG:                 Revision 1.9  2006/04/18 17:34:24  tomcat
|| LOG:                 fixed dateformat
|| LOG:
|| LOG:                 Revision 1.8  2006/04/06 20:17:53  mikhailt
|| LOG:                 handle IDN labels
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

#include "Logger.hpp"
#include "CompiledInstrument.hpp"
#include "CanpxApp.hpp"
#include "CanpxTrade.hpp"
#include "Reflection.hpp"
#include "Scheduler.hpp"
#include "StorageManager.hpp"
#include "Formatter.hpp"
#include "ConfigMap.hpp"

bool
TradeByInstrument::operator()(const CanpxTrade *t1)
{
	return _inst->realName() == t1->tradeInst();

}


CompiledInstrument::CompiledInstrument(CanpxInstrument *inst):_myDataGraph(NULL), _avol(0)
{
	_code = inst->getInstrumentCode();
	_idnLabel = inst->getIDNLabel();
	_bmark = inst->getFieldStr("BENCHMARK");
	_bid = inst->bid();
	if( _bid){
		_bidTime = Scheduler::getCurrentTime(TIME_FORMAT);
		_bidSource = inst->realName(true);
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
		_askSource = inst->realName(true);
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

		_bidSource = i->realName(true);
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
		_askSource = i->realName(true);
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
		_close_bid = Formatter::ATOF((_myDataGraph->getValue("LAST_BID")).c_str());
		_close_bidYield = Formatter::ATOF((_myDataGraph->getValue("LAST_BID_YIELD")).c_str()); 
		_close_bidTime = _myDataGraph->getValue("LAST_BID_TIME"); 
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
		_lastTradePrice = Formatter::ATOF((_myDataGraph->getValue("LAST_TRADE_PRICE")).c_str());
		_lastTradeSize = Formatter::ATOF((_myDataGraph->getValue("LAST_TRADE_SIZE")).c_str()); 
		_lastTradeYield = Formatter::ATOF((_myDataGraph->getValue("LAST_TRADE_YIELD")).c_str());
		_lastTradeTimeStart = _myDataGraph->getValue("LAST_TRADE_TIME_START"); 
		if (_lastTradeTimeStart == "FIELD_NOT_FOUND")
			_lastTradeTimeStart = "";
		_lastTradeTimeEnd =  _myDataGraph->getValue("LAST_TRADE_TIME_END");
		if (_lastTradeTimeEnd == "FIELD_NOT_FOUND")
			_lastTradeTimeEnd = "";
		_lastTradeTone = _myDataGraph->getValue("LAST_TRADE_TONE"); 
		if (_lastTradeTone == "FIELD_NOT_FOUND")
			_lastTradeTone = "";
		_lastTradeSource = _myDataGraph->getValue("LAST_TRADE_SOURCE"); 
		if (_lastTradeSource == "FIELD_NOT_FOUND")
			_lastTradeSource = "";
	
		//if lastTradeStart is today, so pick up AVOL
		if (!_lastTradeTimeStart.empty()){
			string today = Scheduler::getCurrentTime("%d/%m/%y");
			if (_lastTradeTimeStart.find(today) != string::npos){
				_avol = Formatter::ATOF((_myDataGraph->getValue("AVOL")).c_str()); 
			}
		}
		_last_bid = Formatter::ATOF((_myDataGraph->getValue("LAST_BID")).c_str()); 
		_last_bidYield = Formatter::ATOF((_myDataGraph->getValue("LAST_BID_YIELD")).c_str());
		_last_bidTime = _myDataGraph->getValue("LAST_BID_TIME"); 
		if (_last_bidTime == "FIELD_NOT_FOUND")
			_last_bidTime = "";
	}
}
 
void
CompiledInstrument::setStaticFields()
{
	if(_myDataGraph){
		string sbj = _prefix + string(".") + _code;
		_myDataGraph->updateField("INST_CODE", DataGraph::Variant(DataGraph::SVAL, sbj.c_str()));
		_myDataGraph->updateField("SYMBOL", DataGraph::Variant(DataGraph::SVAL, sbj.c_str()));
		_myDataGraph->updateField("SOURCE", DataGraph::Variant(DataGraph::SVAL, _prefix.c_str()));
		_myDataGraph->updateField("DESC", DataGraph::Variant(DataGraph::SVAL, (_idnLabel.empty()?_code.c_str():_idnLabel.c_str())));
		_myDataGraph->updateField("BENCHMARK", DataGraph::Variant(DataGraph::SVAL, _bmark.c_str()));

	}

}

void
CompiledInstrument::updateDataGraph()
{
	if (_myDataGraph){
		_myDataGraph->updateField("BID", DataGraph::Variant(DataGraph::DVAL, _bid)); 

		_myDataGraph->updateField("BID_SIZE", DataGraph::Variant(DataGraph::DVAL, _bidSize));
		_myDataGraph->updateField("BID_YIELD", DataGraph::Variant(DataGraph::DVAL, _bidYield));
		_myDataGraph->updateField("BID_TIME", DataGraph::Variant(DataGraph::SVAL, _bidTime.c_str()));
		_myDataGraph->updateField("LAST_BID", DataGraph::Variant(DataGraph::DVAL, _last_bid)); 
		_myDataGraph->updateField("LAST_BID_YIELD", DataGraph::Variant(DataGraph::DVAL, _last_bidYield));
		_myDataGraph->updateField("LAST_BID_TIME", DataGraph::Variant(DataGraph::SVAL, _last_bidTime.c_str()));
		_myDataGraph->updateField("CLOSE_BID", DataGraph::Variant(DataGraph::DVAL, _close_bid)); 
		_myDataGraph->updateField("CLOSE_BID_YIELD", DataGraph::Variant(DataGraph::DVAL, _close_bidYield));
		_myDataGraph->updateField("CLOSE_BID_TIME", DataGraph::Variant(DataGraph::SVAL, _close_bidTime.c_str()));
		_myDataGraph->updateField("BID_CHG", DataGraph::Variant(DataGraph::DVAL, bidChg())); 
		_myDataGraph->updateField("BID_YIELD_CHG", DataGraph::Variant(DataGraph::DVAL, bidYieldChg()));
		_myDataGraph->updateField("ASK", DataGraph::Variant(DataGraph::DVAL, _ask)); 
		_myDataGraph->updateField("ASK_SIZE", DataGraph::Variant(DataGraph::DVAL, _askSize));
		_myDataGraph->updateField("ASK_YIELD", DataGraph::Variant(DataGraph::DVAL, _askYield));
		_myDataGraph->updateField("ASK_TIME", DataGraph::Variant(DataGraph::SVAL, _askTime.c_str()));
		_myDataGraph->updateField("MASTER_TRADE_PRICE", DataGraph::Variant(DataGraph::DVAL, masterTradePrice())); 
		_myDataGraph->updateField("MASTER_TRADE_SIZE", DataGraph::Variant(DataGraph::DVAL, masterTradeSize()));
		_myDataGraph->updateField("MASTER_TRADE_YIELD", DataGraph::Variant(DataGraph::DVAL, masterTradeYield()));
		_myDataGraph->updateField("MASTER_TRADE_TIME_START", DataGraph::Variant(DataGraph::SVAL, (masterTradeTimeStart()).c_str()));
		_myDataGraph->updateField("MASTER_TRADE_TIME_END", DataGraph::Variant(DataGraph::SVAL, (masterTradeTimeEnd()).c_str()));
		_myDataGraph->updateField("MASTER_TRADE_TONE", DataGraph::Variant(DataGraph::SVAL, (masterTradeTone()).c_str()));
		_myDataGraph->updateField("MASTER_TRADE_SOURCE", DataGraph::Variant(DataGraph::SVAL, (masterTradeSource()).c_str()));
		_myDataGraph->updateField("TRADE_PRICE", DataGraph::Variant(DataGraph::SVAL, (tradePrice()).c_str())); 
		_myDataGraph->updateField("TRADE_SIZE", DataGraph::Variant(DataGraph::SVAL, (tradeSize()).c_str()));
		_myDataGraph->updateField("TRADE_YIELD", DataGraph::Variant(DataGraph::SVAL, (tradeYield()).c_str()));
		_myDataGraph->updateField("TRADE_TIME_START", DataGraph::Variant(DataGraph::SVAL, (tradeTimeStart()).c_str()));
		_myDataGraph->updateField("TRADE_TIME_END", DataGraph::Variant(DataGraph::SVAL, (tradeTimeEnd()).c_str()));
		_myDataGraph->updateField("TRADE_TONE", DataGraph::Variant(DataGraph::SVAL, (tradeTone()).c_str()));
		_myDataGraph->updateField("TRADE_SOURCE", DataGraph::Variant(DataGraph::SVAL, (tradeSource()).c_str()));
		
		_myDataGraph->updateField("LAST_TRADE_PRICE", DataGraph::Variant(DataGraph::DVAL, _lastTradePrice)); 
		_myDataGraph->updateField("LAST_TRADE_SIZE", DataGraph::Variant(DataGraph::DVAL, _lastTradeSize));
		_myDataGraph->updateField("LAST_TRADE_YIELD", DataGraph::Variant(DataGraph::DVAL, _lastTradeYield));
		_myDataGraph->updateField("LAST_TRADE_TIME_START", DataGraph::Variant(DataGraph::SVAL, _lastTradeTimeEnd.c_str()));
		_myDataGraph->updateField("LAST_TRADE_TIME_END", DataGraph::Variant(DataGraph::SVAL, _lastTradeTimeEnd.c_str()));
		_myDataGraph->updateField("LAST_TRADE_TONE", DataGraph::Variant(DataGraph::SVAL, _lastTradeTone.c_str()));
		_myDataGraph->updateField("LAST_TRADE_SOURCE", DataGraph::Variant(DataGraph::SVAL, _lastTradeSource.c_str()));
		_myDataGraph->updateField("AVOL", DataGraph::Variant(DataGraph::DVAL, _avol));
		
		string timestmp = Scheduler::getCurrentTime(TIME_FORMAT);
		_myDataGraph->updateField("TIMESTAMP", DataGraph::Variant(DataGraph::SVAL, timestmp.c_str()));
		string raw = string("BID:") + _bidSource + string("; ASK:") + _askSource;
		_myDataGraph->updateField("ML_RAW_DATA", DataGraph::Variant(DataGraph::SVAL, raw.c_str()));
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
		_bid = Formatter::ATOF(newdata.getValue("BID").c_str());
		_bidSize = Formatter::ATOF(newdata.getValue("BID_SIZE").c_str());
		_bidYield = Formatter::ATOF(newdata.getValue("BID_YIELD").c_str());
		_ask = Formatter::ATOF(newdata.getValue("ASK").c_str());
		_askSize = Formatter::ATOF(newdata.getValue("ASK_SIZE").c_str());
		_askYield = Formatter::ATOF(newdata.getValue("ASK_YIELD").c_str());
		_avol = Formatter::ATOF(newdata.getValue("AVOL").c_str());	
		
	}
	this->updateDataGraph();

}

