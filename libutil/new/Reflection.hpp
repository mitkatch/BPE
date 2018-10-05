
/************************************************************************
||
|| DATE:                $Date: 2005/06/29 18:55:58 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/Reflection.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Reflection.hpp,v 1.2 2005/06/29 18:55:58 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: Reflection.hpp,v $
|| LOG:                 Revision 1.2  2005/06/29 18:55:58  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.1  2005/03/29 16:09:36  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef REFLECTION_HPP
#define REFLECTION_HPP
#include <string>
#include <map>
#include <set>
#include <vector>

using std::map;
using std::set;
using std::string;
using std::vector;


typedef struct StringCheck {
		StringCheck(const string& s1, bool b):str1(s1), checked(b){};
		StringCheck(const string& s1, const string& s2, bool b):
				str1(s1),str2(s2), checked(b){};
		StringCheck(const string& s1, const string& s2, const string& s3, bool b):
				str1(s1),str2(s2),str3(s3), checked(b){};
		StringCheck(const string& s1, const string& s2, const string& s3, const string s4, bool b):
				str1(s1),str2(s2),str3(s3),str4(s4), checked(b){};
		StringCheck(const string& s1, const string& s2, const string& s3, const string s4, const string s5,  bool b):
				str1(s1),str2(s2),str3(s3),str4(s4), str5(s5), checked(b){};
		~StringCheck(){};
		string str1;
		string str2;
		string str3;
		string str4;
		string str5;
		bool checked;
		bool operator< (const StringCheck& sc)const {
			return str1 < sc.str1;
		};
		bool operator== (const StringCheck& sc) const {
			return str1 == sc.str1;
		};
		
	} StringCheck;

typedef map<StringCheck, vector<StringCheck> > StringCheckMap;

class Reflection
{
	public:
		virtual ~Reflection(){};
		static Reflection* getReflection();
		virtual set<StringCheck> getSet(const string& var);
		virtual StringCheckMap getMap(const string& var);

		virtual bool addSetVar(const string& var, const StringCheck& sc); 
		virtual bool addMapVar(const string& var, const StringCheck& key, const StringCheck& val); 
		
	protected:
		static Reflection* _reflection;
		map<string, StringCheckMap> _rMap;
		Reflection(){};

};
#endif //REFLECTION_HPP
