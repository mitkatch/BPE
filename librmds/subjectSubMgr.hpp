/************************************************************************
||
|| DATE:                $Date: 2006/05/02 15:10:18 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/subjectSubMgr.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: subjectSubMgr.hpp,v 1.3 2006/05/02 15:10:18 nickz Exp $
|| REVISION:		$Revision: 1.3 $
|| LOG:                 $Log: subjectSubMgr.hpp,v $
|| LOG:                 Revision 1.3  2006/05/02 15:10:18  nickz
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.2  2006/04/01 12:09:24  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.1  2006/03/28 20:11:43  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

#ifndef _SUBJECT_SUB_MGR_HPP_
#define _SUBJECT_SUB_MGR_HPP_

// System includes
#include <map>
#include <vector>

// Application includes
#include "subjectMgr.hpp"
#include "TimerCallback.hpp"

using std;

class subjectSubMgr : public subjectMgr, public TimerCallback
{
public:	
    subjectSubMgr(const std::string& name);
    virtual ~subjectSubMgr();
    
public:
    typedef std::pair<std::string, std::string> SMpair;
    typedef std::vector<SMpair> SMspace;

public:
    virtual int init();
    
    // Virtual requirement (TibrvMsgCallback)
    virtual void onMsg(TibrvListener* listener, TibrvMsg& msg);

    // Virtual override (for deriving classes)
    virtual void registerAddress(const std::string& contributor, const std::string& type, 
				 const std::string& service, const std::string& item) {}
    virtual void registerAddress(const std::string& contributor, const std::string& type, 
				 SMspace& collection) {}

public:
    // Request for list based on type
    bool requestList(const std::string& type);
    
private:
    struct SubjectSubManagerNS
    {
	// Subject to subscribe to new items of this type
	std::string		subject;
	
	// Listener for subscription
	TibrvListener* 		listener;
    };

private:
    typedef std::map<std::string, SubjectSubManagerNS> SMSubMap;
    
private:
    void defineSubscribeList();
    void processListResponse(TibrvListener* listener, TibrvMsg& msg);
    void processItemUpdate(TibrvListener* listener, TibrvMsg& msg);
    
private:
    // Subscribe Subject namespace for multiple asset types (BOND, BILL, PROV) etc
    SMSubMap		m_subNamespace;
    
    // Subject to manage list request
    std::string		m_requestListSubj;
    
    // Inbox (return address) used for broadcast list request
    char		m_inbox[128];

private:
    // Tibrv details
    TibrvListener*	m_replyListener;
};


#endif // _SUBJECT_SUB_MGR_HPP_
