/************************************************************************
||
|| DATE:                $Date: 2005/06/29 18:55:58 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/Formatter.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Formatter.hpp,v 1.9 2005/06/29 18:55:58 mikhailt Exp $
|| REVISION:    $Revision: 1.9 $
|| LOG:                 $Log: Formatter.hpp,v $
|| LOG:                 Revision 1.9  2005/06/29 18:55:58  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.8  2005/01/10 16:11:50  mikhailt
|| LOG:                 formatted output
|| LOG:
|| LOG:                 Revision 1.7  2004/09/10 15:44:18  mikhailt
|| LOG:                 asCapSize based on investmentGrade
|| LOG:
|| LOG:                 Revision 1.6  2004/09/09 14:41:22  mikhailt
|| LOG:                 maps to as.. functions
|| LOG:
|| LOG:                 Revision 1.5  2004/08/26 17:04:34  mikhailt
|| LOG:                 split added
|| LOG:
|| LOG:                 Revision 1.4  2004/08/04 20:45:59  mikhailt
|| LOG:                 asPrice, asSize, asCoupon
|| LOG:
|| LOG:                 Revision 1.3  2004/06/28 16:34:13  mikhailt
|| LOG:                 daylight saving time handled
|| LOG:
|| LOG:                 Revision 1.2  2004/04/29 19:49:16  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:52:40  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <iostream.h>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class Formatter
{
	public:
		typedef string (*Modifier)(const string& str, int pad_num, char pad);
		typedef map<string, vector<string> > FormatterMap; 
		typedef map<int, Modifier > ModifierMap; 
		static string FORMAT ;
		static string MAP ;
		static string DELIM ;
	public:
		Formatter(string & schema);
		virtual ~Formatter();
		static void format(string& frm, vector<string>& val, string& fmted);
		virtual void setFormat(string& frm);
		virtual void setMap(vector<string>& map);
		virtual vector<string>& getMap();
		virtual string& getFormat();
		virtual void loadCSVMap(string& strmap);
		virtual void loadDelimitedMap(string& strmap, char delim);

		virtual Formatter::Modifier getModifier(int i) ;
		virtual string getRawFormatModifier(int i);

		virtual string cleanFormatString(const string &fmt);

		//static Modifiers
		static string asSize(const string& size);
		static string asSize(const string& size, int pad_num, char pad);
		static string asCapSize(const string& size, int pad_num, char investGrade);
		static string asPrice(const string& price, int pad_num, char pad);
		static string asYield(const string& yield, int pad_num, char pad);
		static string asCoupon(const string& yield, int pad_num, char pad);
		static string asPaddedFloatNum(const string& num, int pad_num, char pad);
		static void split(const string& str, const string& delim, vector<string>& words);
		static string trim(const string& str);
		static double ATOF(const char *str);
	protected:

		virtual void activateModifiers(vector<string>& formmap);
	private:
		FormatterMap _map;
		string	_schema;
		ModifierMap _modifierMap;
		string _rawFormat;

};

#endif //FORMATTER_HPP
