/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:33:33 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/subjectSubMgr.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: subjectSubMgr.cpp,v 1.7 2006/06/08 15:33:33 nickz Exp $
|| REVISION:		$Revision: 1.7 $
|| LOG:                 $Log: subjectSubMgr.cpp,v $
|| LOG:                 Revision 1.7  2006/06/08 15:33:33  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.6  2006/06/08 15:30:20  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.5  2006/06/05 13:17:34  nickz
|| LOG:                 Added some logging
|| LOG:
|| LOG:                 Revision 1.4  2006/06/04 11:34:16  nickz
|| LOG:                 Changed record producer from Unmanaged to managed publisher
|| LOG:
|| LOG:                 Revision 1.3  2006/05/02 15:10:17  nickz
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

// Application includes
#include "subjectSubMgr.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "Formatter.hpp"

#define SUBJ_SUBSCRIBE	     	"SUBJECT_SUBSCRIBE."
#define SUBJ_MANAGER_NAME    	".NAME"
#define SUBJ_MANAGER_SUBJECT 	".SUBJECT"
#define SUBJ_REQUEST_LIST    	"SUBJECT_REQUEST_LIST"
#define SUBJ_REQUEST_LIST_TIMER	"SUBJECT_REQUEST_LIST_TIMER"

subjectSubMgr::subjectSubMgr(const string& name) : subjectMgr(name)
{
    m_replyListener = NULL;
}

subjectSubMgr::~subjectSubMgr()
{
    // Iterate through the SUB map and delete each listener
    SMSubMap::iterator iter = m_subNamespace.begin();
    for (iter; iter != m_subNamespace.end(); iter++)
    {
	SubjectSubManagerNS &ns = (*iter).second;
	
	if ( ns.listener != NULL )
	    delete ns.listener;
    }
}


/*****************************************************************************
 * init
 * Construct the SUBSCRIBE lists as well as the request/reply subject details.
 *****************************************************************************/
int
subjectSubMgr::init()
{
    if ( subjectMgr::init() == 0 )
    {
	ConfigMap* confMap = ConfigMap::getConfig();

	// Retrieve the SUBSCRIBE configuration stanza
	defineSubscribeList();
	
	// Retrieve the subject to request for a subject list
	m_requestListSubj = confMap->getValue(SUBJ_REQUEST_LIST);

	// Create inbox for our list request
	s_transport.createInbox(m_inbox, sizeof(m_inbox));

	// Create Reply listener (to list request)
	if ( !createListener(m_replyListener, string(m_inbox)) )
	{
	    Tibrv::close();
	    return(-1);
	}

	// Iterate through subscribe list and listen
	SMSubMap::iterator iter = m_subNamespace.begin();
	for (iter; iter != m_subNamespace.end(); iter++)
	{
	    SubjectSubManagerNS &ns = (*iter).second;
	    
	    // Subscribe to item type updates
	    createListener(ns.listener, ns.subject, &ns);

	    // Request for the list
	    requestList((*iter).first);
	}

	// Set timer (if defined)
	string value = confMap->getValue(SUBJ_REQUEST_LIST_TIMER);
	long interval;

	if ( (!value.empty()) && (interval=atol(value.c_str())) >= 0 )
	    addTimer(interval*1000, 1, this);

	return( processMsgs() );
    }

    return(-1);
}

bool
subjectSubMgr::requestList(const string& type)
{
    if ( !m_requestListSubj.empty() )
    {
	TibrvMsg request;
	
	request.addString(LIST_TYPE, type.c_str());
	request.setSendSubject(m_requestListSubj.c_str());
	request.setReplySubject(m_inbox);
	
	// Send request (broadcast request/reply)
	s_transport.send(request);

	Logger::getLogger()->log(Logger::TYPE_INFO, 
				 "subjectSubMsg::requestList request for subject list.  Type: %s, subj: %s",
				 type.c_str(), m_requestListSubj.c_str());

    }

    return(true);
}



/*****************************************************************************
 * defineSubscribeList
 *
 * Retrieve the SUBJECT_MANAGER stanza and build configuration collection for
 * the SUBSCRIBE details.
 *****************************************************************************/
void
subjectSubMgr::defineSubscribeList()
{
    ConfigMap* confMap = ConfigMap::getConfig();
    
    string section_id = SUBJ_SUBSCRIBE;
    map<string, string>::const_iterator it = confMap->begin();
    
    for (it ; it != confMap->end(); it++)
    {
	int section = (*it).first.find(section_id);
	
	if (section == 0)
	{
	    int name_id = (*it).first.find(SUBJ_MANAGER_NAME);

	    string name;
	    string value;
	    
	    if (name_id != string::npos)
	    {
		SubjectSubManagerNS ns;

		int i = (*it).first.length();
		string id = (*it).first.substr(section_id.length(), 
					   i - strlen(SUBJ_MANAGER_NAME) - section_id.length());

		// Determine the 'name' field
		value = string(SUBJ_SUBSCRIBE) + id + string(SUBJ_MANAGER_NAME);
		name = confMap->getValue(Formatter::trim(value).c_str());

		// Populate the 'subject' field
		value = string(SUBJ_SUBSCRIBE) + id + string(SUBJ_MANAGER_SUBJECT);	
		ns.subject = confMap->getValue(Formatter::trim(value).c_str());

		// Initialize listener
		ns.listener = NULL;
		
		m_subNamespace[name] = ns;
	    }
	}
    }
}



/********************************************************************************
 * onMsg
 *
 * Main tibrv event callback.  Determine the realtime event based on subject.
 *
 * The following events are supported:
 *
 * List Subject response: The response to a list request.
 * Item update: An update to an interested asset type to determine RMDS subject.
 *
 ********************************************************************************/
void 
subjectSubMgr::onMsg(TibrvListener* listener, TibrvMsg& msg)
{
    // Determine how to process based on subject
    const char* subject;

    msg.getSendSubject(subject);

    if ( subject != NULL )
    {
	if ( strcmp(subject, m_inbox) == 0 )
	{
	    // Process the response to the list request
	    processListResponse(listener, msg);
	}
	else
	{
	    // Process an Item update
	    processItemUpdate(listener, msg);
	}
    }
}

/********************************************************************************
 * processListResponse
 *
 * Process the response to the list request.
 ********************************************************************************/
void
subjectSubMgr::processListResponse(TibrvListener* listener, TibrvMsg& msg)
{
    const char* type;

    TibrvStatus status = msg.getString(ITEM_TYPE, type);
    if ( status != TIBRV_OK )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Error: Could not retrieve field [%s] from list response message.  status=%d, text=%s",
				 ITEM_TYPE, (int)status, status.getText());
	
	return;
    }

    const char* contributor;

    status = msg.getString(CONTRIBUTOR, contributor);
    if ( status != TIBRV_OK )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Error: Could not retrieve field [%s] from list response message.  status=%d, text=%s",
				 CONTRIBUTOR, (int)status, status.getText());
	
	return;
    }

    // Make sure we care about this data
    SMSubMap::iterator iter = m_subNamespace.find(type);
    if ( iter == m_subNamespace.end( ) )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Error: Received invalid type: %s in list response.  Ignoring.",
				 type);

	return;
    }

    SubjectSubManagerNS& ns = (*iter).second;

    // Retrieve the data
    TibrvMsg data;

    status = msg.getMsg(LIST_DATA, data);
    if ( status != TIBRV_OK )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Error: Could not retrieve field [%s] from list response message.  status=%d, text=%s",
				 LIST_DATA, (int)status, status.getText());

	const char* dump;
	msg.convertToString(dump);
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Error: Response message looks like: [%s]", dump);
	
	return;
    }

    TibrvMsgField field;
    unsigned int numFields = 0;

    data.getNumFields(numFields);

    string address;

    SMspace collection;

    // Iterate through the data (subject collection) and process each
    for (int x=0; x < numFields; x++)
    {
	if ( data.getFieldByIndex(field, x) == TIBRV_OK )
	{
	    // For each entry, build the service/item pair based on address
	    address = field.getName();

	    int dot = address.find_first_of(".", 0);
	    if (dot != string::npos)
	    {
		collection.push_back( SMpair(address.substr(0, dot),
				             address.substr(dot+1, address.size() - (dot+1))) );
	    }
	    else
	    {
		// Address has improper form
		Logger::getLogger()->log(Logger::TYPE_WARNING,
					 "List entry contains invalid address: %s",
					 address.c_str());
	    }
	}
    }

    registerAddress( contributor, type, collection );
}




void
subjectSubMgr::processItemUpdate(TibrvListener* listener, TibrvMsg& msg)
{
    const char* type;

    // Get the type
    TibrvStatus status = msg.getString(ITEM_TYPE, type);
    if ( status != TIBRV_OK )
    {
	Logger::getLogger()->log(Logger::TYPE_WARNING,
				 "Warning: Failed to process Item update.  Failed to retrieve field %s. Status: %d (%s)",
				 ITEM_TYPE, (int)status, status.getText());

	return;
    }

    const char* service;

    // Get the service
    status = msg.getString(SERVICE, service);
    if ( status != TIBRV_OK )
    {
	Logger::getLogger()->log(Logger::TYPE_WARNING,
				 "Warning: Failed to process Item update.  Failed to retrieve field %s. Status: %d (%s)",
				 SERVICE, (int)status, status.getText());

	return;
    }

    const char* contributor;

    status = msg.getString(CONTRIBUTOR, contributor);
    if ( status != TIBRV_OK )
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Warning: Failed to process Item update.  Failed to retrieve field %s. Status: %d (%s)",
				 CONTRIBUTOR, (int)status, status.getText());
	
	return;
    }

    const char* item;

    // Get the item
    status = msg.getString(ITEM, item);
    if ( status != TIBRV_OK )
    {
	Logger::getLogger()->log(Logger::TYPE_WARNING,
				 "Warning: Failed to process Item update.  Failed to retrieve field %s. Status: %d (%s)",
				 ITEM, (int)status, status.getText());

	return;
    }

    // Register address details with application
    registerAddress( contributor, type, service, item );
}


