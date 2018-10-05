/************************************************************************
||
|| DATE:                $Date: 2005/06/29 18:55:58 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/DataGraph.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: DataGraph.hpp,v 1.10 2005/06/29 18:55:58 mikhailt Exp $
|| REVISION:    $Revision: 1.10 $
|| LOG:                 $Log: DataGraph.hpp,v $
|| LOG:                 Revision 1.10  2005/06/29 18:55:58  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.9  2005/03/29 16:09:36  mikhailt
|| LOG:                 delta added
|| LOG:
|| LOG:                 Revision 1.8  2004/12/03 18:42:56  mikhailt
|| LOG:                 handles HTTP POST
|| LOG:
|| LOG:                 Revision 1.7  2004/11/19 21:08:35  mikhailt
|| LOG:                 updated for Subscriber
|| LOG:
|| LOG:                 Revision 1.6  2004/11/15 17:52:41  mikhailt
|| LOG:                 more-less stable for parser
|| LOG:
|| LOG:                 Revision 1.5  2004/10/20 19:47:10  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.4  2004/09/23 17:54:33  mikhailt
|| LOG:                 using Datum
|| LOG:
|| LOG:                 Revision 1.3  2004/04/29 19:49:16  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.2  2004/01/09 21:35:03  tomcat
|| LOG:                 SunOS compatible
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:52:40  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef DATAGRAPH_HPP
#define DATAGRAPH_HPP

#include <stdio.h>
#include <pthread.h>
//#include <stdarg.h>

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>

using std::map;
using std::string;
using std::vector;
using std::set;

#include "Datum.hpp"
#include "MsgGraph.hpp"

#include "Subscriber.hpp"
#include "Formatter.hpp"

#define DATAGRAPH_FLAT_DELIMITER	" : "

class DataGraph : public Datum
{
	public:
		enum {NVAL, SVAL, LVAL, DVAL};
		struct Variant {
			Variant();
			Variant(int t, const char *val);
			Variant(int t, long val);
			Variant(int t, double val);
			Variant(const Variant& var);
			Variant& operator=(const Variant& var);
			bool operator==(const Variant& var);
			int type;
			struct {
				char *sval;
				long lval;
				double dval;
			} data ;
			~Variant();
		} ;
		static string serializeVariantValue(const Variant& var);
		static string serializeVariantType(const Variant& var);
	public:
		typedef map<string, Variant> DataMap;
	public:
		explicit DataGraph(const string& address);
		virtual ~DataGraph();

		virtual bool addField(string key, string value );
		virtual bool addVariant(const string& key, const Variant& var);

		virtual bool updateField(string key, string value);
		virtual bool updateField(const string& key, const Variant& var);

		virtual bool removeField(const string& field);

		virtual set<string> getFieldSet();

		virtual string getValue(const string& field);
		virtual DataGraph::Variant getVariant(const string& field);

		virtual bool registerSubscriber(Subscriber *subscriber, bool b_notify=true);
		virtual bool unregisterSubscriber(Subscriber *subscriber);
		
		virtual string toXmlString(Subscriber* s);
		virtual string toXmlString();
		virtual string toString();
		virtual string toCSVString(Subscriber* s);
		virtual string toFormattedString(Subscriber* s);
		virtual int numOfSubscribers();
		virtual void updated();
		virtual void updated(Subscriber *s);
		virtual DataMap delta(Subscriber *s); 
		virtual void getDataClone(Datum *datum);
		virtual void setImage(const DataGraph::DataMap& new_map);
		virtual DataGraph::DataMap getImage();
		virtual void buildImageFromFlat(const vector<string>& vimage, const string& delim); 
		virtual string updateFromFlat(DataGraph& flatdata);
		virtual string getKeyValHtmlTable(bool indexed, bool editable, const string& tblParm);
		virtual void persistent(bool p){
			_persistent = p;
		}
		virtual bool persistent() const {
			return _persistent;
		}

	protected:

		DataGraph::Variant updateVariantFromFlat( const DataGraph::Variant& var, const string& str);
		DataMap _dataImage;
		char _buf[BUFSIZ];
		set<Subscriber *> _subscribers;
		pthread_mutex_t m_DataMutex;
		pthread_mutex_t m_SubsctiberMutex;
		bool _persistent;
};
#endif //DATAGRAPH_HPP
