/************************************************************************
||
|| DATE:                $Date: 2006/05/02 15:10:17 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/subjectPubMgr.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: subjectPubMgr.hpp,v 1.3 2006/05/02 15:10:17 nickz Exp $
|| REVISION:		$Revision: 1.3 $
|| LOG:                 $Log: subjectPubMgr.hpp,v $
|| LOG:                 Revision 1.3  2006/05/02 15:10:17  nickz
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.2  2006/04/01 12:09:23  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.1  2006/03/28 20:11:42  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

#ifndef _SUBJECT_PUB_MGR_HPP_
#define _SUBJECT_PUB_MGR_HPP_

// System includes
#include <map>

// Application includes
#include "subjectMgr.hpp"
#include "Datum.hpp"
#include "Subscriber.hpp"


class subjectPubMgr : public subjectMgr, public Subscriber
{
public:	
    subjectPubMgr(const string& name);
    virtual ~subjectPubMgr();

public:
    virtual int init();
    
    // Virtual requirement (Subscriber)	
    virtual void notify(Datum *datum);
    
    // Virtual requirement (TibrvMsgCallback)
    virtual void onMsg(TibrvListener* listener, TibrvMsg& msg);

public:
    typedef std::multimap<string, string> 	SMParam;

private:
    struct SubjectPubManagerNS
    {
	// Subject to publish the item collection
	string		subject;
	
	// Condition to verify if data should be mapped to this node
	SMParam		condition;
	
	// Collection of unique items for this type.  Each name/value pair
	// is of the form:  <address>/<empty>.  Where the <address> details
	// are of the form:  service.item.  The value portion is not used.
	TibrvMsg*	pItems;
    };

private:
    typedef map<string, SubjectPubManagerNS>	SMPubMap;

    
private:
    bool check_condition( SMParam& condition, Datum* datum );
    void buildParameters( string& condition, SMParam& parameters);
    void definePublishList();
    
private:
    // Publish Subject namespace for multiple asset types (BOND, BILL, PROV)
    SMPubMap		m_pubNamespace;
    
    // Subject to manage list request
    string		m_respondListSubj;

    // Contributor Name
    string		m_contribName;
    
private:
    // Tibrv details
    TibrvListener*	m_listListener;
};


#endif // _SUBJECT_PUB_MGR_HPP_
