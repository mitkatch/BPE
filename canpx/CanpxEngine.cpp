
/************************************************************************
||
|| DATE:                $Date: 2006/04/01 12:11:52 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxEngine.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxEngine.cpp,v 1.4 2006/04/01 12:11:52 nickz Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: CanpxEngine.cpp,v $
|| LOG:                 Revision 1.4  2006/04/01 12:11:52  nickz
|| LOG:                 Changed to make RMDS aware.
|| LOG:
|| LOG:                 Revision 1.3  2005/10/04 16:51:28  mikhailt
|| LOG:                 prov added
|| LOG:
|| LOG:                 Revision 1.2  2005/07/29 18:10:28  mikhailt
|| LOG:                 handle failed attempt to getList
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <strings.h>
#include <assert.h>
#include "Logger.hpp"
#include "Reflection.hpp"
#include "CanpxEngine.hpp"
#include "CanpxEngineGOCImpl.hpp"
#include "CanpxEngineTBillImpl.hpp"
#include "CanpxEngineProvImpl.hpp"
#include "ConfigMap.hpp"
#include <functional>

CanpxEngine * CanpxEngine::_engine = NULL;

CanpxEngine::CanpxEngine()
{
	_subject =  ConfigMap::getConfig()->getValue("CANPX_ENGINE.SUBSCRIBER");
}

CanpxEngine::~CanpxEngine()
{
	CompiledMap::iterator it = _compiledMap.begin();
	for (it ; it != _compiledMap.end(); it++){
		CompiledInstrument *ci = (*it).second;
		delete ci;
	}
	_compiledMap.clear();

}

CanpxEngine*
CanpxEngine::getEngine()
{
	if (_engine == NULL){
		string type =  ConfigMap::getConfig()->getValue("CANPX_ENGINE.TYPE");

		if (type == "GOC"){
			_engine = new CanpxEngineGOCImpl();
		} else if (type == "TBILL"){
			_engine = new CanpxEngineTBillImpl();
		} else if (type == "PROV"){
			_engine = new CanpxEngineProvImpl();
		}
	}
	assert(_engine);
	return _engine;

}

bool
CanpxEngine::add(CanpxInstrument *inst)
{
	if (addImpl(inst)){
		processTrade(inst);
		processBest(inst);
		publishInit(inst);
	} else {
		Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngine::add rejected %s", (inst->realName()).c_str());
	}
	return true;
}

bool
CanpxEngine::update(CanpxInstrument *inst)
{
	if (updateImpl(inst)){
		processTrade(inst);
		processBest(inst);
		publishUpdate(inst);
	} else {
		Logger::getLogger()->log(Logger::TYPE_INFO, "CanpxEngine::update rejected %s", (inst->realName()).c_str());
	}
	return true;
}

bool
CanpxEngine::remove(CanpxInstrument *inst)
{
	removeImpl(inst);
	return true;
}

void
CanpxEngine::publishInit(CanpxInstrument *inst)
{
    CompiledInstrument *ci = compile(inst);
    ci->update(inst);
    string address = _subject + string(".") + inst->getInstrumentCode();
    DataGraph *graph = (DataGraph *)DataCache::getCache()->getDatum(address);
    
    if (graph)
    {
	vector<Subscriber *>* subvec = DataCache::getCache()->getSubscriberByWCard(address);
	if ( subvec != NULL )
	{
	    // Iterate through vector and register each subscriber for this graph
	    vector<Subscriber *>::iterator it = subvec->begin();
	    for( it; it != subvec->end(); it++ )
	    {
		// Don't notify when it's created, it means it's blank
		graph->registerSubscriber((*it), false);
	    }
	}

	graph->updated();
    }
    
}

void
CanpxEngine::publishUpdate(CanpxInstrument *inst)
{
	CompiledInstrument *ci = compile(inst);
	ci->update(inst);
	string address = _subject + string(".") + inst->getInstrumentCode();
	DataGraph *graph = (DataGraph *)DataCache::getCache()->getDatum(address);
	if (graph){
		graph->updated();
	}

}

void
CanpxEngine::processBest(CanpxInstrument *inst)
{
	string code = inst->getInstrumentCode();
	double bBid = bestBid(code);
	double bAsk = bestAsk(code);
	double askSize = aggregatedAskSize(code, bAsk);
	double bidSize = aggregatedBidSize(code, bBid); 
	CompiledInstrument *ci = compile(inst);
	ci->setBestBid(inst, bBid);
	ci->setBestAsk(inst, bAsk);
	ci->setAskSize(inst, askSize);
	ci->setBidSize(inst, bidSize);	
	ci->setAskYield(inst, askYield(code, bAsk));
	ci->setBidYield(inst, bidYield(code, bBid));	

}

void
CanpxEngine::processTrade(CanpxInstrument *inst)
{
	CompiledInstrument *ci = compile(inst);
	ci->handleTrade(inst);
}

CompiledInstrument *
CanpxEngine::compile(CanpxInstrument *i)
{
	CompiledInstrument *ci = NULL;
	CompiledMap::iterator it = _compiledMap.find(i->getInstrumentCode());
	if (it != _compiledMap.end()){
		ci = (*it).second;

	} else {
		ci = new CompiledInstrument(i);
		_compiledMap[i->getInstrumentCode()] = ci;


	}
	return ci;
}

int
CanpxEngine::resetAvol()
{
	CompiledMap::iterator it = _compiledMap.begin();
	for (it ; it != _compiledMap.end(); it++){
		CompiledInstrument *ci = (*it).second;
		ci->avol(0);
		ci->refresh();
	}

	return _compiledMap.size();

}

string
CanpxEngine::updateCompiledInstrument(DataGraph& newdata)
{
	string ret;
	string address = newdata.getValue("ML_GRAPH_ADDRESS");
	int dot = address.find_first_of(".");
	if (dot != string::npos){
		string code = address.substr(dot + 1, address.size() - (dot + 1));
		CompiledMap::iterator it = _compiledMap.find(code);
		if ( it != _compiledMap.end()){
			ret = "Found " + code + " for update";
			CompiledInstrument *ci = (*it).second;
			ci->updateFromFlat(newdata);
			ci->refresh();
		} else {
			ret = " Not Found " + code + " for update";
		}
	}else{
		ret = "Error processing [dot not found] for " + address;
	}

	return ret;

}

