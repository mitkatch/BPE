
/************************************************************************
||
|| DATE:                $Date: 2005/06/24 16:21:53 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxTrade.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxTrade.cpp,v 1.1 2005/06/24 16:21:53 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: CanpxTrade.cpp,v $
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include "CanpxApp.hpp"
#include "CanpxTrade.hpp"
#include "Logger.hpp"
#include "Scheduler.hpp"
#include "ControlSession.hpp"

CanpxTrade::CanpxTrade(CompiledInstrument *comp):_compInst(comp)
{

}

CanpxTrade::~CanpxTrade()
{

}

void
CanpxTrade::init(CanpxInstrument *inst)
{
	_instName = inst->realName();
	_tradeTone = inst->tradeTone();
	_tradePrice = inst->tradePrice(_tradeTone);
	_tradeSize = inst->tradeSize(_tradeTone);
	_tradeYield = inst->tradeYield(_tradeTone);
	_tradeTimeStart = Scheduler::getCurrentTime(TIME_FORMAT);
}

void
CanpxTrade::update(CanpxInstrument *inst)
{
	//only size can be changed
	_tradeSize = inst->tradeSize(_tradeTone);

}

void
CanpxTrade::commit(CanpxInstrument *inst)
{
	_tradeTimeEnd = Scheduler::getCurrentTime(TIME_FORMAT);
	broadcastControlMsg("COMMIT");
}
void
CanpxTrade::broadcastControlMsg(const char* type)
{
	MCMsg msg;
	char buf[12];
	msg.buildPrefix("TRADE");
	msg.buildSuffix(theCanpxApp->name());
	msg.addKeyValue("TYPE", type);
	msg.addKeyValue("ML_GRAPH_ADDRESS", _instName.c_str());
	msg.addKeyValue("TRADE_TIME_START", _tradeTimeStart.c_str());
	msg.addKeyValue("TRADE_TIME_END", _tradeTimeEnd.c_str());

	sprintf(buf, "%.3f", _tradePrice);
	msg.addKeyValue("TRADE_PRICE", buf);

	sprintf(buf, "%.3f", _tradeSize);
	msg.addKeyValue("TRADE_SIZE", buf);

	sprintf(buf, "%.3f", _tradeYield); 
	msg.addKeyValue("TRADE_YIELD", buf);

	if (_tradeTone == CanpxInstrument::HIT){
		strcpy(buf, "H");
	} else if (_tradeTone == CanpxInstrument::TAKE){
		strcpy(buf, "T");
	}else{
		strcpy(buf, "");
	}	

	msg.addKeyValue("TRADE_TONE", buf );
	ControlSession *cs = NULL;
	if (cs = (ControlSession *)theCanpxApp->getControlSession()){
		cs->broadcast(msg);
	}
}

void
CanpxTrade::onTimer(TimerId id)
{

}

string
CanpxTrade::tradeToneString()const
{
	char buf[5];

	if (_tradeTone == CanpxInstrument::HIT){
		strcpy(buf, "H");
	} else if (_tradeTone == CanpxInstrument::TAKE){
		strcpy(buf, "T");
	}else{
		strcpy(buf, "");
	}	

	return string(buf);
}
