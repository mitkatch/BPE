
/************************************************************************
||
|| DATE:                $Date: 2006/05/02 14:40:13 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/Datum.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: Datum.hpp,v 1.6 2006/05/02 14:40:13 nickz Exp $
|| REVISION:    $Revision: 1.6 $
|| LOG:                 $Log: Datum.hpp,v $
|| LOG:                 Revision 1.6  2006/05/02 14:40:13  nickz
|| LOG:                 Added state to determine initial (IMAGE) vs update
|| LOG:
|| LOG:                 Revision 1.5  2006/04/06 18:00:54  nickz
|| LOG:                 Fixed spelling errors in output
|| LOG:
|| LOG:                 Revision 1.4  2006/04/01 12:08:31  nickz
|| LOG:                 Added additional methods to Datum
|| LOG:
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
#include "Logger.hpp"

class Datum
{
public:
    enum datum_type {PAGE, GRAPH};
    explicit Datum(const string& address) : _initial(true)
	{
	    int dot = address.find_first_of(".", 0);
	    if (dot != string::npos)
	    {
		_address = address;
		_service = address.substr(0, dot);
		_item = address.substr(dot +1, address.size() - (dot +1));
	    }
	    else
	    {
		// Address has improper form
		Logger::getLogger()->log(Logger::TYPE_WARNING,
					 "Datum::Datum contains incorrect address: %s",
					 address.c_str());
	    }
	};
    virtual ~Datum(){};
    virtual bool registerSubscriber(Subscriber *subscriber, bool b_notify=true) = 0;
    virtual bool unregisterSubscriber(Subscriber *subscriber) = 0;
    
    virtual int numOfSubscribers() = 0; 
    virtual datum_type type() = 0;
    virtual string address() const{
	return _address;
    }

    virtual const string& service()
	{
	    return(_service);
	}

    virtual const string& item()
	{
	    return(_item);
	}

    virtual const bool initial()
	{
	    return(_initial);
	}

    virtual void initial(bool val)
	{
	    _initial = val;
	}

    virtual string toString(){
	return string("Datum::toString is not implemented");
    }
    
    virtual void getDataClone(Datum *datum) = 0;
protected:
    string _address;

    string _service;
    string _item;
    bool   _initial;
};

#endif//DATUM_HPP
