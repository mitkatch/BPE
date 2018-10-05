/************************************************************************
||
|| DATE:                $Date: 2005/06/24 16:21:53 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxOpenServer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxOpenServer.hpp,v 1.1 2005/06/24 16:21:53 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: CanpxOpenServer.hpp,v $
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CANPXOPENSERVER_HPP
#define CANPXOPENSERVER_HPP

#include <map>
#include <string>

#include "OpenServer.hpp"
#include "CanpxHttpClient.hpp"

using std::string;
using std::map;

class CanpxOpenServer: public OpenServer
{
	public:
		CanpxOpenServer(const ConfigMap &confMap);
		virtual ~CanpxOpenServer();
		virtual void removeClient(CanpxHttpClient *cl);
		virtual string root()const{
			return _root_ns;
		}
	protected:

		virtual void onRead (int fid, InputId id);
		virtual void onWrite (int fid, InputId id){};
		virtual void onExcept (int fid, InputId id){};
		map<int, CanpxHttpClient *> _mapClients;
		string _root_ns;

};
#endif//CANPXOPENSERVER_HPP
