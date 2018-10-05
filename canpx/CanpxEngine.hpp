
/************************************************************************
||
|| DATE:                $Date: 2005/09/13 19:01:44 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxEngine.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxEngine.hpp,v 1.4 2005/09/13 19:01:44 mikhailt Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: CanpxEngine.hpp,v $
|| LOG:                 Revision 1.4  2005/09/13 19:01:44  mikhailt
|| LOG:                 chain fixed
|| LOG:
|| LOG:                 Revision 1.3  2005/09/12 21:57:06  mikhailt
|| LOG:                 builds ldap chain
|| LOG:
|| LOG:                 Revision 1.2  2005/07/29 18:10:28  mikhailt
|| LOG:                 handle failed attempt to getList
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CANPXENGINE_HPP
#define CANPXENGINE_HPP

#include <map>
#include <set>
#include <string>
#include <algorithm>

#include "CanpxInstrument.hpp"
#include "CompiledInstrument.hpp"
#include "DataGraph.hpp"
#include "CanpxAlgorithm.hpp"

using std::map;
using std::set;
using std::string;

class CanpxEngine
{
	public:
		typedef map<string, CompiledInstrument*> CompiledMap;
		virtual ~CanpxEngine();
		static CanpxEngine * getEngine();
		
		virtual bool add(CanpxInstrument *inst);

		virtual bool update(CanpxInstrument *inst);

		virtual bool remove(CanpxInstrument *inst);

		virtual int resetAvol();

		virtual void processBest(CanpxInstrument *inst);
		virtual void processTrade(CanpxInstrument *inst);
		virtual double toChain(const string& address) = 0;
		virtual string updateCompiledInstrument(DataGraph& newdata);
	protected:
		CanpxEngine();
		//Interface
		virtual bool addImpl(CanpxInstrument *inst) =0;
		virtual bool updateImpl(CanpxInstrument *inst) =0;
		virtual bool removeImpl(CanpxInstrument *inst) =0;
		virtual double bestBid (const string& code) = 0;
		virtual double bestAsk (const string& code) = 0;
		virtual double aggregatedAskSize(const string& code, double price) = 0;
		virtual double aggregatedBidSize(const string& code, double price) = 0;
		virtual double askYield(const string& code, double price) = 0;
		virtual double bidYield(const string& code, double price) = 0;
	protected:	
		CompiledInstrument* compile(CanpxInstrument *i);
		void publishInit(CanpxInstrument *i);
		void publishUpdate(CanpxInstrument *i);
		static CanpxEngine* _engine;
		CompiledMap _compiledMap;
	private:
		string _subject;

};

#endif //CANPXENGINE_HPP

