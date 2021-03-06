
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
