
/************************************************************************
||
|| DATE:                $Date: 2006/05/02 15:12:08 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/ChainManager.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ChainManager.hpp,v 1.3 2006/05/02 15:12:08 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: ChainManager.hpp,v $
|| LOG:                 Revision 1.3  2006/05/02 15:12:08  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.2  2006/04/04 13:19:31  nickz
|| LOG:                 ported to rmds
|| LOG:
|| LOG:                 Revision 1.1  2005/09/21 17:49:55  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CHAINMANAGER_HPP
#define CHAINMANAGER_HPP

#include <vector>
#include <map>
#include <set>

#include  "Logger.hpp"
#include "Subscriber.hpp"
#include "DBConnectorLdap.hpp"
#include "Datum.hpp"
#include "DataGraph.hpp"
#include "DataCache.hpp"

using std::vector;
using std::map;
using std::set;

class ChainManager:	public Subscriber
{
	public:
		ChainManager();
		virtual ~ChainManager();

		virtual bool requestChain(const string& req);
		virtual string getAddressNext();
	protected:
		virtual bool setAddress(const string& req);
		virtual bool collectItems();
		virtual void collectInstruments(MsgGraph *rset);
	private:
		vector<string> _vecRequest;
		string _connstr, _host, _port;
		string _source;
		map<int, string> _chainMap;
		int _next;
};

#endif //CHAINMANAGER_HPP
