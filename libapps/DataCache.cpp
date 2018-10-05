
/************************************************************************
||
|| DATE:                $Date: 2006/04/06 17:58:39 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/DataCache.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DataCache.cpp,v 1.6 2006/04/06 17:58:39 nickz Exp $
|| REVISION:    $Revision: 1.6 $
|| LOG:                 $Log: DataCache.cpp,v $
|| LOG:                 Revision 1.6  2006/04/06 17:58:39  nickz
|| LOG:                 Fixed spelling errors in output
|| LOG:
|| LOG:                 Revision 1.5  2006/03/23 18:54:32  nickz
|| LOG:                 Added vector for wildcard retrieval
|| LOG:
|| LOG:                 Revision 1.4  2005/06/29 18:53:33  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.3  2004/10/20 19:48:24  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.2  2004/09/23 17:58:24  mikhailt
|| LOG:                 using Datum
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include  "Application.hpp"
#include "DataFeed.hpp"
#include "Logger.hpp"
#include "DataGraph.hpp"
#include "DataPage.hpp"


#include "DataCache.hpp"

DataCache * DataCache::_theCache = NULL;
pthread_mutex_t localMutex;

DataCache::DataCache():_persistent(false)
{
	pthread_mutex_init(&m_dMutex, NULL);
	pthread_mutex_init(&m_sMutex, NULL);

}

DataCache::~DataCache()
{
	//MTRS unsubscribe from all instruments
	pthread_mutex_destroy(&m_dMutex);
	pthread_mutex_destroy(&m_sMutex);
}

DataCache *
DataCache::getCache()
{
	pthread_mutex_lock(&localMutex);
	if (_theCache == NULL)
		_theCache = new DataCache();
	pthread_mutex_unlock(&localMutex);

	return _theCache;
}

void
DataCache::subscribe(string& address, Subscriber *subscriber, int datum)
{
	map <string, Datum*>::iterator it = _mapCache.find(address);
	if ( it != _mapCache.end()){
		Datum *data = (*it).second;
		data->registerSubscriber(subscriber);
	}else{
		//subscribe from the source if not found
		assert(theApplication);
		DataFeed * feed = theApplication->getDataFeed();
		if (feed){
			string source, instr;
			int delim = address.find_first_of('.');
			if (delim != string::npos){
				source = address.substr(0, delim);
				instr = address.substr(delim +1, string::npos);
				Datum *newdata = NULL;
				if (datum == Datum::PAGE){
					newdata = new DataPage(address);
					Logger::Logger::getLogger()->log(Logger::TYPE_INFO,"DataCache::subscribe PAGE %s", address.c_str());
				} else if (datum == Datum::GRAPH){
					newdata = new DataGraph(address);
					Logger::Logger::getLogger()->log(Logger::TYPE_INFO,"DataCache::subscribe GRAPH %s", address.c_str());
				} else{
					Logger::Logger::getLogger()->log(Logger::TYPE_ERROR,"DataCache::subscribe unknown datum type %d", datum);

				}
				_mapCache[address] = newdata;
				newdata->registerSubscriber(subscriber, false);
				feed->setUpRequestAsync(source.c_str(), instr.c_str());
			}else{
				Logger::Logger::getLogger()->log(Logger::TYPE_ERROR,"DataCache::subscribe no delimiter found in %s", address.c_str());
			}
		}
	}

}

void
DataCache::unsubscribe(string& address, Subscriber *subscriber)
{
	Datum *data = NULL;
	map <string, Datum *>::iterator it = _mapCache.find(address);
	if ( it != _mapCache.end()){
		data = (*it).second;
		data->unregisterSubscriber(subscriber);
	}
	if (data ){
		if (data->numOfSubscribers() <= 0 && !_persistent){
			//remove from active list and send async unsubscribe 
			_mapCache.erase(address);
			DataFeed * feed = theApplication->getDataFeed();
			if (feed){
				string source, instr;
				int delim = address.find_first_of('.');
				if (delim != string::npos){
					source = address.substr(0, delim);
					instr = address.substr(delim +1, string::npos);
					feed->cancelRequestAsync(address.c_str());
				}else{
					Logger::Logger::getLogger()->log(Logger::TYPE_ERROR,"DataCache::unsubscribe no delimiter found in %s", address.c_str());
				}
			}
		}	

	}
}

Datum *
DataCache::getData(string& address, int datum)
{
    Datum * data = NULL;
    pthread_mutex_lock(&m_dMutex);
    map<string, Datum *>::iterator it = _mapCache.find(address) ;
    if (it != _mapCache.end())
    {
	data = (*it).second;
    }
    else
    {
	//subscribe from the source if not found
	assert(theApplication);
	DataFeed * feed = theApplication->getDataFeed();
	if (feed)
	{
	    string source, instr;
	    int delim = address.find_first_of('.');
	    if (delim != string::npos)
	    {
		source = address.substr(0, delim);
		instr = address.substr(delim +1, string::npos);

		if (datum == Datum::PAGE)
		{
		    data = new DataPage(address);
		    cout<<"DataCache::subscribe new "<<address<<endl;
		} else if (datum == Datum::GRAPH)
		{
		    data = new DataGraph(address);
		    cout<<"DataCache::subscribe new "<<address<<endl;
		} else 
		{
		    Logger::Logger::getLogger()->log(Logger::TYPE_ERROR,"DataCache::getData unknown datum type %d", address.c_str());
		}

		_mapCache[address] = data;
		feed->setUpRequestAsync(source.c_str(), instr.c_str());
	    }
	    else
	    {
		Logger::Logger::getLogger()->log(Logger::TYPE_ERROR,"DataCache::getData no delimiter found in %s", address.c_str());
	    }
	}
    }
    pthread_mutex_unlock(&m_dMutex);
    return data;
}

	
void
DataCache::subscribePortfolio(vector<string>& tickers, Subscriber *subscriber)
{

	vector< Datum*> pvec;
	vector<string>::iterator it = tickers.begin();

	for(it; it != tickers.end(); it++){
		Datum *data = getData((*it), Datum::GRAPH);
		if (data){
			pvec.push_back(data);
			cout<<"subscribePortfolio "<<(*it)<<endl;
			data->registerSubscriber(subscriber);
		}
	}
	_portfolioHolder[subscriber]= pvec; 	
}


int
DataCache::removePortfolio(Datum *data, Subscriber *subscriber)
{
	int retsize = 0;
	map< Subscriber *, vector< Datum *> >::iterator it =_portfolioHolder.find(subscriber); 
	if (it != _portfolioHolder.end()){
		vector <Datum *> &pvec =  (*it).second;
		vector < Datum *>::iterator itv = pvec.begin();	
		for (itv; itv != pvec.end(); itv++){
			if ((*itv) == data){
				data->unregisterSubscriber(subscriber);
				vector <Datum *>::iterator it2 = pvec.erase(itv);
				if (it2 == pvec.end()){
					retsize = 0;
					break;
				}
				retsize = pvec.size();
			}
		}
	}
	return retsize;
}

bool 
DataCache::addDatum(const string& address, int type)
{
	bool ret = false;
	map<string, Datum *>::iterator it = _mapCache.find(address) ;
	if (it == _mapCache.end()){
		if (type == Datum::PAGE){
			_mapCache[address] =  new DataPage(address);
					cout<<"DataCache::subscribe new "<<address<<endl;
			ret = true;
		} else if (type == Datum::GRAPH){
			_mapCache[address] = new DataGraph(address);
					cout<<"DataCache::subscribe new "<<address<<endl;
			ret = true;
		} else {
			Logger::Logger::getLogger()->log(Logger::TYPE_ERROR,"DataCache::getData unknown datum type %d", address.c_str());
		}
	}
	return ret;
}

Datum* 
DataCache::removeDatum(const string& address)
{
	Datum * data = NULL;
	map<string, Datum *>::iterator it = _mapCache.find(address) ;
	if (it != _mapCache.end()){
		data = (*it).second;
		_mapCache.erase(it);
		if (data ){
			if (data->numOfSubscribers() <= 0 && !_persistent){
				//remove from active list and send async unsubscribe 
				DataFeed * feed = theApplication->getDataFeed();
				if (feed){
					string source, instr;
					int delim = address.find_first_of('.');
					if (delim != string::npos){
						source = address.substr(0, delim);
						instr = address.substr(delim +1, string::npos);
						feed->cancelRequestAsync(address.c_str());
					}else{
						Logger::Logger::getLogger()->log(Logger::TYPE_ERROR,"DataCache::unsubscribe no delimiter found in %s", address.c_str());
					}
				}
			}	
		}
	}
	return data;

}

bool 
DataCache::deleteDatum(const string& address)
{
	bool ret = false;
	map<string, Datum *>::iterator it = _mapCache.find(address) ;
	if (it != _mapCache.end()){
		Datum * data = NULL;
		data = (*it).second;
		_mapCache.erase(it);
		if (data ){
			if (data->numOfSubscribers() <= 0 && !_persistent){
				//remove from active list and send async unsubscribe 
				DataFeed * feed = theApplication->getDataFeed();
				if (feed){
					string source, instr;
					int delim = address.find_first_of('.');
					if (delim != string::npos){
						source = address.substr(0, delim);
						instr = address.substr(delim +1, string::npos);
						feed->cancelRequestAsync(address.c_str());
					}else{
						Logger::Logger::getLogger()->log(Logger::TYPE_ERROR,"DataCache::unsubscribe no delimiter found in %s", address.c_str());
					}
				}
			}	
			delete data;
			ret = true;
		}
	}
	return ret;
}


Datum *
DataCache::getDatum(const string& address)
{
	Datum * data = NULL;
	map<string, Datum *>::iterator it = _mapCache.find(address) ;
	if (it != _mapCache.end()){
		data = (*it).second;
	}
	return data;
}

bool
DataCache::setSubscriberByWCard(Subscriber *subscriber, const string& card)
{
    map<string, vector<Subscriber *> >::iterator it = _wcardSubscriberMap.find(card);

    if ( it != _wcardSubscriberMap.end() )
    {
	vector<Subscriber *>& vec = (*it).second;

	// Found the card, append new subscriber to vector only if not already there
	vector<Subscriber *>::iterator vecItr = vec.begin();
	for( vecItr; vecItr != vec.end(); vecItr++ )
	{
	    if ( subscriber == *vecItr )
		return(false);
	}

	// Not found, add
	vec.push_back(subscriber);
    }
    else
    {
	// Card not found, add card and new subscriber
	vector< Subscriber *> subvec;

	subvec.push_back(subscriber);

	_wcardSubscriberMap[card] = subvec;
    }

    return(true);
}

vector<Subscriber *>*
DataCache::getSubscriberByWCard(const string& card)
{
    map<string, vector<Subscriber *> >::iterator it = _wcardSubscriberMap.begin();

    for (it; it != _wcardSubscriberMap.end(); it++)
    {
	string wcard = (*it).first;
	if (card.find(wcard) != string::npos)
	{
	    return( &(*it).second );
	}
    }
    
    return(NULL);
}

void
DataCache::getDataClone(const string& address, Datum *datum)
{
	pthread_mutex_lock(&m_dMutex);
	map<string, Datum *>::iterator it = _mapCache.find(address) ;
	if (it != _mapCache.end()){
		(*it).second->getDataClone(datum);
	}
	pthread_mutex_unlock(&m_dMutex);
}

set<string>
DataCache::getAddressSet()
{
	set<string> addressSet;
	pthread_mutex_lock(&m_dMutex);
	map<string, Datum *>::iterator it = _mapCache.begin() ;
	for (it; it != _mapCache.end(); it++){
		addressSet.insert((*it).first);
	}
	pthread_mutex_unlock(&m_dMutex);

	return addressSet;
}
