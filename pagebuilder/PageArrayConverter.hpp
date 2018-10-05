
/************************************************************************
||
|| DATE:                $Date: 2006/04/06 17:57:30 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PageArrayConverter.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PageArrayConverter.hpp,v 1.3 2006/04/06 17:57:30 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: PageArrayConverter.hpp,v $
|| LOG:                 Revision 1.3  2006/04/06 17:57:30  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:                 Revision 1.2  2006/04/04 13:19:33  nickz
|| LOG:                 ported to rmds
|| LOG:
|| LOG:                 Revision 1.1  2005/09/21 17:49:55  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef PAGEARRAYCONVERTER_HPP
#define PAGEARRAYCONVERTER_HPP

#include <map>
#include <string>
#include "MsgGraph.hpp"
#include "PageConverter.hpp"
#include "ChainManager.hpp"

using std::map;
using std::string;

class PageArrayConverter
{
	public:
		PageArrayConverter(const string& url);
		virtual ~PageArrayConverter();
		virtual void load(const string& search_url);

		virtual string connstr() const {
			return _connstr;
		}
		virtual string host() const {
			return _host;
		}
		virtual string port() const {
			return _port;
		}

	protected:
		virtual void processArray(MsgGraph *rset);
		virtual void addDynamic(const string& url, MsgGraph *rset);
	private:
		map<string, PageConverter*> _pageArray;
		string _arrayUrl;
		string _port, _host, _connstr;
		vector<string> _dynpnmV; //keep order of dyn pages
		ChainManager *_chainMgr;
};

#endif //PAGEARRAYCONVERTER_HPP
