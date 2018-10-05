
/************************************************************************
||
|| DATE:                $Date:$
|| SOURCE:              $Source:$
|| STATE:               $State:$
|| ID:                  $Id:$
|| REVISION:            $Revision:$
|| LOG:                 $Log:$
|| LOG:
************************************************************************/

#include "MKLThrottleHandler.hpp"



MKLThrottleHandler* MKLThrottleHandler::_throttleHandler = new MKLThrottleHandler();


MKLThrottleHandler::~MKLThrottleHandler()
{

}

//static
void
MKLThrottleHandler::addRic(const string& ric, MKLThrottled* s)
{
	if (_throttleHandler->addThrottledRic(ric, s) == false)
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLThrottleHandler::addRic failed to add [%s]", ric.c_str());

}

//static
void
MKLThrottleHandler::setCeiling(const string& ric, int c)
{
	MKLThrottled *ps = _throttleHandler->findRic(ric);
	if (ps){
		ps->addCeiling(c);
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLThrottleHandler::setCeiling %i [%s]", c, ric.c_str());
	} else	
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLThrottleHandler::setCeiling not found [%s]", ric.c_str());
}

//static
void
MKLThrottleHandler::setCredit(const string& ric, int c)
{
	MKLThrottled *ps = _throttleHandler->findRic(ric);
	if (ps){
		ps->addCredit(c);
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLThrottleHandler::setCredit %i [%s]", c, ric.c_str());
	} else	
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLThrottleHandler::setCredit not found [%s]", ric.c_str());
}

//static
void
MKLThrottleHandler::setRate(const string& ric, int rate)
{
	MKLThrottled *ps = _throttleHandler->findRic(ric);
	if (ps){
		ps->addRate(rate);
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLThrottleHandler::setRate %i [%s]", rate, ric.c_str());
	}else	
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLThrottleHandler::setRate not found [%s]", ric.c_str());
}

//static
void
MKLThrottleHandler::addWaitTime(const string& ric, int sec)
{
	MKLThrottled *ps = _throttleHandler->findRic(ric);
	if (ps)
		ps->addWaitTime(sec);

	else	
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLThrottleHandler::addWaitTime not found [%s]", ric.c_str());
}

//static
void
MKLThrottleHandler::addRefresh(const string& ric)
{
	MKLThrottled *ps = _throttleHandler->findRic(ric);
	if (ps)
		ps->addRefresh();
	else
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLThrottleHandler::addRefresh not found [%s]", ric.c_str());
}


bool
MKLThrottleHandler::addThrottledRic(const string& ric, MKLThrottled* s)
{
	return (_ricSubscribers.insert(map<string, MKLThrottled*>::value_type(ric, s))).second;
}


MKLThrottled*
MKLThrottleHandler::findRic(const string& ric)
{
	MKLThrottled* ps = NULL;
	map<string, MKLThrottled*>::iterator it = _ricSubscribers.find(ric);
	if (it != _ricSubscribers.end())
		ps = (*it).second;

	return ps;
}


