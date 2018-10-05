
/************************************************************************
||
|| DATE:                $Date: 2006/03/18 13:19:57 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/NSMap.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: NSMap.hpp,v 1.3 2006/03/18 13:19:57 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: NSMap.hpp,v $
|| LOG:                 Revision 1.3  2006/03/18 13:19:57  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.2  2004/11/15 17:40:09  mikhailt
|| LOG:                 more-less stable
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:44:25  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:42:08  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef NSMAP_HPP
#define NSMAP_HPP

// System includes
#include <string>
#include <map>

using std::string;
using std::map;


struct URLNameSpace 
{
    string url;
    string source;
    string page;
    string parser;
    string parser_map;
    string subscriber;
};

typedef map<string, URLNameSpace> NSMap;




#endif //NSMAP_HPP
