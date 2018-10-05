/************************************************************************
||
|| DATE:                $Date: 2004/04/29 19:49:16 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/Logger.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Logger.hpp,v 1.2 2004/04/29 19:49:16 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: Logger.hpp,v $
|| LOG:                 Revision 1.2  2004/04/29 19:49:16  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:52:40  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <iostream>

class Logger
{
	public:
		enum Res {
			LOG_CONSOLE, LOG_FILE, LOG_SERVER, SYS_LOGGER
		};
		enum Type {
			TYPE_INFO = 1,          /*001*/
			TYPE_WARNING =2 ,       /*010*/
			TYPE_ERROR = 4          /*100*/
		};

		/**
                **      level can be 7 or nothing -> to show ALL
                **      6 -> to show WARNING & ERROR
                **      4 -> ERROR only
                **/

		static Logger* getLogger(int res);
		static Logger* getLogger();
		virtual void setLogger(int res, const char *ident) ;
		virtual ~Logger();
		virtual void log(int type, const char *format, ...);
		virtual void setLevel(int level);

	protected:
		static Logger* _logger;
		Logger();	
		char _buf[BUFSIZ *2];
		int _myres;
		int _level;
};
#endif//LOGGER_HPP
