/************************************************************************
||
|| DATE:                $Date: 2006/04/28 20:06:47 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/DataGraph.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DataGraph.cpp,v 1.10 2006/04/28 20:06:47 mikhailt Exp $
|| REVISION:    $Revision: 1.10 $
|| LOG:                 $Log: DataGraph.cpp,v $
|| LOG:                 Revision 1.10  2006/04/28 20:06:47  mikhailt
|| LOG:                 copy constructor . no error checking
|| LOG:
|| LOG:                 Revision 1.9  2006/04/28 18:17:18  mikhailt
|| LOG:                 memory leak fixed
|| LOG:
|| LOG:                 Revision 1.8  2005/06/29 18:55:58  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.7  2005/03/29 16:09:36  mikhailt
|| LOG:                 delta added
|| LOG:
|| LOG:                 Revision 1.6  2004/12/03 18:42:56  mikhailt
|| LOG:                 handles HTTP POST
|| LOG:
|| LOG:                 Revision 1.5  2004/11/19 21:08:35  mikhailt
|| LOG:                 updated for Subscriber
|| LOG:
|| LOG:                 Revision 1.4  2004/11/15 17:52:41  mikhailt
|| LOG:                 more-less stable for parser
|| LOG:
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

#include <stdio.h>
#include "DataGraph.hpp"
#include "Logger.hpp"
#include "StorageManager.hpp"

//m_DataMutex;
//m_SubsctiberMutex;

DataGraph::Variant::Variant()
{
	type = DataGraph::NVAL;
	data.sval = NULL;
	data.dval = -1;
	data.lval = -1;
	
}

DataGraph::Variant::Variant(int t, const char *val)
{
	if(t == DataGraph::SVAL){
		type = t;
		data.sval = (val?strdup(val):strdup(""));
		data.lval = -1;
		data.dval = -1;
	} 
}

DataGraph::Variant::Variant(int t, double val)
{
	if(t == DataGraph::DVAL){
		type = t;
		data.dval = val;
		data.sval = NULL;
		data.lval = -1;
	} 

}

DataGraph::Variant::Variant(int t, long val)
{
	if(t == DataGraph::LVAL){
		type = t;
		data.lval = val;
		data.sval = NULL;
		data.dval = -1;
	} 

}

DataGraph::Variant::Variant(const Variant& var)
{
	//if (type == DataGraph::SVAL)
	//	delete data.sval;

	type = var.type;
	switch(type){
		case SVAL:
			data.sval = strdup(var.data.sval);
			data.dval = data.lval = -1;
			break;
		case LVAL:
			data.lval = var.data.lval;
			data.sval = NULL;
			data.dval = -1;
			break;
		case DVAL:
			data.dval = var.data.dval;
			data.sval = NULL;
			data.lval = -1;
			break;
		default:
			Logger::getLogger()->log(Logger::TYPE_ERROR, "DataGraph::Variant::Variant error, unkown Variant.type =%d", var.type);
	}
}

DataGraph::Variant&
DataGraph::Variant::operator=(const Variant& var)
{
	if (type == DataGraph::SVAL){
		if (data.sval != NULL){
			delete [] data.sval;
			data.sval = NULL;
		}
	}


	type = var.type;
	switch(type){
		case SVAL:
			data.sval = strdup(var.data.sval);
			data.lval = data.dval = -1;
			break;
		case LVAL:
			data.lval = var.data.lval;
			data.dval = -1;
			data.sval = NULL;
			break;
		case DVAL:
			data.dval = var.data.dval;
			data.sval = NULL;
			data.lval = -1;
			break;
		default:
			Logger::getLogger()->log(Logger::TYPE_ERROR, "DataGraph::Variant::operator= error, unkown Variant.type =%d", var.type);
	}

	return *this;
}

bool
DataGraph::Variant::operator==(const Variant& var)
{
	type = var.type;
	bool ret = false;;
	switch(type){
		case SVAL:
			ret = strcmp(data.sval, var.data.sval) == 0;
			break;
		case LVAL:
			ret = data.lval == var.data.lval;
			break;
		case DVAL:
			ret = data.dval == var.data.dval;
			break;
		default:
			Logger::getLogger()->log(Logger::TYPE_ERROR, "DataGraph::Variant::operator== error, unkown Variant.type =%d", var.type);
	}

	return ret;
}


DataGraph::Variant::~Variant()
{
	if (type == SVAL)
		delete []  data.sval;

}

DataGraph::DataGraph(const string& address):Datum(address), _persistent(true)
{
	pthread_mutex_init(&m_DataMutex, NULL);
	pthread_mutex_init(&m_SubsctiberMutex, NULL);
	Variant var;
	var.type = SVAL;
	var.data.sval = strdup(address.c_str());
	_dataImage.insert(map<string, Variant>::value_type(string("ML_GRAPH_ADDRESS"),var));
}

DataGraph::~DataGraph()
{
	
	pthread_mutex_destroy(&m_DataMutex);
	pthread_mutex_destroy(&m_SubsctiberMutex);

}

bool
DataGraph::addField(string key, string value)
{
	bool ret = false;
	pthread_mutex_lock(&m_DataMutex);
	Variant var(DataGraph::SVAL, value.c_str());
	ret = _dataImage.insert(map<string, Variant>::value_type(key, var)).second;
	pthread_mutex_unlock(&m_DataMutex);
	
	return ret;
}

bool
DataGraph::addVariant(const string& key, const Variant& value)
{
	bool ret = false;
	pthread_mutex_lock(&m_DataMutex);
	ret = _dataImage.insert(map<string, Variant>::value_type(key, value)).second;
	pthread_mutex_unlock(&m_DataMutex);
	
	return ret;
}

bool
DataGraph::updateField(const string& key, const Variant& value)
{
	bool ret = false;
	pthread_mutex_lock(&m_DataMutex);
	map<string, Variant>::iterator it = _dataImage.find(key);
	if (it != _dataImage.end()){
		(*it).second = value;
		ret = true;
	}else{
		ret = _dataImage.insert(map<string, Variant>::value_type(key, value)).second;
	}
	pthread_mutex_unlock(&m_DataMutex);
	
	return ret;	
}

bool
DataGraph::removeField(const string& field)
{
	bool ret = false;
	pthread_mutex_lock(&m_DataMutex);
	map<string, Variant>::iterator it = _dataImage.find(field);
	if (it != _dataImage.end()){
		_dataImage.erase(it);
		ret = true;
	}
	pthread_mutex_unlock(&m_DataMutex);
	
	return ret;	
	

}


bool
DataGraph::updateField(string key, string value)
{
	bool ret = false;
	pthread_mutex_lock(&m_DataMutex);
	map<string, Variant>::iterator it = _dataImage.find(key);
	Variant var(DataGraph::SVAL, value.c_str());;
	if (it != _dataImage.end()){
		(*it).second = var;
		ret = true;
	}else{
		ret = _dataImage.insert(map<string, Variant>::value_type(key, var)).second;
	}
	pthread_mutex_unlock(&m_DataMutex);
	
	return ret;	
}

string
DataGraph::toXmlString(Subscriber *s)
{
	string out;
	pthread_mutex_lock(&m_DataMutex);
	bool updated = false;
	Subscriber::FieldMap fm;
	string address;
	map<string, Variant>::iterator its = _dataImage.find("ML_GRAPH_ADDRESS");
	if (its != _dataImage.end()){
		address = serializeVariantValue((*its).second);

	} 
	s->getMappedField(address, fm);
	out = fm.header;
	map<string, Variant>::const_iterator it = _dataImage.begin();
	for (it; it!= _dataImage.end(); it++){
		string value = serializeVariantValue((*it).second);
		string type = serializeVariantType((*it).second);
		map<string, string>::iterator itf = fm.fields.find((*it).first);
		if (itf != fm.fields.end()){
			sprintf(_buf, "\t<FIELD key=\"%s\" type=\"%s\" value=\"%s\" />\n", (*itf).second.c_str(), type.c_str(), value.c_str());
			out += _buf;
			updated = true;
		}

	}
	out += fm.footer;
	out = (updated?out:"");
	pthread_mutex_unlock(&m_DataMutex);
	
	return out;
}

string
DataGraph::toXmlString()
{
	string out;
	pthread_mutex_lock(&m_DataMutex);
	map<string, Variant>::iterator it = _dataImage.begin();
	for (it; it!= _dataImage.end(); it++){
		string value = serializeVariantValue((*it).second);
		sprintf(_buf, "<FIELD key=\"%s\" value=\"%s\" />\n", (*it).first.c_str(), value.c_str());
		out += _buf;
	}
	pthread_mutex_unlock(&m_DataMutex);
	
	return out;
}

string
DataGraph::toString()
{
	string out;
	pthread_mutex_lock(&m_DataMutex);
	map<string, Variant>::iterator it = _dataImage.begin();
	for (it; it!= _dataImage.end(); it++){
		string value = serializeVariantValue((*it).second);
		sprintf(_buf, "%s%s%s\n", (*it).first.c_str(), DATAGRAPH_FLAT_DELIMITER, value.c_str());
		out += _buf;
	}
	pthread_mutex_unlock(&m_DataMutex);
	
	return out;
}


string
DataGraph::toCSVString(Subscriber *s)
{
	string out;
	pthread_mutex_lock(&m_DataMutex);
	bool updated = false;
	Subscriber::FieldMap fm;
	string address = _dataImage["ML_GRAPH_ADDRESS"].data.sval;
	s->getMappedField(address, fm);
	out = fm.header;
	map<string, Variant>::const_iterator it = _dataImage.begin();
	for (it; it!= _dataImage.end(); it++){
		string value = serializeVariantValue((*it).second);
		map<string, string>::iterator itf = fm.fields.find((*it).first);
		if (itf != fm.fields.end()){
			sprintf(_buf, "\"%s\",", value.c_str());
			out += _buf;
			updated = true;
		}

	}
	out += fm.footer;
	out = (updated?out:"");
	pthread_mutex_unlock(&m_DataMutex);
	
	return out;
}

string
DataGraph::toFormattedString(Subscriber *s)
{
	string out;
	pthread_mutex_lock(&m_DataMutex); 
	Formatter *formatter = s->getFormatter();
	if (formatter){
		vector<string> mapedFields = formatter->getMap();
		vector<string> valFields;
		vector<string>::iterator it;
		for(it = mapedFields.begin();it != mapedFields.end(); it++){
			string val;
			map<string, Variant>::iterator itt = _dataImage.find((*it));
			if (itt != _dataImage.end()){
				val = serializeVariantValue((*itt).second);
			}else{
				val = "N/A";
			}
			valFields.push_back(val);	
		}
		Formatter::format(formatter->getFormat(), valFields, out);
	}
	pthread_mutex_unlock(&m_DataMutex);

	return out;
}

DataGraph::DataMap
DataGraph::delta(Subscriber *s)
{
	DataGraph::DataMap out;
	pthread_mutex_lock(&m_DataMutex);
	bool updated = false;
	Subscriber::FieldMap fm;
	//string address = _dataImage["ML_GRAPH_ADDRESS"].sval;
	s->getMappedField(this->address(), fm);
	map<string, Variant>::iterator it = _dataImage.begin();
	for (it; it!= _dataImage.end(); it++){
		map<string, string>::iterator itf = fm.fields.find((*it).first);
		if (itf != fm.fields.end()){
			DataGraph::Variant v((*it).second);
			out.insert(DataMap::value_type((*it).first, v));
		}

	}
	pthread_mutex_unlock(&m_DataMutex);
	
	return out;
}

bool
DataGraph::registerSubscriber(Subscriber *subscriber, bool b_notify)
{
	bool ret = false;
	if (subscriber){
		pthread_mutex_lock(&m_SubsctiberMutex); 
		ret = _subscribers.insert(subscriber).second;
		pthread_mutex_unlock(&m_SubsctiberMutex);
		if (ret && b_notify)
			subscriber->notify(this);
	}
	return ret;
}	

bool
DataGraph::unregisterSubscriber(Subscriber *subscriber)
{
	bool ret = false;
	pthread_mutex_lock(&m_SubsctiberMutex); 
	ret = _subscribers.erase(subscriber) == 1;
	pthread_mutex_unlock(&m_SubsctiberMutex);

	return ret;
}

void
DataGraph::updated()
{
	pthread_mutex_lock(&m_SubsctiberMutex); 
	for (set<Subscriber *>::iterator it=_subscribers.begin(); it != _subscribers.end(); it++){
		pthread_mutex_unlock(&m_SubsctiberMutex);
		(*it)->notify(this);
		pthread_mutex_lock(&m_SubsctiberMutex);
	}
	if (StorageManager::activated() && _persistent)
		StorageManager::getStorage(this->address())->save(this);
	pthread_mutex_unlock(&m_SubsctiberMutex);
}	

void
DataGraph::updated(Subscriber *s)
{
	pthread_mutex_lock(&m_SubsctiberMutex); 
	set<Subscriber *>::iterator it=_subscribers.find(s);
	if (it != _subscribers.end()){
		pthread_mutex_unlock(&m_SubsctiberMutex);
		(*it)->notify(this);
		pthread_mutex_lock(&m_SubsctiberMutex);
	}
	pthread_mutex_unlock(&m_SubsctiberMutex);
}	

int
DataGraph::numOfSubscribers()
{
	int i = 0;
	pthread_mutex_lock(&m_SubsctiberMutex); 
	i = _subscribers.size();
	pthread_mutex_unlock(&m_SubsctiberMutex);
	return i; 
}

set<string> 
DataGraph::getFieldSet()
{
	pthread_mutex_lock(&m_DataMutex); 
	set<string> fset;
	DataMap::iterator it = _dataImage.begin();
	for (it; it != _dataImage.end(); it++){
		fset.insert((*it).first);
	}	

	pthread_mutex_unlock(&m_DataMutex); 
	return fset;
}

DataGraph::Variant
DataGraph::getVariant(const string& field)
{
	pthread_mutex_lock(&m_DataMutex); 
	Variant var;
	var.type = SVAL;
	var.data.sval = strdup("FIELD_NOT_FOUND");
	DataMap::iterator it = _dataImage.find(field);
	if (it != _dataImage.end()){
		var = (*it).second;
	}
	pthread_mutex_unlock(&m_DataMutex); 
	return var;
}

string 
DataGraph::getValue(const string& field)
{
	pthread_mutex_lock(&m_DataMutex); 
	string val = "FIELD_NOT_FOUND";
	DataMap::iterator it = _dataImage.find(field);
	if (it != _dataImage.end()){
		val = serializeVariantValue((*it).second);
	}
	pthread_mutex_unlock(&m_DataMutex); 
	return val;
}

void
DataGraph::getDataClone(Datum *datum)
{
	DataGraph *graph = dynamic_cast<DataGraph *>(datum);
	if (graph){
		pthread_mutex_lock(&m_DataMutex);
		graph->setImage(_dataImage);
		pthread_mutex_unlock(&m_DataMutex);
	}

}

void
DataGraph::setImage(const DataGraph::DataMap& new_map)
{
	pthread_mutex_lock(&m_DataMutex);

	_dataImage = new_map;

	pthread_mutex_unlock(&m_DataMutex);
}

DataGraph::DataMap
DataGraph::getImage() 
{
	DataGraph::DataMap outmap;
	pthread_mutex_lock(&m_DataMutex);

	outmap = _dataImage;

	pthread_mutex_unlock(&m_DataMutex);
	
	return outmap;
}

string
DataGraph::serializeVariantValue(const Variant& var)
{
	string str;
	char buf[12];
	switch (var.type){
		case SVAL:
			str = string(var.data.sval);		
			break;
		case LVAL:
			sprintf(buf, "%l", var.data.lval);
			str = string(buf);
			break;
		case DVAL:
			sprintf(buf, "%f", var.data.dval);
			str = string(buf);
			break;
		default:
			Logger::getLogger()->log(Logger::TYPE_ERROR, "DataGraph::serializeVariant error, unkown Variant.type =%d", var.type);
	}


	return str;

}

string
DataGraph::serializeVariantType(const Variant& var)
{
	string str;
	char buf[12];
	switch (var.type){
		case SVAL:
			str = string("text");		
			break;
		case LVAL:
			str = string("long");
			break;
		case DVAL:
			str = string("double");
			break;
		default:
			Logger::getLogger()->log(Logger::TYPE_ERROR, "DataGraph::serializeVariant error, unkown Variant.type =%d", var.type);
	}
	return str;

}

string
DataGraph::getKeyValHtmlTable(bool indexed, bool editable, const string& tblParm)
{
	string str = "<table " + tblParm + ">";
	pthread_mutex_lock(&m_DataMutex);

	map<string, Variant>::iterator it = _dataImage.begin();
	for (it; it != _dataImage.end(); it++){
		string row = "<tr><td>";	
		row += (*it).first + "</td>";
		if (editable){
			row += "<td><input type=\"text\" name=\"" + (*it).first;
			row += "\" value=\"" + DataGraph::serializeVariantValue((*it).second) +"\" " ;
			row += " mltype=\"" + DataGraph::serializeVariantType((*it).second) + "\"</td>";

		} else {
			row += "<td>" + DataGraph::serializeVariantValue((*it).second) + "</td>";
		}
		row += "<td>" + DataGraph::serializeVariantType((*it).second) + "</td>";
		row += "</tr>";
		str += row;

	}
	pthread_mutex_unlock(&m_DataMutex);
	str += "</table>";
	return str;

}

void
DataGraph::buildImageFromFlat(const vector<string>& vimage, const string& delim)
{
	//vector contains <KEY[delim]Value> pairs 
	for (int i = 0; i < vimage.size(); i++){
		int st = vimage[i].find(delim, 0);
		if (st != string::npos){
			string key = vimage[i].substr(0, st);
			string value = vimage[i].substr(st + delim.size(), vimage[i].size() - (st + delim.size()));
			//cout<<"buildImageFromFlat key=["<<key<<"] ["<<value<<"]"<<endl;
			this->updateField(key, value);

		}

	}
}

string
DataGraph::updateFromFlat(DataGraph& flatdata)
{
	string err;
	set<string> fset = getFieldSet();	
	set<string>::iterator it = fset.begin();
	for (it; it != fset.end(); it++){
		string newval = flatdata.getValue((*it));
		if (newval != "FIELD_NOT_FOUND"){
			DataGraph::Variant myvar = getVariant((*it));
			DataGraph::Variant newvar = updateVariantFromFlat(myvar, newval);
			updateField((*it), newvar);

		}else{
			err += address() + ":Error filed not found  " + (*it);
		}

	}
	updated();
	return err;
}

DataGraph::Variant
DataGraph::updateVariantFromFlat( const DataGraph::Variant& var, const string& str)
{
	Variant newvar;
	newvar.type = var.type;
	switch(var.type){
		case SVAL:
			newvar.data.sval = strdup(str.c_str());
			break;
		case LVAL:
			newvar.data.lval = atol(str.c_str());
			break;
		case DVAL:
			newvar.data.dval = atof(str.c_str());
			break;
		default:
			Logger::getLogger()->log(Logger::TYPE_ERROR, "DataGraph::updateVariantFromFlat error, unkown Variant.type =%d", var.type);
	}
	
	return newvar;
}

