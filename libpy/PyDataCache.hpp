
/************************************************************************
||
|| DATE:                $Date: 2004/11/15 17:51:07 $
|| SOURCE:              $Source: /repository/projects/Canpx/libpy/PyDataCache.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PyDataCache.hpp,v 1.2 2004/11/15 17:51:07 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: PyDataCache.hpp,v $
|| LOG:                 Revision 1.2  2004/11/15 17:51:07  mikhailt
|| LOG:                 more-less stable
|| LOG:
|| LOG:                 Revision 1.1  2004/10/20 19:44:34  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef PYDATACACHE_HPP
#define PYDATACACHE_HPP

#include "Python.h"
#include "DataGraph.hpp"

class PyDataCache
{
	public:
		PyDataCache(){};
		virtual ~PyDataCache(){};

		static PyObject* DataCache_addInstrument(
			PyObject *self, PyObject *args);	

		static PyObject* DataCache_updateInstrument(
			PyObject *self, PyObject *args);	

		static PyObject* DataCache_getVideo(
			PyObject *self, PyObject *args);	
	protected:
		static DataGraph::Variant PyDataCache::pyObjectToVariant(PyObject* val);

};

extern "C"{
	void initDataCache(void);
}

#endif //PYDATACACHE_HPP
