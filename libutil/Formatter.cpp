/************************************************************************
||
|| DATE:                $Date: 2005/07/29 18:38:26 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/Formatter.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Formatter.cpp,v 1.8 2005/07/29 18:38:26 mikhailt Exp $
|| REVISION:    $Revision: 1.8 $
|| LOG:                 $Log: Formatter.cpp,v $
|| LOG:                 Revision 1.8  2005/07/29 18:38:26  mikhailt
|| LOG:                 fixed ATOF
|| LOG:
|| LOG:                 Revision 1.7  2005/06/29 18:55:58  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.6  2004/09/10 15:43:51  mikhailt
|| LOG:                 asCapSize based on investmentGrade
|| LOG:
|| LOG:                 Revision 1.5  2004/09/09 14:41:22  mikhailt
|| LOG:                 maps to as.. functions
|| LOG:
|| LOG:                 Revision 1.4  2004/08/26 17:04:34  mikhailt
|| LOG:                 split added
|| LOG:
|| LOG:                 Revision 1.3  2004/08/04 20:45:29  mikhailt
|| LOG:                 asPrice, asSize, asCoupon
|| LOG:
|| LOG:                 Revision 1.2  2004/06/28 16:34:13  mikhailt
|| LOG:                 AsPrice, AsYield, AsSize defined
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:52:40  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <ctype.h>
#include "Formatter.hpp"
#include "Logger.hpp"

string Formatter::FORMAT = string("format");
string Formatter::MAP = string("map");
string Formatter::DELIM = string("%s");

Formatter::Formatter(string& schema):_schema(schema)
{

}

Formatter::~Formatter()
{

}

void
Formatter::format(string& frm, vector<string>& val, string& fmted)
{
	int start, stop, counter =0;
	int n = frm.length();
	char buf[100];
	vector<string> vfmt;
	//cout <<"Formatter::format frm= "<<frm<<endl;
	start = frm.find(DELIM);
	if (start > 0){
		fmted += frm.substr(0, start);
		//cout << "Formatter::format found prefix "<<fmted<<endl;
	}
	while( (start >= 0) && (start <n)){
		stop = frm.find(DELIM, start+1);
		if ((stop < 0) || (stop > n )) stop =n;

		if (counter < val.size()){
			//cout<<"Formatter::format Start="<<start<<", Stop="<<stop<<endl;
			//cout <<"Formatter::format about to write "<<frm.substr(start, 2).c_str() <<" to "<<val[counter].c_str()<<endl;
			sprintf(buf, frm.substr(start, 2).c_str(), val[counter].c_str());
			fmted += buf;
			counter++;
			//cout<<"Formatter::format before "<<buf<<"["<<fmted<<"]"<<endl;
			fmted += frm.substr(start + DELIM.length(), stop - (start+DELIM.length()));//
			//cout<<"Formatter::format after "<<buf<<"["<<fmted<<"]"<<endl;
		} else {
			Logger::getLogger()->log(Logger::TYPE_ERROR," Formatter::format Corrupted format for %s", frm.c_str());
		}
		//vfmt.push_back(fmt.substr(start, stop - start);
		start = frm.find(DELIM, stop - 1);
	}	


}


void
Formatter::setFormat(string& fmt)
{
	vector<string> v;
	v.push_back(fmt);
	_map[FORMAT]=v;
}

void
Formatter::setMap(vector<string>& map)
{
	_map[MAP] = map;
}

void
Formatter::activateModifiers(vector<string>& formmap)
{
	string format;
	for(int i = 0; i < formmap.size(); i++){
		format = formmap[i];
		int asSize = format.find(":asSize");
		int asCapSize = format.find(":asCapSize");
		int asPrice = format.find(":asPrice");
		int asCoupon = format.find(":asCoupon");
		int asYield = format.find(":asYield");
	
		if (asPrice != string::npos)
			_modifierMap[i] = Formatter::asPrice;
		if (asCapSize != string::npos)
			_modifierMap[i] = Formatter::asCapSize;
		if (asSize != string::npos)
			_modifierMap[i] = Formatter::asSize;
		if (asCoupon != string::npos)
			_modifierMap[i] = Formatter::asCoupon;
		if (asYield != string::npos)
			_modifierMap[i] = Formatter::asYield;

	}


}

vector<string>&
Formatter::getMap()
{
	FormatterMap::iterator it =_map.find(MAP); 
	if (it != _map.end()){
		return (*it).second;
	}
	//vector<string> v;
	_map[MAP] = vector<string>();
	return _map[MAP];
}

string&
Formatter::getFormat()
{
	FormatterMap::iterator it =_map.find(FORMAT); 
	if (it != _map.end()){
		return (*it).second[0];
	}
	//vector<string> v;
	_map[FORMAT] = vector<string>();
	_map[FORMAT][0] = string();
	return _map[FORMAT][0];
	
}

void
Formatter::loadCSVMap(string& str)
{
	vector<string> vmap;
	int n = str.length();
	int start, stop;
	start = str.find_first_not_of(',');
	while((start >= 0) && (start < n)){
		stop = str.find_first_of(',', start);
		if((stop < 0) || (stop > n)) stop = n;
		vmap.push_back(str.substr(start, stop - start));
		start = str.find_first_not_of(',', stop + 1);
	}
	setMap(vmap);
}

void
Formatter::loadDelimitedMap(string& str, char delim)
{
	vector<string> vmap;
	int n = str.length();
	int start, stop;
	start = str.find_first_not_of(delim);
	while((start >= 0) && (start < n)){
		stop = str.find_first_of(delim, start);
		if((stop < 0) || (stop > n)) stop = n;
		vmap.push_back(Formatter::trim(str.substr(start, stop - start)));
		start = str.find_first_not_of(delim, stop + 1);
	}
	activateModifiers(vmap);
	setMap(vmap);
}

//static
string
Formatter::asCapSize(const string& val, int pad_num, char grade)
{
	long ltradesize;
	string stradesize;
	if (grade == 'I'){ //investment grade
		ltradesize = atol(val.c_str());
		if (ltradesize > 2000000)
			stradesize = "2000000 +";
		else
			stradesize = val;
	} else if (grade == 'N'){ //non-investment grade
		ltradesize = atol(val.c_str());
		if (ltradesize > 200000)
			stradesize = "200000 +";
		else
			stradesize = val;
	} else {
		stradesize = val;
	}

	return Formatter::asSize(stradesize);
}

string
Formatter::asSize(const string& size, int pad_num, char pad)
{
	return Formatter::asSize(size);
}

string
Formatter::asSize(const string& size)
{
	// size like 200000 + | 12304320
	// return 200,000 + | 12,304,320
	string retstr, tmpstr;
	string nums("1234567890");

	int dec_dot = size.find_last_of(".");
	int str_end = size.find_last_of(nums);
	if (dec_dot != string::npos && dec_dot < str_end)
		str_end = dec_dot - 1;
	int i = -1;
	for (i = 3; i <= str_end; i = i +3){
		tmpstr = string(1,',') + size.substr(str_end -i +1, 3);
		retstr = tmpstr + retstr;
	}
	string footer = size.substr(str_end +1, size.size() - str_end);
	string header = size.substr(0, str_end - (i-3) +1) ;
	return header + retstr + footer;
}

string 
Formatter::asPrice(const string& price, int pad_num, char pad)
{
	return Formatter::asPaddedFloatNum(price, pad_num, pad);
	/*****
	***/
}

string
Formatter::asYield(const string& yield, int pad_num, char pad)
{
	return Formatter::asPaddedFloatNum(yield, pad_num, pad);

}

string
Formatter::asCoupon(const string& coupon, int pad_num, char pad)
{
	return Formatter::asPaddedFloatNum(coupon, pad_num, pad);

}


string 
Formatter::asPaddedFloatNum(const string& strnum, int pad_num, char pad)
{

	string retstr, tmpstr;
	string nums("1234567890");

	retstr = strnum;
	int dec_dot = retstr.find_last_of(".");
	int str_end = retstr.find_last_of(nums);

	if (str_end == string::npos ){
		return strnum;
	}

	if (dec_dot == string::npos ){
		retstr.insert(str_end+1, ".");
		dec_dot = str_end + 1;
		str_end = dec_dot;
	}

	int padsize = pad_num - (str_end - dec_dot);
	if (padsize < 0){ // the string is longer we need to cut it of
		retstr.erase(str_end + padsize, -1 * padsize); //
	} else {
		string padder = string(padsize, pad);
		retstr.insert(str_end+1, padder);
	}
	return retstr;

}

string
Formatter::trim(const string& str)
{
	if (str.size() == 0)
		return str;

	string retstr = str;
	while(retstr.find(' ') == 0){
		retstr = retstr.erase(0, 1);
	}
	while(retstr.rfind(' ') == retstr.length() -1){
		retstr = retstr.erase(retstr.length()-1, 1);
	}
	return retstr;

}

void
Formatter::split(const string& text, const string &separators, vector<string>& words)
{
	int n = text.length();
	int start, stop;

	start = text.find_first_not_of(separators);
	while ((start >= 0) && (start < n)) {
		stop = text.find_first_of(separators, start);
		if ((stop < 0) || (stop > n)) stop = n;
		words.push_back(text.substr(start, stop - start));
		start = text.find_first_not_of(separators, stop+1);
	}

}

Formatter::Modifier
Formatter::getModifier(int i)
{
	Formatter::Modifier mod = NULL;
	map<int, Modifier>::iterator it = _modifierMap.find(i);
	if (it != _modifierMap.end())
		mod = (*it).second;

	return mod;
	

}

string
Formatter::getRawFormatModifier(int i)
{
	string str;
	map<string, vector<string> >::iterator it = _map.find(MAP);

	if( it != _map.end()){
		vector<string> vec_fmt = (*it).second;
		if (vec_fmt.size() > i){
			string fmt = vec_fmt[i];
			int start = fmt.find_first_of(":");
			if (start != string::npos){
				int end = fmt.find_first_of(" ", start); // like _:asPrice_		
				if (end != string::npos ){
					str = fmt.substr(start, end - start +1); //including the followed space	
				}
			}
		}

	} 	


	return str;

}

string
Formatter::cleanFormatString(const string &fmt)
{
//remove modifiers from format string
	string str(fmt);

	int start = str.find(" :");
	while( start != string::npos){
		int end = str.find_first_of(' ', start+1);
		if (end != string::npos){
			str.erase(start, end - start ); //like _:asPrice_
		}else {
			break;
		}

		start = str.find(" :");
	}
	
	return str;
}

double
Formatter::ATOF(const char *str)
{
        bool convertable = true;
        if (str){
		if (strlen(str)){
                	for (int i=0; i < strlen(str); i++){
                        	if (isalpha(str[i]))
                                	convertable = false;
					break;
			}
                }else{
			convertable = false;
		}
	}else{
		convertable = false;
	}
        return (convertable?atof(str):0);

}

double
Formatter::prc(int precision, double d)
{
	// format as "%.3f" where 3 is precision
	double val = 0.0;
	if (d && precision){
		char f[12];
		char buf[25];
		sprintf(f,"%%.%if", precision);		
		sprintf(buf, f, d);
		val = Formatter::ATOF(buf);
	}

	return val;

}
