
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

#include "FieldFormatter.hpp"


FieldFormatter* FieldFormatter::_ff = NULL;
map<string, AField *> _formatMap;

FieldFormatter::FieldFormatter()
{
	_formatMap[""] = new AField();
	_formatMap["TIME5"] = new Time5Field();
	_formatMap["TIME8"] = new Time8Field();
	_formatMap["DATE"] = new DateField();
	_formatMap["STATIC"] = new StaticField();
}


FieldFormatter::~FieldFormatter()
{
	map<string, AField* >::iterator it = _formatMap.begin();
	for(it; it != _formatMap.end(); it++){
		Logger::getLogger()->log(Logger::TYPE_INFO, "~FieldFormatter delete %s ", (*it).first);
		delete (*it).second;
	}

}


//static
AField*
FieldFormatter::getFormatter(const string& f)
{
	AField *format = NULL;
	if (_ff == NULL)
		_ff = new FieldFormatter();

	
	map<string, AField* >::iterator it = _formatMap.find(f);
	if (it != _formatMap.end()){
		format = (*it).second;
	}else{
		map<string, AField* >::iterator it0 = _formatMap.find("");
		format = (*it0).second;
	}

	return format;
}

DataGraph::Variant
AField::formatVariant(const DataGraph::Variant& v)
{
	return v;

}

DataGraph::Variant
Time5Field::formatVariant(const DataGraph::Variant& v)
{
	// "DD/MM/YY HH:MM:SS"
	string str;
	bool done = false;
	DataGraph::Variant newV;
	if (v.type == DataGraph::SVAL){
		str = string(v.data.sval);	
		int del = str.find(" ");
		if (del != string::npos){
			str = str.substr(del + 1, str.size() - del -1);
			str = str.substr(0, 5);
			DataGraph::Variant tmpV(DataGraph::SVAL, str.c_str());
			newV = tmpV;
			done = true;
		}	
	}
	return (done? newV : v);
}

DataGraph::Variant
Time8Field::formatVariant(const DataGraph::Variant& v)
{
	// "DD/MM/YY HH:MM:SS"
	string str;
	bool done = false;
	DataGraph::Variant newV;
	if (v.type == DataGraph::SVAL){
		str = string(v.data.sval);	
		int del = str.find(" ");
		if (del != string::npos){
			str = str.substr(del + 1, str.size() - del -1);
			DataGraph::Variant tmpV(DataGraph::SVAL, str.c_str());
			newV = tmpV;
			done = true;
		}	
	}
	return (done? newV : v);
}

DataGraph::Variant
DateField::formatVariant(const DataGraph::Variant& v)
{
	// "DD/MM/YY HH:MM:SS"
	string str;
	bool done = false;
	DataGraph::Variant newV;
	if (v.type == DataGraph::SVAL){
		str = string(v.data.sval);	
		int del = str.find(" ");
		if (del != string::npos){
			str = str.substr(0, del);
			DataGraph::Variant tmpV(DataGraph::SVAL, str.c_str());
			newV = tmpV;
			done = true;
		}	
	}
	return (done? newV : v);
}

