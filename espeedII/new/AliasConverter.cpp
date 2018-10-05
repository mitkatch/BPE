
/************************************************************************
||
|| DATE:                $Date: 2005/11/21 22:11:02 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/AliasConverter.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: AliasConverter.cpp,v 1.2 2005/11/21 22:11:02 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: AliasConverter.cpp,v $
|| LOG:                 Revision 1.2  2005/11/21 22:11:02  mikhailt
|| LOG:                 label->alais
|| LOG:
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <pthread.h>

#include "AliasConverter.hpp"
#include "Logger.hpp"
#include "DBConnectorLdap.hpp"
#include "Formatter.hpp"

AliasConverter* AliasConverter::_myConverter = NULL;
pthread_mutex_t _mutex;

AliasConverter::~AliasConverter()
{
	pthread_mutex_destroy(&_mutex);
}

AliasConverter::AliasConverter()
{
	pthread_mutex_init(&_mutex, NULL);
}

AliasConverter*
AliasConverter::getConverter()
{
	AliasConverter *ac = NULL;
	pthread_mutex_lock(&_mutex);
	if (_myConverter == NULL)
		_myConverter = new AliasConverter();
	ac = _myConverter;
	pthread_mutex_unlock(&_mutex);
	return ac;

}

void
AliasConverter::load(const string& url, const string& s)
{
	pthread_mutex_lock(&_mutex);
	string host, port, connstr;
	DBConnectorLdap::getConnPar(url, host, port, connstr);
	DBConnectorLdap *conn = new DBConnectorLdap(host, port);
	if (conn->connect(connstr)){
		if (conn->fetch(s)){
			MsgGraph * rset = conn->search();
			if (rset){
				processAliases(rset);
			}
		}
	}
	conn->disconnect();
	delete conn;
	pthread_mutex_unlock(&_mutex);
}

void
AliasConverter::processAliases(MsgGraph *rset)
{
	set<string> keys = rset->getKeys("");
	set<string>::iterator it = keys.begin();
	for (it; it != keys.end(); it++){
		Logger::getLogger()->log(Logger::TYPE_INFO, "AliasConverter::processAliases Key found %s", (*it).c_str());
		string rec ;
		rset->getAttributeValue("feedRec@" + (*it), 0, rec);
		if (!addAlias((*it), rec)){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "AliasConverter::processAliases failed addAlias [%s]->[%s]", (*it).c_str(), rec.c_str());
		}
	}

}

bool
AliasConverter::addAlias(const string& aliasUrl, const string& record)
{
	// aliasUrl "alias=MAY04,ou=CantorFeed,o=moneyline.ca"
	vector<string> v;
	string a_id = "alias=";
	Formatter::split(aliasUrl, ",", v);
	for (int i = 0; i <= v.size(); i++){
		int ia_id = v[i].find(a_id);
		if (ia_id != string::npos){
			string alias = v[i].substr(ia_id + a_id.size(), v[i].size() -(ia_id + a_id.size()));
			return (_aliasMap.insert(map<string, string>::value_type(alias, record))).second;
		}
		
	}
	return false;

}

string
AliasConverter::getAlias(const string& label)
{
	pthread_mutex_lock(&_mutex);

	string newLabel;
	for (unsigned i=0; i<label.length(); i++){
		if (label[i] == ' '){
			if (newLabel[newLabel.size() -1] != '_') //exclude double _
				newLabel.append(1, '_');
		}else if (label[i] == '/'){
			newLabel.append(1, '$');
		}else{
			newLabel.append(1, label[i]);
		}
	}

	map<string, string>::iterator it = _aliasMap.begin();
	for (it; it!= _aliasMap.end(); it++){
		if (newLabel == (*it).second){
			newLabel = (*it).first;
			Logger::getLogger()->log(Logger::TYPE_INFO, "AliasConverter::getAliases old [%s]->new [%s]", (label).c_str(), newLabel.c_str());
			break;
		}
	}
	pthread_mutex_unlock(&_mutex);
	return newLabel; 
}
