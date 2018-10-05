/************************************************************************
||
|| DATE:                $Date: 2006/04/07 16:00:29 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/Logger.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Logger.cpp,v 1.5 2006/04/07 16:00:29 tomcat Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: Logger.cpp,v $
|| LOG:                 Revision 1.5  2006/04/07 16:00:29  tomcat
|| LOG:                 log with timestamp
|| LOG:
|| LOG:                 Revision 1.4  2006/03/30 21:50:02  tomcat
|| LOG:                 command line -V 4 for ERROR
|| LOG:
|| LOG:                 Revision 1.3  2004/12/15 20:22:18  mikhailt
|| LOG:                 if syslog failed send to console
|| LOG:
|| LOG:                 Revision 1.2  2004/04/29 19:49:16  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:52:40  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <iostream.h>
#include <string>
#include <syslog.h>
#include "Scheduler.hpp"
#include "Logger.hpp"


Logger* Logger::_logger = NULL;
pthread_mutex_t m_mutex;

Logger::Logger():_level(7)
{
	//default level is 7 in bin 111, to show all TYPEs
	_myres=Logger::LOG_CONSOLE;
	pthread_mutex_init(&m_mutex, NULL);	
}

Logger::~Logger()
{
	pthread_mutex_destroy(&m_mutex);
}


Logger*
Logger::getLogger(int res)
{
	return Logger::getLogger();
}

Logger*
Logger::getLogger()
{
	pthread_mutex_lock(&m_mutex);
	if (_logger == NULL){
		_logger = new Logger();
	}
	pthread_mutex_unlock(&m_mutex);
	return _logger;

}

void
Logger::setLevel(int level)
{
	pthread_mutex_lock(&m_mutex);
	_level = level;
	pthread_mutex_unlock(&m_mutex);

}

void
Logger::log(int type, const char *format, ...)
{
	pthread_mutex_lock(&m_mutex);
	if ((_level & type) == type){
		va_list val;

		va_start(val, format);
		vsnprintf(_buf, sizeof(_buf), format, val);	
		va_end(val);
		switch(_myres){
			case Logger::SYS_LOGGER:
				{
					int priority = LOG_NOTICE;
					string spr;
					switch(type){
						case Logger::TYPE_INFO:
							priority = LOG_INFO;
							spr = "INFO";
							break;
						case Logger::TYPE_ERROR:
							priority = LOG_ERR;
							spr = "ERROR";
							break;
						case Logger::TYPE_WARNING:
							priority = LOG_WARNING;
							spr = "WARNING";
							break;
					}
					syslog(priority, "%s | %s", spr.c_str(),  _buf);	
				}
				break;
			case Logger::LOG_CONSOLE:
				cout <<Scheduler::getCurrentTime("%T %m/%d/%y")<<"::Type="<<type<<"::"<<_buf<<endl;
				break;
		}
	}
	pthread_mutex_unlock(&m_mutex);
}
void
Logger::setLogger(int res, const char *ident)
{
	switch(res){
		case Logger::SYS_LOGGER:
			openlog(ident, LOG_PID | LOG_CONS, LOG_USER);
			_myres = res;
			break;
		default:
			closelog();
	}
}
