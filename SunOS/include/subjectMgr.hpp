/************************************************************************
||
|| DATE:                $Date: 2006/05/02 15:10:16 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/subjectMgr.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: subjectMgr.hpp,v 1.3 2006/05/02 15:10:16 nickz Exp $
|| REVISION:		$Revision: 1.3 $
|| LOG:                 $Log: subjectMgr.hpp,v $
|| LOG:                 Revision 1.3  2006/05/02 15:10:16  nickz
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.2  2006/04/01 12:09:23  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.1  2006/03/28 20:11:41  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

#ifndef _SUBJECT_MGR_HPP_
#define _SUBJECT_MGR_HPP_

// System includes
#include <string>

// Tibrv includes
#include "tibrvcpp.h"

// Common Tibrv fieldnames used in request/reply communication
#define ITEM_TYPE	"itemType"
#define CONTRIBUTOR	"contributor"

#define LIST_DATA	"data"
#define LIST_TYPE	"listType"

#define SERVICE		"SERVICE"
#define ITEM		"ITEM"



class subjectMgr : public TibrvMsgCallback
{
public:	
    subjectMgr(const std::string& name);
    virtual ~subjectMgr();
    
public:
    virtual int init();

    // Virtual requirement (TibrvMsgCallback)
    virtual void onMsg(TibrvListener* listener, TibrvMsg& msg) = 0;

protected:
    int processMsgs();
    bool createListener(TibrvListener*& listener, std::string& subject, void* closure=NULL);
    
protected:
    // Tibrv details
    static TibrvNetTransport	s_transport;
    static TibrvDispatcher	s_dispatcher;

    static pthread_mutex_t	s_DataMutex;
    
private:
    std::string	m_name;
    
private:
    static bool			s_initialized;
    static bool			s_dispatched;
};


#endif // _SUBJECT_MGR_HPP_
