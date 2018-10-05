
/************************************************************************
||
|| DATE:                $Date: 2004/11/15 17:51:07 $
|| SOURCE:              $Source: /repository/projects/Canpx/libpy/PyProxy.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PyProxy.cpp,v 1.2 2004/11/15 17:51:07 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: PyProxy.cpp,v $
|| LOG:                 Revision 1.2  2004/11/15 17:51:07  mikhailt
|| LOG:                 more-less stable
|| LOG:
|| LOG:                 Revision 1.1  2004/10/20 19:44:34  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <iostream.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include<iostream>
#include<fstream>

#include "PyProxy.hpp"
#include "Logger.hpp"
#include "PyLocalModule.hpp"

using std::string;
using std::ifstream;



static PyObject *
     spam_system(PyObject *self, PyObject *args)
     {
         char *command;
         int sts;

         if (!PyArg_ParseTuple(args, (char *)"s", &command))
             return NULL;
         sts = system(command);
         return Py_BuildValue((char *)"i", sts);
     }

static PyMethodDef SpamMethods[] = {
         {"system",  spam_system, METH_VARARGS,
          "Execute a shell command."},
         {NULL, NULL, 0, NULL}        /* Sentinel */
     };

extern "C"{
void
     initspam(void)
     {
	PyImport_AddModule("spam");
         (void) Py_InitModule("spam", SpamMethods);

     }
}

PyProxy::PyProxy():_pArgs(NULL), _pName(NULL), _pModule(NULL)
{
	if (!Py_IsInitialized())
		Py_Initialize();
	initspam();
	PyLocalModule::init();
}

PyProxy::~PyProxy()
{
	if (_pArgs) Py_DECREF(_pArgs);
	if (_pModule) Py_DECREF(_pModule);
	if (_pName) Py_DECREF(_pName);
	Py_Finalize();
}

bool
PyProxy::loadFrom(const string& address)
{
	bool ret= true;
	
	string name = string(basename((char*)address.c_str()));
	int dot = name.find_first_of('.') ;
	if (dot != string::npos)
		name = name.substr(0, dot);

	//if module already exist skip all together
	if (!uniqueModule(name)){
		Logger::getLogger()->log(Logger::TYPE_WARNING,"Python module %s already loaded", name.c_str());
		return true;
	}
	string pycode;
	ifstream is;
	is.open(address.c_str(), ifstream::in);
	if (is.bad()){
		Logger::getLogger()->log(Logger::TYPE_ERROR,"Python file %s not found", address.c_str());
	} else {
		char line[1024];
		while (is.good()){
			is.getline(line, 1024);
			pycode.append(line + string("\n"));
		}
		is.close();
	}

	_pName = Py_CompileString((char *)pycode.c_str(), (char *)address.c_str(), Py_file_input);
	_pModule = PyImport_ExecCodeModule((char *)name.c_str(), _pName);

	PyRun_SimpleString("import sys\n"); 
	PyRun_SimpleString("sys.path.append('')\n");
        //PyRun_SimpleString("import sys\n");
        PyRun_SimpleString("print sys.builtin_module_names\n");
        PyRun_SimpleString("print sys.modules.keys()\n");
        PyRun_SimpleString("print sys.executable\n");
	if (_pModule == NULL){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "PyProxy::loadFrom failed to load %s", address.c_str());
		ret = false;
		errPrint();
	} else {
		_pDict = PyModule_GetDict(_pModule);
		PyObject *list = PyDict_Keys(_pDict);
		int counter = PyList_GET_SIZE(list);
		for (int i = 0; i < counter; i++){
			PyObject *o = PyList_GetItem(list, i);
			Logger::getLogger()->log(Logger::TYPE_INFO, "PyProxy::loadFrom load %s ", PyString_AsString(o));

		}
		Logger::getLogger()->log(Logger::TYPE_INFO, "PyProxy::loadFrom loaded %d objects", counter);
		Py_DECREF(list); list = NULL;
		Py_DECREF(_pModule); _pModule = NULL;	
		 
        	/* pDict is a borrowed reference */
	}

	Py_DECREF(_pName); _pName = NULL;

	return ret;

}

bool
PyProxy::uniqueModule(const string& name)
{
	bool ret = true;
	_pDict = PyImport_GetModuleDict();
	PyObject *list = PyDict_Keys(_pDict);
	int counter = PyList_GET_SIZE(list);
	for (int i = 0; i < counter; i++){
		PyObject *o = PyList_GetItem(list, i);
		Logger::getLogger()->log(Logger::TYPE_INFO, "PyProxy::uniqueModule loaded %s ", PyString_AsString(o));
		if (!strcmp(name.c_str(), PyString_AsString(o))){
			ret = false;
			//module found get it to _pDict
			_pModule = PyImport_ImportModule((char *)name.c_str()); 
			if (_pModule != NULL){
				_pDict = PyModule_GetDict(_pModule);
				PyObject *list = PyDict_Keys(_pDict);
				int counter = PyList_GET_SIZE(list);
				for (int i = 0; i < counter; i++){
					PyObject *o = PyList_GetItem(list, i);
					Logger::getLogger()->log(Logger::TYPE_INFO, "PyProxy::uniqueModule load %s ", PyString_AsString(o));

				}
				Logger::getLogger()->log(Logger::TYPE_INFO, "PyProxy::uniqueModule loaded %d objects", counter);
				Py_DECREF(list); list = NULL;
				Py_DECREF(_pModule); _pModule = NULL;
			}


		}

	}

	Py_DECREF(list); list = NULL;
	return ret;
}

bool
PyProxy::runFunctionAsLong(const char* func, long& out)
{
	bool ret = false;
	if (_pDict){
		_pFunc = PyDict_GetItemString(_pDict, (char *)func); 
		if (_pFunc && PyCallable_Check(_pFunc)) {
			_pValue = PyObject_CallObject(_pFunc, NULL);
			if(_pValue != NULL){
				out = PyInt_AsLong(_pValue);
				Py_DECREF(_pValue); _pValue = NULL;
				ret = true;	
			}
		}
	}

	return ret;
}

void
PyProxy::errPrint()
{
	PyErr_Print();
}

bool
PyProxy::runFunctionAsTupleLong(const char *func, const vector<long>& in, vector<long>& out)
{
	bool ret = false;
	if (_pDict){
		_pFunc = PyDict_GetItemString(_pDict, (char *)func); 
		if (_pFunc && PyCallable_Check(_pFunc)) {
			_pArgs = PyTuple_New(in.size());
			for(int k = 0; k < in.size(); k++){
				_pValue = PyInt_FromLong(in[k]);
				PyTuple_SetItem(_pArgs, k, _pValue);
			}
			_pValue = PyObject_CallObject(_pFunc, _pArgs);
			if(_pValue != NULL && PyTuple_Check(_pValue)){
				int size = PyTuple_Size(_pValue);
				for (int i = 0; i < size; i++){ 
					PyObject* item = PyTuple_GetItem(_pValue, i);
					out.push_back(PyInt_AsLong(item));
				}
				Py_DECREF(_pValue); _pValue = NULL;
				ret = true;	
			}
			Py_DECREF(_pArgs); _pArgs = NULL;
		}
	}

	return ret;
}

bool
PyProxy::runFunctionAsTupleString(const char *func, const vector<long>& in, vector<string>& out)
{
	bool ret = false;
	if (_pDict){
		_pFunc = PyDict_GetItemString(_pDict, (char *)func); 
		if (_pFunc && PyCallable_Check(_pFunc)) {
			_pArgs = PyTuple_New(in.size());
			for(int k = 0; k < in.size(); k++){
				_pValue = PyInt_FromLong(in[k]);
				PyTuple_SetItem(_pArgs, k, _pValue);
			}
			_pValue = PyObject_CallObject(_pFunc, _pArgs);
			if(_pValue != NULL && PyTuple_Check(_pValue)){
				int size = PyTuple_Size(_pValue);
				for (int i = 0; i < size; i++){ 
					PyObject* item = PyTuple_GetItem(_pValue, i);
					out.push_back(PyString_AsString(item));
				}
				Py_DECREF(_pValue); _pValue = NULL;
				ret = true;	
			}
			Py_DECREF(_pArgs); _pArgs = NULL;
		}
	}

	return ret;
}

bool
PyProxy::runFunctionAsTupleString(const char *func, const vector<string>& in, vector<long>& out)
{
	bool ret = false;
	if (_pDict){
		_pFunc = PyDict_GetItemString(_pDict, (char *)func); 
		if (_pFunc && PyCallable_Check(_pFunc)) {
			_pArgs = PyTuple_New(in.size());
			for(int k = 0; k < in.size(); k++){
				_pValue = PyString_FromString(in[k].c_str());
				PyTuple_SetItem(_pArgs, k, _pValue);
			}
			_pValue = PyObject_CallObject(_pFunc, _pArgs);
			if(_pValue != NULL && PyTuple_Check(_pValue)){
				int size = PyTuple_Size(_pValue);
				for (int i = 0; i < size; i++){ 
					PyObject* item = PyTuple_GetItem(_pValue, i);
					out.push_back(PyInt_AsLong(item));
				}
				Py_DECREF(_pValue); _pValue = NULL;
				ret = true;	
			}
			Py_DECREF(_pArgs); _pArgs = NULL;
		}
	}

	return ret;
}
bool
PyProxy::runFunctionAsTupleString(const char *func, const vector<string>& in, vector<string>& out)
{
	bool ret = false;
	if (_pDict){
		_pFunc = PyDict_GetItemString(_pDict, (char *)func); 
		if (_pFunc && PyCallable_Check(_pFunc)) {
			_pArgs = PyTuple_New(in.size());
			for(int k = 0; k < in.size(); k++){
				_pValue = PyString_FromString(in[k].c_str());
				PyTuple_SetItem(_pArgs, k, _pValue);
			}
			_pValue = PyObject_CallObject(_pFunc, _pArgs);
			if(_pValue != NULL && PyTuple_Check(_pValue)){
				int size = PyTuple_Size(_pValue);
				for (int i = 0; i < size; i++){ 
					PyObject* item = PyTuple_GetItem(_pValue, i);
					out.push_back(PyString_AsString(item));
				}
				Py_DECREF(_pValue); _pValue = NULL;
				ret = true;	
			}else{
				Logger::getLogger()->log(Logger::TYPE_ERROR, "PyProxy::PyProxy::runFunctionAsTupleString failed for _pValue = %p", _pValue);
				errPrint();

			}
			Py_DECREF(_pArgs); _pArgs = NULL;
		} else {
			Logger::getLogger()->log(Logger::TYPE_ERROR, "PyProxy::PyProxy::runFunctionAsTupleString failed for _pFunc");
		}
	}else {
		Logger::getLogger()->log(Logger::TYPE_ERROR, "PyProxy::PyProxy::runFunctionAsTupleString failed no _pDict");
	}
	return ret;
}
