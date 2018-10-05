
/************************************************************************
||
|| DATE:                $Date: 2004/10/20 19:47:10 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/Subscriber.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Subscriber.cpp,v 1.3 2004/10/20 19:47:10 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: Subscriber.cpp,v $
|| LOG:                 Revision 1.3  2004/10/20 19:47:10  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.2  2004/09/23 17:54:33  mikhailt
|| LOG:                 using Datum
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:52:40  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include "Logger.hpp"
#include "Subscriber.hpp"
#include "DataGraph.hpp"
#include "DataPage.hpp"
#include "Datum.hpp"


Subscriber::Subscriber():_pFormatter(NULL)
{

}

Subscriber::~Subscriber()
{
	if (_pFormatter != NULL){
		delete _pFormatter;
		cout<<"Formatter deleted...+"<<endl;
	}
}

/********************
void
Subscriber::notify(DataGraph *data)
{
}

void
Subscriber::notify(DataPage *data)
{
}
******************/

void
Subscriber::notify(Datum * datum)
{


}

void
Subscriber::setSubscribedFields(string& address, string& in, string& out)
{
	if (out.empty()){
		out = in;
		//if user doesn't provide out field it means out = in subscribed field
	}

	map<string, FieldMap>::iterator it = _mapField.find(address);
	if (it != _mapField.end()){
		(*it).second.fields[in] = out;
	} else {	
		//_mapField[address][in]=out;
		FieldMap newFM;
		newFM.fields[in] = out;
		_mapField.insert(map<string, FieldMap>::value_type(address, newFM));
	}

}

void
Subscriber::getMappedField(string &address, FieldMap &fm)
{
	//string str;

	map<string, FieldMap >::iterator it1 = _mapField.find(address);
	if (it1 != _mapField.end()){
		fm = (*it1).second;
		/*********
		map<string, string>::iterator it2 = (*it1).second.find(in);
		if (it2 != (*it1).second.end()){
			str = (*it2).second;
		}
		*********/
	}
	//return str;
}

void
Subscriber::removeMappedFields(string &address)
{
	_mapField.erase(address);

}

void
Subscriber::setHeaderFooter( string& address, string& header, string& footer)
{
	map<string, FieldMap>::iterator it = _mapField.find(address);
	if (it != _mapField.end()){
		(*it).second.header = header;
		(*it).second.footer = footer;
	} else {	
		FieldMap newFM;
		newFM.header = header;
		newFM.footer = footer;
		_mapField.insert(map<string, FieldMap>::value_type(address, newFM));
	}


}
