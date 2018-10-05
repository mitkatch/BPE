/************************************************************************
||
|| DATE:                $Date: 2004/11/15 17:51:07 $
|| SOURCE:              $Source: /repository/projects/Canpx/libpy/PyProxy.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PyProxy.hpp,v 1.2 2004/11/15 17:51:07 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: PyProxy.hpp,v $
|| LOG:                 Revision 1.2  2004/11/15 17:51:07  mikhailt
|| LOG:                 more-less stable
|| LOG:
|| LOG:                 Revision 1.1  2004/10/20 19:44:34  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/09/23 17:54:33  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef PYPROXY_HPP
#define PYPROXY_HPP
#include <string>
#include <vector>

#include <Python.h>

using std::string;
using std::vector;

class PyProxy
{
	public:
		PyProxy();
		virtual ~PyProxy();

		virtual bool loadFrom(const string& address);
		
		virtual bool runFunctionAsLong(const char* func, long& out);
		virtual bool runFunctionAsTupleLong(const char *func, const vector<long>& in, vector<long>& out);
		virtual bool runFunctionAsTupleString(const char *func, const vector<long>& in, vector<string>& out);
		virtual bool runFunctionAsTupleString(const char *func, const vector<string>& in, vector<string>& out);
		virtual bool runFunctionAsTupleString(const char *func, const vector<string>& in, vector<long>& out);


		virtual void errPrint();
	protected:
		virtual bool uniqueModule(const string& name);


		PyObject* _pName;
		PyObject* _pModule;
		PyObject* _pDict;
		PyObject* _pFunc;
		PyObject* _pArgs;
		PyObject* _pValue;


};
#endif//PYPROXY_HPP
