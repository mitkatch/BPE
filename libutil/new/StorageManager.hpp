
/************************************************************************
||
|| DATE:                $Date: 2005/07/29 18:39:09 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/StorageManager.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: StorageManager.hpp,v 1.2 2005/07/29 18:39:09 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: StorageManager.hpp,v $
|| LOG:                 Revision 1.2  2005/07/29 18:39:09  mikhailt
|| LOG:                 read yesterday
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 18:55:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef STORAGEMANAGER_HPP
#define STORAGEMANAGER_HPP

#include <db.h>
#include <string>
#include <map>
#include "Logger.hpp"
#include "Datum.hpp"


using std::string;
using std::map;

#define DAY_STORAGE_NAMEFORMAT 	"%Y%m%d"

class Storage
{
	public:
		Storage(const char* loc, const char* addr);
		virtual void save(Datum *datum);
		virtual void save(const string& str);
		virtual bool closeDB();
		virtual bool openDB();
		virtual string asLast();
		virtual string asYesterday();
	protected:
		string buildLocation(const char* loc, const char* addr);
		long getLastCounter();
	protected:
		long _counter;
		string _location;
		const char* _addr;
		const char* _dir;
		DB *_db;
};

class StorageManager
{
	public:
		~StorageManager();
		static Storage* getStorage(const string& addr);
		static bool activated(){
			return _activated;
		}
		static bool setActive(const char* loc);
		static void turnOff();
		virtual void turnOffImpl();
		virtual Storage* getStorageImpl(const string& addr);
		virtual string last(const char* addr);
		virtual string yesterday(const char* addr);

	private:
		friend Storage;
		static StorageManager* _storageMgr;
		StorageManager(const char *);
		map<long, string> getListLocation();
		static bool _activated;
		const char* _location;
		map<string, Storage *> _storageMap;
		map<long, string> _listLocation;
};

#endif //STORAGEMANAGER_HPP
