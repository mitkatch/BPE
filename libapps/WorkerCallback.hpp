/************************************************************************
||
|| DATE:                $Date: 2005/09/12 16:00:55 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/WorkerCallback.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: WorkerCallback.hpp,v 1.2 2005/09/12 16:00:55 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: WorkerCallback.hpp,v $
|| LOG:                 Revision 1.2  2005/09/12 16:00:55  mikhailt
|| LOG:                 typo fixed
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 18:53:33  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/
// the main task is running one thread in the background 
//and delivery a DataGraph in the main (called) thread

#ifndef WORKERCALLBACK_HPP
#define WORKERCALLBACK_HPP
#include <pthread.h>
#include "Runnable.hpp"
#include "IOCallback.hpp"
#include "DataGraph.hpp"

typedef pthread_t WorkerId;


class WorkerCallback
{
	public:
		typedef bool (*DATAGRAPHWORKER)(DataGraph *dgr);
		class Worker: public Runnable, public IOCallback
		{
			public:
				Worker(DATAGRAPHWORKER rw, WorkerCallback *wc);
				virtual ~Worker();
				virtual long Run();
				void setChanels();
			protected:
				virtual void onRead(int fid, InputId id);
				virtual void onWrite(int fid, InputId id){};
				virtual void onExcept(int fid, InputId id){};

			protected:
				WorkerCallback* _wc;
				DATAGRAPHWORKER _rw;
				string _address;
		};

		friend Worker;
	public:
		WorkerCallback();
		virtual ~WorkerCallback();
		virtual string dataGraphAddress() = 0;
		virtual void addWorker(DATAGRAPHWORKER rw, WorkerCallback *wc);
		virtual void onWorker(DataGraph *dgr) = 0;
		virtual void onRead();
	protected:
		pthread_mutex_t _dataMutex;
		vector<DataGraph *> _dataVector;
		int _read;
		int _write;		


		Worker* _worker;

		WorkerId _id;


};

#endif //WORKERCALLBACK_HPP
