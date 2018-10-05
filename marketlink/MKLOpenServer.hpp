/************************************************************************
||
|| DATE:                $Date:$
|| SOURCE:              $Source:$
|| STATE:               $State:$
|| ID:                  $Id:$
|| REVISION:            $Revision:$
|| LOG:                 $Log:$
|| LOG:
************************************************************************/

#ifndef MKLOPENSERVER_HPP
#define MKLOPENSERVER_HPP

#include <map>
#include <string>

#include "OpenServer.hpp"
#include "MKLHttpClient.hpp"

using std::string;
using std::map;

class MKLOpenServer: public OpenServer
{
	public:
		MKLOpenServer(const ConfigMap &confMap);
		virtual ~MKLOpenServer();
		virtual void removeClient(MKLHttpClient *cl);
		virtual string root()const{
			return _root_ns;
		}
	protected:

		virtual void onRead (int fid, InputId id);
		virtual void onWrite (int fid, InputId id){};
		virtual void onExcept (int fid, InputId id){};
		map<int, MKLHttpClient *> _mapClients;
		string _root_ns;

};
#endif//MKLOPENSERVER_HPP
