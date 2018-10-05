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

#ifndef NSMAP_HPP
#define NSMAP_HPP

#include <string>
#include <map>

using std::string;
using std::map;

#define URL "URL."
#define URL_SOURCE ".SOURCE"
#define URL_PAGE ".PAGE"
#define URL_SLOT ".SLOT"
#define URL_SECURITY_ID ".SECURITY_ID"
#define URL_ROW_TMPL ".ROW_TMPL"
#define URL_THROTTLE_DELAY ".THROTTLE_DELAY"

struct URLNameSpace {
        string url;
        string source;
        string page;
	string slot;
	string security_id;
	string row_tmpl;
	string throttle_delay;
};

typedef map<string, URLNameSpace> NSMap;




#endif //NSMAP_HPP
