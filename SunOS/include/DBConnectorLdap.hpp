/************************************************************************
||
|| DATE:                $Date: 2005/09/09 16:56:21 $
|| SOURCE:              $Source: /repository/projects/Canpx/libdbldaputil/DBConnectorLdap.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DBConnectorLdap.hpp,v 1.2 2005/09/09 16:56:21 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: DBConnectorLdap.hpp,v $
|| LOG:                 Revision 1.2  2005/09/09 16:56:21  mikhailt
|| LOG:                 add, delete to/from LDAP
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 18:11:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2005/06/21 15:35:21  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef DBCONNECTORLDAP_HPP
#define DBCONNECTORLDAP_HPP
#include <string>
#include <vector>
#include <map>
#include <iostream.h>
#include <lber.h>
#include <ldap.h>

using std::string;
using std::vector;
using std::map;

#include "MsgGraph.hpp"
#include "DBConnector.hpp"
#include "Logger.hpp"

class DBConnectorLdap : public DBConnector
{
	public:
		enum {LDAP_DISCONNECTED, LDAP_CONNECTED};
		DBConnectorLdap(const string& host, const string& port); 
		DBConnectorLdap(const string& host, 
				const string& port,
				const string& user,
				const string& passwd); 
		virtual ~DBConnectorLdap();
		
		virtual bool fetch(const string& tmt);
		virtual bool connect(const string& conn);
		virtual MsgGraph *search();
		virtual void disconnect(); 
		virtual void setCounter(int i){
			_recCounter = i;
		}

		virtual bool addEntry(const string& url, MsgGraph& graph);
		virtual bool deleteLeaf(const string& url);

		static void getConnPar(const string& ldapurl, string& host, string& port, string& con);
	protected:
		bool get_data(MsgGraph *rset);

		void onExit();

		string _host, _port, _conn;
		string _user, _passwd;
		int _status;
		LDAP *_ld;
		LDAPMessage* _pMsg;
		MsgGraph * _myRecordSet;
		int _recCounter;

};
#endif //DBCONNECTORLDAP_HPP
