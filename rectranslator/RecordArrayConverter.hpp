
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

#ifndef RECORDARRAYCONVERTER_HPP
#define RECORDARRAYCONVERTER_HPP
#include <map>
#include <string>
#include "MsgGraph.hpp"
#include "RecordConverter.hpp"

using std::map;
using std::string;



class RecordArrayConverter
{
	public:
		RecordArrayConverter(const string& url);
		virtual ~RecordArrayConverter();
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
	private:
		map<string, RecordConverter*> _recArray;
		string _arrayUrl;
		string _port, _host, _connstr;
};

#endif //RECORDARRAYCONVERTER_HPP

