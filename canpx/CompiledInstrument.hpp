
/************************************************************************
||
|| DATE:                $Date: 2005/10/04 16:51:28 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CompiledInstrument.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CompiledInstrument.hpp,v 1.4 2005/10/04 16:51:28 mikhailt Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: CompiledInstrument.hpp,v $
|| LOG:                 Revision 1.4  2005/10/04 16:51:28  mikhailt
|| LOG:                 prov added
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

#ifndef COMPILEDINSTRUMENT_HPP
#define COMPILEDINSTRUMENT_HPP

#include <string>
#include <vector>

#include "CanpxInstrument.hpp"
#include "DataGraph.hpp"

class CanpxTrade;

using std::string;
using std::vector;

class TradeByInstrument
{
	public:
		TradeByInstrument(CanpxInstrument *inst):_inst(inst){};
		virtual ~TradeByInstrument(){};
		bool operator()(const CanpxTrade *t1);
	private:
		CanpxInstrument* _inst;
};

class CompiledInstrument
{
	public:
		CompiledInstrument(CanpxInstrument *i);
		virtual ~CompiledInstrument();

		virtual void setBestBid(CanpxInstrument *i, double bestBid);
		virtual void setBestAsk(CanpxInstrument *i, double bestAsk);
		virtual void setAskSize(CanpxInstrument *i, double size);
		virtual void setBidSize(CanpxInstrument *i, double size);
		virtual void setAskYield(CanpxInstrument *i, double y);
		virtual void setBidYield(CanpxInstrument *i, double y);
		virtual void handleTrade(CanpxInstrument *inst);
		virtual void update(CanpxInstrument *inst);
		virtual void updateFromFlat(DataGraph& newdata);
		static string str(double d);
		virtual void refresh();
		virtual void avol(double d){
			_avol = d;
		}
	protected:
		virtual void initDataGraph();	
		virtual void updateReflection(CanpxInstrument *inst);
		virtual void updateDataGraph();
		virtual void setStaticFields();
		virtual void setLastTradeFields();
		virtual void setCloseFields();
		virtual void setLastTrade(CanpxTrade *trade);
		virtual string asHREF(const string& code);
		virtual double masterTradeSize();
		virtual double masterTradePrice();
		virtual double masterTradeYield();
		virtual string masterTradeTimeStart();
		virtual string masterTradeTimeEnd();
		virtual string masterTradeTone();
		virtual string masterTradeSource();

		virtual double bidChg();
		virtual double bidYieldChg();
		virtual string tradeSize();
		virtual string tradePrice();
		virtual string tradeYield();
		virtual string tradeTimeStart();
		virtual string tradeTimeEnd();
		virtual string tradeTone();
		virtual string tradeSource();

		CanpxTrade * currentTrade();

	protected:
		double _bid, _last_bid, _close_bid, _ask, _bidSize, _askSize;
		double _bidYield, _last_bidYield, _close_bidYield, _askYield;
		string _code, _subject, _prefix, _idnLabel;
		DataGraph * _myDataGraph;
		string _askSource, _bidSource;
		string _askTime, _bidTime, _last_bidTime, _close_bidTime;
		string _bmark;
		double _lastTradePrice, _lastTradeSize,  _lastTradeYield;
		string _lastTradeTimeStart, _lastTradeTimeEnd, _lastTradeTone;
		string _lastTradeSource;
		double _avol;


		vector<CanpxTrade*> _tradeMap;
};
	


#endif //COMPILEDINSTRUMENT_HPP
