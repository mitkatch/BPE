/************************************************************************
||
|| DATE:                $Date: 2006/05/02 15:14:22 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbldaputil/DBConnectorLdap.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBConnectorLdap.cpp,v 1.4 2006/05/02 15:14:22 nickz Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: DBConnectorLdap.cpp,v $
|| LOG:                 Revision 1.4  2006/05/02 15:14:22  nickz
|| LOG:                 commented out extraneous log message
|| LOG:
|| LOG:                 Revision 1.3  2005/09/21 17:05:44  mikhailt
|| LOG:                 printf commented out
|| LOG:
|| LOG:                 Revision 1.2  2005/09/09 16:56:21  mikhailt
|| LOG:                 add, delete to/from LDAP
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 18:11:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2005/06/21 15:35:04  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <strings.h>
#include <string.h>
#include <errno.h>

#include "Logger.hpp"
#include "DBConnectorLdap.hpp"

DBConnectorLdap::DBConnectorLdap(const string& host, const string& port):DBConnector(host, port, ""), _host(host), _port(port),  _status(LDAP_DISCONNECTED), _ld(NULL), _pMsg(NULL), _myRecordSet(NULL)
{

}

DBConnectorLdap::DBConnectorLdap(const string& host, 
				const string& port,
				const string& user,
				const string& passwd):
				DBConnector(host, port, ""), 
				_host(host), _port(port),  
				_user(user), _passwd(passwd),
				_status(LDAP_DISCONNECTED), 
				_ld(NULL), 
				_pMsg(NULL), _myRecordSet(NULL)
{

}

DBConnectorLdap::~DBConnectorLdap()
{
	if (_myRecordSet)
		delete _myRecordSet;

}

void
DBConnectorLdap::onExit()
{
	if (_pMsg){
		ldap_msgfree(_pMsg); 
		_pMsg = NULL;
	}

	if (_ld){
		ldap_unbind_s(_ld); 
		_ld = NULL;
	}
	_status = LDAP_DISCONNECTED;	

}

bool
DBConnectorLdap::connect(const string& conn)
{
	bool ret = false;
	string myPort;
	_conn = conn;
	if (_port.empty()){
		_port = "389";
	}
	string searchURL = "ldap://" + _host + ":" + _port + _conn; 
	//cout<<"ldap_is_ldap_url "<<searchURL<<endl;
	if ( ldap_is_ldap_url( (char *)searchURL.c_str() ) == 0 ) {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "DBConnectorLdap::connect %s : %i Error:is not LDAP url %s", _host.c_str(), _port, _conn);
		_status = LDAP_DISCONNECTED;
		return ret;
	}

	int port = (_port.empty()? 389 :atoi(_port.c_str()));
	//cout << "ldap_open " << _host<<":" << port<<endl;
	if ( ( ( _ld = ldap_open( (char *)_host.c_str(), port)) ) == NULL ) {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "DBConnectorLdap::connect %s : %i Error %s", _host.c_str(), _port, strerror(errno));
		_status = LDAP_DISCONNECTED;
	} else {
		_status = LDAP_CONNECTED;
	}	

	return  _status == LDAP_CONNECTED;
}

bool
DBConnectorLdap::addEntry(const string& url, MsgGraph& graph)
{
	if (_status != LDAP_CONNECTED)
		return false;

	int ver = LDAP_VERSION3;
	ldap_set_option(_ld, LDAP_OPT_PROTOCOL_VERSION, (void *)&ver);
	if (ldap_bind_s(_ld, (char*)_user.c_str(), (char*)_passwd.c_str(), LDAP_AUTH_SIMPLE) == -1) {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "DBConnectorLdap::addEntry %s : Error: %s", url.c_str(), ldap_err2string(ldap_result2error(_ld, _pMsg, 0)));
		onExit();
		return false;
	}
	int level = graph.getLevels(url);
	LDAPMod **mod = (LDAPMod **) new float[level +1];
	LDAPMod **mod_tmp = mod;
	set<string> uniqueAttr;
	for (int lev = 0; lev < level; lev++){ 

		set<string> entr = graph.getKeys(url, lev);

		set<string>::iterator it = entr.begin();
		char* c_attr = NULL;
		char **m_val = NULL; // (char **) new float[2];
		char **m_val_tmp = NULL; //m_val;
		if (it != entr.end()){
			string attr = (*it);
			set<string>::iterator uit = uniqueAttr.find(attr);
			if (uit == uniqueAttr.end()){
				uniqueAttr.insert(attr);
			} else {
				continue; //exclude duplications of the attr
			}
			c_attr = strdup(attr.c_str());
			vector<string> val;
			if(graph.getAttributeValue(attr + string("@") + url, val)){
				m_val = (char **) new float[val.size() +1];
				m_val_tmp = m_val;
				for (int z = 0; z < val.size(); z++){
					char *c_val = strdup(val[z].c_str());
					*m_val_tmp = c_val;
					m_val_tmp++;
				}
			}
		}
		*m_val_tmp = NULL; //close
		LDAPMod *mod_one = new LDAPMod;
		mod_one->mod_op = LDAP_MOD_ADD;
		mod_one->mod_type = c_attr;
		mod_one->mod_values = m_val;	
		*mod_tmp = mod_one;
		mod_tmp++;
	}
	*mod_tmp = NULL;

	if (ldap_add_s(_ld, (char*)url.c_str(), mod) != LDAP_SUCCESS){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "DBConnectorLdap::addEntry %s : Error: %s", url.c_str(), ldap_err2string(ldap_result2error(_ld, _pMsg, 0)));
		ldap_mods_free(mod, 1);
		return false;
	}
	ldap_mods_free(mod, 1);
	return true;
}

bool
DBConnectorLdap::deleteLeaf(const string& url)
{
	if (_status != LDAP_CONNECTED)
		return false;

	int ver = LDAP_VERSION3;
	ldap_set_option(_ld, LDAP_OPT_PROTOCOL_VERSION, (void *)&ver);
	if (ldap_bind_s(_ld, (char*)_user.c_str(), (char*)_passwd.c_str(), LDAP_AUTH_SIMPLE) == -1) {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "DBConnectorLdap::deleteLeaf %s : Error: %s", url.c_str(), ldap_err2string(ldap_result2error(_ld, _pMsg, 0)));
		onExit();
		return false;
	}

	if (ldap_delete_s(_ld, (char *)url.c_str()) != LDAP_SUCCESS){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "DBConnectorLdap::deleteLeaf %s : Error: %s", url.c_str(), ldap_err2string(ldap_result2error(_ld, _pMsg, 0)));
		onExit();
		return false;
	}

}


bool
DBConnectorLdap::fetch(const string& stmt)
{
	if (_status != LDAP_CONNECTED)
		return false;

	string searchUrl = "ldap://" + _host + ":" + _port +_conn + stmt;
	//char * newurl = strdup(searchUrl.c_str());
	//cout <<"ldap_url_search_s" <<newurl<<endl;
	if ( ldap_url_search_s( _ld, /*newurl*/ (char*)searchUrl.c_str(), 0, &_pMsg ) != LDAP_SUCCESS ) {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "DBConnectorLdap::fetch %s : Error: %s", searchUrl.c_str(), ldap_err2string(ldap_result2error(_ld, _pMsg, 0)));
		onExit();
		return false;
	}

	if (_pMsg == NULL){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "DBConnectorLdap::fetch %s : Error: empty _pMsg", searchUrl.c_str());
		onExit();
		return false;
	}

	return true;
}

MsgGraph*
DBConnectorLdap::search()
{
	if ( _myRecordSet){
		delete _myRecordSet;
		_myRecordSet = NULL;
	}

	int nCount = ldap_count_entries( _ld, _pMsg );
	if ( nCount <= 0) {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "DBConnectorLdap::execute %d entries found ", nCount);
		onExit();
	} else {
		_myRecordSet = new MsgGraph();
		if (!get_data(_myRecordSet)){
			Logger::getLogger()->log(Logger::TYPE_ERROR,"Failed to build DBRecordSet is empty" );
		}
	}
	return _myRecordSet;
}

bool
DBConnectorLdap::get_data(MsgGraph *rset)
{
    LDAPMessage* pEntry = NULL;
    string dn;	
    for ( pEntry = ldap_first_entry( _ld, _pMsg );
	  pEntry != NULL ;
	  pEntry = ldap_next_entry( _ld, pEntry ) )
    {
	char* pszDN = ldap_get_dn ( _ld, pEntry );	
	if ( pszDN != NULL ) 
	{
	    dn = string(pszDN);//value for "dn"
	    rset->addEntry(dn); //header is "dn"
	    //Logger::getLogger()->log(Logger::TYPE_INFO, 
	    //		     "DBConnectorLdap::get_data : addEntry %s", dn.c_str());
	    free( pszDN );
	} 
	else 
	{
	    Logger::getLogger()->log(Logger::TYPE_ERROR, 
				     "DBConnectorLdap::get_data : Error: %s", 
				     ldap_err2string(ldap_result2error(_ld, _pMsg, 0)));
	    onExit();
	    return false;
	}
	
	BerElement* pBer = NULL;
	char* pszAttrib = NULL;
	for ( pszAttrib = ldap_first_attribute( _ld, pEntry, &pBer );
	      pszAttrib != NULL;
	      pszAttrib = ldap_next_attribute( _ld, pEntry, pBer ) )
	{
	    char** ppszValues = NULL;
	    ppszValues = ldap_get_values( _ld, pEntry, pszAttrib );	
	    if ( ppszValues != NULL ) 
	    {
		for ( int i=0; ppszValues[i] != 0; i++ ) 
		{
		    //Logger::getLogger()->log(Logger::TYPE_INFO, 
		    //			     "DBConnectorLdap::get_data Attrib[%s] Value[%s]", 
		    //		     pszAttrib, ppszValues[i] );
		    //printf("Attrib[%s]: Value[%s]\n", pszAttrib, ppszValues[i] );
		    //string attr(pszAttrib);
		    rset->addGraphNode(dn, 0, pszAttrib, ppszValues[i]);
		}
	    } 
	    else 
	    {
		Logger::getLogger()->log(Logger::TYPE_ERROR, 
					 "DBConnectorLdap::get_data : get_values Error: %s", 
					 ldap_err2string(ldap_result2error(_ld, _pMsg, 0)));
	    }
	    ldap_value_free( ppszValues );
	}
	ber_free(pBer, 0);
    }

    onExit();	
    return true;
}

void
DBConnectorLdap::disconnect()
{
	onExit();
}

//static
void
DBConnectorLdap::getConnPar(const string& ldapurl, string& host, string& port, string& con)
{
	string id = "ldap://";
	int ldap_id = ldapurl.find(id);
	if (ldap_id != string::npos){
		int port_id = ldapurl.find_first_of(":", ldap_id + id.size() + 1);
		if (port_id != string::npos){
			host = ldapurl.substr(ldap_id +id.size(), port_id - (ldap_id + id.size()));
			int conn_id = ldapurl.find_first_of("/", port_id + 1);
			if (conn_id != string::npos){
				port = ldapurl.substr(port_id + 1, conn_id - (port_id + 1));
				con = ldapurl.substr(conn_id , ldapurl.size() - conn_id);
			}
		}
	}
}
