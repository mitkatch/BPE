
/************************************************************************
||
|| DATE:                $Date: 2004/06/28 16:34:13 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/Scheduler.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Scheduler.hpp,v 1.2 2004/06/28 16:34:13 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: Scheduler.hpp,v $
|| LOG:                 Revision 1.2  2004/06/28 16:34:13  mikhailt
|| LOG:                 daylight saving time handled
|| LOG:
|| LOG:                 Revision 1.1  2004/04/29 19:49:16  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <iostream.h>
#include <string>
#include <map>

using std::string;
using std::map;

class Scheduler
{
	public:
		Scheduler();
		virtual ~Scheduler();

		static string getCurrentTime(const string& format);
		static string getTimeAs(long timelong, const string& format);
		static long getTimeLong(const string& str, const string& format);
		static bool setTimeValue(const string& key, const string& value);
		static string getTimeValue(const string &key);

		virtual long getSecondsToNext();
		virtual bool readCronLine(const string& cron);

	protected:
		int _nowSec;
		int _nowMin;
		int _nowHour;
		int _nowMDay;
		int _nowMon;
		int _nowYear;
		int _nowWDay;
		int _cMin;
		int _cHour;
		int _cWDay;
		
		static map<string, string>* _timeMap;

};

#endif//SCHEDULER_HPP
