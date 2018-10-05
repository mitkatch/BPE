
/************************************************************************
||
|| DATE:                $Date: 2005/06/29 18:55:58 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libutil/RCS/Datum.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Datum.hpp,v 1.3 2005/06/29 18:55:58 mikhailt Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: Datum.hpp,v $
|| LOG:                 Revision 1.3  2005/06/29 18:55:58  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.2  2004/10/20 19:47:10  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.1  2004/09/23 17:54:33  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef DATUM_HPP
#define DATUM_HPP
#include "Subscriber.hpp"
class Datum
{
	public:
		enum datum_type {PAGE, GRAPH};
		explicit Datum(const string& address):_address(address){};
		virtual ~Datum(){};
		virtual bool registerSubscriber(Subscriber *subscriber, bool b_notify=true) = 0;
		virtual bool unregisterSubscriber(Subscriber *subscriber) = 0;

		virtual int numOfSubscribers() = 0; 
		virtual string address() const{
			return _address;
		}

		virtual string toString(){
			return string("Datum::toString is not implemented");
		}

		virtual void getDataClone(Datum *datum) = 0;
	protected:
		string _address;
};

#endif//DATUM_HPP
