
/************************************************************************
||
|| DATE:                $Date: 2005/06/24 16:21:53 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/NSMap.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: NSMap.hpp,v 1.1 2005/06/24 16:21:53 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: NSMap.hpp,v $
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef NSMAP_HPP
#define NSMAP_HPP

#define CONTRIBUTOR	"CONTRIBUTOR." 
#define REQUEST		".REQUEST"
#define SOURCE		".SOURCE"
#define ALIAS		".ALIAS"


struct ContribNameSpace {
	string source;
	string request;	
};

typedef map<string, ContribNameSpace> NSMap;




#endif //NSMAP_HPP

