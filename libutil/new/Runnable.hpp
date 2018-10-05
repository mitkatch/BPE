
/************************************************************************
||
|| DATE:                $Date: 2004/04/29 19:49:16 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/Runnable.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Runnable.hpp,v 1.2 2004/04/29 19:49:16 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: Runnable.hpp,v $
|| LOG:                 Revision 1.2  2004/04/29 19:49:16  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/12 19:22:22  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

class Runnable
{
	public:
		Runnable() {};
		virtual ~Runnable() {};

		virtual long Run() = 0;
};

#endif //RUNNABLE_HPP
