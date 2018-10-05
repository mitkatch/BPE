
/************************************************************************
||
|| DATE:                $Date: 2006/04/19 01:13:47 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/ESInstrument.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESInstrument.cpp,v 1.7 2006/04/19 01:13:47 nickz Exp $
|| REVISION:    $Revision: 1.7 $
|| LOG:                 $Log: ESInstrument.cpp,v $
|| LOG:                 Revision 1.7  2006/04/19 01:13:47  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.6  2006/02/20 20:00:45  tomcat
|| LOG:                 normalized Labels
|| LOG:
|| LOG:                 Revision 1.5  2006/02/17 22:12:15  tomcat
|| LOG:                 normalization of the Prov Labels
|| LOG:
|| LOG:                 Revision 1.4  2006/02/08 16:38:27  mikhailt
|| LOG:                 normalization for PROV
|| LOG:
|| LOG:                 Revision 1.3  2005/11/21 22:11:02  mikhailt
|| LOG:                 label->alais
|| LOG:
|| LOG:                 Revision 1.2  2005/11/21 16:54:05  mikhailt
|| LOG:                 TIME formatted
|| LOG:
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "ESCFETIProcessor.hpp"
#include "AliasConverter.hpp"
#include "ESInstrument.hpp"

#include "ConfigMap.hpp"
#include "DataCache.hpp"
#include "Scheduler.hpp"
#include "Logger.hpp"
#include "cfeti.h"

#define _TRADE_TONE_	"ROW66_20"
#define _TRADE_PRICE_	"ROW66_15"
#define _TRADE_SIZE_	"ROW66_16"
#define _TRADE_YIELD_	"ROW66_21"
#define _TRADE_TIME_	"ROW66_18"
#define _SEQ_OUT_	"WEB_ADRS"
#define _CUSIP_		"SECUR_NAME"
#define _BENCHMARK_	"ROW66_2"

static map<unsigned short, string> _fieldMap;

ESInstrument::ESInstrument(const string& label): _data(NULL), _label(label), _isProv(false), _isNorm(false)
{
	buildName(label);
	if (_fieldMap.size() == 0)
		setFieldMap();
}

ESInstrument::~ESInstrument()
{

}

ESInstrument::MInstr::MInstr():lastBid(0), counter(0)
{
        initTime = Scheduler::getCurrentTime("%Y%m%d%H%M%S");
}

bool
ESInstrument::activate()
{
	bool ret = false;
        Datum *datum = DataCache::getCache()->getDatum(_name);
        DataGraph *dg = dynamic_cast<DataGraph *>(datum);
        if (dg == NULL){
                if (DataCache::getCache()->addDatum(_name, Datum::GRAPH)){
                        Datum *datum = DataCache::getCache()->getDatum(_name);
                        _data = dynamic_cast<DataGraph *>(datum);
			if (_data){
				ret = true;
				Logger::getLogger()->log(Logger::TYPE_INFO, "ESInstrument::activate() addDatum %s as %s", _label.c_str(), _name.c_str());
				_data->updateField("SECUR_NAME",DataGraph::Variant(DataGraph::SVAL, _name.c_str()));
				_data->updateField("INST_CODE",DataGraph::Variant(DataGraph::SVAL, _inst_code.c_str()));
 
			}  else {
				Logger::getLogger()->log(Logger::TYPE_ERROR, "ESInstrument::activate() failed addDatum %s as %s", _label.c_str(), _name.c_str());
			}
		}
        }else{
		Logger::getLogger()->log(Logger::TYPE_ERROR, "ESInstrument::activate() %s has been already activated as %s", _label.c_str(), _name.c_str());
	}

	return ret;
}

//////////////////////////////
void
ESInstrument::setValByIndex(int val, unsigned short index)
{
	string field = getField(index);
	if (field.size() && _data){
		if (index == CFETF_STATE){
			_currentState = val;
			string state = ESCFETIProcessor::getStateInfo(val);
			_data->updateField(field, DataGraph::Variant(DataGraph::SVAL, state.c_str())); 
			setTradeTone(val);
		} else {
			_data->updateField(field, DataGraph::Variant(DataGraph::LVAL, (long)val)); 
		}
	}
}


void
ESInstrument::setValByIndex(double val, unsigned short index)
{
	string field = getField(index);
	bool done = false; //set true if updated inside the set.. routine
	if (field.size() && _data){
		switch (index){
			case CFETF_TICK:
				done = setTick(val);
				break;
			case CFETF_CLOSE:
				if (_isProv) val = val/100;
				setClose(val);
				break;
			case CFETF_BID:
				if (_isProv) val = val/100;
				setBidChg(val);
				setBid(val);
				break;
			case CFETF_ASK:
				if (_isProv) val = val/100;
				setAsk(val);
				break;
			case CFETF_BID_SIZE:
				setBidSize(val);
				break;
			case CFETF_ASK_SIZE:
				setAskSize(val);
				break;
			case CFETF_BID_YIELD:
				setBidYield(val);
				break;
			case CFETF_ASK_YIELD:
				setAskYield(val);
				break;
			case CFETF_OPEN:
				if (_isProv) val = val/100;
				break;
			case CFETF_HIGH:
				if (_isProv) val = val/100;
				break;
			case CFETF_LOW:
				if (_isProv) val = val/100;
				break;
		}
		if (!done)
			_data->updateField(field, DataGraph::Variant(DataGraph::DVAL, Formatter::prc(3, val))); 
	}
}

void
ESInstrument::setValByIndex(long val, unsigned short index)
{
	string field = getField(index);
	if (field.size() && _data){
		switch(index){
			case CFETF_MAT_DATE:
			case CFETF_ISSUE_DATE:
			case CFETF_SETTLEMENT_DATE:
			{
				string matdate = Scheduler::getTimeAs(val, "%d/%m/%y");
				_data->updateField(field, DataGraph::Variant(DataGraph::SVAL, matdate.c_str())); 
				break;
			}
			case CFETF_LAST_TIME:
			{
				string trtime = Scheduler::getTimeAs(val, "%d/%m/%y %H:%M:%S");
				_data->updateField(field, DataGraph::Variant(DataGraph::SVAL, trtime.c_str())); 
				break;

			}
			default:
				_data->updateField(field, DataGraph::Variant(DataGraph::LVAL, val)); 
		}

	}
}

void
ESInstrument::setValByIndex(const string& val, unsigned short index)
{
	string field = getField(index);
	bool done = false;
	if (field.size() && _data){
		switch(index){
			case CFETF_GROUP:
				if (val == "PROV")
					_isProv = true;
				if (!_isNorm ){
					_data->updateField(field, DataGraph::Variant(DataGraph::SVAL, "PROV_NOT_NORMALIZED")); 
					done = true;
				}
				break;
			case CFETF_SUBGROUP:
				setSubtype(val);
				break;
		}
		if (!done)
			_data->updateField(field, DataGraph::Variant(DataGraph::SVAL, val.c_str())); 
	}
}

///////////////////////////

void
ESInstrument::update()
{
    if (_data)
    {
	setSeqCount();
	setTimeStamp();
	_data->updated();
	Logger::getLogger()->log(Logger::TYPE_INFO, "ESInstrument::update() %s\n", 
				 _name.c_str());
    }
}

string
ESInstrument::normalizeLabel(const string &label)
{
		// BC_57512  ->  BC_05750_12
		string normLabel = label;
		if (label.size()){
			int id = label.find_first_of("_");
			if ( id != string::npos ){
				bool isProv = true;
				string prov = label.substr(0, id);
				string cpnmat = label.substr(id+1);
				for (int i = 0; i < prov.size(); i++){
					if (!isupper(prov[i]))	
						isProv = false;
				}
				for (int i = 0; i < cpnmat.size(); i++){
					if (!isdigit(cpnmat[i]))	
						isProv = false;
				}
				if (isProv && cpnmat.size() > 4){
					string mat = cpnmat.substr(cpnmat.size() - 2);
					string cpn = cpnmat.substr(0, cpnmat.size() - 2);
					normLabel = prov + "_0" + cpn + "0_" + mat;
					_isNorm = true;
				}

			}
		}

	return normLabel;
}

void
ESInstrument::buildName(const string& label)
{
	// repleace empty space and forwrd slash with _
	string prefix = ConfigMap::getConfig()->getValue("ES_FEED.SUBSCRIBER");
	if (prefix.size()){
		prefix += ".";
	}
	string alias = AliasConverter::getConverter()->getAlias(label);
	alias = normalizeLabel(alias);
	_name = prefix + alias;
	_inst_code = alias;

}


void
ESInstrument::setFieldMap()
{
	_fieldMap[CFETF_BID] = "BID";
	_fieldMap[CFETF_BID_SIZE] = "BIDSIZE";
	_fieldMap[CFETF_ASK] = "ASK";
	_fieldMap[CFETF_ASK_SIZE] = "ASKSIZE";
	_fieldMap[CFETF_STATE] = "STATE";
	_fieldMap[CFETF_LAST] = _TRADE_PRICE_;
	_fieldMap[CFETF_LAST_SIZE] = "LAST_SIZE";
	_fieldMap[CFETF_LAST_YIELD] = "LAST_YIELD";
	_fieldMap[CFETF_LAST_TIME] = "LAST_TRADE_TIME";
	_fieldMap[CFETF_VOLUME] = "VOLUME";
	_fieldMap[CFETF_OPEN] = "OPEN";
	_fieldMap[CFETF_HIGH] = "HIGH";
	_fieldMap[CFETF_LOW] = "LOW";
	_fieldMap[CFETF_CLOSE] = "CLOSE";
	_fieldMap[CFETF_TICK] = "TICK";
	_fieldMap[CFETF_BID_YIELD] = "BID_YIELD";
	_fieldMap[CFETF_ASK_YIELD] = "ASK_YIELD";
	_fieldMap[CFETF_COUPON] = "CPN";
	_fieldMap[CFETF_MAT_DATE] = "MATURITY";
	_fieldMap[CFETF_ISSUE_DATE] = "ISSUEDATE";
	_fieldMap[CFETF_SETTLEMENT_DATE] = "SETTLE_DATE";
	_fieldMap[CFETF_CANTORID] = "CANTORID";
	_fieldMap[CFETF_CUSIP] = _CUSIP_;
	_fieldMap[CFETF_ISIN] = "ISIN";
	_fieldMap[CFETF_DESCRIPTION] = "INSTR_DESC";
	_fieldMap[CFETF_STATUS] = "STATUS";
	_fieldMap[CFETF_TRADE_PRICE] = _TRADE_PRICE_;
	_fieldMap[CFETF_TRADE_SIZE] = _TRADE_SIZE_;
	_fieldMap[CFETF_ALT_INST] = _BENCHMARK_;
	_fieldMap[CFETF_LABEL] = "LABEL";
	_fieldMap[CFETF_GROUP] = "INSTR_SUBTYPE_2";
	_fieldMap[CFETF_SUBGROUP] = "SUBGROUP";
	_fieldMap[CFETF_INSTGROUP] = "INSTGROUP";
	_fieldMap[CFETF_LAST_BID] = "LAST_BID";
	_fieldMap[CFETF_LAST_ASK] = "LAST_ASK";

}

//static
string
ESInstrument::getField(unsigned short index)
{
	string f;
	map<unsigned short, string>::iterator it = _fieldMap.find(index);
	if (it != _fieldMap.end()){
		f = (*it).second;
	}else{
		;//Logger::getLogger()->log(Logger::TYPE_ERROR, "ESInstrument::getField not registered field requested %d", index);
	}
	return f;
}

bool
ESInstrument::setClose(double val)
{
	_hist.lastBid = val;

	return true; 

}
bool
ESInstrument::setTradeYield(int t)
{
	double noYield = 0;
	bool changed = false;
	if (CFETI_IS_SELL(t)){
		if(_data){
			DataGraph::Variant bid_yield = _data->getVariant("BID_YIELD");
			_data->updateField("BID_YIELD", DataGraph::Variant(DataGraph::DVAL, noYield));
			_data->updateField(_TRADE_YIELD_, bid_yield);
			changed = true;
		}
	}else if (CFETI_IS_BUY(t)){
		if(_data){
			DataGraph::Variant ask_yield = _data->getVariant("ASK_YIELD");
			_data->updateField("ASK_YIELD", DataGraph::Variant(DataGraph::DVAL, noYield));
			_data->updateField(_TRADE_YIELD_, ask_yield);
			changed = true;
		}
	} else {
		if (_data)
			_data->updateField(_TRADE_YIELD_, DataGraph::Variant(DataGraph::DVAL, noYield));

	}
	return changed;
}

bool
ESInstrument::setTradeTone(int istate)
{
	string tradetone("");
	if (CFETI_IS_SELL(istate)){
		tradetone = "H"; //BID
	}else if (CFETI_IS_BUY(istate)){
		tradetone = "T"; //OFFER
	}
	if (_data){
		_data->updateField(_TRADE_TONE_, DataGraph::Variant(DataGraph::SVAL, tradetone.c_str())); 
	}

	setTradeYield(istate);
	
	return true;
}

bool
ESInstrument::setSeqCount()
{
	if (_data){
		char buf[20];
		sprintf(buf, "%d", _hist.counter);
		string sq = _hist.initTime + "." + _name + "." +string(buf);
		_data->updateField(_SEQ_OUT_, DataGraph::Variant(DataGraph::SVAL, sq.c_str())); 
		_hist.counter++;

	}
	return true;
}

bool
ESInstrument::setTimeStamp()
{
	if (_data){
		string ts = Scheduler::getCurrentTime("%d/%m/%y %H:%M:%S");
		_data->updateField("TIMESTAMP", DataGraph::Variant(DataGraph::SVAL, ts.c_str())); 
	}
	return true;
}

bool
ESInstrument::setBidChg(double val)
{
	double bidChg = 0;
	if (_hist.lastBid && val){	
		bidChg = val - _hist.lastBid;
	}
	if (_data)
		_data->updateField("BID_CHG", DataGraph::Variant(DataGraph::DVAL, bidChg));

	return true;
}

bool
ESInstrument::setTick(double val)
{
	string tick = " ";
	if (val != 0)
		tick = (val > 0) ? "+": "-";

	if (_data)
		_data->updateField("TICK", DataGraph::Variant(DataGraph::SVAL, tick.c_str()));

	return true;
}

bool
ESInstrument::setSubtype(const string& t)
{
	cout << "SUBTYPE "<<t<<endl;
	//MASTER - BILLS
	//MASTER - MIDS BONDS
	//MASTER - BONDS 1-3
	//SWAP - B.C.
	string subtype, subtype_1;
	int sps = t.find_first_of(" :-");
	if (sps != string::npos){
		subtype = t.substr(0, sps);
		if (t.find("BONDS") != string::npos){
			subtype_1 = "BOND";
		} else if (t.find("BILLS") != string::npos){
			subtype_1 = "BILL";
		}else{
			subtype_1 = t.substr(sps+1, t.size() - (sps +1));
			subtype_1 = Formatter::trim(subtype_1);
		}
	}
	if (_data){
		_data->updateField("INSTR_SUBTYPE", DataGraph::Variant(DataGraph::SVAL, subtype.c_str()));
		_data->updateField("INSTR_SUBTYPE_1", DataGraph::Variant(DataGraph::SVAL, subtype_1.c_str()));
	} 
	
	return true;
}

/*******
bool
ESInstrument::addHistory(const char* instr, const ESInstrument::MInstr& hist)
{
	bool ret = true;
	MMAP::iterator it = _history.find(instr);
	if (it != _history.end()){
		(*it).second = hist;
	} else {
		_history.insert(MMAP::value_type(instr, hist));
	}
	return ret;
}

ESInstrument::MInstr
ESInstrument::getHistory(const char* instr)
{
	ESInstrument::MInstr ret ;
	MMAP::iterator it = _history.find(instr);
	if (it != _history.end()){
		ret = (*it).second;
	} else {
		_history.insert(MMAP::value_type(instr, ret));
	}
	return ret;
}
********/
