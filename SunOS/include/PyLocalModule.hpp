
/************************************************************************
||
|| DATE:                $Date: 2004/10/20 19:44:34 $
|| SOURCE:              $Source: /repository/projects/Canpx/libpy/PyLocalModule.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PyLocalModule.hpp,v 1.1 2004/10/20 19:44:34 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: PyLocalModule.hpp,v $
|| LOG:                 Revision 1.1  2004/10/20 19:44:34  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef PYLOCALMODULE_HPP
#define PYLOCALMODULE_HPP

#include <Python.h>

class PyLocalModule
{
	public:
		PyLocalModule(){};
		virtual ~PyLocalModule(){};

		static void init();

};


#endif //PYLOCALMODULE_HPP
