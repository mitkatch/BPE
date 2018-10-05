
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

#ifndef FIELDFORMATTER_HPP
#define FIELDFORMATTER_HPP

#include "Logger.hpp"
#include "DataGraph.hpp"
#include <map>

using std::map;

class AField
{
	public:
		virtual DataGraph::Variant formatVariant(const DataGraph::Variant& v);


};

class Time5Field: public AField
{
	public:
		virtual DataGraph::Variant formatVariant(const DataGraph::Variant& v);

};

class Time8Field: public AField
{
	public:
		virtual DataGraph::Variant formatVariant(const DataGraph::Variant& v);

};

class DateField: public AField
{
	public:
		virtual DataGraph::Variant formatVariant(const DataGraph::Variant& v);

};

class StaticField: public AField
{

};


class FieldFormatter
{
	public:
		~FieldFormatter();
		static AField* getFormatter(const string& format);
	protected:
		static FieldFormatter* _ff;
		FieldFormatter();
		//static map<string, AField* > _formatMap;


};

#endif //FIELDFORMATTER_HPP
