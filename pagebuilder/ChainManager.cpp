
/************************************************************************
||
|| DATE:                $Date: 2006/05/02 15:12:08 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/ChainManager.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ChainManager.cpp,v 1.3 2006/05/02 15:12:08 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: ChainManager.cpp,v $
|| LOG:                 Revision 1.3  2006/05/02 15:12:08  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.2  2006/04/06 17:57:28  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:                 Revision 1.1  2005/09/21 17:49:55  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "Scheduler.hpp"
#include "ConfigMap.hpp"
#include "ChainManager.hpp"


ChainManager::ChainManager():_next(1)
{

}

ChainManager::~ChainManager()
{

}


bool
ChainManager::requestChain(const string& req)
{
    bool ret = false;
    
    vector<string>::iterator it = std::find(_vecRequest.begin(), _vecRequest.end(), req);

    if (it == _vecRequest.end())
    {
	if(setAddress(req))
	    collectItems();

	_vecRequest.push_back(req);	
	ret = true;
	Logger::getLogger()->log(Logger::TYPE_INFO, "ChainManager::requestChain processing %s", req.c_str());
    } 
    
    return ret;
}

bool
ChainManager::setAddress(const string& req)
{
	//req = "producer=canpx2, ou=Chain, o=moneyline.ca@ldap://192.168.5.104:389"
	// if no @ it means <default_host>::389
	bool ret = true;
	int host_id = req.find_last_of('@');
	if (host_id != string::npos){
		_connstr= req.substr(0, host_id);
		string hosturl = req.substr(host_id +1, req.size() - (host_id +1));
		string dummy;
		DBConnectorLdap::getConnPar(hosturl, _host, _port, dummy);
	}else{
		_connstr=req;
		if (_connstr.empty())
			ret = false;
		string hosturl = ConfigMap::getConfig()->getValue("LDAP_URL");
		if (hosturl.empty()){
			ret = false;
		}else{
			string dummy;
			DBConnectorLdap::getConnPar(hosturl, _host, _port, dummy);
		}

	}

	return ret;
}

bool
ChainManager::collectItems()
{
	bool ret = false;
	string searchurl = ConfigMap::getConfig()->getValue("LDAP_SEARCH_ITEM");
	DBConnectorLdap *conn = new DBConnectorLdap(_host, _port);
	if (conn->connect(string("/") +_connstr)){
		if (conn->fetch(searchurl)){
			MsgGraph * rset = conn->search();
			if (rset){
				collectInstruments(rset);
			}
		}
	}
	conn->disconnect();
	delete conn;

	return ret;
}

void
ChainManager::collectInstruments(MsgGraph *rset)
{
	string producer = "producer=";
	set<string> keys = rset->getKeys("");
	set<string>::iterator it = keys.begin();
	for(it; it!= keys.end(); it++){
		if ((*it).size()){
			string num, rec;
			string sit = (*it);
			rset->getAttributeValue("item@" + sit, 0, num);
			rset->getAttributeValue("chainRec@" + sit, 0, rec);
			int num_i = Formatter::ATOF(num.c_str());	

			std::pair<map<int, string>::iterator, bool> itadd = _chainMap.insert(map<int, string>::value_type(num_i, rec));
			//cout<<"collectInstruments ["<<num<<"] = "<<rec<<endl;
			if (itadd.second == false){
				Logger::getLogger()->log(Logger::TYPE_ERROR, "ChainManager::collectInstruments failed _chainMap.insert for [%i] = %s", num_i, rec.c_str());
			}
		}
	}
}

string
ChainManager::getAddressNext()
{
	string address;
	map<int, string>::iterator it = _chainMap.find(_next);
	if (it != _chainMap.end()){
		address = (*it).second;
		_next++;
	}
	return address;
}


