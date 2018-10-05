
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

// Application includes
#include "MKLFeedApp.hpp"
#include "MKLSubjectSubMgr.hpp"
#include "Logger.hpp"
#include "DataCache.hpp"

MKLSubjectSubMgr::MKLSubjectSubMgr(const string& name, MKLFeedApp* app) : 
    subjectSubMgr(name)
{
    assert(app != NULL);
    m_mklFeedApp = app;
}

MKLSubjectSubMgr::~MKLSubjectSubMgr()
{
}


/*****************************************************************************
 * registerAddress()
 *
 * Register address within data cache.  Address will be monitored by the 
 * application for IMAGE and UPDATE events.
 *****************************************************************************/
void
MKLSubjectSubMgr::registerAddress(const string& contributor, const string& type, 
				    SMspace& collection)
{
    Logger::getLogger()->log(Logger::TYPE_INFO,
			     "Request for LIST type: %s returned %d entries from Contributor: %s",
			     type.c_str(), collection.size(), contributor.c_str());

    // Iterate through the subject space passed and build the chain or collection
    // of real items.
    SMspace::iterator itr;

    for ( itr = collection.begin(); itr != collection.end(); itr++ )
    {
	// Register an item entry
	registerAddress( contributor, type, (*itr).first, (*itr).second );
    }

}

/***************************************************************************
 * registerAddress
 *
 * Register the service/item pair with this manager.  The service represents
 * the SERVICE such as: SHORCAN, PREBON, CANTOR.  The item represents the
 * full item name within the SERVICE, ie  5.AUG11, 157.SAS_303 etc.
 ***************************************************************************/
void
MKLSubjectSubMgr::registerAddress(const string& contributor, const string& type, 
				    const string& service, const string& item)
{
    string realItem;
    
    // Determine the real item
    int dot = item.find_first_of(".", 0);
    if (dot != string::npos)
	realItem = item.substr(dot +1, item.size() - (dot +1));
    else
	realItem = item;
    
    // Build the unique address (key) to our map
    string uaddress = service + "." + realItem;
    
    // Now determine if the address is already registered
    RealAddrMap::iterator iter = m_realItemMap.find(uaddress);
    
    if ( iter == m_realItemMap.end( ) )
    {
	// No.  Address not register.  Add.
	Instrument instr;
    
	instr.type = type;
	instr.item = item;
	instr.imageProcessed = false;
	instr.contributor = contributor;
	
	// No, let's register
	m_realItemMap[uaddress] = instr;
	
	string address = service + "." + item;
    
	// Subscribe to this address

	DataCache::getCache()->subscribe(address, m_mklFeedApp, Datum::GRAPH);
    }
}

bool
MKLSubjectSubMgr::imageProcessed(const string& i)
{
	cout << "MKLSubjectSubMgr::imageProcessed Process Image "<<i<<endl;
    bool ret = false;
    
    RealAddrMap::iterator itr = m_realItemMap.find(i);
    if ( itr != m_realItemMap.end())
    {
	(*itr).second.imageProcessed = ret = true;
    }
    else 
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "MKLSubjectSubMgr::toChain %s not registered for chain", 
				 i.c_str());
    }
    
    return ret;
}

bool
MKLSubjectSubMgr::chainLoaded()
{
    bool ret = (m_realItemMap.size() > 0);
    int reg = 0, unreg = 0;

    RealAddrMap::iterator it = m_realItemMap.begin();
    for (it; it != m_realItemMap.end(); it++)
    {
	ret &= (*it).second.imageProcessed;

	if ((*it).second.imageProcessed)
	    reg++;
	else
	    unreg++;
    }

    cout << "MKLSubjectSubMgr::chainLoaded() registerd " << reg 
	 << ", unregisterd " << unreg << endl;
    
    return ret;
}

void
MKLSubjectSubMgr::onTimer(TimerId id)
{
    if (!chainLoaded()){
	addTimer(1*1000, 1, this);	
    }
}

string 
MKLSubjectSubMgr::contributor(const string& i)
{
	string contr;
    	RealAddrMap::iterator itr = m_realItemMap.find(i);
    	if ( itr != m_realItemMap.end()){
		contr = (*itr).second.contributor;
    	}
	return contr;
}
