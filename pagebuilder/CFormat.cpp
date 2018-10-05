
/************************************************************************
||
|| DATE:                $Date: 2005/07/29 19:22:02 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/CFormat.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CFormat.cpp,v 1.2 2005/07/29 19:22:02 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: CFormat.cpp,v $
|| LOG:                 Revision 1.2  2005/07/29 19:22:02  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "DataGraph.hpp"
#include "CellConverter.hpp"
#include "CFormat.hpp"
#include "Scheduler.hpp"

CFormatFactory* CFormatFactory::_factory = NULL;

CFormatFactory::CFormatFactory()
{
	_formatMap["TODAYTIME"] = new CF_TODAYTIME("TODAYTIME");
	_formatMap["HANDLER"] = new CF_HANDLER("HANDLER");
	_formatMap["HANDLER_CND"] = new CF_HANDLER_CND("HANDLER_CND");
	_formatMap["FRACTION"] = new CF_FRACTION("FRACTION");
	_formatMap["FRACTION_CND"] = new CF_FRACTION_CND("FRACTION_CND");
	_formatMap["FRACTION_STRING_CND"] = new CF_FRACTION_STRING_CND("FRACTION_STRING_CND");
	_formatMap["FRACTION_SIGN"] = new CF_FRACTION_SIGN("FRACTION_SIGN");
	_formatMap["HANDLER_DD"] = new CF_HANDLER_DD("HANDLER_DD");
	_formatMap["GENERIC"] = new CF_GENERIC("GENERIC");
	_formatMap["HANDLER_COLLECTED"] = new CF_HANDLER_COLLECTED("HANDLER_COLLECTED");


}

CFormatFactory::~CFormatFactory()
{

	map<string, CFormat *>::iterator it = _formatMap.begin();
	for (it ; it != _formatMap.end(); it++){
		delete (*it).second;
	}
	_formatMap.clear();

}

CFormat*
CFormatFactory::getFormat(const string& f)
{
	if (_factory == NULL){
		_factory = new CFormatFactory();
	}

	return _factory->getThisFormat(f);
}

CFormat*
CFormatFactory::getThisFormat(const string& f)
{
	CFormat* format = NULL;
	map<string, CFormat *>::iterator it = _formatMap.find(f);
	if (it != _formatMap.end()){
		format = (*it).second;
	} else {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "CFormatFactory::getFormat not found %s", f.c_str());
	}
	return format;
}

//static
vector<string>
CFormat::toVectorFromFlat(const string& str)
{
	//(1)str "[123.000],[110.000],[123.000]"
	//(2)str "[123.000]" 
	//(3)str "[]" empty 
	//(4)str "123.000"
	vector<string> tmp;
	vector<string> ret;
	Formatter::split(str, ",", tmp);
	for(int i = 0; i < tmp.size(); i++){
		int b1 = tmp[i].find_first_of('[');
		int b2 = tmp[i].find_first_of(']');
		if (b1 == 0 && b2 == ((tmp[i]).size() -1) ){
			ret.push_back(tmp[i].substr(1, tmp[i].size() - 2));
		} else {
			ret.push_back(tmp[i]); //if it's not an array (4), collect it
		}
	}
	return ret;
}

//static
bool
CFormat::isCondition(const string& f)
{
	//TRADE_PRICE@TRADE_TONE=H
	bool ret = false;
	int at = f.find_first_of('@', 0);
	if (at != string::npos){
		int eq = f.find_first_of('=', at);
		if (eq != string::npos)
			ret = true;
	}

	return ret;
}

string 
CFormat::conditionField(const string& f)
{
	//TRADE_PRICE@TRADE_TONE=H
	string field;
	int at = f.find_first_of('@', 0);
	if (at != string::npos){
		field = f.substr(0, at);
	}
	return field;
}

string 
CFormat::conditionString(const string& f)
{
	//V:=HIT@TRADE_TONE=H
	string str;
	int at = f.find_first_of('@', 0);
	int v = f.find_first_of("V:=", 0);
	if (at != string::npos && v != string::npos && at > v + 3){
		str = f.substr(v + 3, at);
	}
	return str;
}

string
CFormat::conditionKey(const string& f)
{
	//TRADE_PRICE@TRADE_TONE=H
	string key;
	int at = f.find_first_of('@', 0);
	int eq = f.find_first_of('=', at);
	if (at != string::npos && eq != string::npos){
		key = f.substr(at+1, eq - (at +1));
	}
	return key;

}

string
CFormat::conditionValue(const string& f)
{
	//TRADE_PRICE@TRADE_TONE=H
	string val;
	int at = f.find_first_of('@', 0);
	int eq = f.find_first_of('=', at);
	if (at != string::npos && eq != string::npos){
		val = f.substr(eq+1, f.size() - (eq +1));
	}
	return val;


}

string
CF_GENERIC::compile(DataGraph *data, CellConverter* cell)
{
	string formval;
	char buf[12];
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		//string val = data->getValue(fields[i]);
		vector<string> vval = toVectorFromFlat(data->getValue(fields[i])); 
		if (vval.size() == 0)
			continue;	
		string tmpval = vval[0];
		double d = Formatter::ATOF(tmpval.c_str());
		if (d){
			sprintf(buf, "%.3f", d);
			formval = buf;
			break;	
		} 
	}

	return formval;
}


string
CF_HANDLER_DD::compile(DataGraph *data, CellConverter* cell)
{
	string formval;
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		//string val = data->getValue(fields[i]);
		vector<string> vval = toVectorFromFlat(data->getValue(fields[i])); 
		if (vval.size() == 0)
			continue;	
		string tmpval = vval[0];
		double d = Formatter::ATOF(tmpval.c_str());
		if (d){
			int dot = tmpval.find_first_of('.', 0);
			if (dot != string::npos){
				formval = tmpval.substr(0, dot+1);
				break;	
			}
		} 
	}

	return formval;
}

string
CF_HANDLER::compile(DataGraph *data, CellConverter* cell)
{
	string formval;
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		//string val = data->getValue(fields[i]);
		vector<string> vval = toVectorFromFlat(data->getValue(fields[i])); 
		if (vval.size() == 0)
			continue;	
		string tmpval = vval[0];
		double d = Formatter::ATOF(tmpval.c_str());
		if (d){
			int dot = tmpval.find_first_of('.', 0);
			if (dot != string::npos){
				formval = tmpval.substr(0, dot);
				break;	
			}
		} 
	}
	return formval;
}

string
CF_HANDLER_CND::compile(DataGraph *data, CellConverter* cell)
{
	string formval;
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		vector<string> vval;
		string sval;
		if (CFormat::isCondition(fields[i])){
			//TRADE_PRICE@TRADE_TONE=H
			//c_field@c_key=c_val
			string c_field = CFormat::conditionField(fields[i]);
			string c_key = CFormat::conditionKey(fields[i]);
			string c_val = CFormat::conditionValue(fields[i]);
			vector<string> cVal = toVectorFromFlat(data->getValue(c_key));
			vval = toVectorFromFlat(data->getValue(c_field)); 
			if (cVal.size() == vval.size()){
				for (int i = 0; i < cVal.size(); i++){
					if (c_val == cVal[i])
						sval = vval[i];
						break; //get first one
				}
			}
			
		} else {
			vval = toVectorFromFlat(data->getValue(fields[i]));
			if (vval.size()) 
				sval = vval[0];
		}
		double d = Formatter::ATOF(sval.c_str());
		if (d){
			int dot = sval.find_first_of('.', 0);
			if (dot != string::npos){
				formval = sval.substr(0, dot);
				break;	
			}
		} 
	}
	return formval;
}

string
CF_HANDLER_COLLECTED::compile(DataGraph *data, CellConverter* cell)
{
	string formval;
	double d = 0;
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		string val;
		if (CFormat::isCondition(fields[i])){
			//TRADE_SIZE@TRADE_TONE=H
			string c_field = CFormat::conditionField(fields[i]);
			string c_key = CFormat::conditionKey(fields[i]);
			string c_val = CFormat::conditionValue(fields[i]);
			string cVal = data->getValue(c_key);
			string cCol;
			string formPar = cell->formatParameter();
			if (!formPar.empty()){
				cCol = data->getValue(formPar);	
			}
			string rawval = data->getValue(c_field); 
			//TRADE_TONE:  [H],[T],[H] as cVal
			//TRADE_PRICE: [123.000],[100.123],[123.000] as cCol
			//TRADE_SIZE: [5.000],[1.000],[3.000] as rawval 
			// c_val = H
			vector<string> vVal = toVectorFromFlat(cVal);
			vector<string> vCol = toVectorFromFlat(cCol);
			vector<string> vval = toVectorFromFlat(rawval);
			string colVal; //as TRADE_PRICE to compare with
			if (vVal.size() == vCol.size() && vVal.size() == vval.size()){
				for (int i = 0; i < vVal.size(); i++){ 
					if (c_val == vVal[i]){
						if (colVal.empty()){
							colVal = vCol[i];	
							d = Formatter::ATOF((vval[i]).c_str())	;
						} else if (colVal == vCol[i]){
							//identical TRADE_PRICE
							d += Formatter::ATOF((vval[i]).c_str())	;
						}
					}
				}
				if (d){
					char buf[12];
					sprintf(buf, "%f", d);
					val = buf;
				}
			}else{
				Logger::getLogger()->log(Logger::TYPE_ERROR, "CF_HANDLER_COLLECTED::compile corrupted vectors size %d %d %d", vVal.size(), vCol.size(), vval.size());
			}
			
		} else {
			val = data->getValue(fields[i]);
			d = Formatter::ATOF(val.c_str());
		}

		if (d){
			int dot = val.find_first_of('.', 0);
			if (dot != string::npos){
				formval = val.substr(0, dot);
				break;	
			}
		} 
	}
	return formval;
}

string
CF_FRACTION::compile(DataGraph *data, CellConverter* cell)
{
	string formval;
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		//string val = data->getValue(fields[i]);
		vector<string> vval = toVectorFromFlat(data->getValue(fields[i])); 
		if (vval.size() == 0)
			continue;	

		string tmpval = vval[0];
		double d = Formatter::ATOF(tmpval.c_str());
		if (d){
			int dot = tmpval.find_first_of('.', 0);
			if (dot != string::npos){
				formval = tmpval.substr(dot+1, tmpval.size() - (dot +1));
				break;	
			}
		} 
	}
	return formval;
}

string
CF_FRACTION_STRING_CND::compile(DataGraph *data, CellConverter* cell)
{
	//return fraction or HIT/TAK for trades
	//if H returns HIT for ask price
	//if T returns TAK for bid price
	//
	string formval;
	bool val_set = false;
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		vector<string> vval;
		string sval;
		if (CFormat::isCondition(fields[i])){
			//TRADE_PRICE@TRADE_TONE=H
			//V:=HIT@TRADE_TONE=H
			//V:=TAK@TRADE_TONE=T
			//c_field@c_key=c_val
			string c_str = CFormat::conditionString(fields[i]);
			string c_field; 
			if (c_str.size() == 0){
				c_field = CFormat::conditionField(fields[i]);
				vval = toVectorFromFlat(data->getValue(c_field)); 
			}

			string c_key = CFormat::conditionKey(fields[i]);
			string c_val = CFormat::conditionValue(fields[i]);
			vector<string> cVal = toVectorFromFlat(data->getValue(c_key));

			if (cVal.size() != 0 && cVal.size() == vval.size()){
				for (int i = 0; i < cVal.size(); i++){
					if (c_val == cVal[i])
						sval = vval[i];
					break; //get first one
				}
			}else if (vval.size() == 0 && cVal.size() != 0){ // must be c_str
				for (int i = 0; i < cVal.size(); i++){
					if (c_val == cVal[i]){
						//H(from config) == H(from record)
						formval = c_str;
						val_set = true;
						break;
					}

				}

			}
			
		} else {
			vval = toVectorFromFlat(data->getValue(fields[i]));
			if (vval.size()) 
				sval = vval[0];
		}

		double d = Formatter::ATOF(sval.c_str());
		if (d){
			int dot = sval.find_first_of('.', 0);
			if (dot != string::npos){
				formval = sval.substr(dot +1, sval.size() - (dot+1));
				val_set = true;
				break;	
			}
		} 
		
		if (val_set) break;

	}
	return formval;
}

string
CF_FRACTION_CND::compile(DataGraph *data, CellConverter* cell)
{
	string formval;
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		vector<string> vval;
		string sval;
		if (CFormat::isCondition(fields[i])){
			//TRADE_PRICE@TRADE_TONE=H
			//c_field@c_key=c_val
			string c_field = CFormat::conditionField(fields[i]);
			string c_key = CFormat::conditionKey(fields[i]);
			string c_val = CFormat::conditionValue(fields[i]);
			vector<string> cVal = toVectorFromFlat(data->getValue(c_key));
			vval = toVectorFromFlat(data->getValue(c_field)); 
			if (cVal.size() == vval.size()){
				for (int i = 0; i < cVal.size(); i++){
					if (c_val == cVal[i])
						sval = vval[i];
						break; //get first one
				}
			}
			
		} else {
			vval = toVectorFromFlat(data->getValue(fields[i]));
			if (vval.size()) 
				sval = vval[0];
		}
		double d = Formatter::ATOF(sval.c_str());
		if (d){
			int dot = sval.find_first_of('.', 0);
			if (dot != string::npos){
				formval = sval.substr(dot +1, sval.size() - (dot+1));
				break;	
			}
		} 
	}
	return formval;
}

string
CF_FRACTION_SIGN::compile(DataGraph *data, CellConverter* cell)
{
	string formval;
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		//string val = data->getValue(fields[i]);
		vector<string> vval = toVectorFromFlat(data->getValue(fields[i])); 
		if (vval.size() == 0)
			continue;	

		string tmpval = vval[0];
		double d = Formatter::ATOF(tmpval.c_str());
		if (d){
			int dot = tmpval.find_first_of('.', 0);
			if (dot != string::npos){
				//formval = tmpval.substr(dot+1, tmpval.size() - (dot +1));
				formval = tmpval.erase(dot, 1);
				if (tmpval[0] == '-')
					;//formval = string("-") + formval;
				else{
					formval = string("+") + formval;
				}
				break;	
			}
		} 
	}
	return formval;
}

string
CF_TODAYTIME::compile(DataGraph *data, CellConverter* cell)
{
	string formval;
	string today = Scheduler::getCurrentTime("%d/%m/%y");
	vector<string> fields = cell->datumField();
	for(int i = 0; i < fields.size(); i++){
		string val = data->getValue(fields[i]);
		if (val.size() == 17 /* "05/18/05 11:57:21" */){
			string foundday = val.substr(0, 8);
			//cout<<"CF_TODAYTIME::compile val="<<val<<", foundday="<<foundday<<endl;
			string foundtime = val.substr(9, 5);
			if ( today == foundday){
				formval = foundtime; 
				break;	
			} else {
				formval = foundday.substr(0, 5);
				break;	
			}
		} 
		continue;
	}
	return formval;
}

