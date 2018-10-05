
/************************************************************************
||
|| DATE:                $Date: 2005/09/13 19:01:44 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxEngineTBillImpl.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxEngineTBillImpl.hpp,v 1.3 2005/09/13 19:01:44 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: CanpxEngineTBillImpl.hpp,v $
|| LOG:                 Revision 1.3  2005/09/13 19:01:44  mikhailt
|| LOG:                 chain fixed
|| LOG:
|| LOG:                 Revision 1.2  2005/09/12 21:57:06  mikhailt
|| LOG:                 builds ldap chain
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CANPXENGINETBillIMPL_HPP
#define CANPXENGINETBillIMPL_HPP

#include <map>
#include <set>
#include <string>

#include "CanpxInstrument.hpp"
#include "DataGraph.hpp"
#include "CanpxAlgorithm.hpp"
#include "CanpxEngine.hpp"

using std::map;
using std::set;
using std::string;

class CanpxEngineTBillImpl: public CanpxEngine
{

	public:
		typedef set<BestBidTBill> CanpxBidSSet;
		typedef set<BestAskTBill> CanpxAskSSet;

		typedef struct _CBestPrice {
			CanpxBidSSet *bid;
			CanpxAskSSet *ask;
		} CBestPrice;

		typedef map<string, CBestPrice> CanpxSMap;

		CanpxEngineTBillImpl();

		virtual ~CanpxEngineTBillImpl();
		
		virtual bool addImpl(CanpxInstrument *inst);
		virtual bool updateImpl(CanpxInstrument *inst);
		virtual bool removeImpl(CanpxInstrument *inst);
		virtual double bestBid(const string& code);
		virtual double bestAsk(const string& code);
		virtual double aggregatedBidSize(const string& code, double d);
		virtual double aggregatedAskSize(const string& code, double d);
		virtual double bidYield(const string& code, double d);
		virtual double askYield(const string& code, double d);
	
		virtual double toChain(const string& address);

	protected:
		virtual string bidString(const string& code, int level);
		virtual string askString(const string& code, int level);
		CanpxSMap _canpxMap;
		virtual string buildIDNLabel(const string& code);
		map<string, string> _monthMap;
	protected:
	class FindCanpxInstrumentByRealName
	{
		public:
			FindCanpxInstrumentByRealName(CanpxInstrument *inst):_i(inst){};
			~FindCanpxInstrumentByRealName(){};
			bool FindCanpxInstrumentByRealName::operator()(const BestBidTBill& bid);
			bool FindCanpxInstrumentByRealName::operator()(const BestAskTBill& ask);
		protected:
			CanpxInstrument* _i;
	};


};




#endif //CANPXENGINETBillIMPL_HPP
