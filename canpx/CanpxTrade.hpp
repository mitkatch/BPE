
/************************************************************************
||
|| DATE:                $Date: 2005/06/24 16:21:53 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxTrade.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxTrade.hpp,v 1.1 2005/06/24 16:21:53 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: CanpxTrade.hpp,v $
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CANPXTRADE_HPP
#define CANPXTRADE_HPP
#include "TimerCallback.hpp"
#include "CanpxInstrument.hpp"
#include "CompiledInstrument.hpp"

class CanpxTrade: public TimerCallback
{
	public:
		CanpxTrade(CompiledInstrument *comp);
		virtual ~CanpxTrade();
	
		virtual void init(CanpxInstrument *inst);
		virtual void update(CanpxInstrument *inst);
		virtual void commit(CanpxInstrument *inst);

		virtual double tradePrice() const{
			return _tradePrice;
		}

		virtual double tradeSize() const {
			return _tradeSize;
		}

		virtual double tradeYield() const {
			return _tradeYield;
		}

		virtual string tradeTimeStart() const {
			return _tradeTimeStart;
		}
		virtual string tradeTimeEnd() const {
			return _tradeTimeEnd;
		}
		virtual string tradeInst() const {
			return _instName;
		}

		virtual int tradeTone() const{
			return _tradeTone;
		}
		virtual string tradeToneString() const;
	protected:
		virtual void onTimer(TimerId id);
		virtual void broadcastControlMsg(const char* type);
	private:
		CompiledInstrument *_compInst;
		double _tradePrice, _tradeSize, _tradeYield;
		string _tradeTimeStart, _tradeTimeEnd;
		string _instName;
		int _tradeTone;
};


#endif //CANPXTRADE_HPP
