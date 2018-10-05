
/************************************************************************
||
|| DATE:                $Date: 2006/04/06 17:57:30 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PageArrayConverter.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PageArrayConverter.cpp,v 1.4 2006/04/06 17:57:30 nickz Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: PageArrayConverter.cpp,v $
|| LOG:                 Revision 1.4  2006/04/06 17:57:30  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:                 Revision 1.3  2006/04/04 13:19:32  nickz
|| LOG:                 ported to rmds
|| LOG:
|| LOG:                 Revision 1.2  2005/09/28 19:23:40  mikhailt
|| LOG:                 re-use duplicated LDAP messages
|| LOG:
|| LOG:                 Revision 1.1  2005/09/21 17:49:55  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/
#include <strings.h>

#include "ConfigMap.hpp"
#include "DBConnectorLdap.hpp"
#include "PageArrayConverter.hpp"

PageArrayConverter::PageArrayConverter(const string& ldapurl):_arrayUrl(ldapurl)
			, _chainMgr(NULL)
{

	//url = ldap://192.168.5.104:389/ou=PageBuilder, o=moneyline.ca
	_connstr = ""; //"/ou=PageBuilder, o=moneyline.ca";
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

PageArrayConverter::~PageArrayConverter()
{
	map<string, PageConverter *>::iterator it = _pageArray.begin();
	for (it; it != _pageArray.end(); it++){
		PageConverter *pc = (PageConverter *)(*it).second;
		delete pc;
	}
	_pageArray.clear();

	if (_chainMgr)
		delete _chainMgr;
}

void
PageArrayConverter::load(const string& s)
{
	cout<<"host: "<<_host << " port:"<<_port<<" connstr:"<<_connstr<<" s:"<<s<<endl;
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
PageArrayConverter::processArray(MsgGraph *rset)
{
	set<string> keys = rset->getKeys("");
	set<string>::iterator it = keys.begin();
	for (it; it != keys.end(); it++){
		Logger::getLogger()->log(Logger::TYPE_INFO, "PageArrayConverter::processArray Keys found %s", (*it).c_str());
		if ((*it).size()){
			string ptype, pname;

			rset->getAttributeValue("pageType@" + (*it), 0, ptype);
			if (strcasecmp(ptype.c_str(), "dynamic") == 0){
				rset->getAttributeValue("pageName@" + (*it), 0, pname);
				//cout << "Found type ["<<ptype<<"], name ["<<pname<<"]"<<endl;
				addDynamic((*it), rset);
			}else{
				string pname = PageConverter::getPageName((*it));
				PageConverter *pc = new PageConverter(this, (*it), pname);
				pc->load(ConfigMap::getConfig()->getValue("LDAP_SEARCH_ROW"));
				_pageArray[pname] = pc;
			}


		}
	}
}


void
PageArrayConverter::addDynamic(const string& url, MsgGraph *rset)
{
	string pname;
	rset->getAttributeValue("pageName@" + url, 0, pname);
	Formatter::split(pname, ",", _dynpnmV);

	if(_chainMgr == NULL) 
		_chainMgr = new ChainManager();

	for (int i = 0; i < _dynpnmV.size(); i++ ){
		//cout<<"PageArrayConverter::addDynamic page "<<_dynpnmV[i]<<endl;
		PageConverter *pc = new PageConverter(this, url, _dynpnmV[i]);
		pc->dynamic(true);
		pc->setChainMgr(_chainMgr);

		if((_pageArray.insert(map<string, PageConverter*>::value_type(_dynpnmV[i], pc))).second ){
			Logger::getLogger()->log(Logger::TYPE_INFO, "PageArrayConverter::addDynamic %s page added", _dynpnmV[i].c_str());
				pc->load(ConfigMap::getConfig()->getValue("LDAP_SEARCH_ROW"));
		} else {
			Logger::getLogger()->log(Logger::TYPE_ERROR, "PageArrayConverter::addDynamic %s page failed", _dynpnmV[i].c_str());
			delete pc;
		}
	}
}

