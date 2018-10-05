
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

#ifndef FIELDTRANSLATOR_HPP
#define FIELDTRANSLATOR_HPP

#include <utility>
#include <map>

#include "DataGraph.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"

using std::pair;
using std::map;

class FieldTranslator
{
	public:
		typedef pair<string, string> FormatedField;
		typedef map<string, FormatedField> TemplateMap;
	public:
		~FieldTranslator(){};

		static FieldTranslator* getTranslator();
		vector<string> translateField(const string& f);
		vector<DataGraph::Variant> translateVariant(const vector<string>& f, const DataGraph::Variant& v);
	protected:
		FieldTranslator();
		void buildTemplateMap();
		static FieldTranslator* _translator;
		TemplateMap _templateMap;

};

#endif //FIELDTRANSLATOR_HPP
