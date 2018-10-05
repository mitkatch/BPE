/************************************************************************
||
|| DATE:                $Date: 2006/06/19 21:32:57 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxInstrument.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxInstrument.cpp,v 1.6 2006/06/19 21:32:57 mikhail Exp $
|| REVISION:    $Revision: 1.6 $
|| LOG:                 $Log: CanpxInstrument.cpp,v $
|| LOG:                 Revision 1.6  2006/06/19 21:32:57  mikhail
|| LOG:                 cannot read the set of fileds from the parser
|| LOG:
|| LOG:                 Revision 1.5  2006/04/01 12:11:53  nickz
|| LOG:                 Changed to make RMDS aware.
|| LOG:
|| LOG:                 Revision 1.4  2005/10/04 16:51:28  mikhailt
|| LOG:                 prov added
|| LOG:
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

#include "Formatter.hpp" 
#include "Logger.hpp"
#include "Reflection.hpp"
#include "CanpxInstrument.hpp"
#include "CanpxEngine.hpp"
#include "CanpxSubjectSubMgr.hpp"

/***************************************************************************
 * Construct a unique instrument.
 * The following parameters are represented:
 *
 * realName:  	The full address of the instrument.
 * Example	SHORCAN.14.AUG11
 *
 * name:	The normalized address of the instrument.
 * Example	SHORCAN.AUG11
 *
 * item:	The normalize item of the instrument.
 * Example	AUG11
 ***************************************************************************/
CanpxInstrument::CanpxInstrument(const string& address, const string& name, 
				 const string& item, CanpxSubjectSubMgr* subjSubMgr) :
    _realName(address), _name(name), _instCode(item), _subjSubMgr(subjSubMgr), _dataGraph(NULL)
{
    cout << "Create CanpxInstrument realName ["<<_realName<<"] name ["<<_name
	 << "] item [" << _instCode << "]" << endl;
}

CanpxInstrument::~CanpxInstrument()
{
    if (_dataGraph)
    {
	delete _dataGraph;
    }
}

void
CanpxInstrument::notify(Datum *datum)
{
    DataGraph *data = dynamic_cast<DataGraph *>(datum);

    if (data)
    {
	if (_dataGraph == NULL)
	{
	    _dataGraph = data;

	    if(CanpxEngine::getEngine()->add(this))
	    {
		_subjSubMgr->toChain(this);
	    }
	    else
	    {
		Logger::getLogger()->log(Logger::TYPE_WARNING, 
					 "CanpxInstrument::notify Failed to add instrument: %s to Engine.", 
					 data->address().c_str());
	    }
	}
	else
	{
	    CanpxEngine::getEngine()->update(this);
	}
    }
}



string
CanpxInstrument::bidString()const
{
	double b = 0;
	string sbid;
	DataGraph::Variant bid1 = _dataGraph->getVariant(_BID_);
	if (bid1.type == DataGraph::DVAL){
		char buf[10];
		b = bid1.data.dval;
		if (b){
			sprintf(buf, "%.3f", b);
			sbid = string(buf);
		}
	} else if( bid1.type == DataGraph::SVAL ){
		sbid = bid1.data.sval;
	}

	
	return sbid; 
}

string
CanpxInstrument::bidSizeString()const
{
	double b = 0;
	string sbid;
	DataGraph::Variant bid1 = _dataGraph->getVariant(_BIDSIZE_);
	if (bid1.type == DataGraph::DVAL){
		char buf[10];
		b = bid1.data.dval;
		if (b){
			sprintf(buf, "%.3f", b);
			sbid = string(buf);
		}
	} else if( bid1.type == DataGraph::SVAL ){
		sbid = bid1.data.sval;
	}
	return sbid; 
}

string
CanpxInstrument::askString()const
{
	double b = 0;
	string sask;
	DataGraph::Variant ask1 = _dataGraph->getVariant(_ASK_);
	if (ask1.type == DataGraph::DVAL){
		char buf[10];
		b = ask1.data.dval;
		if (b){
			sprintf(buf, "%.3f", b);
			sask = string(buf);
		}
	} else if(ask1.type == DataGraph::SVAL ){
		sask = ask1.data.sval;
	}

	return sask; 

}

string
CanpxInstrument::askSizeString()const
{
	double b = 0;
	string sask;
	DataGraph::Variant ask1 = _dataGraph->getVariant(_ASKSIZE_);
	if (ask1.type == DataGraph::DVAL){
		char buf[10];
		b = ask1.data.dval;
		if (b){
			sprintf(buf, "%.3f", b);
			sask = string(buf);
		}
	} else if(ask1.type == DataGraph::SVAL ){
		sask = ask1.data.sval;
	}

	return sask; 

}

double
CanpxInstrument::bid() const
{
	double s = 0;
	DataGraph::Variant ask1 = _dataGraph->getVariant(_BID_);
	if (ask1.type == DataGraph::DVAL){
		s = ask1.data.dval;
	} else if(ask1.type == DataGraph::SVAL ){
		s = Formatter::ATOF(ask1.data.sval);
	}

	return s;	
}

double
CanpxInstrument::ask() const
{
	double s = 0;
	DataGraph::Variant ask1 = _dataGraph->getVariant(_ASK_);
	if (ask1.type == DataGraph::DVAL){
		s = ask1.data.dval;
	} else if(ask1.type == DataGraph::SVAL ){
		s = Formatter::ATOF(ask1.data.sval);
	}

	return s;	
}

double
CanpxInstrument::askSize() const
{
	double s = 0;
	DataGraph::Variant ask1 = _dataGraph->getVariant(_ASKSIZE_);
	if (ask1.type == DataGraph::DVAL){
		s = ask1.data.dval;
	} else if(ask1.type == DataGraph::SVAL ){
		s = Formatter::ATOF(ask1.data.sval);
	}

	return s;	
}

double
CanpxInstrument::bidSize() const
{
	double s = 0;
	DataGraph::Variant ask1 = _dataGraph->getVariant(_BIDSIZE_);
	if (ask1.type == DataGraph::DVAL){
		s = ask1.data.dval;
	} else if(ask1.type == DataGraph::SVAL ){
		s = Formatter::ATOF(ask1.data.sval);
	}

	return s;	


}

double
CanpxInstrument::askYield() const
{
	double s = 0;
	DataGraph::Variant ask1 = _dataGraph->getVariant(_ASK_YIELD_);
	if (ask1.type == DataGraph::DVAL){
		s = ask1.data.dval;
	} else if(ask1.type == DataGraph::SVAL ){
		s = Formatter::ATOF(ask1.data.sval);
	}

	return s;	
}

double
CanpxInstrument::bidYield() const
{
	double s = 0;
	DataGraph::Variant ask1 = _dataGraph->getVariant(_BID_YIELD_);
	if (ask1.type == DataGraph::DVAL){
		s = ask1.data.dval;
	} else if(ask1.type == DataGraph::SVAL ){
		s = Formatter::ATOF(ask1.data.sval);
	}

	return s;	


}

int
CanpxInstrument::tradeTone() const
{
	int i = CanpxInstrument::CLEAR;
	string tt = _dataGraph->getValue(_TRADE_TONE_);
	if(tt == "H"){
		i = CanpxInstrument::HIT;
	}else if (tt == "T"){
		i = CanpxInstrument::TAKE;
	}

	return i;	


}

void
CanpxInstrument::onTimer(TimerId id)
{
}

double
CanpxInstrument::tradeSize(int tt) const
{
	double s = 0;
	DataGraph::Variant size1 = _dataGraph->getVariant(_TRADE_SIZE_);
	if (size1.type == DataGraph::DVAL){
		s = size1.data.dval;
	} else if(size1.type == DataGraph::SVAL ){
		s = Formatter::ATOF(size1.data.sval);
	}
	if (!s){
		switch(tt){
			case CanpxInstrument::HIT:{
				s = bidSize();
				break;
			}
			case CanpxInstrument::TAKE:{
				s = askSize();
				break;
			}
			default:
				Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxInstrument::tradeSize not known,  %d", tt);

		}
	}
	return s;	
}

double
CanpxInstrument::tradePrice(int tt) const
{
	double s = 0;
	DataGraph::Variant size1 = _dataGraph->getVariant(_TRADE_PRICE_);
	if (size1.type == DataGraph::DVAL){
		s = size1.data.dval;
	} else if(size1.type == DataGraph::SVAL ){
		s = Formatter::ATOF(size1.data.sval);
	}
	if (!s){
		switch(tt){
			case CanpxInstrument::HIT:{
				s = bid();
				break;
			}
			case CanpxInstrument::TAKE:{
				s = ask();
				break;
			}
			default:
				Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxInstrument::tradePrice not known,  %d", tt);

		}
	}
	return s;	
}

double
CanpxInstrument::tradeYield(int tt) const
{
	double s = 0;
	DataGraph::Variant size1 = _dataGraph->getVariant(_TRADE_YIELD_);
	if (size1.type == DataGraph::DVAL){
		s = size1.data.dval;
	} else if(size1.type == DataGraph::SVAL ){
		s = Formatter::ATOF(size1.data.sval);
	}
	if (!s){
		switch(tt){
			case CanpxInstrument::HIT:{
				s = bidYield();
				break;
			}
			case CanpxInstrument::TAKE:{
				s = askYield();
				break;
			}
			default:
				Logger::getLogger()->log(Logger::TYPE_ERROR, "CanpxInstrument::tradeYield not known,  %d", tt);

		}
	}
	return s;
}

string
CanpxInstrument::getFieldStr(const string& f)
{
	return _dataGraph->getValue(f);
}

void
CanpxInstrument::printTable()
{
	if (_dataGraph){
		string s = _dataGraph->toString();
		cout<<"CanpxInstrument {"<< s <<" }"<<endl;
	}
}
