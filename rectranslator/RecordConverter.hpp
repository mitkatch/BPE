
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

#ifndef RECORDCONVERTER_HPP
#define RECORDCONVERTER_HPP

#include "Datum.hpp"
#include "DataGraph.hpp"
#include "Subscriber.hpp"
#include "Logger.hpp"


class RecordConverter: public Subscriber
{
	public:
		RecordConverter(const string& recName, const string& idnName);
		~RecordConverter();

		static string getIdnName( const string& id);
		virtual void notify(Datum *datum);

	protected:
		bool updateIdnRecord(Datum *datum);
		bool activateIdnRecord(Datum *datum);

		string _recName;
		string _idnName;
		bool _activated;

};
#endif //RECORDCONVERTER_HPP
