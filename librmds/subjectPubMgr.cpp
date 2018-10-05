/************************************************************************
||
|| DATE:                $Date: 2006/05/02 15:10:17 $
|| SOURCE:              $Source: /repository/projects/Canpx/librmds/subjectPubMgr.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: subjectPubMgr.cpp,v 1.4 2006/05/02 15:10:17 nickz Exp $
|| REVISION:		$Revision: 1.4 $
|| LOG:                 $Log: subjectPubMgr.cpp,v $
|| LOG:                 Revision 1.4  2006/05/02 15:10:17  nickz
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.3  2006/04/03 19:36:40  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.2  2006/04/01 12:09:23  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.1  2006/03/28 20:11:42  nickz
|| LOG:                 .
|| LOG:
|| LOG:
************************************************************************/

// Application includes
#include "subjectPubMgr.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "DataGraph.hpp"


using std::multimap;

// Configuration names
#define SUBJ_RESPOND_LIST    "SUBJECT_RESPOND_LIST"
#define SUBJ_CONTRIB_NAME    "SUBJECT_CONTRIBUTOR_NAME"
#define SUBJ_PUBLISH	     "SUBJECT_PUBLISH."
#define SUBJ_MANAGER_NAME    ".NAME"
#define SUBJ_MANAGER_SUBJECT ".SUBJECT"
#define SUBJ_MANAGER_COND    ".CONDITION"


subjectPubMgr::subjectPubMgr(const string& name) : subjectMgr(name)
{
    m_listListener = NULL;
}

subjectPubMgr::~subjectPubMgr()
{
    // Iterate through the PUB map and delete each TibrvMsg
    SMPubMap::iterator iter = m_pubNamespace.begin();
    for (iter; iter != m_pubNamespace.end(); iter++)
    {
	SubjectPubManagerNS &ns = (*iter).second;
	
	if ( ns.pItems != NULL )
	    delete ns.pItems;
    }

    if ( m_listListener != NULL )
	delete m_listListener;
}


/*****************************************************************************
 * init
 * Construct the PUBLISH list and initialize the Tibrv environment.
 *****************************************************************************/
int
subjectPubMgr::init()
{
    if ( subjectMgr::init() == 0 )
    {
	ConfigMap* confMap = ConfigMap::getConfig();

	// Retrieve the PUBLISH configuration stanza
	definePublishList();
	
	// Retrieve the subject to subscribe for a subject list
	m_respondListSubj = confMap->getValue(SUBJ_RESPOND_LIST);

	// Retrieve a name identifier for this publisher (contributor)
	m_contribName = confMap->getValue(SUBJ_CONTRIB_NAME);

	// Create List listener
	if ( !createListener(m_listListener, m_respondListSubj) )
	{
	    Tibrv::close();
	    return(-1);
	}

	return( processMsgs() );
    }

    return(-1);
}


/*****************************************************************************
 * definePublishList
 *
 * Retrieve the SUBJECT_MANAGER stanza and build configuration collection for
 * the PUBLISH details.
 *****************************************************************************/
void
subjectPubMgr::definePublishList()
{
    ConfigMap* confMap = ConfigMap::getConfig();
    
    string section_id = SUBJ_PUBLISH;
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
		SubjectPubManagerNS ns;

		int i = (*it).first.length();
		string id = (*it).first.substr(section_id.length(), 
					       i - strlen(SUBJ_MANAGER_NAME) - section_id.length());
		
		value = string(SUBJ_PUBLISH) + id + string(SUBJ_MANAGER_NAME);
		name = confMap->getValue(Formatter::trim(value).c_str());

		value = string(SUBJ_PUBLISH) + id + string(SUBJ_MANAGER_SUBJECT);	
		ns.subject = confMap->getValue(Formatter::trim(value).c_str());
		
		value = string(SUBJ_PUBLISH) + id + string(SUBJ_MANAGER_COND);
		
		string condition = confMap->getValue(Formatter::trim(value).c_str());
		buildParameters( condition,  ns.condition );

		ns.pItems = new TibrvMsg();
		m_pubNamespace[name] = ns;
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
 * List Subject request:  The request to a list response.
 *
 ********************************************************************************/
void 
subjectPubMgr::onMsg(TibrvListener* listener, TibrvMsg& msg)
{
    if ( listener = m_listListener )
    {
	const char* replySubject = NULL;
	
	// If there was a reply subject, get it
	msg.getReplySubject(replySubject);
	
	// A reply subject must exist in order to respond
	if ( replySubject == NULL )
	    return;
	
	// Process list request and respond
	const char* ltype = NULL;
	
	TibrvStatus status = msg.getString(LIST_TYPE, ltype);
	if ( status == TIBRV_OK )
	{
	    pthread_mutex_lock(&s_DataMutex);
	    SMPubMap::iterator iter = m_pubNamespace.find(ltype);
	    if ( iter != m_pubNamespace.end( ) )
	    {
		SubjectPubManagerNS &ns = (*iter).second;
		TibrvMsg* data = ns.pItems;

		TibrvMsg response;

		response.addString(ITEM_TYPE, ltype);
		response.addString(CONTRIBUTOR, m_contribName.c_str());
		response.addMsg(LIST_DATA, *data);
		response.setSendSubject(replySubject);
		
		s_transport.send(response);
	    }
	    pthread_mutex_unlock(&s_DataMutex);
	}
    }
}


/*****************************************************************************
 * notify
 * Called when update occurs within the DataCache.
 * Routine determines if node is new.  If so, add to local cache and notify
 * Canpx components of new element within the data cache.
 *****************************************************************************/
void
subjectPubMgr::notify(Datum *datum)
{
    if (datum)
    {
	bool found = false;
	TibrvMsgField empty;
	
	pthread_mutex_lock(&s_DataMutex);

	// Iterate through map and try to locate item
	SMPubMap::iterator iter = m_pubNamespace.begin();
	for (iter; !found && iter != m_pubNamespace.end(); iter++)
	{
	    SubjectPubManagerNS &ns = (*iter).second;
	    
	    found = (ns.pItems->getField(datum->address().c_str(), empty)) == TIBRV_OK;
	}
	
	if ( !found )
	{
	    // Determine where, if any, to insert item
	    iter = m_pubNamespace.begin();
	    for (iter; iter != m_pubNamespace.end(); iter++)
	    {
		SubjectPubManagerNS &ns = (*iter).second;
		
		if ( check_condition( ns.condition, datum ) )
		{
		    // Insert subject into this list
		    ns.pItems->addString(datum->address().c_str(), "");
		    
		    // Publish notification to Canpx network
		    TibrvMsg msg;
		    
		    msg.setSendSubject(ns.subject.c_str());
		    msg.addString(ITEM_TYPE, (*iter).first.c_str());
		    msg.addString(SERVICE, datum->service().c_str());
		    msg.addString(CONTRIBUTOR, m_contribName.c_str());
		    msg.addString(ITEM, datum->item().c_str());
		    
		    s_transport.send(msg);

		    const char* msgStr;
		    msg.convertToString(msgStr);
		    
		    Logger::getLogger()->log(Logger::TYPE_INFO, 
					     "subjectPubMgr::notify new subject [%s]",
					     msgStr);
		}
	    }
	}
	pthread_mutex_unlock(&s_DataMutex);
    }
}


/******************************************************************************
 * check_condition
 *
 * Determine if the datum passes the condition.  The value pairs within the 
 * condition cache are of the form:
 *
 *	FIELD:VALUE	implies FIELD == VALUE
 *	FILED!:VALUE	implies FIELD != VALUE
 *
 * Returns:  TRUE  (if condition successful for this datum)
 *	     FALSE (otherwise)
 ******************************************************************************/
bool
subjectPubMgr::check_condition( SMParam& condition, Datum* datum )
{
    if ( datum->type() == Datum::PAGE )
	return(true);

    DataGraph* graph = dynamic_cast<DataGraph *>(datum);

    string field;
    
    SMParam::iterator iter = condition.begin();
    for( iter; iter != condition.end(); iter++)
    { 
	bool equals = true;
	
	field = (*iter).first;	
	if (field[field.size() - 1] == '!')
	{
	    equals = false;
	    field = field.erase(field.size()- 1, 1);
	}
	
	// Retrieve the value from the node
	string val = graph->getValue(field);
	
	if ( equals )
	{
	    if ( val != (*iter).second )
		return(false);
	}
	else
	{
	    if ( val == (*iter).second )
		return(false);
	}
    }
    
    return(true);
}

/******************************************************************************
 * buildParameters
 * Based on the condition, build the parameter list.
 ******************************************************************************/
void
subjectPubMgr::buildParameters( string& condition, SMParam& parameters)
{
    // request FILED1=VALUE&FIELD2=VALUE&FIELD3!=VALUE
    vector<string> spv;
    Formatter::split(condition, "&", spv);
    
    for (int i = 0; i < spv.size(); i++)
    {
	string req1 = spv[i];
	int eq = req1.find_first_of('=', 0);
	if (eq != string::npos)
	{
	    string field = req1.substr(0, eq);
	    string val = req1.substr(eq +1, req1.size() -(eq + 1));
	    
	    parameters.insert(multimap<string, string>::value_type(field, val));
	}
    }
}

