/************************************************************************
||
|| DATE:                $Date: 2005/09/12 21:57:06 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxContributor.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxContributor.hpp,v 1.3 2005/09/12 21:57:06 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: CanpxContributor.hpp,v $
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



#ifndef CANPXCONTRIBUTOR_HPP
#define CANPXCONTRIBUTOR_HPP

#include "Datum.hpp"
#include "Logger.hpp"
#include "TrsRecordHandler.hpp"
#include "Subscriber.hpp"
#include "TimerCallback.hpp"
#include "WorkerCallback.hpp"
#include "NSMap.hpp"

#include "CanpxInstrument.hpp"

class CanpxContributor: public Subscriber, 
			public TimerCallback, 
			public WorkerCallback
{
	public:
		typedef vector<CanpxInstrument *> CanpxInstVector;
		CanpxContributor(TrsRecordHandler *trsHandler, const string& name, const ContribNameSpace& ns);
		virtual ~CanpxContributor();

		virtual string name() const{
			return _name;
		};
		virtual bool active() const {
			return _active;
		}

		virtual void notify(Datum *datum);
		virtual void onTimer(TimerId id);

		static string stripPageOf(const string& rec);

		virtual void onWorker(DataGraph *dgr);
		virtual string dataGraphAddress(){
			return _dataAddress;
		}
		virtual void getList();
		virtual bool descardDuplications(CanpxInstrument *i);
		virtual bool toChain( CanpxInstrument *i);
		virtual bool chainLoaded();

	protected:
		virtual string toAlias(const string& relaName);

		virtual void buildAlias();
	protected:
		ContribNameSpace _ns;
		string _name, _configName;
		string _dataAddress;
		TrsRecordHandler *_trsHandler;
		bool _active;
		map<string, CanpxInstVector > _mapInst;
		map<string, string> _aliasMap;
		map<CanpxInstrument*, bool> _chainMap;
		int _counter;	
		CanpxInstVector _vDuplicated;

};


#endif //CANPXCONTRIBUTOR_HPP
