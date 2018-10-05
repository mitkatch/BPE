
/************************************************************************
||
|| DATE:                $Date: 2004/06/28 16:34:13 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/Scheduler.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Scheduler.cpp,v 1.2 2004/06/28 16:34:13 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: Scheduler.cpp,v $
|| LOG:                 Revision 1.2  2004/06/28 16:34:13  mikhailt
|| LOG:                 AsPrice, AsYield, AsSize defined
|| LOG:
|| LOG:                 Revision 1.1  2004/04/29 19:49:16  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Scheduler.hpp"


#include <time.h>

#include "Logger.hpp"

pthread_mutex_t s_mutex; //for static functions
pthread_mutex_t _mutex; //for non-static functions
map<string, string>* Scheduler::_timeMap = new map<string, string>;
Scheduler::Scheduler()
{
	pthread_mutex_init(&_mutex, NULL);

        pthread_mutex_lock(&_mutex);

	struct tm *ptm, tmnow;
	time_t now = time(NULL);
	if((ptm = localtime_r(&now, &tmnow)) != NULL){
		_nowSec = tmnow.tm_sec;
		_nowMin = tmnow.tm_min;
		_nowHour = tmnow.tm_hour;
		_nowMDay = tmnow.tm_mday;
		_nowYear = tmnow.tm_year;
		_nowWDay = tmnow.tm_wday;
	}	
	pthread_mutex_unlock(&_mutex);
}

Scheduler::~Scheduler()
{
	pthread_mutex_destroy(&_mutex);
}

string //static
Scheduler::getCurrentTime(const string& format)
{
	pthread_mutex_init(&s_mutex, NULL);
	pthread_mutex_lock(&s_mutex);	
	time_t caltime;
	struct tm *tm, tm_r;
	char line[128];
	string strtm;	
	if ((caltime = time(NULL)) != -1){
		if ( (tm = localtime_r(&caltime, &tm_r)) != NULL){
			if (strftime(line, 128, format.c_str(), tm) != 0){
				strtm = string(line);
			}
		}
	}
	if (strtm.size() == 0)
		Logger::getLogger()->log(Logger::TYPE_ERROR, "Error Scheduler::getCurrentTime for %s", format.c_str());
	pthread_mutex_unlock(&s_mutex);
	pthread_mutex_destroy(&s_mutex);
	return strtm;
}

string//static
Scheduler::getTimeAs(long timelong, const string& format)
{
	pthread_mutex_init(&s_mutex, NULL);
	pthread_mutex_lock(&s_mutex);	
	time_t caltime = (time_t)timelong;
	struct tm *tm, tm_r;
	char line[128];
	string strtm;	
	if ( (tm = localtime_r(&caltime, &tm_r)) != NULL){
		if (strftime(line, 128, format.c_str(), tm) != 0){
			strtm = string(line);
		}
	}
	
	if (strtm.size() == 0)
		Logger::getLogger()->log(Logger::TYPE_ERROR, "Error Scheduler::getCurrentTime for %s", format.c_str());
	pthread_mutex_unlock(&s_mutex);
	pthread_mutex_destroy(&s_mutex);
	return strtm;
}

long //static
Scheduler::getTimeLong(const string& str, const string& format)
{
	pthread_mutex_init(&s_mutex, NULL);
	pthread_mutex_lock(&s_mutex);
	struct tm ttm, ttm_tmp;
	time_t tt = time(NULL);
	localtime_r(&tt, &ttm_tmp);
	if (str.size() && format.size()){
		strptime(str.c_str(), format.c_str(), &ttm);
		ttm.tm_isdst = ttm_tmp.tm_isdst;
	}
	long retl = (long)mktime(&ttm);

	pthread_mutex_unlock(&s_mutex);
	pthread_mutex_destroy(&s_mutex);
	//return (long)mktime(&ttm);
	return retl;
}

bool
Scheduler::readCronLine(const string& cron)
{
	// takes as a parameter " 0 9-17 * * 1-5 "
	// I'll read only range of hours and days

	pthread_mutex_lock(&_mutex);
	char min[10], hour[10], mday[10], ymonth[10], wday[10];
	int n = sscanf(cron.c_str(), "%s%s%s%s%s", min, hour, mday, ymonth, wday);

	if (n < 5){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "Error Scheduler::readCronLine failed for %s", cron.c_str());
		pthread_mutex_unlock(&_mutex);
		return false;
	}
		
	int imin = atoi(min);
	int fh = -1, sh = -1, fd = -1, sd = -1;
	int k = sscanf(hour, "%i-%i", &fh, &sh);
	int i = sscanf(wday, "%i-%i", &fd, &sd);
	//_cMin, _cHour, _cWDay
	if (_nowMin > imin) 
		_cMin = 60 - _nowMin + imin; 
	else if (_nowMin == imin)
		_cMin = 0;
	else
		_cMin = imin - _nowMin;

	if ((_nowHour >= fh) && (_nowHour < sh))
		_cHour = 0;
	else if (_nowHour < fh)
		_cHour = fh - _nowHour ;
	else
		_cHour = 23 - _nowHour + fh; 

	if ((_nowWDay >= fd) && (_nowWDay <= sd))
		_cWDay = 0;
	else if (_nowWDay < fd)
		_cWDay = fd - _nowWDay;
	else
		_cWDay = 6 - _nowWDay + fd;

	pthread_mutex_unlock(&_mutex);
	return true;
}


long 
Scheduler::getSecondsToNext()
{
	pthread_mutex_lock(&_mutex);
	//cout <<"Now sec="<<_nowSec<<", min="<<_nowMin<<", hour="<<_nowHour;
	//cout <<", wday="<<_nowWDay<<endl;

	//cout <<" Expect to run in min="<<_cMin<<", hour="<<_cHour<<", days="<<_cWDay<<endl;
	
	long secnum;
	secnum = (_cMin * 60) + (_cHour * 60 * 60) + (_cWDay * 24 * 60 * 60);

	//if in the range, run in 1 hour == 3600 sec

	//return (secnum ? secnum : 3600); 	
	pthread_mutex_unlock(&_mutex);
	return secnum ; 	

}

bool //static
Scheduler::setTimeValue(const string& key, const string& value)
{
	//pthread_mutex_init(&s_mutex, NULL);
	pthread_mutex_lock(&s_mutex);

	map<string, string>::iterator it = _timeMap->find(key);
	if (it == _timeMap->end()){
		_timeMap->insert(map<string, string>::value_type(key, value));
	} else {
		(*it).second = value;
	}

	pthread_mutex_unlock(&s_mutex);
	//pthread_mutex_destroy(&s_mutex);
	return true;

}

string //static
Scheduler::getTimeValue(const string &key)
{
	//pthread_mutex_init(&s_mutex, NULL);
	pthread_mutex_lock(&s_mutex);

	map<string, string>::iterator it = _timeMap->find(key);
	string value;
	if (it != _timeMap->end())
		value = (*it).second;

	pthread_mutex_unlock(&s_mutex);
	//pthread_mutex_destroy(&s_mutex);

	return value;
}
