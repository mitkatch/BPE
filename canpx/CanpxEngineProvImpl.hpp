
/************************************************************************
||
|| DATE:                $Date: 2005/10/04 16:51:28 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxEngineProvImpl.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxEngineProvImpl.hpp,v 1.1 2005/10/04 16:51:28 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: CanpxEngineProvImpl.hpp,v $
|| LOG:                 Revision 1.1  2005/10/04 16:51:28  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CANPXENGINEPROVIMPL_HPP
#define CANPXENGINEPROVIMPL_HPP

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

class CanpxEngineProvImpl: public CanpxEngine
{
	public:
		typedef set<BestBidProv> CanpxBidSSet;
		typedef set<BestAskProv> CanpxAskSSet;

		typedef struct _CBestPrice {
			CanpxBidSSet *bid;
			CanpxAskSSet *ask;
		} CBestPrice;

		typedef map<string, CBestPrice> CanpxSMap;
		typedef map<string, vector<string> > ProvConverterMap;

		CanpxEngineProvImpl();

		virtual ~CanpxEngineProvImpl();
		
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
		virtual void buildConverterMap();
		virtual string getConvertedAlias(const string& prov);
		CanpxSMap _canpxMap;
		ProvConverterMap _provConvMap;

	protected:
	class FindCanpxInstrumentByRealName
	{
		public:
			FindCanpxInstrumentByRealName(CanpxInstrument *inst):_i(inst){};
			~FindCanpxInstrumentByRealName(){};
			bool FindCanpxInstrumentByRealName::operator()(const BestBidProv& bid);
			bool FindCanpxInstrumentByRealName::operator()(const BestAskProv& ask);
		protected:
			CanpxInstrument* _i;
	};


};




#endif //CANPXENGINEPROVIMPL_HPP
