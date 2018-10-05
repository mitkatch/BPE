
/************************************************************************
||
|| DATE:                $Date: 2005/09/12 16:00:55 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/WorkerCallback.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: WorkerCallback.cpp,v 1.3 2005/09/12 16:00:55 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: WorkerCallback.cpp,v $
|| LOG:                 Revision 1.3  2005/09/12 16:00:55  mikhailt
|| LOG:                 typo fixed
|| LOG:
|| LOG:                 Revision 1.2  2005/07/29 18:09:14  mikhailt
|| LOG:                 pass NULL onWorker failed
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 18:53:33  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <stropts.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>


#include "WorkerCallback.hpp"
#include "CreateThread.hpp"
#include "Logger.hpp"

WorkerCallback::Worker::Worker(DATAGRAPHWORKER rw, WorkerCallback *wc):_rw(rw) ,_wc(wc)
{
	_address = _wc->dataGraphAddress();
}

WorkerCallback::Worker::~Worker()
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "WorkerCallback::~Worker for %s , address %p", _address.c_str(), this);

}

void
WorkerCallback::Worker::setChanels()
{
	int fd[2];
	socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
	_wc->_read = fd[0];
	_wc->_write = fd[1];
	pthread_mutex_init(&_wc->_dataMutex, NULL);
	//newIO(_wc->_read, IOCallback::IO_READ, this);
}

long
WorkerCallback::Worker::Run()
{
	//run from thread_id == _id
	// thread expires when Run exits
	// address like SOURCE.INSTR if it's for MTRS
	Logger::getLogger()->log(Logger::TYPE_INFO, "WorkerCallback::Worker::Run() for %s", _address.c_str());
	DataGraph *data = new DataGraph(_address);
	if ((*_rw)(data)){ 
		pthread_mutex_lock(&_wc->_dataMutex);
		_wc->_dataVector.push_back(data);	
		pthread_mutex_unlock(&_wc->_dataMutex);
		send(_wc->_write, &_wc->_read, sizeof(_wc->_read), 0); 
		Logger::getLogger()->log(Logger::TYPE_INFO, "WorkerCallback::Worker::Run() write %d to %d", _wc->_read, _wc->_write);
	}else{
		pthread_mutex_lock(&_wc->_dataMutex);
		_wc->_dataVector.push_back(data);//collect empty to delete
		pthread_mutex_unlock(&_wc->_dataMutex);
	}
	close(_wc->_write);
	return 0L; 
}

void
WorkerCallback::Worker::onRead(int fid, InputId id)
{
	_wc->onRead();
}

void
WorkerCallback::onRead()
{
	// run from main thread

	int ntoread = 0, nread;
	int buf;
	nread = read(_read, &buf, sizeof(buf));
	Logger::getLogger()->log(Logger::TYPE_INFO, "WorkerCallback::Worker::onRead()  read %d", buf);
	DataGraph *data = NULL;
	if (buf == _read){
		//we've been notified
		pthread_mutex_lock(&_dataMutex);
		data = _dataVector.front();	
		_dataVector.erase(_dataVector.begin());
		pthread_mutex_unlock(&_dataMutex);
		this->onWorker(data);
		delete data;
	} else {
		pthread_mutex_lock(&_dataMutex);
		data = _dataVector.front();	
		_dataVector.erase(_dataVector.begin());
		delete data;
		this->onWorker(NULL); //worker Run failed to bring result from thr worker
	}
	close(_read);
}

WorkerCallback::WorkerCallback():_id(0), _worker(NULL)
{

}

WorkerCallback::~WorkerCallback()
{
	pthread_mutex_destroy(&_dataMutex);
}

void
WorkerCallback::addWorker(DATAGRAPHWORKER rw, WorkerCallback *wc)
{
	if (_worker != NULL){
		Logger::getLogger()->log(Logger::TYPE_INFO, "WorkerCallback::addWorker  found _worker %p , reset ...", _worker);

	}
	_worker = new Worker(rw, wc);
	_worker->setChanels();
	_id = CreateThread(_worker); //_worker is deleted after ::Run 
	//pthread_join(_id, NULL);
	wc->onRead();
}


