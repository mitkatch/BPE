
/************************************************************************
||
|| DATE:                $Date: 2005/11/17 21:57:58 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/espeedII/RCS/ESOpenServer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESOpenServer.hpp,v 1.1 2005/11/17 21:57:58 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ESOpenServer.hpp,v $
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ESOPENSERVER_HPP
#define ESOPENSERVER_HPP

#include <map>
#include <string>

#include "OpenServer.hpp"
#include "ESHttpClient.hpp"

using std::string;
using std::map;

class ESOpenServer: public OpenServer
{
	public:
		ESOpenServer(const ConfigMap &confMap);
		virtual ~ESOpenServer();
		virtual void removeClient(ESHttpClient *cl);
		virtual string root()const{
			return _root_ns;
		}
	protected:

		virtual void onRead (int fid, InputId id);
		virtual void onWrite (int fid, InputId id){};
		virtual void onExcept (int fid, InputId id){};
		map<int, ESHttpClient *> _mapClients;
		string _root_ns;

};
#endif//ESOPENSERVER_HPP
