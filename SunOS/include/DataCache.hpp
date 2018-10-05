
/************************************************************************
||
|| DATE:                $Date: 2006/03/23 18:54:32 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/DataCache.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DataCache.hpp,v 1.5 2006/03/23 18:54:32 nickz Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: DataCache.hpp,v $
|| LOG:                 Revision 1.5  2006/03/23 18:54:32  nickz
|| LOG:                 Added vector for wildcard retrieval
|| LOG:
|| LOG:                 Revision 1.4  2004/10/20 19:48:24  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.3  2004/09/23 17:58:24  mikhailt
|| LOG:                 using Datum
|| LOG:
|| LOG:                 Revision 1.2  2004/04/29 19:52:56  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef DATACACHE_HPP
#define DATACACHE_HPP

//#include "DataGraph.hpp"
#include "Datum.hpp"
#include <iostream.h>
#include <map>
#include <vector>
#include <set>

using std::map;
using std::vector;
using std::set;

class DataCache
{
	public:
		virtual ~DataCache();
		static DataCache* getCache();
		
		virtual void subscribe(string& address, Subscriber *subscriber, int datum );
		virtual void subscribePortfolio(vector<string> &tickers, Subscriber *subscriber );
		virtual void unsubscribe(string& address, Subscriber *subscriber );
		virtual bool setSubscriberByWCard(Subscriber *subscriber, const string& card);
		virtual vector<Subscriber *>* getSubscriberByWCard(const string& card);

		virtual Datum * getData(string& address, int datum); 
		virtual void setPersistent(bool p){
			_persistent = p;
		}
		virtual bool getPersistent(){
			return _persistent;
		}
		virtual int removePortfolio(Datum *data, Subscriber *subscriber);
		virtual bool addDatum(const string& address, int type);
		virtual Datum* removeDatum(const string& address);
		virtual bool deleteDatum(const string& address);
		virtual Datum * getDatum(const string& address);
		virtual void getDataClone(const std::string &, Datum*);

		virtual set<string> getAddressSet();

	protected:
		static DataCache * _theCache;
		DataCache();
		bool _persistent;

		map <string, Datum*> _mapCache;
		map<Subscriber *, vector<Datum *> > _portfolioHolder;
		map<string, vector<Subscriber *> > _wcardSubscriberMap;

	private:

		pthread_mutex_t m_dMutex;
		pthread_mutex_t m_sMutex;
};




#endif //DATACACHE_HPP
