
/************************************************************************
||
|| DATE:                $Date: 2004/10/06 19:44:25 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/ParserOpenServer.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ParserOpenServer.hpp,v 1.1 2004/10/06 19:44:25 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ParserOpenServer.hpp,v $
|| LOG:                 Revision 1.1  2004/10/06 19:44:25  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:42:08  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef PARSEROPENSERVER_HPP
#define PARSEROPENSERVER_HPP

#include <map>
#include <string>

#include "OpenServer.hpp"
#include "ParserHttpClient.hpp"
#include "NSMap.hpp"

using std::string;
using std::map;

class ParserOpenServer: public OpenServer
{
	public:
		ParserOpenServer(const ConfigMap &confMap);
		virtual ~ParserOpenServer();
		virtual void removeClient(ParserHttpClient *cl);
		virtual void setNameSpace(const NSMap& nsmap){
			_my_namespace = nsmap;
		}

		virtual const NSMap& getNameSpace()const{
			return _my_namespace;
		}
	protected:

		virtual void onRead (int fid, InputId id);
		virtual void onWrite (int fid, InputId id){};
		virtual void onExcept (int fid, InputId id){};
		map<int, ParserHttpClient *> _mapClients;		
		string _root_ns;
		NSMap _my_namespace;

};
#endif//PARSEROPENSERVER_HPP
