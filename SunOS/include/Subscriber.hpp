
/************************************************************************
||
|| DATE:                $Date: 2004/10/20 19:47:10 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/Subscriber.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Subscriber.hpp,v 1.4 2004/10/20 19:47:10 mikhailt Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: Subscriber.hpp,v $
|| LOG:                 Revision 1.4  2004/10/20 19:47:10  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.3  2004/09/23 17:54:33  mikhailt
|| LOG:                 using Datum
|| LOG:
|| LOG:                 Revision 1.2  2004/04/29 19:49:16  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:52:40  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include <iostream.h>
#include <map>
#include <string>
#include "Formatter.hpp"

using std::map;
using std::string;

//class DataGraph;
//class DataPage;
class Datum;

class Subscriber
{
	public:
		typedef struct {
			map<string, string> fields;
			string header;
			string footer;
		} FieldMap;

		enum SubscribeMode {SNAPSHOT, UPDATE};
	public:
		Subscriber();
		virtual ~Subscriber();

		virtual void notify(Datum *datum);
		virtual void setSubscribeMode (SubscribeMode mode){
			_mode = mode;
		}

		virtual SubscribeMode getSubscribeMode(){
			return _mode;
		}
		virtual void setSubscribedFields(const string &address, const string& in, const string& out);
		virtual void setHeaderFooter(string &address, string &header, string &footer);
		virtual void getMappedField(const string &address, FieldMap &fm);
		virtual void removeMappedFields(string &address);//when unsubscribe
		virtual Formatter * getFormatter(){
			return _pFormatter;
		}

		virtual void setFormatter(Formatter * fmtr){
			_pFormatter = fmtr;
		}

	protected:
		SubscribeMode _mode;
		map<string, FieldMap > _mapField;
		Formatter * _pFormatter;
};
#endif //SUBSCRIBER_HPP
