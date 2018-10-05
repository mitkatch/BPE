
/************************************************************************
||
|| DATE:                $Date: 2005/09/13 19:01:44 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/ChainManager.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ChainManager.hpp,v 1.2 2005/09/13 19:01:44 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: ChainManager.hpp,v $
|| LOG:                 Revision 1.2  2005/09/13 19:01:44  mikhailt
|| LOG:                 chain fixed
|| LOG:
|| LOG:                 Revision 1.1  2005/09/12 21:57:06  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CHAINMANAGER_HPP
#define CHAINMANAGER_HPP

#include <string>
#include <utility>
#include "MsgGraph.hpp"

using std::string;
using std::pair;

class ChainManager
{
	public:
		virtual ~ChainManager();
		static ChainManager * getChain();
		virtual void update();
	protected:
		static ChainManager* _chMgr;
		ChainManager();
		virtual void updateLdapEntry( MsgGraph *rset);
		virtual int deleteOldChain(MsgGraph *rset);
		virtual int addNewChain();
		virtual void init();
	private:
		string _host, _port, _user, _passwd, _connstr;	
		string _name, _producer, _connSuffix;
		map<double, string > _chain;
};


#endif //CHAINMANAGER_HPP
