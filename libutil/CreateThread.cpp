
/************************************************************************
||
|| DATE:                $Date: 2004/04/29 19:49:16 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/CreateThread.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CreateThread.cpp,v 1.2 2004/04/29 19:49:16 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: CreateThread.cpp,v $
|| LOG:                 Revision 1.2  2004/04/29 19:49:16  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/12 19:22:41  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include <pthread.h>
#include "Logger.hpp"
#include "CreateThread.hpp"


void *
ThreadStartFunc(void *arg)
{
	Runnable *thr = (Runnable *)arg;
	if (thr){
		thr->Run();
		delete thr;
	}
	return 0;
}

long
CreateThread(Runnable *thr)
{
	pthread_t tid;
	if (pthread_create(&tid, NULL, ThreadStartFunc, thr) != 0) {
		Logger::getLogger()->log(Logger::TYPE_ERROR,"Thread create failed");
		return 1L;
	}	
	//return 0L;
	return (long)tid;
}
