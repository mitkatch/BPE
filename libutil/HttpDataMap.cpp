/************************************************************************
||
|| DATE:                $Date: 2004/12/03 18:42:56 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/HttpDataMap.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: HttpDataMap.cpp,v 1.1 2004/12/03 18:42:56 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: HttpDataMap.cpp,v $
|| LOG:                 Revision 1.1  2004/12/03 18:42:56  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "Logger.hpp"
#include "HttpDataMap.hpp"

HttpDataMap * HttpDataMap::_httpMap = new HttpDataMap();

HttpDataMap::HttpDataMap()
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "Create HttpDataMap::HttpDataMap()");
	_httpData["+"] = " ";
	_httpData["%20"]=   " ";   //(space)
	_httpData["%21"]=   "!";    //(exclamation mark)
	_httpData["%22"]=   "\"";    //(double quote)
	_httpData["%23"]=   "#";    //(number sign)
	_httpData["%24"]=   "$";    //(dollar sign)
	_httpData["%25"]=   "%";    //(percent)
	_httpData["%26"]=   "&";    //(ampersand)
	_httpData["%27"]=   "'";    //(single quote)
	_httpData["%28"]=   "(";    //(left/opening parenthesis)
	_httpData["%29"]=   ")";    //(right/closing parenthesis)
	_httpData["%2A"]=   "*";    //(asterisk)
	_httpData["%2B"]=   "+";    //(plus)
	_httpData["%2C"]=   ",";    //(comma)
	_httpData["%2D"]=   "-";    //(minus or dash)
	_httpData["%2E"]=   ".";    //(dot)
	_httpData["%2F"]=   "/";    //(forward slash)
	_httpData["%30"]=   "0";
	_httpData["%31"]=   "1";
	_httpData["%32"]=   "2";
	_httpData["%33"]=   "3";
	_httpData["%34"]=   "4";
	_httpData["%35"]=   "5";
	_httpData["%36"]=   "6";
	_httpData["%37"]=   "7";
	_httpData["%38"]=   "8";
	_httpData["%39"]=   "9";
	_httpData["%3A"]=   ":";    //(colon)
	_httpData["%3B"]=   ";";    //(semi-colon)
	_httpData["%3C"]=   "<";    //(less than)
	_httpData["%3D"]=   "=";    //(equal sign)
	_httpData["%3E"]=   ">";    //(greater than)
	_httpData["%3F"]=   "?";    //(question mark)
	_httpData["%40"]=   "@";    //(AT symbol)
	_httpData["%41"]=   "A";
	_httpData["%42"]=   "B";
	_httpData["%43"]=   "C";
	_httpData["%44"]=   "D";
	_httpData["%45"]=   "E";
	_httpData["%46"]=   "F";
	_httpData["%47"]=   "G";
	_httpData["%48"]=   "H";
	_httpData["%49"]=   "I";
	_httpData["%4A"]=   "J";
	_httpData["%4B"]=   "K";
	_httpData["%4C"]=   "L";
	_httpData["%4D"]=   "M";
	_httpData["%4E"]=   "N";
	_httpData["%4F"]=   "O";
	_httpData["%50"]=   "P";
	_httpData["%51"]=   "Q";
	_httpData["%52"]=   "R";
	_httpData["%53"]=   "S";
	_httpData["%54"]=   "T";
	_httpData["%55"]=   "U";
	_httpData["%56"]=   "V";
	_httpData["%57"]=   "W";
	_httpData["%58"]=   "X";
	_httpData["%59"]=   "Y";
	_httpData["%5A"]=   "Z";
	_httpData["%5B"]=   "[";    //(left/opening bracket)
	_httpData["%5C"]=   "\\";    //(back slash)
	_httpData["%5D"]=   "]";   // (right/closing bracket)
	_httpData["%5E"]=   "^";    //(caret/cirumflex)
	_httpData["%5F"]=   "_";    //(underscore)
	_httpData["%60"]=   "`";
	_httpData["%61"]=   "a";
	_httpData["%62"]=   "b";
	_httpData["%63"]=   "c";
	_httpData["%64"]=   "d";
	_httpData["%65"]=   "e";
	_httpData["%66"]=   "f";
	_httpData["%67"]=   "g";
	_httpData["%68"]=   "h";
	_httpData["%69"]=   "i";
	_httpData["%6A"]=   "j";
	_httpData["%6B"]=   "k";
	_httpData["%6C"]=   "l";
	_httpData["%6D"]=   "m";
	_httpData["%6E"]=   "n";
	_httpData["%6F"]=   "o";
	_httpData["%70"]=   "p";
	_httpData["%71"]=   "q";
	_httpData["%72"]=   "r";
	_httpData["%73"]=   "s";
	_httpData["%74"]=   "t";
	_httpData["%75"]=   "u";
	_httpData["%76"]=   "v";
	_httpData["%77"]=   "w";
	_httpData["%78"]=   "x";
	_httpData["%79"]=   "y";
	_httpData["%7A"]=   "z";
	_httpData["%7B"]=   "{";    //(left/opening brace)
	_httpData["%7C"]=   "|";    //(vertical bar)
	_httpData["%7D"]=   "}";    //(right/closing brace)
	_httpData["%7E"]=   "~";    

}

//static
/***********
string
HttpDataMap::toHttpString(char ch)
{
	return _httpMap->getHttpDataCode(ch);

}
**********/
//static
string
HttpDataMap::toString(const string& httpStr)
{
	string tmp = httpStr;
	int plus = tmp.find_first_of('+');
	while(plus != string::npos){
		tmp.replace(plus, 1, " ");
		plus = tmp.find_first_of('+', plus+1);	

	}
	int imp = tmp.find_first_of("%", 0);
	while(imp != string::npos){
		if (tmp.size() >= imp + 3){
			string rep = tmp.substr(imp, 3);
			map<string, string>::iterator it = _httpMap->_httpData.find(rep);
			if (it != _httpMap->_httpData.end()){
				tmp.replace(imp, 3, (*it).second);
			}
			imp = tmp.find_first_of("%", imp+1);
		} else {
			break;
		}
	}
	return tmp;

}

/***************
string
HttpDataMap::getHttpDataCode(char ch)
{
	string str;
	map<string, string>::iterator it = _httpData.find(ch);
	if (it != _httpData.end()){
		str = (*it).second;
	} else {
		str = string(1, ch);
	}

	return str;

}
******************/
