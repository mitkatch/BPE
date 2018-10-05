/************************************************************************
||
|| DATE:                $Date: 2005/03/29 16:05:55 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/HtmlBeanProcessor.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: HtmlBeanProcessor.hpp,v 1.1 2005/03/29 16:05:55 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: HtmlBeanProcessor.hpp,v $
|| LOG:                 Revision 1.1  2005/03/29 16:05:55  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef HTMLBEANPROCESSOR_HPP 
#define HTMLBEANPROCESSOR_HPP

#include<iostream>
#include<fstream>
#include <string>
#include <map>

#include "Logger.hpp"

using std::string;
using std::map;
using std::ifstream;

class MLCheckTable 
{
	public:
		MLCheckTable(){};
		virtual ~MLCheckTable(){};
		static const char* name() { return "mlchecktable";};
		static string toString(const string& var);
		static int build(string& var, int start);
};

class MLCheckList
{
	public:
		MLCheckList(){};
		virtual ~MLCheckList(){};
		static const char * name() { return  "mlchecklist";};
		static string toString(const string& var);
		static int build(string& var, int start);

};

class HtmlBeanProcessor
{
	public:
		HtmlBeanProcessor();
		virtual ~HtmlBeanProcessor();

		virtual string process(const string& filename);
		static bool loadFile(const string& file);

		static HtmlBeanProcessor* getProcessor();

		virtual void addNewFile(const string& filename, const string& file);
		virtual bool isFileLoaded(const string& filename);

	protected:
		static HtmlBeanProcessor* _processor; 

		string build(const string& src);

		map<string, string> _filesLoaded;
};

#endif //HTMLBEANPROCESSOR_HPP
