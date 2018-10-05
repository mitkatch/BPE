
/************************************************************************
||
|| DATE:                $Date: 2006/06/19 21:32:58 $
|| SOURCE:              $Source: /repository/projects/Canpx/libpy/PyDataCache.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PyDataCache.cpp,v 1.5 2006/06/19 21:32:58 mikhail Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: PyDataCache.cpp,v $
|| LOG:                 Revision 1.5  2006/06/19 21:32:58  mikhail
|| LOG:                 cannot read the set of fileds from the parser
|| LOG:
|| LOG:                 Revision 1.4  2006/03/23 18:53:58  nickz
|| LOG:                 Added vector for wildcard retrieval
|| LOG:
|| LOG:                 Revision 1.3  2005/09/12 15:49:34  tomcat
|| LOG:                 with dval, lcal, sval Variant
|| LOG:
|| LOG:                 Revision 1.2  2004/11/15 17:51:07  mikhailt
|| LOG:                 more-less stable
|| LOG:
|| LOG:                 Revision 1.1  2004/10/20 19:44:34  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <map>

#include "PyDataCache.hpp"
#include "DataCache.hpp"
#include "DataPage.hpp"

using std::map;

PyObject*
PyDataCache::DataCache_addInstrument(PyObject *self, PyObject *args)
{
    char * address;
    bool sts;

    if(!PyArg_ParseTuple(args, "s", &address))
	return NULL;
    
    sts = DataCache::getCache()->addDatum(address, Datum::GRAPH);	

    if (sts)
    {
	DataGraph *graph = (DataGraph *)DataCache::getCache()->getDatum(address);
	if (graph)
	{
	    vector<Subscriber *>* subvec = DataCache::getCache()->getSubscriberByWCard(address);
	    if ( subvec != NULL )
	    {
		// Iterate through vector and register each subscriber for this graph
		vector<Subscriber *>::iterator it = subvec->begin();
		for( it; it != subvec->end(); it++ )
		{
		    // Don't notify when it's created, it means it's blank
		    sts |= graph->registerSubscriber((*it), false);
		}
	    }
	}
    }

    return Py_BuildValue("i", (int)sts);
}


DataGraph::Variant
PyDataCache::pyObjectToVariant(PyObject* val)
{
	DataGraph::Variant var;

	if (PyFloat_Check(val)){
		var.type = DataGraph::DVAL;
		var.data.dval = PyFloat_AsDouble(val);	
	}else if (PyInt_Check(val)){
		var.type = DataGraph::LVAL;
		var.data.lval = PyInt_AsLong(val);
	}else if (PyString_Check(val)){
		var.type = DataGraph::SVAL;
		var.data.sval = strdup(PyString_AsString(val));
	}else{
		var.type = DataGraph::SVAL;
		var.data.sval = strdup("FAILED");

	}
	return var;	
}

PyObject*
PyDataCache::DataCache_updateInstrument(PyObject *self, PyObject *args)
{
	char * address;
	PyObject *pyInst;
	int sts = 0;
	if(!PyArg_ParseTuple(args, "Os", &pyInst, &address))
		return NULL;
	if (address){
		Datum *datum = DataCache::getCache()->getDatum(address);
		DataGraph *graph = dynamic_cast<DataGraph *>(datum);
		if (graph){
			if (PyMapping_Check(pyInst)){
				PyObject *keys = PyMapping_Keys(pyInst);
					if (PySequence_Check(keys)){
						int len = PySequence_Size(keys);
						if (len != -1){
							for (int i = 0; i < len; i++){
								PyObject *key_val = PySequence_GetItem(keys, i);
								char *ckey = PyString_AsString(key_val);
								PyObject *val_val = PyMapping_GetItemString(pyInst, ckey); 
								sts = (int)graph->updateField(ckey, pyObjectToVariant(val_val));
								//cout<<"Update map["<<ckey<<"]="<<DataGraph::serializeVariantValue(pyObjectToVariant(val_val));
								Py_DECREF(key_val);		
								Py_DECREF(val_val);
							}
						}
					}

				Py_DECREF(keys);		
			}
			graph->updated();
		}
	}
	//cout <<"PyDataCache::DataCache_updateInstrument About to Update "<<address<<endl;	

	return Py_BuildValue("i", sts);
}

PyObject*
PyDataCache::DataCache_getVideo(PyObject *self, PyObject *args)
{
	char * address;
	int video = -1;
	int r1, c1;
	if(!PyArg_ParseTuple(args, "sii", &address, &r1, &c1))
		return NULL;

	if(address){
		DataPage *page = (DataPage *)DataCache::getCache()->getDatum(address);
		if (page)
			video = (int)page->getVideo(r1, c1);
	}

	return Py_BuildValue("i", video);

}

static PyMethodDef DataCacheMethods[] = {
		{ "addInstrument", PyDataCache::DataCache_addInstrument, 
		METH_VARARGS, "Add DataGraph to the DataCache , returns true/false"},
		{ "updateInstrument", PyDataCache::DataCache_updateInstrument,
		METH_VARARGS, "Update Instrument found by the address"},

		{ "getVideo", PyDataCache::DataCache_getVideo,
		METH_VARARGS, "Get video attributes for specified location"},

		{NULL, NULL, 0, NULL}        /* Sentinel */
	};


extern "C" {
void
initDataCache(void)
{
	PyImport_AddModule("DataCache");
	Py_InitModule("DataCache", DataCacheMethods);
}
}
