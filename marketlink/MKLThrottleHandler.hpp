
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

#ifndef MLKTHROTTLEHANDLER_HPP
#define MLKTHROTTLEHANDLER_HPP


#include "Logger.hpp"
#include "MKLThrottled.hpp"
#include <map>
#include <string>

using std::map;
using std::string;

class MKLThrottleHandler
{
	public:
		virtual ~MKLThrottleHandler();
		static void setRate(const string& ric, int sec);
		static void setCredit(const string& ric, int sec);
		static void setCeiling(const string& ric, int sec);
		static void addWaitTime(const string& ric, int sec);
		static void addRefresh(const string& ric);
		static void addRic(const string& ric, MKLThrottled* subscriber);
		static MKLThrottleHandler* getHandler();
	protected:
		static MKLThrottleHandler* _throttleHandler;
		MKLThrottleHandler(){};
		virtual bool addThrottledRic(const string& ric, MKLThrottled* subscriber);
		virtual MKLThrottled* findRic(const string& ric);
		map<string, MKLThrottled*> _ricSubscribers;



};



#endif //MLKTHROTTLEHANDLER_HPP
