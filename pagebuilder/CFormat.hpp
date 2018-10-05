
/************************************************************************
||
|| DATE:                $Date: 2005/07/29 19:22:02 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/CFormat.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CFormat.hpp,v 1.2 2005/07/29 19:22:02 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: CFormat.hpp,v $
|| LOG:                 Revision 1.2  2005/07/29 19:22:02  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CFORMAT_HPP
#define CFORMAT_HPP

#include <map>
#include <vector>
#include <string>
#include "Logger.hpp"

class CellConverter;
class DataGraph;

using std::map;
using std::vector;
using std::string;

class CFormat
{
	public:
		CFormat(const string& name):_name(name){};
		virtual ~CFormat(){};
		virtual string compile(DataGraph *data, CellConverter* cell) = 0;
		//static
		static bool isCondition(const string& f);
		static string conditionField(const string& f);
		static string conditionKey(const string& f);
		static string conditionValue(const string& f);
		static string conditionString(const string& f);
		static vector<string> toVectorFromFlat(const string& str);

	protected:
		string _name;

};

class CF_GENERIC: public CFormat
{
	//returns double as string, empty string for 0 
	public:
		CF_GENERIC(const string& name):CFormat(name){};
		virtual ~CF_GENERIC(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};

class CF_HANDLER_DD: public CFormat
{
	//returns only handler from float number + dec dot attached
	public:
		CF_HANDLER_DD(const string& name):CFormat(name){};
		virtual ~CF_HANDLER_DD(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};

class CF_HANDLER: public CFormat
{
	//returns only handler from float number
	public:
		CF_HANDLER(const string& name):CFormat(name){};
		virtual ~CF_HANDLER(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};

class CF_HANDLER_CND: public CFormat
{
	//returns only handler from float number
	// with condition
	public:
		CF_HANDLER_CND(const string& name):CFormat(name){};
		virtual ~CF_HANDLER_CND(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};

class CF_FRACTION: public CFormat
{
	//returns only fraction from float number
	public:
		CF_FRACTION(const string& name):CFormat(name){};
		virtual ~CF_FRACTION(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};

class CF_FRACTION_CND: public CFormat
{
	//returns only fraction from float number
	// with condition
	public:
		CF_FRACTION_CND(const string& name):CFormat(name){};
		virtual ~CF_FRACTION_CND(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};

class CF_FRACTION_STRING_CND: public CFormat
{
	//returns only fraction from float number
	// or HIT/TAK for conditions
	public:
		CF_FRACTION_STRING_CND(const string& name):CFormat(name){};
		virtual ~CF_FRACTION_STRING_CND(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};

class CF_FRACTION_SIGN: public CFormat
{
	//returns only fraction from float number
	// with +/- 
	public:
		CF_FRACTION_SIGN(const string& name):CFormat(name){};
		virtual ~CF_FRACTION_SIGN(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};

class CF_HANDLER_COLLECTED: public CFormat
{
	//returns only handler from float number
	// with condition collected form the provided field
	public:
		CF_HANDLER_COLLECTED(const string& name):CFormat(name){};
		virtual ~CF_HANDLER_COLLECTED(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};

class CF_TODAYTIME: public CFormat
{
	//converts time to -> if day == today shows hh::mi
	// if day != today shows mm/dd
	public:
		CF_TODAYTIME(const string& name):CFormat(name){};
		virtual ~CF_TODAYTIME(){};
		virtual string compile(DataGraph *data, CellConverter* cell);
		
};



class CFormatFactory
{
	public:
		virtual ~CFormatFactory();

		static CFormat* getFormat(const string& f);
	protected:
		static CFormatFactory* _factory;
		CFormat* getThisFormat(const string& f);
		CFormatFactory();
		map<string, CFormat* > _formatMap;
};

#endif //CFORMAT_HPP

