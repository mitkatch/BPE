/************************************************************************
||
|| DATE:                $Date: 2005/03/29 16:05:55 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/MCMsg.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: MCMsg.hpp,v 1.3 2005/03/29 16:05:55 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: MCMsg.hpp,v $
|| LOG:                 Revision 1.3  2005/03/29 16:05:55  mikhailt
|| LOG:                 more build
|| LOG:
|| LOG:                 Revision 1.2  2005/01/05 17:24:48  mikhailt
|| LOG:                 control mc msg sent
|| LOG:
|| LOG:                 Revision 1.1  2004/07/30 16:16:06  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef MCMSG_HPP
#define MCMSG_HPP

#include <string>
#include <map>
using std::string;
using std::map;

class MCMsg
{
	public:
		MCMsg(){};
		MCMsg(const MCMsg& msg);
		virtual ~MCMsg(){};

		virtual string toString() const;
		virtual int length()const{
			return _stub.length() 
				+ _suffix.length()
				+ _prefix.length() + 4;//header+footer + 2 nl
		}
		virtual void addKeyValue(const char* key, const char* val);
		virtual bool isKey(const char* key);
		virtual void buildSuffix(const string& pname, const string& phost, const string& pid);
		virtual void buildSuffix(const string& pname);
		virtual void buildPrefix(const string& prefix){
			_prefix = prefix;
		}

	protected:
		//int _length;
		string _stub;
		string _suffix;
		string _prefix;
		map<string, string> _stabMap;
};

class MCMsgHeartbeat: public MCMsg
{
	public:
		MCMsgHeartbeat(const string& str);
		virtual ~MCMsgHeartbeat(){};
};

#endif //MCMSG_HPP
