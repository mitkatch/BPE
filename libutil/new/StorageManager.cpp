
/************************************************************************
||
|| DATE:                $Date: 2005/07/29 18:39:09 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/StorageManager.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: StorageManager.cpp,v 1.2 2005/07/29 18:39:09 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: StorageManager.cpp,v $
|| LOG:                 Revision 1.2  2005/07/29 18:39:09  mikhailt
|| LOG:                 read yesterday
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 18:55:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

#include "Scheduler.hpp"
#include "StorageManager.hpp"

bool StorageManager::_activated = false;
StorageManager* StorageManager::_storageMgr = NULL;

Storage::Storage(const char* location, const char* addr):_db(NULL), _counter(0),_dir(location), _addr(addr)
{
	//openDB();
}

bool
Storage::openDB()
{
	
	_location = buildLocation(_dir, _addr);
	u_int32_t flags = DB_CREATE;
	int ret = db_create(&_db, NULL, 0);
	if (ret == 0){

		ret = _db->open(_db, NULL, _location.c_str(),
				NULL, DB_RECNO, flags, 0);
		if (ret != 0){
			Logger::getLogger()->log(Logger::TYPE_ERROR," Open _db failed for %s", _location.c_str());
			_db->close(_db, 0);
			_db = NULL;

		} else {
			_counter = getLastCounter();

		}

	}else{
		Logger::getLogger()->log(Logger::TYPE_ERROR," Create _db failed for %s", _location);
	}

	return ret == 0;
}

bool
Storage::closeDB()
{
	int ret = -1;
	if (_db != NULL){
		ret = _db->close(_db, 0);
		_db = NULL;
	}
	return (ret == 0);

}

string
Storage::buildLocation(const char* location, const char* addr)
{
	//location is a dir where timestamp dir is located
	//location + / + timestamp
	// if doesn't exist mkdir it
	//Returns: location + / + timestamp + / + addr
	struct stat buf;
	string ret;
	string today = Scheduler::getCurrentTime(DAY_STORAGE_NAMEFORMAT);
	string newloc = string(location) + string("/") + today;
	if(stat(newloc.c_str(), &buf) == -1){
		if (errno == ENOENT){
			if (mkdir(newloc.c_str(), S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) != 0){
				Logger::getLogger()->log(Logger::TYPE_ERROR,"Storage::buildLocation mkdir failed for %s", newloc.c_str());
			} else {
				ret = newloc + string("/") + addr;
			}
		} else {
			char * err = strerror(errno);
			Logger::getLogger()->log(Logger::TYPE_ERROR,"Storage::buildLocation  errno not ENOENT failed for %s ", (err?err:"NONE"));
		}
	}else{
		if (S_ISDIR(buf.st_mode)){
			ret = newloc + string("/") + addr;
		} else {
			char * err = strerror(errno);
			Logger::getLogger()->log(Logger::TYPE_ERROR,"Storage::buildLocation S_ISDIR failed for %s [%s]", newloc.c_str(), (err?err:"NONE"));
		}
	} 

	return ret;
}

long
Storage::getLastCounter()
{
	long c = 1;
	DBC *cursorp = NULL;
	DBT key1, data1;
	memset(&key1, 0, sizeof(DBT));
	memset(&data1, 0, sizeof(DBT));

	_db->cursor(_db, NULL, &cursorp, 0);
	if (cursorp != NULL){
		cursorp->c_get(cursorp, &key1, &data1, DB_LAST);
		if (key1.size == sizeof(c)){
			c = *(long *)key1.data +1; //increment it
		}
	}
	cursorp->c_close(cursorp);
	return c;
}

void
Storage::save(Datum *datum)
{
	string str = datum->toString();
	save(str);

}

void
Storage::save(const string& str)
{
	if (!openDB())
		return;

	DBT key1, data1;
	memset(&key1, 0, sizeof(DBT));
	memset(&data1, 0, sizeof(DBT));

	key1.data = &_counter;
	key1.size = sizeof(_counter);


	data1.data = (void *)str.c_str();
	data1.size = str.size();

	int ret = _db->put(_db, NULL, &key1, &data1, DB_NOOVERWRITE);	
	if (ret != 0){
		Logger::getLogger()->log(Logger::TYPE_ERROR," Storage::save failed %s", _location.c_str());

	}
	//_db->sync(_db, 0);
	_counter +=1;
	closeDB();
}

string
Storage::asLast()
{
	string rec;
	string loc = StorageManager::_storageMgr->last(_addr);
	if (loc.empty())
		return rec;
 
	loc = string(_dir) + string("/") + loc + string("/")+ string(_addr);
	u_int32_t flags = DB_RDONLY;
	DB* db = NULL;
	int ret = db_create(&db, NULL, 0);
	if (ret == 0){
		ret = db->open(db, NULL, loc.c_str(),
				NULL, DB_RECNO, flags, 0);
		if (ret == 0){
			DBC *cursorp = NULL;
			DBT key1, data1;
			memset(&key1, 0, sizeof(DBT));
			memset(&data1, 0, sizeof(DBT));

			db->cursor(db, NULL, &cursorp, 0);
			if (cursorp != NULL){
				cursorp->c_get(cursorp, &key1, &data1, DB_LAST);
				if (data1.size != 0 ){
					rec = (char *)data1.data;
				}
			}
			cursorp->c_close(cursorp);
			db->close(db, 0);
		}
	}
	return rec;	
}

string
Storage::asYesterday()
{
	string rec;
	string loc = StorageManager::_storageMgr->yesterday(_addr);
	if (loc.empty())
		return rec;
 
	loc = string(_dir) + string("/") + loc + string("/")+ string(_addr);
	u_int32_t flags = DB_RDONLY;
	DB* db = NULL;
	int ret = db_create(&db, NULL, 0);
	if (ret == 0){
		ret = db->open(db, NULL, loc.c_str(),
				NULL, DB_RECNO, flags, 0);
		if (ret == 0){
			DBC *cursorp = NULL;
			DBT key1, data1;
			memset(&key1, 0, sizeof(DBT));
			memset(&data1, 0, sizeof(DBT));

			db->cursor(db, NULL, &cursorp, 0);
			if (cursorp != NULL){
				cursorp->c_get(cursorp, &key1, &data1, DB_LAST);
				if (data1.size != 0 ){
					rec = (char *)data1.data;
				}
			}
			cursorp->c_close(cursorp);
			db->close(db, 0);
		}
	}
	return rec;	
}

StorageManager::StorageManager(const char* loc):_location(loc)
{
	
	StorageManager::_activated = true;
	_listLocation = getListLocation();

}

map<long, string>
StorageManager::getListLocation()
{
	vector<string>  vs;
	map<long, string> ms;
	struct stat buf;
	struct dirent *dp = NULL;
	DIR * odir = opendir(_location);
	if (odir){
		while ((dp = readdir(odir)) != NULL){
			if (strcmp(dp->d_name, ".") == 0) continue;
			if (strcmp(dp->d_name, "..") == 0) continue;
			string fn = string(_location) + string("/") + string(dp->d_name);
			if(stat(fn.c_str(), &buf) == 0){
				if (S_ISDIR(buf.st_mode)){
					vs.push_back(string(dp->d_name));
				}
			}
		}
	}
	closedir(odir);
	for (int i = 0; i < vs.size(); i++){
		string name = vs[i];
		long l = Scheduler::getTimeLong(name, DAY_STORAGE_NAMEFORMAT);
		ms[l] = name;	 
	}

	return ms;

}


string
StorageManager::last(const char* addr)
{
	//take the latest one
	//if isn't found, take previous (skip Sat and Sun
	// if not found return empty
	string ret;
	struct stat buf;

	map<long, string>::reverse_iterator it(_listLocation.end());
	map<long, string>::reverse_iterator it_end(_listLocation.begin());
	for(it; it != it_end ; it++){
		ret = (*it).second;
		long t = (*it).first;
		string day = Scheduler::getTimeAs(t, "%w");
		if (day == "0" || day == "6")//Sun = 0, Sat = 6
			continue;
		string current = string(_location) + string("/") + ret + string("/") + string(addr);	

		if(stat(current.c_str(), &buf) != -1){
			if (S_ISREG(buf.st_mode)){
				break; //get the latest one;
			}
		}
	}
	return ret;
}

string
StorageManager::yesterday(const char* addr)
{
	//take the latest one
	//if isn't found, take previous (skip Sat and Sun
	// if not found return empty
	// returns only yesterday, not older
	string ret;
	struct stat buf;

	map<long, string>::reverse_iterator it(_listLocation.end());
	map<long, string>::reverse_iterator it_end(_listLocation.begin());
	for(it; it != it_end ; it++){
		ret = (*it).second;
		long t = (*it).first;
		string today = Scheduler::getCurrentTime(DAY_STORAGE_NAMEFORMAT);
		if (today == ret)
			continue; //skip today
		string day = Scheduler::getTimeAs(t, "%w");
		if (day == "0" || day == "6")//Sun = 0, Sat = 6
			continue;

		break; //one iteration, skip today and weekends;
		/*****
		string current = string(_location) + string("/") + ret + string("/") + string(addr);	

		if(stat(current.c_str(), &buf) != -1){
			if (S_ISREG(buf.st_mode)){
				break; //get the latest one;
			}
		}
		****/
	}
	return ret;
}



void
StorageManager::turnOffImpl()
{
	map<string, Storage* >::iterator it = _storageMap.begin();
	for (it; it != _storageMap.end(); it++){
		Storage *s = (*it).second;
		s->closeDB();
		delete s;

	}
	_storageMap.clear();
}


//static
bool
StorageManager::setActive(const char* loc)
{
	if (_storageMgr == NULL){
		_storageMgr = new StorageManager(loc);
	}
	return StorageManager::_activated;
}

Storage*
StorageManager::getStorageImpl(const string& addr)
{
	Storage *st = NULL;
	map<string, Storage *>::iterator it = _storageMap.find(addr);
	if (it != _storageMap.end()){
		st = (*it).second;
	}else{
		st = new Storage(_location, addr.c_str());
		_storageMap[addr] = st;
	}
	return st;
}


void
StorageManager::turnOff()
{
	if (_storageMgr != NULL){
		_storageMgr->turnOffImpl();
	}

}

Storage*
StorageManager::getStorage(const string& addr)
{
	assert(_storageMgr);
	return _storageMgr->getStorageImpl(addr);

}

