
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

#include <strings.h>

#include "DataCache.hpp"
#include "ConfigMap.hpp"
#include "DBConnectorLdap.hpp"
#include "RecordArrayConverter.hpp"

RecordArrayConverter::RecordArrayConverter(const string& ldapurl):_arrayUrl(ldapurl)
			
{

	//url = ldap://192.168.5.104:389/ou=Translator, o=moneyline.ca
	_connstr = ""; //"/ou=Translator, o=moneyline.ca";
	_host = ""; //"192.168.5.104";
	string id = "ldap://";
	int ldap_id = ldapurl.find(id);
	if (ldap_id != string::npos){
		int port_id = ldapurl.find_first_of(":", ldap_id + id.size() + 1);
		if (port_id != string::npos){
			_host = ldapurl.substr(ldap_id +id.size(), port_id - (ldap_id + id.size()));
			int conn_id = ldapurl.find_first_of("/", port_id + 1);
			if (conn_id != string::npos){
				_port = ldapurl.substr(port_id + 1, conn_id - (port_id + 1));
				_connstr = ldapurl.substr(conn_id , ldapurl.size() - conn_id);
			}
		}
	}
}

RecordArrayConverter::~RecordArrayConverter()
{
	map<string, RecordConverter *>::iterator it = _recArray.begin();
	for (it; it != _recArray.end(); it++){
		RecordConverter *pc = (RecordConverter *)(*it).second;
		delete pc;
	}
	_recArray.clear();

}

void
RecordArrayConverter::load(const string& s)
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "RecordArrayConverter::load %s %s %s %s", _host.c_str(), _port.c_str(), _connstr.c_str(), s.c_str());
	DBConnectorLdap *conn = new DBConnectorLdap(_host, _port);
	if (conn->connect(_connstr)){
		if (conn->fetch(s)){
			MsgGraph * rset = conn->search();
			if (rset){
				processArray(rset);
			}
		}
	}
	conn->disconnect();
	delete conn;
}


void
RecordArrayConverter::processArray(MsgGraph *rset)
{
	set<string> keys = rset->getKeys("");
	set<string>::iterator it = keys.begin();
	for (it; it != keys.end(); it++){
		Logger::getLogger()->log(Logger::TYPE_INFO, "RecordArrayConverter::processArray Keys found %s", (*it).c_str());
		if ((*it).size()){
			string recName;

			rset->getAttributeValue("chainRec@" + (*it), 0, recName);
			string idnName = RecordConverter::getIdnName((*it));
			if ( recName.size() != 0 && idnName.size() != 0){
				RecordConverter *pc = new RecordConverter(recName, idnName);
				DataCache::getCache()->subscribe(recName, pc, Datum::GRAPH);
				cout << "subscribe for "<<recName<<endl;
				_recArray[idnName] = pc;
			}else {
				Logger::getLogger()->log(Logger::TYPE_ERROR, "RecordArrayConverter::processArray failed for [%s], [%s]", recName.c_str(), idnName.c_str());

			}
		}
	}
}



