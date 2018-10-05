
/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:29:17 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/ChainManager.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ChainManager.cpp,v 1.4 2006/06/08 15:29:17 nickz Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: ChainManager.cpp,v $
|| LOG:                 Revision 1.4  2006/06/08 15:29:17  nickz
|| LOG:                 ported to managed publishing
|| LOG:
|| LOG:                 Revision 1.3  2006/04/03 19:36:53  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.2  2005/09/13 19:01:44  mikhailt
|| LOG:                 chain fixed
|| LOG:
|| LOG:                 Revision 1.1  2005/09/12 21:56:39  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "ChainManager.hpp"
#include "DBConnectorLdap.hpp"
#include "CanpxApp.hpp"
#include "DataCache.hpp"
#include "CanpxEngine.hpp"

ChainManager * ChainManager::_chMgr = NULL;
ChainManager::ChainManager()
{
	//string _host = "192.168.5.104";
	//string _port = "389";
	//string connstr = "/ou=Chain, o=moneyline.ca";
	//string url = "?producer?sub?producer=*";
	string url = ConfigMap::getConfig()->getValue("LDAP_SEARCH_PRODUCER"); 
	_user = ConfigMap::getConfig()->getValue("LDAP_LOGIN");
	_passwd = ConfigMap::getConfig()->getValue("LDAP_PASSWORD"); 
	_name = theCanpxApp->name();
	_producer= string("producer=") + _name; 
	DBConnectorLdap::getConnPar(ConfigMap::getConfig()->getValue("LDAP_CHAIN_URL"), _host, _port, _connstr);
	if (_connstr[0] == '/')
		_connSuffix = _connstr.substr(1, _connstr.size() -1);
	else
		_connSuffix = _connstr;

	init();

	DBConnectorLdap *conn = new DBConnectorLdap(_host, _port);
	if (conn->connect(_connstr)){
		if (conn->fetch(url)){
			updateLdapEntry(conn->search());
		}
	}
	conn->disconnect();
	delete conn;
}

ChainManager::~ChainManager()
{

}

ChainManager*
ChainManager::getChain()
{
	if (_chMgr == NULL){
		_chMgr = new ChainManager();
	}
	return _chMgr;
}

void
ChainManager::update()
{
	//load chain and delete it
	// upload new one

	MsgGraph *rset = NULL;
	string url = ConfigMap::getConfig()->getValue("LDAP_SEARCH_CHAIN_ITEM");
	DBConnectorLdap *conn = new DBConnectorLdap(_host, _port);
	string connstr = string("/") +_producer + string(", ") + _connSuffix;
	if (conn->connect(connstr)){
		if (conn->fetch(url)){
			rset = conn->search();
			if (rset){
				deleteOldChain(rset);
			}	
		}
	}
	conn->disconnect();
	delete conn;

	if (_chain.size()){
		_chain.clear();
	}

	set<string> subj = DataCache::getCache()->getAddressSet();
	string prefix = ConfigMap::getConfig()->getValue("CANPX_ENGINE.SUBSCRIBER");
	set<string>::iterator it = subj.begin();

	for(it; it != subj.end(); it++){

		if ( (*it).find(prefix) != string::npos){
			double i = CanpxEngine::getEngine()->toChain((*it));
			Logger::getLogger()->log(Logger::TYPE_INFO, "ChainManager::update try to add %s as %f", (*it).c_str(), i);
			if ( i > 0 ){
				if ( _chain.count(i) == 0){
					_chain[i]  = (*it);

				}
			}
		}
	}

	if (_chain.size()){
		addNewChain();	
	}
	
	

}

void
ChainManager::init()
{

}
void
ChainManager::updateLdapEntry( MsgGraph *rset)
{
    string producer = _producer + ",";

    bool found = false;
    if (rset)
    {
	set<string> keys = rset->getKeys("");
	set<string>::iterator it = keys.begin();
	
	for (it; it != keys.end() && !found; it++)
	{
	    string val;
	    string attr = (*it);
	    rset->getAttributeValue((*it), 0, val);
	    
	    if (attr.find(producer) != string::npos)
	    {
		found = true;
	    }	
	} 
    }
    
    if ( !found )
    {
	MsgGraph ng ;
	string entry = producer + " " + _connSuffix;
	ng.addEntry(entry);
	ng.addGraphNode(entry, 0, "objectclass", "top");
	ng.addGraphNode(entry, 1, "objectclass", "chainProducer");
	ng.addGraphNode(entry, 2, "producer", _name);
	int lev = ng.getLevels(entry);
	DBConnectorLdap *conn = new DBConnectorLdap(_host, _port, _user, _passwd);
	if (conn->connect(_connstr))
	{
	    conn->addEntry(entry, ng);
	}
	conn->disconnect();
	delete conn;
    }
}


int
ChainManager::deleteOldChain(MsgGraph *rset)
{
	int count = 0;
	set<string> keys = rset->getKeys("");
	set<string>::iterator it = keys.begin();
	for (it ; it != keys.end(); it++){
		//cout << "ChainManager found old "<<(*it)<<endl;
		DBConnectorLdap *conn = new DBConnectorLdap(_host, _port, _user, _passwd);
		string connstr = string("/") +_producer + string(", ") + _connSuffix;
		if (conn->connect(connstr)){
			//cout<< "ChainManager connected "<<connstr<<endl;
			conn->deleteLeaf((*it));
			count++;
		}
		conn->disconnect();
		delete conn;
	}

	return count;
}

int
ChainManager::addNewChain()
{
	int count = 0;
	int item = 1;
	char c_ind[15];
	char lng[15];
	string connstr = string("/") +_producer + string(", ") + _connSuffix;
	map<double, string >::iterator it = _chain.begin();
	for (it ; it != _chain.end(); it++){
		sprintf(lng, "%f", (*it).first);
		Logger::getLogger()->log(Logger::TYPE_INFO, "ChainManager::addNewChain add %s [%s]", lng, ((*it).second).c_str());
		MsgGraph ng ;
		sprintf(c_ind, "%d", item);	
		string s_item = string("item=") + string(c_ind) + string(", ");
		string entry = s_item + _producer + string(", ") + _connSuffix;
		ng.addEntry(entry);
		ng.addGraphNode(entry, 0, "objectclass", "top");
		ng.addGraphNode(entry, 1, "objectclass", "chainItem");
		ng.addGraphNode(entry, 2, "item", c_ind);
		ng.addGraphNode(entry, 3, "chainRec", (*it).second);
		int lev = ng.getLevels(entry);
		DBConnectorLdap *conn = new DBConnectorLdap(_host, _port, _user, _passwd);
		if (conn->connect(connstr)){
			conn->addEntry(entry, ng);
			count++;
		}
		conn->disconnect();
		delete conn;
		item++;
	}
	return count;
}

